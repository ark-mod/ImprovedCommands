#include "DoRespecDinoCommand.h"

void DoRespecDinoRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	auto result = DoRespecDinoInternal(&msg);
	if (result.length() == 0) return;

	FString reply(result.c_str());
	reply.AppendChar(L'\n');
	rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);
}

void DoRespecDinoConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	auto result = DoRespecDinoInternal(cmd, aPlayerController);
	if (result.length() == 0) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", result.c_str());
}

std::wstring DoRespecDinoInternal(FString* cmd, APlayerController* aPlayerController)
{
	TArray<FString> Parsed;
	cmd->ParseIntoArray(Parsed, L" ", true);

	int dinoId1;
	int dinoId2;
	APrimalDinoCharacter* dino = nullptr;

	if (Parsed.IsValidIndex(2))
	{
		try
		{
			dinoId1 = std::stoi(*Parsed[1]);
			dinoId2 = std::stoi(*Parsed[2]);
		}
		catch (const std::exception&)
		{
			return L"Invalid arguments";
		}
	}
	else if (aPlayerController)
	{
		auto world = ArkApi::GetApiUtils().GetWorld();
		if (!world) return {};

		auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
		if (!aShooterPlayerController) return {};

		auto character = aShooterPlayerController->CharacterField()();
		if (!character || !character->IsA(APrimalCharacter::GetPrivateStaticClass())) return {};

		auto primalCharacter = static_cast<APrimalCharacter*>(character);
		auto actor = primalCharacter->GetAimedActor(ECollisionChannel::ECC_GameTraceChannel2, 0i64, 0.0, 0.0, 0i64, 0i64, 0, 0);
		if (actor && actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
			dino = static_cast<APrimalDinoCharacter*>(actor);
	}

	if (!dino)
	{
		dino = APrimalDinoCharacter::FindDinoWithID(ArkApi::GetApiUtils().GetWorld(), dinoId1, dinoId2);
		if (!dino) return L"Could not find a dino with supplied id";
	}

	UPrimalCharacterStatusComponent* status = dino->MyCharacterStatusComponentField()();
	auto statusEx = reinterpret_cast<ArkExtensions::UPrimalCharacterStatusComponent*>(status);
	if (status)
	{
		// 0: health
		// 1: stamina
		// 2: torpor
		// 3: oxygen
		// 4: food
		// 5: water
		// 6: temperature
		// 7: weight
		// 8: melee damage
		// 9: movement speed
		// 10: fortitude
		// 11: crafting speed
		
		auto stat_indices = { 0, 1, 3, 4, 7, 8, 9 };

		status->ExtraCharacterLevelField() = 0;

		auto levelUpPointsAppliedTamed = status->NumberOfLevelUpPointsAppliedTamedField()();
		for (auto i : stat_indices) levelUpPointsAppliedTamed[i] = 0;

		status->RescaleAllStats();

		auto maxStatusValues = status->MaxStatusValuesField()();
		auto currentStatusValues = status->CurrentStatusValuesField()();
		for (auto i : stat_indices) currentStatusValues[i] = maxStatusValues[i];

		auto status_component = reinterpret_cast<UActorComponent*>(status);
		auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);

		dino->ForceNetUpdate(false);
		dino->BPNotifyLevelUp(0);

		auto func_ClientNotifyLevelUp = dino->FindFunctionChecked(FName(L"ClientNotifyLevelUp", EFindName::FNAME_Find, true));
		if (func_ClientNotifyLevelUp) dino->ProcessEvent(func_ClientNotifyLevelUp, nullptr);

		auto func_ClientNotifyLevelUpPC = aShooterPlayerController->FindFunctionChecked(FName(L"ClientNotifyLevelUp", EFindName::FNAME_Find, true));
		if (func_ClientNotifyLevelUpPC) aShooterPlayerController->ProcessEvent(func_ClientNotifyLevelUpPC, &TArray<unsigned short>());

		status->CharacterUpdatedInventory(true);
		status->ServerSyncReplicatedValues();
		
		auto baseLevelMaxStatusValues = status->BaseLevelMaxStatusValuesField()();
		auto func_NetSyncMaxStatusValues = status_component->FindFunctionChecked(FName(L"NetSyncMaxStatusValues", EFindName::FNAME_Find, true));
		if (func_NetSyncMaxStatusValues)
		{
			TArray<float> aMaxStatusValues, aBaseLevelMaxStatusValues;
			for (auto i = 0; i < 12; i++)
			{
				aMaxStatusValues.Add(maxStatusValues[i]);
				aBaseLevelMaxStatusValues.Add(baseLevelMaxStatusValues[i]);
			}
			TArray<float> Args[] = { aMaxStatusValues, aBaseLevelMaxStatusValues };
			status_component->ProcessEvent(func_NetSyncMaxStatusValues, Args);
		}

		status->UpdateWeightStat(true);
		dino->ForceNetUpdate(false);

		return L"Successfully triggered dino respec";
	}

	return {};
}