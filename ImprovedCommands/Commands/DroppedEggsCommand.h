#pragma once
#include "../Utils.h"

DECLARE_COMMAND(DroppedEggs_Rcon);
DECLARE_COMMAND(DroppedEggs_Console);

void DroppedEggsRconCommand(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld);
void DroppedEggsConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog);
std::wstring DroppedEggsInternal(FString* msg, ArkLibrary::CommandDefinition *cmd);