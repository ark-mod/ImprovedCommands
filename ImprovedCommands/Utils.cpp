#include "Utils.h"

bool CheckPermission(const uint64 &steam_id, const FString &commandName, AShooterPlayerController *aShooterPlayerController)
{
	if (!Permissions::IsPlayerHasPermission(steam_id, commandName))
	{
		ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", "<RichColor Color=\"1, 0, 0, 1\">You don't have permissions to use this command</>");
		return false;
	}

	return true;
}