#include "SuicideCommand.h"

void SuicideChatCommand(AShooterPlayerController* aShooterPlayerController, FString* message, EChatSendMode::Type mode)
{
	auto cmd = ArkLibrary::GetCommand(CommandName_Suicide_Chat);
	const uint64 steam_id = ArkApi::IApiUtils::GetSteamIdFromController(aShooterPlayerController);
	if (cmd && cmd->Permissions == true && !CheckPermission(steam_id, "ImprovedCommands.Suicide", aShooterPlayerController)) return;

	aShooterPlayerController->GetPlayerCharacter()->Suicide();

	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", L"You killed yourself!");
}