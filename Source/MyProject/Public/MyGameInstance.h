// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)

struct FPlayerScore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Pseudo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Score;
};

UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UMyGameInstance(){};

	virtual void Init()override{} ;
	
	void AddPlayer(const FString& Pseudo, float NewScore);

	void TriArray(TArray<FPlayerScore>& _PlayerScores);

	void SavePlayerData(const TArray<FPlayerScore>& ArrayToSave);

	void RemovePlayerDataByName(const FString& PlayerName);

	TArray<FPlayerScore> LoadPlayerData();

	TArray<FPlayerScore> GetPlayerScores(){ return PlayerScores; }
	
	UFUNCTION(BlueprintCallable)
	TArray<FPlayerScore> GetClassement();
private:
	TArray<FPlayerScore> PlayerScores;
};
