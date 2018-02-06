#pragma once
#include "../Utils.h"

DECLARE_COMMAND(DoRespecDino_Rcon);
DECLARE_COMMAND(DoRespecDino_Console);

void DoRespecDinoRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld);
void DoRespecDinoConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog);
std::wstring DoRespecDinoInternal(FString* cmd, APlayerController* aPlayerController = nullptr);