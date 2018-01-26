#include "DestroyMyDinoCommand.h"
#include <sstream>

// Note: This command does not check tribe permissions
void DestroyMyDinoChatCommand(AShooterPlayerController* aShooterPlayerController, FString* message, EChatSendMode::Type mode)
{
	auto cmd = ArkLibrary::GetCommand(CommandName_DestroyMyDino_Chat);
	const uint64 steam_id = ArkApi::IApiUtils::GetSteamIdFromController(aShooterPlayerController);
	if (cmd && cmd->Permissions == true && !CheckPermission(steam_id, "ImprovedCommands.DestroyMyDino", aShooterPlayerController)) return;

	bool methodSuicide = cmd && cmd->Json.value("Method", "destroy").compare("suicide") == 0;

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

	FString* className = new FString();
	dino->DinoNameTagField()().ToString(className); //species name
	std::string name = dino->TamedNameField()().ToString(); //tamed name
	std::string classNameStr = className->ToString();

	if (methodSuicide) dino->Suicide();
	else dino->Destroy(false, true);

	std::stringstream ss;

	if (name.length() > 0) ss << "Destroyed creature '" << name << "' (" << classNameStr << ")";
	else ss << "Destroyed targeted creature (" << classNameStr << ")";

	auto wcstring = ArkApi::Tools::ConvertToWideStr(ss.str());
	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", wcstring.c_str());

	delete className;
}