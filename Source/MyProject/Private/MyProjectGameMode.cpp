// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "MyProjectPawn.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"


AMyProjectGameMode::AMyProjectGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AMyProjectPlayerController::StaticClass();
}

void AMyProjectGameMode::Tick(float Delta)
{

	Super::Tick(Delta);
	
	UMyProjectUI* RaceUI = GetRaceUI();
	
	if (bRaceStarted && RaceUI)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds() - StartTime;

		RaceUI -> UpdateChrono(CurrentTime);
		
	}
}

UMyProjectUI* AMyProjectGameMode::GetRaceUI()
{
	AMyProjectPlayerController* PC = Cast<AMyProjectPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	return PC ? PC->GetVehicleUI() : nullptr;
}



void AMyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

}


void AMyProjectGameMode::StartRace()
{
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "Race");
	if (!GetWorld()) return;

	StartTime = GetWorld()->GetTimeSeconds();
	bRaceStarted = true;

	// Add every car to the map
	for (TActorIterator<AMyProjectPawn> It(GetWorld()); It; ++It)
	{
		AMyProjectPawn* Car = *It;
		if (Car) 
		{
			RaceTimers.Add(Car, 0.0f);
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, 
				FString::Printf(TEXT("%s ajouté à la course"), *Car->GetName()));
		}
	}
	// Add every barrier to the map 
	for (TActorIterator<AMyBlockingBarrier> It(GetWorld()); It; ++It)
	{
		AMyBlockingBarrier* Barrier = *It;
		if (Barrier) 
		{
			Barriers.Add(Barrier);
			
		}
	}
	CurrentLap = 0;
	UpdateBarriers();
}

void AMyProjectGameMode::StopRace(AActor* Participant)
{
	if (!bRaceStarted || !RaceTimers.Contains(Participant) || !GetWorld())
		return;

	// Set the end time for the participant in the map
	float EndTime = GetWorld()->GetTimeSeconds();
	RaceTimers[Participant] = EndTime - StartTime;

	// Check if the car is the one controlled by the player
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && Participant == PlayerController->GetPawn())
	{
		bRaceStarted = false;
	}

}

float AMyProjectGameMode::GetRaceTime(AActor* Participant) const
{
	// returns racetimer
	if (RaceTimers.Contains(Participant))
	{
		return RaceTimers[Participant];
	}
	return -1.0f;
}

void AMyProjectGameMode::IncrementLap()
{
	CurrentLap++;
	UpdateBarriers();
}

void AMyProjectGameMode::UpdateBarriers()
{
	for (AMyBlockingBarrier* Barrier : Barriers)
	{
		if (!Barrier) continue;

		bool bShouldBeActive = true;//default is true, set to be hidden

		switch (Barrier->BarrierState)
		{
		case EBarrierState::Begin:
			bShouldBeActive = (CurrentLap ==0) ;
			break;
		case EBarrierState::Mid:
			bShouldBeActive = (CurrentLap >= 2 && CurrentLap < TotalLaps);
			break;
		case EBarrierState::End:
			bShouldBeActive = (CurrentLap >= TotalLaps);
			break;
		}

		// activate or desactivate barrier
		Barrier->SetActorHiddenInGame(bShouldBeActive);
		Barrier->SetActorEnableCollision(!bShouldBeActive);
	}
	
}

