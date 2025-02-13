
#pragma once

#include "CoreMinimal.h"
#include "MyBlockingBarrier.h"
#include "MyPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectUI.h"
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
	void StopRace(AActor* Participant);

	/** Get race timer for the participant */
	float GetRaceTime(AActor* Participant) const;

	/** Get vehicule UI*/
	UMyProjectUI* GetRaceUI();

	/** Current lap information*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Race")
	int32 CurrentLap;

	/** Overall amount of laps*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race")
	int32 TotalLaps;

	/** Manage pause menu*/
	void TogglePause();

private :
	
	/** Gather every car on the map and associate a timer to each of them */
	TMap<AActor*, float> RaceTimers; 
	
	bool bRaceStarted = false;
	float StartTime = 0.0f;

	/** Increment lap current amount*/
	void IncrementLap();

	/** Update blocking barriers position*/
	void UpdateBarriers();

	/** Array for every blocking barrier*/
	TArray<AMyBlockingBarrier*> Barriers;

	

	

	
};



