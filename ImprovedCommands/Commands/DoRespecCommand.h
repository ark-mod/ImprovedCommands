#pragma once
#include "../Utils.h"

DECLARE_COMMAND(DoRespec_Rcon);
DECLARE_COMMAND(DoRespec_Console);

void DoRespecRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld);
void DoRespecConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog);
std::wstring DoRespecInternal(FString* cmd);