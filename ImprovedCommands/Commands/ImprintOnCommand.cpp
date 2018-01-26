#include "ImprintOnCommand.h"

void ImprintOnConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	TArray<FString> Parsed;
	cmd->ParseIntoArray(Parsed, L" ", true);

	unsigned __int64 steamId = 0;

	if (Parsed.IsValidIndex(1))
	{
		try
		{
			steamId = std::stoull(*Parsed[1]);
		}
		catch (const std::exception&)
		{
			return;
		}
	}

	UWorld* world = ArkApi::GetApiUtils().GetWorld();
	if (!world) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	if (!aShooterPlayerController) return;

	ACharacter* character = aShooterPlayerController->CharacterField()();
	if (!character || !character->IsA(APrimalCharacter::GetPrivateStaticClass())) return;

	std::wstringstream ss;

	APrimalCharacter* primalCharacter = static_cast<APrimalCharacter*>(character);
	AActor* actor = primalCharacter->GetAimedActor(ECollisionChannel::ECC_GameTraceChannel2, 0i64, 0.0, 0.0, 0i64, 0i64, 0, 0);
	if (actor && actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass())) {
		APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);

		auto aShooterPC = steamId > 0 ? ArkApi::GetApiUtils().FindPlayerFromSteamId(steamId) : aShooterPlayerController;
		if (aShooterPC)
		{
			long long playerId = aShooterPC->LinkedPlayerIDField()();
			FString* playerName = new FString();
			aShooterPC->GetPlayerCharacterName(playerName);

			dino->UpdateImprintingDetails(playerName, playerId);
			delete playerName;
			if (steamId > 0) ss << "Target creature is now imprinted on " << steamId << ".";
			else ss << "Target creature is now imprinted on you.";
		}
		else ss << "Could not find player.";
	}
	else ss << "Target is not a creature.";

	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", ss.str().c_str());
}