// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectPawn.h"
#include "MyProjectWheelFront.h"
#include "MyProjectWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

DEFINE_LOG_CATEGORY(LogTemplateVehicle);

AMyProjectPawn::AMyProjectPawn()
{
	// construct the front camera boom
	FrontSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Front Spring Arm"));
	FrontSpringArm->SetupAttachment(GetMesh());
	FrontSpringArm->TargetArmLength = 0.0f;
	FrontSpringArm->bDoCollisionTest = false;
	FrontSpringArm->bEnableCameraRotationLag = true;
	FrontSpringArm->CameraRotationLagSpeed = 15.0f;
	FrontSpringArm->SetRelativeLocation(FVector(30.0f, 0.0f, 120.0f));

	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Front Camera"));
	FrontCamera->SetupAttachment(FrontSpringArm);
	FrontCamera->bAutoActivate = false;

	// construct the back camera boom
	BackSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Back Spring Arm"));
	BackSpringArm->SetupAttachment(GetMesh());
	BackSpringArm->TargetArmLength = 650.0f;
	BackSpringArm->SocketOffset.Z = 150.0f;
	BackSpringArm->bDoCollisionTest = false;
	BackSpringArm->bInheritPitch = false;
	BackSpringArm->bInheritRoll = false;
	BackSpringArm->bEnableCameraRotationLag = true;
	BackSpringArm->CameraRotationLagSpeed = 2.0f;
	BackSpringArm->CameraLagMaxDistance = 50.0f;

	BackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Back Camera"));
	BackCamera->SetupAttachment(BackSpringArm);

	// Configure the car mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	// get the Chaos Wheeled movement component
	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

	BoostParticlesLeft = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExhaustParticles"));
	BoostParticlesLeft->SetupAttachment(RootComponent);  // Ou un autre composant du véhicule, comme le moteur
	
}

void AMyProjectPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering 
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AMyProjectPawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AMyProjectPawn::Steering);

		// throttle 
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AMyProjectPawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AMyProjectPawn::Throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AMyProjectPawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &AMyProjectPawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AMyProjectPawn::StopBrake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AMyProjectPawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AMyProjectPawn::StopHandbrake);

		// look around 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AMyProjectPawn::LookAround);

		// toggle camera
		
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &AMyProjectPawn::ToggleCamera);

		// boost
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Triggered, this, &AMyProjectPawn::Boost);
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Started, this, &AMyProjectPawn::ActivateBoost);
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Completed, this, &AMyProjectPawn::DeactivateBoost);

		// reset the vehicle 
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &AMyProjectPawn::ResetVehicle);

		
	}
	else
	{
		UE_LOG(LogTemplateVehicle, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProjectPawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	if (PlayerStart)
	{
		RespawnLocation = PlayerStart->GetActorLocation();
        
	}
}

void AMyProjectPawn::Tick(float Delta)
{
	Super::Tick(Delta);
	if (EnablePowerCutting)
	{
		TorqueCuttingFix();
	}
	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	// realign the camera yaw to face front
	float CameraYaw = BackSpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, Delta, 1.0f);

	BackSpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));

	if (bStopsBoosting)
	{
		DeactivateBoost(FInputActionValue());
	}
	ReloadBoost();
}

void AMyProjectPawn::SetRespawnLocation(FVector _RespawnLocation)
{
	RespawnLocation = _RespawnLocation;
}

void AMyProjectPawn::TorqueCuttingFix()
{
	if (GetVehicleMovementComponent()->GetTargetGear() >= 1)
	{
		if (UChaosWheeledVehicleMovementComponent* VehicleComponent =
			Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
		{
			// Déclaration de la variable pour multiplier le couple
			float TorqueMultiplier = 0;
    
			switch (GetVehicleMovementComponent()->GetTargetGear())
			{
			case 1:
				TorqueMultiplier = 450;
				break;
			case 2:
				TorqueMultiplier = 350;
				break;
			case 3:
				TorqueMultiplier = 250;
				break;
			case 4:
				TorqueMultiplier = 150;
				break;
			default:
				TorqueMultiplier = 0;  
				break;
			}
    
			// Application du couple à toutes les roues
			if (TorqueMultiplier > 0)
			{
				VehicleComponent->SetDriveTorque(MultiplyTorque(TorqueMultiplier), 2);
				VehicleComponent->SetDriveTorque(MultiplyTorque(TorqueMultiplier), 3);
			}
		}
	}
	else
	{
		if (UChaosWheeledVehicleMovementComponent* VehicleComponent =
			Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
		{
			VehicleComponent->SetDriveTorque(GetVehicleMovementComponent()->GetThrottleInput() * 1, 2);
			VehicleComponent->SetDriveTorque(GetVehicleMovementComponent()->GetThrottleInput() * 1, 3);
		}
	}
}

float AMyProjectPawn::MultiplyTorque(float GearTorque)
{
	return GetVehicleMovementComponent()->GetThrottleInput() * 1000 * GearTorque;
}

void AMyProjectPawn::ReloadBoost()
{
	if (IsDrifting && GetVelocity().Size() > 50)
		{
			BoostMeter += BoostRechargeRate;
			if (BoostMeter > BoostMax)  
			{
				BoostMeter = BoostMax;  
			}
		}
}

void AMyProjectPawn::Steering(const FInputActionValue& Value)
{
	float SteeringValue = Value.Get<float>();
	float ResultDot;
	if (USkeletalMeshComponent* VehicleMesh = GetMesh())
	{
		FVector Velocity = VehicleMesh->GetComponentVelocity();
		FVector NormalizedVelocity = Velocity.IsNearlyZero() ? FVector::ZeroVector : Velocity.GetSafeNormal();
		FVector RightVector = VehicleMesh->GetRightVector().GetSafeNormal();
		ResultDot = FVector::DotProduct(NormalizedVelocity, RightVector);
	}

	// add the input
	ChaosVehicleMovement->SetSteeringInput(ResultDot *1 + SteeringValue);
}

void AMyProjectPawn::Throttle(const FInputActionValue& Value)
{
	// get the input magnitude for the throttle
	float ThrottleValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);
}

void AMyProjectPawn::Brake(const FInputActionValue& Value)
{
	// get the input magnitude for the brakes
	float BreakValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetBrakeInput(BreakValue);
}

void AMyProjectPawn::StartBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	
	BrakeLights(true);
}

void AMyProjectPawn::StopBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	
	BrakeLights(false);

	// reset brake input to zero
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void AMyProjectPawn::StartHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(true);
	IsDrifting = true;
	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void AMyProjectPawn::StopHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(false);
	IsDrifting = false;
	// call the Blueprint hook for the break lights
	BrakeLights(false);
}

void AMyProjectPawn::LookAround(const FInputActionValue& Value)
{
	// get the flat angle value for the input 
	float LookValue = Value.Get<float>();

	// add the input
	BackSpringArm->AddLocalRotation(FRotator(0.0f, LookValue, 0.0f));
}

void AMyProjectPawn::ToggleCamera(const FInputActionValue& Value)
{
	// toggle the active camera flag
	bFrontCameraActive = !bFrontCameraActive;

	FrontCamera->SetActive(bFrontCameraActive);
	BackCamera->SetActive(!bFrontCameraActive);
}

void AMyProjectPawn::ResetVehicle(const FInputActionValue& Value)
{
	// reset to a location slightly above our current one
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	// reset to our yaw. Ignore pitch and roll
	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;
	
	// teleport the actor to the reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);

	UE_LOG(LogTemplateVehicle, Error, TEXT("Reset Vehicle"));
}



void AMyProjectPawn::Boost(const FInputActionValue& Value)
{
	if (GetBoostingInput())
	{
		// Check boost availability
		if (BoostMeter < BoostConsumption || !bCanBoost)
		{

			DeactivateBoost(1);
			return;
		}
		if (GetChaosVehicleMovement())
		{
    	
			// Apply an impulse to the car
			UPrimitiveComponent* VehicleRoot = Cast<UPrimitiveComponent>(GetRootComponent());
			if (VehicleRoot)
			{
				FVector BoostDirection = GetActorForwardVector();
				float BoostStrength = 100000.0f;
				VehicleRoot->AddImpulse(BoostDirection * BoostStrength, NAME_None);
			}

			// Set new boost value
			BoostMeter -= BoostConsumption;
			if (BoostMeter <= 0)
			{
				BoostMeter = 0;
				bCanBoost = false; 
			}


			// Camera manager
			FVector TargetCameraPosition =  (vCameraBoostOffsetPosition); 
			FRotator TargetCameraRotation = (rCameraBoostOffsetRotator);  

			FVector CurrentCameraPosition = BackSpringArm->GetRelativeLocation();
			FRotator CurrentCameraRotation = BackCamera->GetRelativeRotation();
    	
			// Lerp to a new camera location
			FVector NewCameraPosition = FMath::Lerp(CurrentCameraPosition, TargetCameraPosition, 0.1f); 
			FRotator NewCameraRotation = FMath::Lerp(CurrentCameraRotation, TargetCameraRotation, 0.1f);  

			BackSpringArm->SetRelativeLocation(NewCameraPosition);
			BackCamera->SetRelativeRotation(NewCameraRotation);

			// Camera FOV
			BackCamera->FieldOfView = FMath::Lerp(BackCamera->FieldOfView, 130, 0.1f);
    	
			// Camera shaking
			FVector ShakeOffset = FVector(FMath::RandRange(-5.0f, 5.0f), FMath::RandRange(-5.0f, 5.0f), FMath::RandRange(-2.0f, 2.0f));
			BackSpringArm->SetWorldLocation(BackSpringArm->GetComponentLocation() + ShakeOffset);

			// Particles manager
			BoostParticlesLeft->Activate();
		}
	}
	
}


void AMyProjectPawn::ActivateBoost(const FInputActionValue& Value)
{
	bStopsBoosting = false;

	SetBoostingInput(true);
}


void AMyProjectPawn::DeactivateBoost(const FInputActionValue& Value)
{
	bStopsBoosting = true;
	SetBoostingInput(false);
	// Stop particles
	BoostParticlesLeft->Deactivate();

	// Reset camera position
	
	FVector TargetCameraPosition = FVector::ZeroVector;
	FRotator TargetCameraRotation = FRotator::ZeroRotator;

	FVector CurrentCameraPosition = BackSpringArm->GetRelativeLocation();
	FRotator CurrentCameraRotation = BackCamera->GetRelativeRotation();
    	
	// Lerp to a new camera location
	FVector NewCameraPosition = FMath::Lerp(CurrentCameraPosition, TargetCameraPosition, 0.1f); 
	FRotator NewCameraRotation = FMath::Lerp(CurrentCameraRotation, TargetCameraRotation, 0.1f);

	BackSpringArm->SetRelativeLocation(NewCameraPosition);
	BackCamera->SetRelativeRotation(NewCameraRotation);

	// Reset camera FOV
	BackCamera->FieldOfView = FMath::Lerp(BackCamera->FieldOfView, 90, 0.1f);

	// Stop camera shaking
	FVector ShakeOffset = FVector(0, 0, 0);
	BackSpringArm->SetWorldLocation(BackSpringArm->GetComponentLocation() + ShakeOffset);

	if (NewCameraPosition == TargetCameraPosition)
	{
		bStopsBoosting = false;
	}
}


#undef LOCTEXT_NAMESPACE