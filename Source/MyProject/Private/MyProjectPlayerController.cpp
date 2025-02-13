// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "MyProjectPawn.h"
#include "MyProjectUI.h"
#include "MyPauseWidget.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void AMyProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// spawn the UI widget and add it to the viewport
	VehicleUI = CreateWidget<UMyProjectUI>(this, VehicleUIClass);

	check(VehicleUI);

	VehicleUI->AddToViewport();

	VehicleUI->SetVisibility(ESlateVisibility::Hidden);
	
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

void AMyProjectPlayerController::ActivateUI()
{
	// Logique pour activer l'UI ou faire ce que tu veux à la fin du décompte
	// Par exemple :
	if (VehicleUI)
	{
		VehicleUI->SetVisibility(ESlateVisibility::Visible);
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


