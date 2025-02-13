// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"

#include "CheckPoint.h"
#include "MyProjectPlayerController.h"
#include "MyProjectPawn.h"
#include "EngineUtils.h"
#include "MyGameInstance.h"
#include "MyLocalPlayerSaveGame.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

void AMyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartRace();

	ValidCheckpoint();
	UpdateCheckPoint();
	/*UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		MyGameInstance->LoadPlayerData();
	}*/
}


void AMyProjectGameMode::StartRace()
{
	if (!GetWorld()) return;

	StartTime = GetWorld()->GetTimeSeconds();
	bRaceStarted = true;
	EndGame = false;

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
}

void AMyProjectGameMode::StopRace(AActor* Participant)
{
	if (!bRaceStarted || !RaceTimers.Contains(Participant) || !GetWorld())
		return;

	// Set the end time for the participant in the map
	float EndTime = GetWorld()->GetTimeSeconds();
	RaceTimers[Participant] = EndTime - StartTime;
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("fffqdqqzfefsfqdqz"));
		MyGameInstance->AddPlayer(TEXT("P0izon"), EndTime);
		MyGameInstance->AddPlayer(TEXT("PZ"), 12);
		MyGameInstance->AddPlayer(TEXT("QLF"), 1);
		MyGameInstance->AddPlayer(TEXT("PNl"), 3);
		MyGameInstance->GetClassement();
	}
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

AMyProjectGameMode::AMyProjectGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AMyProjectPlayerController::StaticClass();
}

bool AMyProjectGameMode::AllTrue()
{
	for (ACheckPoint* Checkpoint : Checkpoints)
	{
		if (!Checkpoint->GetIsEnter()) {
			return false;
		}
	}
	return true;
}



void AMyProjectGameMode::ResetCheckpoint()
{
	for (ACheckPoint* Checkpoint : Checkpoints) 
	{
		Checkpoint->SetIsEnter(false);
	}
}

void AMyProjectGameMode::UpdateCheckPoint()
{
	
	for (ACheckPoint* Checkpoint : Checkpoints)
	{
		if (!Checkpoint) continue;

		bool bShouldBeActive = true;//default is true, set to be hidden

		switch (Checkpoint->GetState())
		{
		case ECheckPointState::Begin:
			bShouldBeActive = (CurrentLap ==0) ;
			break;
		case ECheckPointState::Mid:
			bShouldBeActive = (CurrentLap >= 2 && CurrentLap < TotalLaps);
			break;
		case ECheckPointState::End:
			bShouldBeActive = (CurrentLap >= TotalLaps);
			break;
		}

		// activate or desactivate and skip or not checkpoint
		Checkpoint->SetActorHiddenInGame(!bShouldBeActive);
		Checkpoint->SetActorEnableCollision(bShouldBeActive);
		Checkpoint->SetIsEnter(!bShouldBeActive);
	}
}

void AMyProjectGameMode::AddLap()
{
	CurrentLap++;
	if( CurrentLap < TotalLaps )
	{
		ResetCheckpoint();
		UpdateCheckPoint();
	}
	else
	{
		if (AllTrue())
		{
			EndGame = true;
		}
	}
}

bool AMyProjectGameMode::GetEndGame()
{
	return EndGame;
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

void AMyProjectGameMode::ValidCheckpoint()
{
	//Add every barrier to the map 
	for (TActorIterator<ACheckPoint> It(GetWorld()); It; ++It)
	{
		ACheckPoint* Checkpoint = *It;
		if (Checkpoint) 
		{
			Checkpoints.Add(Checkpoint);
		}
	}
	// Trier les checkpoints par ID
	Algo::Sort(Checkpoints, []( ACheckPoint* A,  ACheckPoint* B) {
		return A->GetId() < B->GetId();
	});
}

bool AMyProjectGameMode::IsPreviousCheckpointValid(int32 id) {
	for (ACheckPoint* Checkpoint : Checkpoints) {
		if (Checkpoint && Checkpoint->GetId() == id) {
			for (ACheckPoint* PreviousCheckpoint : Checkpoints) {
				if (PreviousCheckpoint && PreviousCheckpoint->GetId() == id - 1) {
					if (!PreviousCheckpoint->GetIsEnter()) {
						UE_LOG(LogTemp, Warning, TEXT("Vous avez sauté le checkpoint précédent !"));
						return false;
					}
					return true;
				}
			}
			return true;
		}
	}
	return false;
}


