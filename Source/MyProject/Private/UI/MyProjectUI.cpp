// Copyright Epic Games, Inc. All Rights Reserved.
#include "UI/MyProjectUI.h"

void UMyProjectUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UMyProjectUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}

void UMyProjectUI::UpdateBoost(float NewBoost)
{
	// call the Blueprint handler
	OnBoostUpdate(NewBoost);
}

void UMyProjectUI::UpdateChrono(float NewTime)
{
	// Calcul des minutes, secondes et centièmes
	int32 Minutes = FMath::FloorToInt(NewTime / 60);
	int32 Seconds = FMath::FloorToInt(NewTime) % 60;
	int32 Centiseconds = FMath::FloorToInt((NewTime - FMath::FloorToInt(NewTime)) * 100);

	// Formatage du chrono en string
	FString FormattedTime = FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Centiseconds);

	// Appel de l'event Blueprint pour afficher le chrono
	OnChronoUpdate(FormattedTime);
}