
#pragma once

#include "CoreMinimal.h"
#include "GameObjects/MyBlockingBarrier.h"
#include "UI/MyPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MyProjectUI.h"
#include "GameObjects/CheckPoint.h"  
#include "MyProjectGameMode.generated.h"


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
	UFUNCTION(BlueprintCallable, Category = "UI")
	void StopRace(AActor* Participant);

	/** Get race timer for the participant */
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetRaceTime(AActor* Participant) const;

	/** Get race timer formated for the participant */
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetRaceTimeFormated(AActor* Participant) const;

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


	void SetupCheckPoint();

	
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


	ECheckPointState ConvertStringToState(const FString& FolderName);
	

protected:
	/** Current lap information*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Race")
	int32 CurrentLap;

	/** Overall amount of laps*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race")
	int32 TotalLaps;

	
};



