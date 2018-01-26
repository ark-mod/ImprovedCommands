#pragma once
#include "../Utils.h"

DECLARE_COMMAND(Suicide_Chat);

void SuicideChatCommand(AShooterPlayerController* aShooterPlayerController, FString* message, EChatSendMode::Type mode);