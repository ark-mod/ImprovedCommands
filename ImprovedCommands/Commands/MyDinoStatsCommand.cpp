#include "MyDinoStatsCommand.h"
#include <sstream>

const std::map<EPrimalCharacterStatusValue::Type, std::string> stat_fields =
{
	{ EPrimalCharacterStatusValue::Health, "HP" },
	{ EPrimalCharacterStatusValue::Stamina, "ST" },
	{ EPrimalCharacterStatusValue::Oxygen, "OX" },
	{ EPrimalCharacterStatusValue::Food, "FO" },
	{ EPrimalCharacterStatusValue::Weight, "WE" },
	{ EPrimalCharacterStatusValue::MeleeDamageMultiplier, "ME" },
	{ EPrimalCharacterStatusValue::SpeedMultiplier, "MO" }
};

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
	if (status)
	{
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

		ss << "HP: " << health << ", ST: " << stamina << ", OX: " << oxygen 
			<< ", FO: " << food << ", WE: " << weight << ", ME: " << meleeDamage 
			<< ", MO: " << movementSpeed;

		ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", ArkApi::Tools::ConvertToWideStr(ss.str()).c_str());
		ss.str("");

		auto base_stats_json = cmd->Json.find("BaseStats");
		if (base_stats_json != cmd->Json.end() && base_stats_json->value("Show", false))
		{
			auto imprint_calc_str = ArkLibrary::str_tolower(base_stats_json->value("ImprintCalc", "current"));
			auto te_calc_str = ArkLibrary::str_tolower(base_stats_json->value("TamingEfficiencyCalc", "current"));

			auto imprint_calc = ImprintCalc::Current;
			if (imprint_calc_str.compare("none") == 0) imprint_calc = ImprintCalc::None;
			else if (imprint_calc_str.compare("best") == 0) imprint_calc = ImprintCalc::Best;

			auto te_calc = TamingEffCalc::Current;
			if (te_calc_str.compare("best") == 0) te_calc = TamingEffCalc::Best;

			auto temp_name = FString(L"Temp");
			auto comp_temp = dino->CreateComponentFromTemplate(reinterpret_cast<UActorComponent *>(status), &temp_name);
			auto status_temp = reinterpret_cast<UPrimalCharacterStatusComponent *>(comp_temp);

			comp_temp->OuterField() = reinterpret_cast<UActorComponent *>(status)->OuterField()();
			auto base_levels = status_temp->NumberOfLevelUpPointsAppliedField()();

			if (imprint_calc == ImprintCalc::Current) status_temp->DinoImprintingQualityField() = status->DinoImprintingQualityField()();
			else if (imprint_calc == ImprintCalc::Best) status_temp->DinoImprintingQualityField() = 1.0;

			if (te_calc == TamingEffCalc::Current) status_temp->TamingIneffectivenessMultiplierField() = status->TamingIneffectivenessMultiplierField()();
			
			status_temp->ExtraCharacterLevelField() = 0;
			for (auto i = 0; i < 12; i++) base_levels[i] = stats[i];

			status_temp->RescaleAllStats();

			ss << std::endl;
			auto num = 0;
			for (auto sf : stat_fields)
			{
				FString str;
				status_temp->GetStatusMaxValueString(&str, sf.first, true);

				if (num > 0) ss << ", ";
				ss << sf.second << ": " << str.ToString();

				num++;
			}

			comp_temp->OuterField() = nullptr;

			ArkApi::GetApiUtils().SendChatMessage(aShooterPlayerController, L"[system]", ArkApi::Tools::ConvertToWideStr(ss.str()).c_str());
		}
	}
}