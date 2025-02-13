// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/CheckPoint.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACheckPoint::SetIsEnter(bool _isEnter)
{
	IsEnter = _isEnter;
}

bool ACheckPoint::GetIsEnter()
{
	return IsEnter;
}

void ACheckPoint::SetId(int _id)
{
	Id = _id;
}

int ACheckPoint::GetId()
{
	return Id;
}

void ACheckPoint::SetState(ECheckPointState _state)
{
	CheckPointState = _state;
}
ECheckPointState ACheckPoint::GetState()
{
	return CheckPointState;
}


// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

