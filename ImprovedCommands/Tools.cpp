#include "Tools.h"

AShooterPlayerController* FindPlayerControllerFromSteamId(unsigned __int64 steamId)
{
	AShooterPlayerController* result = nullptr;

	auto playerControllers = Ark::GetWorld()->GetPlayerControllerListField();
	for (uint32_t i = 0; i < playerControllers.Num(); ++i)
	{
		auto playerController = playerControllers[i];

		APlayerState* playerState = playerController->GetPlayerStateField();
		__int64 currentSteamId = playerState->GetUniqueIdField()->UniqueNetId->GetUniqueNetIdField();

		if (currentSteamId == steamId)
		{
			AShooterPlayerController* aShooterPC = static_cast<AShooterPlayerController*>(playerController.Get());

			result = aShooterPC;
			break;
		}
	}

	return result;
}

wchar_t* ConvertToWideStr(const std::string& str)
{
	size_t newsize = str.size() + 1;

	wchar_t* wcstring = new wchar_t[newsize];

	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str.c_str(), _TRUNCATE);

	return wcstring;
}

bool IsPointInsideSphere(FVector point, float sphereX, float sphereY, float sphereZ, float sphereRadius)
{
	long double x = point.X - sphereX;
	long double y = point.Y - sphereY;
	long double z = point.Z - sphereZ;

	long double distancesq = x * x + y * y + z * z;
	return distancesq < sphereRadius * sphereRadius;
}

bool IsPointInside2dCircle(FVector point, float circleX, float circleY, float circleRadius)
{
	long double x = point.X - circleX;
	long double y = point.Y - circleY;

	long double distancesq = x * x + y * y;
	return distancesq < circleRadius * circleRadius;
}