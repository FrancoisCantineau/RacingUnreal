#pragma once

#include "CoreMinimal.h"
#include "Car/CarPawn.h"
#include "InputActionValue.h"

// LAST INCLUDE 
#include "PlayerCar.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class RACINGPROJECTUNREAL_API APlayerCar : public ACarPawn
{
	GENERATED_BODY()

public:

	APlayerCar();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CarMovements;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CarBoost;
	
	void Move(const FInputActionValue& Value);
	void Boost();
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	APlayerController* PlayerCarController;;
};
