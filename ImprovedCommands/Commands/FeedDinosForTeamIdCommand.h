#pragma once
#include "../Utils.h"

DECLARE_COMMAND(FeedDinosForTeamId_Rcon);
DECLARE_COMMAND(FeedDinosForTeamId_Console);

void FeedDinosForTeamIdRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld);
void FeedDinosForTeamIdConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog);
std::wstring FeedDinosForTeamIdInternal(FString* cmd);