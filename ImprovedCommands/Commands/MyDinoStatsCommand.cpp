#include "MyDinoStatsCommand.h"
#include <sstream>

void MyDinoStatsChatCommand(AShooterPlayerController* aShooterPlayerController, FString* message, EChatSendMode::Type mode)
{
	auto cmd = ArkLibrary::GetCommand(CommandName_MyDinoStats_Chat);
	const uint64 steam_id = ArkApi::IApiUtils::GetSteamIdFromController(aShooterPlayerController);
	if (cmd && cmd->Permissions == true && !CheckPermission(steam_id, "ImprovedCommands.MyDinoStats", aShooterPlayerController)) return;

	UWorld* world = ArkApi::GetApiUtils().GetWorld();
	if (!world) return;
	if (!aShooterPlayerController) return;

	ACharacter* character = aShooterPlayerController->CharacterField()();
	if (!character || !character->IsA(APrimalCharacter::GetPrivateStaticClass())) return;

	APrimalCharacter* primalCharacter = static_cast<APrimalCharacter*>(character);
	AActor* actor = primalCharacter->GetAimedActor(ECollisionChannel::ECC_GameTraceChannel2, 0i64, 0.0, 0.0, 0i64, 0i64, 0, 0);
	if (!actor || !actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass())) return;

	APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);
	int dinoTeam = dino->TargetingTeamField()();
	int playerTeam = aShooterPlayerController->TargetingTeamField()();
	if (dinoTeam != playerTeam) return;

	std::stringstream ss;

	UPrimalCharacterStatusComponent* status = dino->GetCharacterStatusComponent();
	if (status) {
		char* stats = status->NumberOfLevelUpPointsAppliedField()();
		int health = (int)stats[0];
		int stamina = (int)stats[1];
		int torpor = (int)stats[2];
		int oxygen = (int)stats[3];
		int food = (int)stats[4];
		int water = (int)stats[5];
		int temperature = (int)stats[6];
		int weight = (int)stats[7];
		int meleeDamage = (int)stats[8];
		int movementSpeed = (int)stats[9];
		int fortitude = (int)stats[10];
		int craftingSpeed = (int)stats[11];

		ss << "HP: " << health << ", ST: " << stamina << ", OX: " << oxygen << ", FO: " << food << ", WE: " << weight << ", ME: " << meleeDamage << ", MO: " << movementSpeed;
	}

	auto wcstring = ArkApi::Tools::ConvertToWideStr(ss.str());
	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", wcstring.c_str());
}