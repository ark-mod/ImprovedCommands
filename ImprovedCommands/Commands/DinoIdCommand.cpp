#include "DinoIdCommand.h"

void DinoIdConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	UWorld* world = ArkApi::GetApiUtils().GetWorld();
	if (!world) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	if (!aShooterPlayerController) return;

	ACharacter* character = aShooterPlayerController->CharacterField()();
	if (!character || !character->IsA(APrimalCharacter::GetPrivateStaticClass())) return;

	std::wstringstream ss;

	APrimalCharacter* primalCharacter = static_cast<APrimalCharacter*>(character);
	AActor* actor = primalCharacter->GetAimedActor(ECollisionChannel::ECC_GameTraceChannel2, 0i64, 0.0, 0.0, 0i64, 0i64, 0, 0);
	if (actor && actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
	{
		APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);

		ss << "ID1=" << dino->DinoID1Field()() << ", ID2=" << dino->DinoID2Field()();
	}
	else ss << "Target is not a creature.";

	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", ss.str().c_str());
}