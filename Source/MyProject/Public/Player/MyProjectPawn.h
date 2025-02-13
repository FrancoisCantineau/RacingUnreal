// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/MyPauseWidget.h"
#include "WheeledVehiclePawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MyProjectPawn.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UChaosWheeledVehicleMovementComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateVehicle, Log, All);

/**
 *  Vehicle Pawn class
 *  Handles common functionality for all vehicle types,
 *  including input handling and camera management.
 *  
 *  Specific vehicle configurations are handled in subclasses.
 */
UCLASS(abstract)
class AMyProjectPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

	/** Spring Arm for the front camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FrontSpringArm;

	/** Front Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FrontCamera;

	/** Spring Arm for the back camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* BackSpringArm;

	/** Back Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* BackCamera;

	/** Cast pointer to the Chaos Vehicle movement component */
	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;


	/** Audio Component Boost */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boost, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* BoostAudioComponent;

protected:

	/** Steering Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SteeringAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BoostAction;
	
	/** Throttle Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ThrottleAction;

	/** Brake Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BrakeAction;

	/** Handbrake Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HandbrakeAction;

	/** Look Around Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAroundAction;

	/** Toggle Camera Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ToggleCameraAction;

	/** Reset Vehicle Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ResetVehicleAction;
	
	/** Reset Vehicle Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseMenuAction;

	/** Boost Management */

	/** Activate/Deactivate boost*/
	void PlayBoostSound();

	void StopBoostSound();
	
	/** Boost Level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostMeter = 100.0f; 

	/** Boost comsumption over second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostConsumption = 40.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostMax = 100.0f;
	
	bool IsDrifting = false;
	
	/** Boost recharge rate over second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostRechargeRate = 10.0f; 

	
	UFUNCTION(BlueprintCallable, Category = "Boost")
	void SetBoostingInput(bool bIsBoostingP){ bIsBoosting = bIsBoostingP; };
	bool GetBoostingInput(){ return bIsBoosting; };
	
	bool bCanBoost = true;
	bool bIsBoosting = false;
	bool bStopsBoosting = false;

	/** Camera management */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOffset")
	FVector vCameraBoostOffsetPosition = FVector (0.0f,0.0f,0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraOffset")
	FRotator rCameraBoostOffsetRotator = FRotator (0.0f,0.0f,0.0f);;
	
	/** Keeps track of which camera is active */
	bool bFrontCameraActive = false;

	void TorqueCuttingFix();
	float MultiplyTorque(float GearTorque);

	
public:
	AMyProjectPawn();

	FORCEINLINE float GetBoostMeter() const { return BoostMeter; }
	
	// Begin Pawn interface

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// End Pawn interface

	// Begin Actor interface

	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;

	// End Actor interface

	UFUNCTION(BlueprintCallable, Category = "MonCategorie")
	void SetRespawnLocation(FVector _RespawnLocation);

	UFUNCTION(BlueprintCallable, Category = "MonCategorie")
	FVector GetRespawnLocation() const { return RespawnLocation; }

	UFUNCTION(BlueprintCallable, Category = "Boost")
	void ReloadBoost();

protected:

	/** Handles steering input */
	void Steering(const FInputActionValue& Value);

	/** Handles throttle input */
	void Throttle(const FInputActionValue& Value);

	/** Handles boost input */
	void Boost(const FInputActionValue& Value);
	
	/** Handles boost start/stop input */
	void ActivateBoost(const FInputActionValue& Value);
	void DeactivateBoost(const FInputActionValue& Value);
	
	/** Handles brake input */
	void Brake(const FInputActionValue& Value);

	/** Handles brake start/stop inputs */
	void StartBrake(const FInputActionValue& Value);
	void StopBrake(const FInputActionValue& Value);

	/** Handles handbrake start/stop inputs */
	void StartHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);

	/** Handles look around input */
	void LookAround(const FInputActionValue& Value);

	/** Handles toggle camera input */
	void ToggleCamera(const FInputActionValue& Value);

	/** Handles reset vehicle input */
	void ResetVehicle(const FInputActionValue& Value);

	/** Handles Pause menu display */
	void SetPauseMenu(const FInputActionValue& Value);
	
	/** Called when the brake lights are turned on or off */
	UFUNCTION(BlueprintImplementableEvent, Category="Vehicle")
	void BrakeLights(bool bBraking);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* BoostParticlesLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* BoostParticlesRight;

	

public:
	/** Returns the front spring arm subobject */
	FORCEINLINE USpringArmComponent* GetFrontSpringArm() const { return FrontSpringArm; }
	/** Returns the front camera subobject */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FrontCamera; }
	/** Returns the back spring arm subobject */
	FORCEINLINE USpringArmComponent* GetBackSpringArm() const { return BackSpringArm; }
	/** Returns the back camera subobject */
	FORCEINLINE UCameraComponent* GetBackCamera() const { return BackCamera; }
	/** Returns the cast Chaos Vehicle Movement subobject */
	FORCEINLINE const TObjectPtr<UChaosWheeledVehicleMovementComponent>& GetChaosVehicleMovement() const { return ChaosVehicleMovement; }

private:
	bool EnablePowerCutting = true;

	/** Variable used to manage menu display*/
	bool IsMenuActive = false;

	FVector RespawnLocation;
	

	UPROPERTY(EditAnywhere, Category="DeadZone")
	float DeadZonCar;
	
};
