// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyProjectPlayerController.generated.h"

class UInputMappingContext;
class AMyProjectPawn;
class UMyProjectUI;
class UMyPauseWidget;

/**
 *  Vehicle Player Controller class
 *  Handles input mapping and user interface
 */
UCLASS(abstract)
class MYPROJECT_API AMyProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	/** Pointer to the controlled vehicle pawn */
	TObjectPtr<AMyProjectPawn> VehiclePawn;

	/** Type of the UI to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UMyProjectUI> VehicleUIClass;

	/** Pointer to the UI widget */
	UPROPERTY(BlueprintReadWrite, Category = UI)
	TObjectPtr<UMyProjectUI> VehicleUI;

	// Begin Actor interface


	
	
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:

	virtual void Tick(float Delta) override;

	// End Actor interface

	// Begin PlayerController interface

	//Returns Vehicule UI
	UMyProjectUI* GetVehicleUI() const { return VehicleUI; }

	// Activate UI
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ActivateUI(bool state);

	

	
protected:

	virtual void OnPossess(APawn* InPawn) override;

	// End PlayerController interface

		
	
};
