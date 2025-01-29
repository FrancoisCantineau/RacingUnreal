#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// LAST INCLUDE 
#include "CarPawn.generated.h"

// FORWARD DECLARATION
class UBoxComponent;

UCLASS()
class RACINGPROJECTUNREAL_API ACarPawn : public APawn
{
	GENERATED_BODY()

public:

	ACarPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;
	
	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float carSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float turnRate = 5.f;
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrontLeftWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrontRightWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BackLeftWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BackRightWheel;
	
	

};
