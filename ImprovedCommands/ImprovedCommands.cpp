#define _USE_MATH_DEFINES

#include <windows.h>
#include <iostream>
#include <sstream>
#include <cmath>
#include <list>
#include <map>
#include "API/Base.h"
#include "API/UE.h"
#include "Tools.h"
#include "ImprovedCommands.h"

#pragma comment(lib, "ArkApi.lib")

typedef void(*callback_function)(void);

void DoRespec(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	TArray<FString> Parsed;
	msg.ParseIntoArray(&Parsed, L" ", true);

	if (Parsed.IsValidIndex(1))
	{
		unsigned __int64 steamId;

		try
		{
			steamId = std::stoull(*Parsed[1]);
		}
		catch (const std::exception&)
		{
			return;
		}

		AShooterPlayerController* aShooterPC = FindPlayerControllerFromSteamId(steamId);
		if (aShooterPC)
		{
			AShooterPlayerState* playerState = static_cast<AShooterPlayerState*>(aShooterPC->GetPlayerStateField());
			if (playerState) {
				playerState->DoRespec();
				// Send a reply
				FString reply = L"Successfully triggered player respec\n";
				rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);
			}
		}
	}
}

//Note: This command does not check tribe permissions
void DestroyMyDino(AShooterPlayerController* aShooterPlayerController, FString* message, int mode)
{
	UWorld* world = Ark::GetWorld();
	if (!world) return;
	if (!aShooterPlayerController) return;

	ACharacter* character = aShooterPlayerController->GetCharacterField();
	if (!character || !character->IsA(APrimalCharacter::GetPrivateStaticClass())) return;

	APrimalCharacter* primalCharacter = static_cast<APrimalCharacter*>(character);
	AActor* actor = primalCharacter->GetAimedActor(ECC_GameTraceChannel2, 0i64, 0.0, 0.0, 0i64, 0i64, 0, 0);
	if (!actor || !actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass())) return;

	APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);
	int dinoTeam = dino->GetTargetingTeamField();
	int playerTeam = aShooterPlayerController->GetTargetingTeamField();
	if (dinoTeam != playerTeam) return;

	FString* className = new FString();
	dino->GetDinoNameTagField().ToString(className); //species name
	std::string name = dino->GetTamedNameField().ToString(); //tamed name
	std::string classNameStr = className->ToString();

	dino->Destroy(false, true);

	std::stringstream ss;

	if (name.length() > 0) ss << "Destroyed creature '" << name << "' (" << classNameStr << ")";
	else ss << "Destroyed targeted creature (" << classNameStr << ")";

	wchar_t* wcstring = ConvertToWideStr(ss.str());
	SendChatMessage(aShooterPlayerController, L"[system]", wcstring);

	delete className;
	delete[] wcstring;
}

void DestroyAllStructuresForTeamId(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	TArray<FString> Parsed;
	msg.ParseIntoArray(&Parsed, L" ", true);

	if (Parsed.IsValidIndex(1))
	{
		__int64 teamId;

		try
		{
			teamId = std::stoull(*Parsed[1]);
		}
		catch (const std::exception&)
		{
			return;
		}

		if (teamId < 50000) return;

		UWorld* world = Ark::GetWorld();
		if (!world) return;

		TArray<AActor*>* FoundActors = new TArray<AActor*>();

		UGameplayStatics::GetAllActorsOfClass(world, APrimalStructure::GetPrivateStaticClass(), FoundActors);

		std::stringstream ss;

		//todo: maybe destroy rafts aswell?
		int num = 0;
		for (uint32_t i = 0; i < FoundActors->Num(); i++)
		{
			AActor* actor = (*FoundActors)[i];

			APrimalStructure* structure = static_cast<APrimalStructure*>(actor);

			int structureTeam = structure->GetTargetingTeamField();
			if (structureTeam == teamId)
			{
				structure->Destroy(false, true);
				num++;
			}
		}

		ss << "Destroyed " << num << " structures belonging to team " << teamId << "\n";

		wchar_t* wcstring = ConvertToWideStr(ss.str());

		FString reply(wcstring);
		rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);

		delete FoundActors;
		delete[] wcstring;
	}
}

void DestroyStructuresForTeamIdAtPosition(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	TArray<FString> Parsed;
	msg.ParseIntoArray(&Parsed, L" ", true);

	//if (Parsed.IsValidIndex(5))
	if (Parsed.IsValidIndex(5))
	{
		__int64 teamId;
		float x;
		float y;
		//float z;
		float radius;
		bool rafts;

		try
		{
			teamId = std::stoull(*Parsed[1]);
			x = std::stof(*Parsed[2]);
			y = std::stof(*Parsed[3]);
			radius = std::stof(*Parsed[4]);
			rafts = (bool)std::stoi(*Parsed[5]);
			//z = std::stof(*Parsed[4]);
			//radius = std::stof(*Parsed[5]);
		}
		catch (const std::exception&)
		{
			return;
		}

		if (teamId < 50000) return;
		if (radius <= 0) return;

		UWorld* world = Ark::GetWorld();
		if (!world) return;

		std::stringstream ss;
		int num = 0;
		FVector* pos = new FVector();

		TArray<AActor*>* FoundActors = new TArray<AActor*>();
		UGameplayStatics::GetAllActorsOfClass(world, APrimalStructure::GetPrivateStaticClass(), FoundActors);

		for (uint32_t i = 0; i < FoundActors->Num(); i++)
		{
			AActor* actor = (*FoundActors)[i];

			APrimalStructure* structure = static_cast<APrimalStructure*>(actor);

			int structureTeam = structure->GetTargetingTeamField();
			structure->GetRootComponentField()->GetCustomLocation(pos);
			//if (structureTeam == teamId && IsPointInsideSphere(pos, x, y, z, radius))
			if (structureTeam == teamId && IsPointInside2dCircle(*pos, x, y, radius))
			{
				structure->Destroy(false, true);
				num++;
			}
		}
		delete FoundActors;

		if (rafts) {
			TArray<AActor*>* FoundActors = new TArray<AActor*>();
			UGameplayStatics::GetAllActorsOfClass(world, APrimalDinoCharacter::GetPrivateStaticClass(), FoundActors);

			for (uint32_t i = 0; i < FoundActors->Num(); i++)
			{
				AActor* actor = (*FoundActors)[i];

				APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);

				int dinoTeam = dino->GetTargetingTeamField();
				
				dino->GetRootComponentField()->GetCustomLocation(pos);
				//if (structureTeam == teamId && IsPointInsideSphere(pos, x, y, z, radius))
				if (dinoTeam == teamId && IsPointInside2dCircle(*pos, x, y, radius))
				{
					FString* className = new FString();
					FName bp = actor->GetClassField()->GetNameField();
					bp.ToString(className);
					std::string classNameStr = className->ToString();

					if (classNameStr.compare("Raft_BP_C") == 0 || classNameStr.compare("MotorRaft_BP_C") == 0) {
						dino->Destroy(false, true);
						num++;
					}

					delete className;
				}
			}
			delete FoundActors;
		}

		//ss << "Destroyed " << num << " structures belonging to team " << teamId << " at position (x: " << x << ", y: " << y << ", z: " << z << ", r: " << radius << ")" << "\n";
		ss << "Destroyed " << num << " structures belonging to team " << teamId << " at position (x: " << x << ", y: " << y << ", r: " << radius << ", rafts: " << rafts << ")" << "\n";

		wchar_t* wcstring = ConvertToWideStr(ss.str());

		FString reply(wcstring);
		rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);

		delete pos;
		delete[] wcstring;
	}
}

void DestroyDinosForTeamId(RCONClientConnection* rconClientConnection, RCONPacket* rconPacket, UWorld* uWorld)
{
	FString msg = rconPacket->Body;

	TArray<FString> Parsed;
	msg.ParseIntoArray(&Parsed, L" ", true);

	if (Parsed.IsValidIndex(1))
	{
		__int64 teamId;

		try
		{
			teamId = std::stoull(*Parsed[1]);
		}
		catch (const std::exception&)
		{
			return;
		}

		if (teamId < 50000) return;

		UWorld* world = Ark::GetWorld();
		if (!world) return;

		TArray<AActor*>* FoundActors = new TArray<AActor*>();
		UGameplayStatics::GetAllActorsOfClass(world, APrimalDinoCharacter::GetPrivateStaticClass(), FoundActors);

		std::stringstream ss;

		int num = 0;
		for (uint32_t i = 0; i < FoundActors->Num(); i++)
		{
			AActor* actor = (*FoundActors)[i];

			APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);

			int dinoTeam = dino->GetTargetingTeamField();
			if (dinoTeam == teamId)
			{
				dino->Destroy(false, true);
				num++;
			}
		}

		ss << "Destroyed " << num << " dinos belonging to team " << teamId << "\n";

		wchar_t* wcstring = ConvertToWideStr(ss.str());

		FString reply(wcstring);
		rconClientConnection->SendMessageW(rconPacket->Id, 0, &reply);

		delete FoundActors;
		delete[] wcstring;
	}
}

void MyDinoStats(AShooterPlayerController* aShooterPlayerController, FString* message, int mode)
{
	UWorld* world = Ark::GetWorld();
	if (!world) return;
	if (!aShooterPlayerController) return;

	ACharacter* character = aShooterPlayerController->GetCharacterField();
	if (!character || !character->IsA(APrimalCharacter::GetPrivateStaticClass())) return;

	APrimalCharacter* primalCharacter = static_cast<APrimalCharacter*>(character);
	AActor* actor = primalCharacter->GetAimedActor(ECC_GameTraceChannel2, 0i64, 0.0, 0.0, 0i64, 0i64, 0, 0);
	if (!actor || !actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass())) return;

	APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(actor);
	int dinoTeam = dino->GetTargetingTeamField();
	int playerTeam = aShooterPlayerController->GetTargetingTeamField();
	if (dinoTeam != playerTeam) return;

	std::stringstream ss;

	UPrimalCharacterStatusComponent* status = dino->GetCharacterStatusComponent();
	if (status) {
		char* stats = status->GetNumberOfLevelUpPointsAppliedField();
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

		ss << "Stats: " << health << ", " << stamina << ", " << oxygen << ", " << food << ", " << weight << ", " << meleeDamage << ", " << movementSpeed << "";
	}

	wchar_t* wcstring = ConvertToWideStr(ss.str());
	SendChatMessage(aShooterPlayerController, L"[system]", wcstring);

	delete[] wcstring;
}

void Init()
{
	//chat (for all players)
	Ark::AddChatCommand(L"/mydinostats", &MyDinoStats);
	Ark::AddChatCommand(L"/destroymydino", &DestroyMyDino);

	//rcon
	Ark::AddRconCommand(L"DoRespec", &DoRespec);
	Ark::AddRconCommand(L"DestroyAllStructuresForTeamId", &DestroyAllStructuresForTeamId);
	Ark::AddRconCommand(L"DestroyStructuresForTeamIdAtPosition", &DestroyStructuresForTeamIdAtPosition);
	Ark::AddRconCommand(L"DestroyDinosForTeamId", &DestroyDinosForTeamId);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
