#include "DoRespecCommand.h"

void DoRespecRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	auto result = DoRespecInternal(&msg);
	if (result.length() == 0) return;

	FString reply(result.c_str());
	reply.AppendChar(L'\n');
	rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);
}

void DoRespecConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	auto result = DoRespecInternal(cmd);
	if (result.length() == 0) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", result.c_str());
}

std::wstring DoRespecInternal(FString* cmd)
{
	TArray<FString> Parsed;
	cmd->ParseIntoArray(Parsed, L" ", true);

	if (Parsed.IsValidIndex(1))
	{
		unsigned __int64 steamId;

		try
		{
			steamId = std::stoull(*Parsed[1]);
		}
		catch (const std::exception&)
		{
			return {};
		}

		auto aShooterPC = ArkApi::GetApiUtils().FindPlayerFromSteamId(steamId);
		if (aShooterPC)
		{
			auto playerState = static_cast<AShooterPlayerState*>(aShooterPC->PlayerStateField()());
			if (playerState)
			{
				//todo: this works - but how does it compare to the way the game does it?
				playerState->DoRespec(nullptr, nullptr, false);

				return L"Successfully triggered player respec";
			}
		}
	}

	return {};
}