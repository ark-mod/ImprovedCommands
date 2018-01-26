#include "NextImprintCommand.h"
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>

void NextImprintConsoleCommand(APlayerController* aPlayerController, FString* cmd, bool bWriteToLog)
{
	TArray<FString> Parsed;
	cmd->ParseIntoArray(Parsed, L" ", true);

	bool isReroll = false;
	std::string rerollTypeStr;
	EBabyCuddleType::Type rerollType = EBabyCuddleType::Type::MAX;

	if (Parsed.IsValidIndex(1))
	{
		std::string action = Parsed[1].ToString();
		std::transform(action.begin(), action.end(), action.begin(), tolower);

		isReroll = action.compare("reroll") == 0;
	}

	try
	{
		if (Parsed.IsValidIndex(2) && isReroll)
		{
			rerollTypeStr = Parsed[2].ToString();
			std::transform(rerollTypeStr.begin(), rerollTypeStr.end(), rerollTypeStr.begin(), tolower);
			if (rerollTypeStr.compare("food") == 0) rerollType = EBabyCuddleType::Type::Food;
			else if (rerollTypeStr.compare("walk") == 0) rerollType = EBabyCuddleType::Type::Walk;
			else if (rerollTypeStr.compare("pet") == 0) rerollType = EBabyCuddleType::Type::Pet;
			else rerollTypeStr = "";
		}
	}
	catch (const std::exception&)
	{
		return;
	}

	UWorld* world = ArkApi::GetApiUtils().GetWorld();
	if (!world) return;

	auto aShooterPlayerController = static_cast<AShooterPlayerController*>(aPlayerController);
	if (!aShooterPlayerController) return;

	ACharacter* character = aShooterPlayerController->CharacterField()();
	if (!character || !character->IsA(APrimalCharacter::GetPrivateStaticClass())) return;

	std::wstringstream ss;

	APrimalCharacter* primalCharacter = static_cast<APrimalCharacter*>(character);
	AActor* actor = primalCharacter->GetAimedActor(ECollisionChannel::ECC_GameTraceChannel2, 0i64, 0.0, 0.0, 0i64, 0i64, 0, 0);
	if (actor && actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass())) {
		APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);

		float babyAge = dino->BabyAgeField()();
		if (babyAge < 1.0f)
		{
			long double time = world->TimeSecondsField()();
			long double prevTime = dino->BabyNextCuddleTimeField()();
			if (isReroll)
			{
				auto cuddleFood = new TSubclassOf<UPrimalItem>();

				if (rerollType == EBabyCuddleType::Type::MAX)
				{
					std::default_random_engine generator;
					generator.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
					std::uniform_int_distribution<int> distribution(EBabyCuddleType::Type::Pet, EBabyCuddleType::Type::MAX - 1);
					rerollType = static_cast<EBabyCuddleType::Type>(distribution(generator));
				}

				dino->GetBabyCuddleFood(cuddleFood);
				EBabyCuddleType::Type cuddleType = rerollType;
				dino->UpdateBabyCuddling(time, cuddleType, *cuddleFood);
				ss << "Unlocked next imprint (rerolled";
				if (rerollTypeStr.length() > 0) ss << " with type " << ArkLibrary::s2ws(rerollTypeStr);
				ss << ").";

				delete cuddleFood;
			}
			else if (time < prevTime)
			{
				TSubclassOf<UPrimalItem> cuddleFood = dino->BabyCuddleFoodField()();
				EBabyCuddleType::Type cuddleType = dino->BabyCuddleTypeField()();
				dino->UpdateBabyCuddling(time, cuddleType, cuddleFood);
				ss << "Unlocked next imprint.";
			}
			else ss << "Cuddle is already available for target creature.";
		}
		else ss << "Target is not a baby creature";
	}
	else ss << "Target is not a creature.";

	ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", ss.str().c_str());
}