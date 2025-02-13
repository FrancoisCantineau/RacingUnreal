// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"
#include "GameFramework/SaveGame.h"
#include "MyProjectGameMode.h"
#include "MyLocalPlayerSaveGame.generated.h"




/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyLocalPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	void SetDataArray(TArray<FPlayerScore> _DataArray){ DataArray = _DataArray; }
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TArray<FPlayerScore> DataArray;
	
	
};
