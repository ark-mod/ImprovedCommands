#include "DestroyDinosForTeamIdCommand.h"
#include <sstream>

void DestroyDinosForTeamIdRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	auto result = DestroyDinosForTeamIdInternal(&msg);
	if (result.length() == 0) return;

	FString reply(result.c_str());
	reply.AppendChar(L'\n');
	rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);
}

void DestroyDinosForTeamIdConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	auto result = DestroyDinosForTeamIdInternal(cmd);
	if (result.length() == 0) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", result.c_str());
}

std::wstring DestroyDinosForTeamIdInternal(FString* cmd)
{
	TArray<FString> Parsed;
	cmd->ParseIntoArray(Parsed, L" ", true);

	if (Parsed.IsValidIndex(1))
	{
		__int64 teamId;

		try
		{
			teamId = std::stoull(*Parsed[1]);
		}
		catch (const std::exception&)
		{
			return {};
		}

		if (teamId < 50000) return {};

		UWorld* world = ArkApi::GetApiUtils().GetWorld();
		if (!world) return {};

		TArray<AActor*>* FoundActors = new TArray<AActor*>();
		UGameplayStatics::GetAllActorsOfClass(world, APrimalDinoCharacter::GetPrivateStaticClass(), FoundActors);

		std::stringstream ss;

		int num = 0;
		for (uint32_t i = 0; i < FoundActors->Num(); i++)
		{
			AActor* actor = (*FoundActors)[i];

			APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);

			int dinoTeam = dino->TargetingTeamField()();
			if (dinoTeam == teamId)
			{
				dino->Destroy(false, true);
				num++;
			}
		}

		ss << "Destroyed " << num << " dinos belonging to team " << teamId;

		delete FoundActors;
		return ArkApi::Tools::ConvertToWideStr(ss.str());
	}

	return {};
}