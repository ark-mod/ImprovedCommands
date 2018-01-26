#pragma once
#include "../Utils.h"

DECLARE_COMMAND(DestroyDinosForTeamId_Rcon);
DECLARE_COMMAND(DestroyDinosForTeamId_Console);

void DestroyDinosForTeamIdRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld);
void DestroyDinosForTeamIdConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog);
std::wstring DestroyDinosForTeamIdInternal(FString* cmd);