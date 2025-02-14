// Copyright Epic Games, Inc. All Rights Reserved.


#include "Player/MyProjectPlayerController.h"

#include "Player/MyProjectPawn.h"
#include "UI/MyProjectUI.h"
#include "UI/MyPauseWidget.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Blueprint/UserWidget.h"

void AMyProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// spawn the UI widget and add it to the viewport
	VehicleUI = CreateWidget<UMyProjectUI>(this, VehicleUIClass);

	check(VehicleUI);

	VehicleUI->AddToViewport();

	ActivateUI(false);
	
}

void AMyProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AMyProjectPlayerController::ActivateUI(bool state)
{
	// Logique pour activer l'UI ou faire ce que tu veux à la fin du décompte
	// Par exemple :
	if (VehicleUI)
	{
		if (state)
		{
			VehicleUI->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			VehicleUI->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
}

void AMyProjectPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
		VehicleUI->UpdateBoost(VehiclePawn->GetBoostMeter());
	}
}

void AMyProjectPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<AMyProjectPawn>(InPawn);
}


