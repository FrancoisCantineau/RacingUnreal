// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"
UENUM(BlueprintType)
enum class ECheckPointState : uint8
{
	Begin UMETA(DisplayName = "Begin"),
	Mid UMETA(DisplayName = "Mid"),
	End UMETA(DisplayName = "End")
};

UCLASS()
class MYPROJECT_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Bool")
	void SetIsEnter(bool _isEnter);
	bool GetIsEnter();
	void SetId(int _id);
	UFUNCTION(BlueprintCallable, Category = "ID")
	int32 GetId();
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetState(ECheckPointState _state);
	UFUNCTION(BlueprintCallable, Category = "State")
	ECheckPointState GetState();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditAnywhere,Blueprintable, Category="Bool")
	bool IsEnter;
	UPROPERTY(EditAnywhere, Blueprintable ,Category="ID")
	int32 Id;

	UPROPERTY(EditAnywhere, Blueprintable ,Category="State")
	ECheckPointState CheckPointState;
	

};
