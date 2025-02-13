#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FinishLine.generated.h"

class UBoxComponent;
class AGameManager;

UCLASS()
class MYPROJECT_API AFinishLine : public AActor
{
	GENERATED_BODY()
	
public:	

	AFinishLine();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	/** Box collider */
	UPROPERTY(VisibleAnywhere, Category="Components")
	UBoxComponent* FinishTrigger;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* RootScene;

	/** Manage collider overlap to end race */
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:	

	

};
