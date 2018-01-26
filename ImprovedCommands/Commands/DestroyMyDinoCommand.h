#pragma once
#include "../Utils.h"

DECLARE_COMMAND(DestroyMyDino_Chat);

void DestroyMyDinoChatCommand(AShooterPlayerController* aShooterPlayerController, FString* message, EChatSendMode::Type mode);