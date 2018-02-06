#pragma once
#include "../Utils.h"

DECLARE_COMMAND(MyDinoStats_Chat);

enum class ImprintCalc { None, Current, Best };
enum class TamingEffCalc { Current, Best };

void MyDinoStatsChatCommand(AShooterPlayerController* aShooterPlayerController, FString* message, EChatSendMode::Type mode);