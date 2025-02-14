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

	SetupCheckPoint();
	
	ValidCheckpoint();
	UpdateCheckPoint();

}

void AMyProjectGameMode::SetupCheckPoint()
{

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), FoundActors);
	

    if (FoundActors.Num() == 0)
    {
        return;
    }

    TArray<ACheckPoint*> BeginCheckpoints;
    TArray<ACheckPoint*> AllCheckpoints;
    TArray<ACheckPoint*> MidCheckpoints;
    TArray<ACheckPoint*> EndCheckpoints;

    for (AActor* Actor : FoundActors) 
    {
        
        ACheckPoint* Checkpoint = Cast<ACheckPoint>(Actor);
        if (!Checkpoint) continue;

        FString FolderName = Actor->GetFolderPath().ToString();
        FolderName = FolderName.RightChop(FolderName.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd) + 1);

        ECheckPointState CheckpointState = ConvertStringToState(FolderName);
        Checkpoint->SetState(CheckpointState);

        switch (CheckpointState)
        {
            case ECheckPointState::Begin: BeginCheckpoints.Add(Checkpoint); break;
            case ECheckPointState::All:   AllCheckpoints.Add(Checkpoint); break;
            case ECheckPointState::Mid:   MidCheckpoints.Add(Checkpoint); break;
            case ECheckPointState::End:   EndCheckpoints.Add(Checkpoint); break;
            default: break;
        }
    }

    TArray<ACheckPoint*> OrderedCheckpoints;
    OrderedCheckpoints.Append(BeginCheckpoints);
    OrderedCheckpoints.Append(AllCheckpoints);
    OrderedCheckpoints.Append(MidCheckpoints);
    OrderedCheckpoints.Append(EndCheckpoints);

    int32 CurrentID = 0;
    for (ACheckPoint* Checkpoint : OrderedCheckpoints)
    {
        Checkpoint->SetId(CurrentID);
        CurrentID++;
    }
	
}


ECheckPointState AMyProjectGameMode:: ConvertStringToState(const FString& FolderName)
{
	if (FolderName.Equals("Begin", ESearchCase::IgnoreCase)) return ECheckPointState::Begin;
	if (FolderName.Equals("Mid", ESearchCase::IgnoreCase)) return ECheckPointState::Mid;
	if (FolderName.Equals("End", ESearchCase::IgnoreCase)) return ECheckPointState::End;
	
	return ECheckPointState::All; 
}


void AMyProjectGameMode::StartRace()
{
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

FString AMyProjectGameMode::GetRaceTimeFormated(AActor* Participant) const
{
	if (RaceTimers.Contains(Participant))
	{
		float NewTime = GetRaceTime(Participant);// Calcul des minutes, secondes et centièmes
		int32 Minutes = FMath::FloorToInt(NewTime / 60);
		int32 Seconds = FMath::FloorToInt(NewTime) % 60;
		int32 Centiseconds = FMath::FloorToInt((NewTime - FMath::FloorToInt(NewTime)) * 100);

		// Formatage du chrono en string
		FString FormattedTime = FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Centiseconds);

		// Appel de l'event Blueprint pour afficher le chrono
		return FormattedTime;
	}
	return TEXT("");
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
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "End	");
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

