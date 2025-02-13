// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/MyProjectPawn.h"
#include "MyProjectSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class MYPROJECT_API AMyProjectSportsCar : public AMyProjectPawn
{
	GENERATED_BODY()
	
public:

	AMyProjectSportsCar();
	
};
