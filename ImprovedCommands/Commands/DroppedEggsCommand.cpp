#include "DroppedEggsCommand.h"
#include <sstream>

void DroppedEggsRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	auto result = DroppedEggsInternal(&msg, ArkLibrary::GetCommand(CommandName_DroppedEggs_Rcon));
	if (result.length() == 0) return;

	FString reply(result.c_str());
	reply.AppendChar(L'\n');
	rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);
}

void DroppedEggsConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	auto result = DroppedEggsInternal(cmd, ArkLibrary::GetCommand(CommandName_DroppedEggs_Console));
	if (result.length() == 0) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", result.c_str());
}

std::wstring DroppedEggsInternal(FString* msg, ArkLibrary::CommandDefinition *cmd)
{
	//PrimalItemConsumable_Egg_Wyvern_Fertilized_

	std::vector<std::string> classNames;
	if (cmd)
	{
		auto classNamesJson = cmd->Json.find("ClassNames");
		if (classNamesJson != cmd->Json.end())
		{
			for (auto it = classNamesJson->begin(); it != classNamesJson->end(); ++it)
			{
				classNames.push_back(*it);
			}
		}
	}

	TArray<FString> Parsed;
	msg->ParseIntoArray(Parsed, L" ", true);

	if (Parsed.IsValidIndex(1))
	{
		std::string action = Parsed[1].ToString();
		std::transform(action.begin(), action.end(), action.begin(), tolower);

		bool isListing = action.compare("list") == 0;
		bool isDestroyAll = action.compare("destroy_all") == 0;
		bool isDestroyAllInclDroppedByPlayer = action.compare("destroy_all_including_dropped_by_player") == 0;
		bool isDestroySpoiled = action.compare("destroy_spoiled") == 0;
		bool isDestroySpoiledInclDroppedByPlayer = action.compare("destroy_spoiled_including_dropped_by_player") == 0;
		bool isDestroyAction = isDestroyAll || isDestroyAllInclDroppedByPlayer || isDestroySpoiled || isDestroySpoiledInclDroppedByPlayer;

		std::stringstream ss, ss2;
		if (isListing || isDestroyAction)
		{
			UWorld* world = ArkApi::GetApiUtils().GetWorld();
			if (!world) return {};

			TArray<AActor*>* FoundActors = new TArray<AActor*>();
			UGameplayStatics::GetAllActorsOfClass(world, ArkExtensions::ADroppedItem::StaticClass(), FoundActors);

			int num = 0, numDestroyed = 0;
			for (uint32_t i = 0; i < FoundActors->Num(); i++)
			{
				AActor* actor = (*FoundActors)[i];

				ADroppedItem* droppedItem = reinterpret_cast<ADroppedItem*>(actor);

				FString* className = new FString();
				FName bp = droppedItem->ClassField()()->NameField()();
				bp.ToString(className);
				std::string classNameStr = className->ToString();
				delete className;

				if (std::any_of(classNames.begin(), classNames.end(), [&classNameStr](std::string str) { return classNameStr.compare(str) == 0; }))
				{
					FItemNetInfo info = droppedItem->MyItemInfoField()();
					long double nextSpoilingTime = info.NextSpoilingTimeField()();
					float durability = info.ItemDurabilityField()();

					UPrimalItem* item = droppedItem->MyItemField()();
					if (item)
					{
						float spoilingTime = item->GetSpoilingTime();
						float spoilingTimeFromField = item->SpoilingTimeField()();
						long double nextSpoilingTime = item->NextSpoilingTimeField()();
						long double lastSpoilingTime = item->LastSpoilingTimeField()();

						/*FString* ItemName = new FString();
						item->GetItemName(ItemName, false, false);
						std::string itemNameStr = ItemName->ToString();
						delete ItemName;*/

						TSubclassOf<APrimalDinoCharacter> dino = item->EggDinoClassToSpawnField()();
						FString* dinoClassName = new FString();
						FName bpDino = dino.uClass->NameField()();
						bpDino.ToString(dinoClassName);
						std::string dinoClassNameStr = dinoClassName->ToString();
						delete dinoClassName;

						char* levelUpPoints = item->EggNumberOfLevelUpPointsAppliedField()();
						int baseLevel = 1;
						for (uint32_t j = 0; j < 12; j++) baseLevel += levelUpPoints[j];

						ss << dinoClassNameStr << " (lvl " << baseLevel;
						int droppedBy = droppedItem->DroppedByPlayerIDField()();
						if (droppedBy > 0)
						{
							FString droppedByName = droppedItem->DroppedByNameField()();
							ss << ", dropped by '" << droppedByName.ToString() << "'";
						}
						ss << "): ";

						int inSeconds = (int)(nextSpoilingTime - (reinterpret_cast<AShooterGameState*>(world->GameStateField()())->NetworkTimeField()()));
						bool destroyedCurrent = false;
						if ((isDestroySpoiled && inSeconds <= 0 && droppedBy == 0) || (isDestroySpoiledInclDroppedByPlayer && inSeconds <= 0) || (isDestroyAll && droppedBy == 0) || isDestroyAllInclDroppedByPlayer)
						{
							droppedItem->Destroy(false, true);
							numDestroyed++;
							destroyedCurrent = true;
						}

						if (inSeconds > 0)
						{
							int days = std::floor(inSeconds / (3600 * 24));
							int hours = std::floor((inSeconds - days * (3600 * 24)) / (3600));
							int minutes = std::floor((inSeconds - days * (3600 * 24) - hours * 3600) / 60);
							int seconds = (inSeconds - days * (3600 * 24) - hours * 3600 - minutes * 60);
							ss << "Spoiling in " << days << "d " << hours << "h " << minutes << "m " << seconds << "s" << (destroyedCurrent ? " [destroyed]" : "") << "\n";
						}
						else ss << "Spoiled" << (destroyedCurrent ? " [destroyed]" : "") << "\n";

						num++;
					}
				}
			}

			if (num == 0) ss2 << "There are no fertilized eggs on the map.";
			else
			{
				ss.seekp(-1, std::ios_base::end);
				ss << "\0";
				ss2 << "Found " << num << " fertilized eggs on the map";
				if (isDestroyAction) ss2 << " (" << numDestroyed << " destroyed)";
				ss2 << "\n" << ss.str();
			}

			delete FoundActors;
		}
		else ss2 << "Command parameter is invalid.";

		return ArkApi::Tools::ConvertToWideStr(ss2.str());
	}

	return {};
}