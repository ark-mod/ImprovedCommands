#pragma once
#include "API/Ark/Ark.h"
#include <ArkPluginLibrary/ArkPluginLibrary.h>
#include <Permissions/Permissions.h>

bool CheckPermission(const uint64 &steam_id, const FString &commandName, AShooterPlayerController *aShooterPlayerController);