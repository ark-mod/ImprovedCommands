#pragma once
#include "../Utils.h"

DECLARE_COMMAND(DestroyStructuresForTeamIdAtPosition_Rcon);
DECLARE_COMMAND(DestroyStructuresForTeamIdAtPosition_Console);

void DestroyStructuresForTeamIdAtPositionRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld);
void DestroyStructuresForTeamIdAtPositionConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog);
std::wstring DestroyStructuresForTeamIdAtPositionInternal(FString* cmd);