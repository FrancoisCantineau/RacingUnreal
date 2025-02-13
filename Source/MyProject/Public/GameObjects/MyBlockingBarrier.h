#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBlockingBarrier.generated.h"

UENUM(BlueprintType)


enum class EBarrierState : uint8
{
	Begin UMETA(DisplayName = "Begin"),
	Mid UMETA(DisplayName = "Mid"),
	End UMETA(DisplayName = "End"),
	All UMETA(DisplayName = "All")
};

UCLASS()

class AMyBlockingBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyBlockingBarrier();

	/** Barrier's state to set active or deactivate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrier Settings")
	EBarrierState BarrierState;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "Barrier"))
	class UStaticMeshComponent* MeshComp;
private:
	
	virtual void Tick(float DeltaTime) override;
	

};
