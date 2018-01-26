#include "DestroyStructuresForTeamIdAtPositionCommand.h"
#include <sstream>

void DestroyStructuresForTeamIdAtPositionRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	auto result = DestroyStructuresForTeamIdAtPositionInternal(&msg);
	if (result.length() == 0) return;

	FString reply(result.c_str());
	reply.AppendChar(L'\n');
	rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);
}

void DestroyStructuresForTeamIdAtPositionConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	auto result = DestroyStructuresForTeamIdAtPositionInternal(cmd);
	if (result.length() == 0) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", result.c_str());
}

std::wstring DestroyStructuresForTeamIdAtPositionInternal(FString* cmd)
{
	TArray<FString> Parsed;
	cmd->ParseIntoArray(Parsed, L" ", true);

	if (Parsed.IsValidIndex(5))
	{
		__int64 teamId;
		float x;
		float y;
		//float z;
		float radius;
		bool rafts;

		try
		{
			teamId = std::stoull(*Parsed[1]);
			x = std::stof(*Parsed[2]);
			y = std::stof(*Parsed[3]);
			radius = std::stof(*Parsed[4]);
			rafts = (bool)std::stoi(*Parsed[5]);
			//z = std::stof(*Parsed[4]);
		}
		catch (const std::exception&)
		{
			return {};
		}

		if (teamId < 50000) return {};
		if (radius <= 0) return {};

		UWorld* world = ArkApi::GetApiUtils().GetWorld();
		if (!world) return {};

		std::stringstream ss;
		int num = 0;
		FVector* pos = new FVector();

		TArray<AActor*>* FoundActors = new TArray<AActor*>();
		UGameplayStatics::GetAllActorsOfClass(world, APrimalStructure::GetPrivateStaticClass(), FoundActors);

		for (uint32_t i = 0; i < FoundActors->Num(); i++)
		{
			AActor* actor = (*FoundActors)[i];

			APrimalStructure* structure = static_cast<APrimalStructure*>(actor);

			int structureTeam = structure->TargetingTeamField()();
			structure->RootComponentField()()->GetCustomLocation(pos);
			//if (structureTeam == teamId && ArkLibrary::IsPointInsideSphere(pos, x, y, z, radius))
			if (structureTeam == teamId && ArkLibrary::IsPointInside2dCircle(*pos, x, y, radius))
			{
				structure->Destroy(false, true);
				num++;
			}
		}
		delete FoundActors;

		if (rafts) {
			TArray<AActor*>* FoundActors = new TArray<AActor*>();
			UGameplayStatics::GetAllActorsOfClass(world, APrimalDinoCharacter::GetPrivateStaticClass(), FoundActors);

			for (uint32_t i = 0; i < FoundActors->Num(); i++)
			{
				AActor* actor = (*FoundActors)[i];

				APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);

				int dinoTeam = dino->TargetingTeamField()();

				dino->RootComponentField()()->GetCustomLocation(pos);
				//if (structureTeam == teamId && ArkLibrary::IsPointInsideSphere(pos, x, y, z, radius))
				if (dinoTeam == teamId && ArkLibrary::IsPointInside2dCircle(*pos, x, y, radius))
				{
					FString* className = new FString();
					FName bp = actor->ClassField()()->NameField()();
					bp.ToString(className);
					std::string classNameStr = className->ToString();

					if (classNameStr.compare("Raft_BP_C") == 0 || classNameStr.compare("MotorRaft_BP_C") == 0) {
						dino->Destroy(false, true);
						num++;
					}

					delete className;
				}
			}
			delete FoundActors;
		}

		//ss << "Destroyed " << num << " structures belonging to team " << teamId << " at position (x: " << x << ", y: " << y << ", z: " << z << ", r: " << radius << ")";
		ss << "Destroyed " << num << " structures belonging to team " << teamId << " at position (x: " << x << ", y: " << y << ", r: " << radius << ", rafts: " << rafts << ")";

		delete pos;
		return ArkApi::Tools::ConvertToWideStr(ss.str());
	}

	return {};
}