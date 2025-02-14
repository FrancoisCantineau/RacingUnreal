
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
	UFUNCTION(BlueprintCallable, Category = "End")
	void StopRace(AActor* Participant);

	/** Get race timer for the participant */
	float GetRaceTime(AActor* Participant) const;

	/** Get vehicule UI*/
	UMyProjectUI* GetRaceUI();
	UFUNCTION(BlueprintCallable, Category = "End")
	/** Returns Current lap */
	int32 GetCurrentLap(){ return CurrentLap; };
	UFUNCTION(BlueprintCallable, Category = "End")
	/** Returns Total lap*/
	int32 GetTotalLap(){ return TotalLaps;};

	
	
	/** Manage pause menu*/
	void TogglePause();

	void ValidCheckpoint();
	UFUNCTION(BlueprintCallable, Category = "MonCategorie")
	bool IsPreviousCheckpointValid( int32 id);
	UFUNCTION(BlueprintCallable, Category = "End")
	bool AllTrue();
	UFUNCTION(BlueprintCallable, Category = "End")
	void ResetCheckpoint();
	UFUNCTION(BlueprintCallable, Category = "End")
	void UpdateCheckPoint();
	UFUNCTION(BlueprintCallable, Category = "End")
	void AddLap();
	bool GetEndGame();
	UFUNCTION(BlueprintCallable, Category = "End")
	/** Update blocking barriers position*/
	void UpdateBarriers();

	void UpdateCheckpointOrder();
	UFUNCTION(BlueprintCallable, Category = "NewOrder")
	void VerifySwitch();

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
	

	
	/** Array for every blocking barrier*/
	TArray<AMyBlockingBarrier*> Barriers;
	TArray<ACheckPoint*> CheckpointsTemp; 
	bool bUseTempCheckpoints = false;   
protected:
	/** Current lap information*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Race")
	int32 CurrentLap;

	/** Overall amount of laps*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race")
	int32 TotalLaps;

	
};



