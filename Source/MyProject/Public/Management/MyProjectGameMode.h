
#pragma once

#include "CoreMinimal.h"
#include "GameObjects/MyBlockingBarrier.h"
#include "UI/MyPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MyProjectUI.h"
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
	UFUNCTION(BlueprintCallable, Category = "UI")
	void StartRace();

	/** Stop race timer for the participant */
	void StopRace(AActor* Participant);

	/** Get race timer for the participant */
	float GetRaceTime(AActor* Participant) const;

	/** Get vehicule UI*/
	UMyProjectUI* GetRaceUI();

	/** Returns Current lap */
	int32 GetCurrentLap(){ return CurrentLap; };

	/** Returns Total lap*/
	int32 GetTotalLap(){ return TotalLaps;};

	
	
	/** Manage pause menu*/
	void TogglePause();

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

	bool EndGame = false;
	FVector RespawnLocation;
	/** Increment lap current amount*/
	void IncrementLap();

	/** Update blocking barriers position*/
	void UpdateBarriers();

	/** Array for every blocking barrier*/
	TArray<AMyBlockingBarrier*> Barriers;

	

protected:
	/** Current lap information*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Race")
	int32 CurrentLap;

	/** Overall amount of laps*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race")
	int32 TotalLaps;

	
};



