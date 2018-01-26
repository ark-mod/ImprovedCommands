#pragma once
#include "../Utils.h"

DECLARE_COMMAND(DestroyAllStructuresForTeamId_Rcon);
DECLARE_COMMAND(DestroyAllStructuresForTeamId_Console);

void DestroyAllStructuresForTeamIdRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld);
void DestroyAllStructuresForTeamIdConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog);
std::wstring DestroyAllStructuresForTeamIdInternal(FString* cmd);