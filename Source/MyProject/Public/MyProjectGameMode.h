
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectUI.h"
#include "MyProjectGameMode.generated.h"


class ACheckPoint;
UCLASS(MinimalAPI)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AMyProjectGameMode();
	virtual void BeginPlay() override;

	virtual void Tick(float Delta) override;
	
	/** Launch race and timer */
	void StartRace();

	/** Stop race timer for the participant */
	void StopRace(AActor* Participant);

	/** Get race timer for the participant */
	float GetRaceTime(AActor* Participant) const;

	/** Get vehicule UI*/
	UMyProjectUI* GetRaceUI();

	void ValidCheckpoint();
	
	UFUNCTION(BlueprintCallable, Category = "MonCategorie")
	bool IsPreviousCheckpointValid( int32 id);
	bool AllTrue();
	void ResetCheckpoint();
	void UpdateCheckPoint();
	void AddLap();
	bool GetEndGame();

	
	
private :
	
	/** Gather every car on the map and associate a timer to each of them */
	TMap<AActor*, float> RaceTimers; 
	
	bool bRaceStarted = false;
	float StartTime = 0.0f;
	TArray<ACheckPoint*> Checkpoints;

	int CurrentLap = 0;
	int TotalLaps = 3;

	bool EndGame = false;
	FVector RespawnLocation;
	FRotator RespawnRotation;
	
};



