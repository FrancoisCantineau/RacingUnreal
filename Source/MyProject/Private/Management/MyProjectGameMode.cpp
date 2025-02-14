// Copyright Epic Games, Inc. All Rights Reserved.

#include "Management/MyProjectGameMode.h"

#include "Player/MyProjectPawn.h"
#include "EngineUtils.h"
#include "GameObjects/CheckPoint.h"
#include "GameObjects/MyBlockingBarrier.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyProjectPlayerController.h"


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
	ValidCheckpoint();
	UpdateCheckPoint();
	UpdateCheckpointOrder();
	bUseTempCheckpoints = false;
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Updating Barriers"));

	for (AMyBlockingBarrier* Barrier : Barriers)
	{
		if (!Barrier) continue;

		bool bShouldBeActive = false; // Par défaut, on désactive tout

		switch (Barrier->BarrierState)
		{
		case EBarrierState::Begin:
			bShouldBeActive = (CurrentLap == 0); // Actif uniquement au premier tour
			break;

		case EBarrierState::Mid:
			bShouldBeActive = (CurrentLap > 0 && CurrentLap < TotalLaps - 1); // Actif entre le premier et le dernier tour
			break;

		case EBarrierState::End:
			bShouldBeActive = (CurrentLap == TotalLaps - 1); // Actif uniquement au dernier tour
			break;
		
		}

		// Appliquer la visibilité et les collisions
		Barrier->SetActorHiddenInGame(bShouldBeActive);
		Barrier->SetActorEnableCollision(!bShouldBeActive);

	}
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Updating Checkpoints"));

	for (ACheckPoint* Checkpoint : Checkpoints)
	{
		if (!Checkpoint) continue;

		bool bShouldBeActive = false; 

		switch (Checkpoint->GetState())
		{
		case ECheckPointState::Begin:
			bShouldBeActive = (CurrentLap == 0);
			
			break;

		case ECheckPointState::Mid:
			bShouldBeActive = (CurrentLap > 0 && CurrentLap < TotalLaps - 1);
			
			break;

		case ECheckPointState::End:
			bShouldBeActive = (CurrentLap == TotalLaps - 1);
			break;

		case ECheckPointState::All:
			bShouldBeActive = !(CurrentLap == TotalLaps - 1);
			break;
		}
		
		FString DebugMessage = FString::Printf(TEXT("Checkpoint State: %d | CurrentLap: %d | TotalLaps: %d | bShouldBeActive: %d"),
		   static_cast<int>(Checkpoint->GetState()), CurrentLap, TotalLaps, bShouldBeActive);
		// Appliquer la visibilité et les collisions
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
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "End");
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
void AMyProjectGameMode::ValidCheckpoint()
{
	//Add every barrier to the map 
	for (TActorIterator<ACheckPoint> It(GetWorld()); It; ++It)
	{
		ACheckPoint* CheckPoint = *It;
		if (CheckPoint) 
		{
			Checkpoints.Add(CheckPoint);
		}
	}
	// Trier les checkpoints par ID
	Algo::Sort(Checkpoints, []( ACheckPoint* A,  ACheckPoint* B) {
		return A->GetId() < B->GetId();
	});
}

bool AMyProjectGameMode::IsPreviousCheckpointValid(int32 id) {
	const TArray<ACheckPoint*>& CurrentCheckpoints = bUseTempCheckpoints ? CheckpointsTemp : Checkpoints;
	
	for (int i = 0; i < CurrentCheckpoints.Num(); i++) {
		if (CurrentCheckpoints[i] && CurrentCheckpoints[i]->GetId() == id) {
			if (i > 0) { // Vérifie s'il y a un checkpoint précédent
				ACheckPoint* PreviousCheckpoint = CurrentCheckpoints[i - 1];
				if (PreviousCheckpoint && !PreviousCheckpoint->GetIsEnter()) {
					UE_LOG(LogTemp, Warning, TEXT("Vous avez sauté le checkpoint précédent !"));
					return false;
				}
			}
			return true;
		}
	}
	return false;
}


void AMyProjectGameMode::UpdateCheckpointOrder() {
	CheckpointsTemp.Empty();

	// Ajouter les checkpoints dans l'ordre défini
	for (ACheckPoint* Checkpoint : Checkpoints) {
		if (Checkpoint && Checkpoint->GetState() == ECheckPointState::Mid) {
			CheckpointsTemp.Add(Checkpoint);
		}
	}
	for (ACheckPoint* Checkpoint : Checkpoints) {
		if (Checkpoint && Checkpoint->GetState() == ECheckPointState::All) {
			CheckpointsTemp.Add(Checkpoint);
		}
	}
	for (ACheckPoint* Checkpoint : Checkpoints) {
		if (Checkpoint && Checkpoint->GetState() == ECheckPointState::Begin) {
			CheckpointsTemp.Add(Checkpoint);
		}
	}
	for (ACheckPoint* Checkpoint : Checkpoints) {
		if (Checkpoint && Checkpoint->GetState() == ECheckPointState::End) {
			CheckpointsTemp.Add(Checkpoint);
		}
	}

	bUseTempCheckpoints = true;
}


void AMyProjectGameMode::VerifySwitch()
{
	if ((CurrentLap == 0) || (CurrentLap == TotalLaps - 1))
	{
		bUseTempCheckpoints = false ;
	}
	else
	{

		bUseTempCheckpoints = true;
	}
}




