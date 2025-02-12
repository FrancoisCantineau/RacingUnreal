// Copyright Epic Games, Inc. All Rights Reserved.;


#include "MyProjectWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UMyProjectWheelRear::UMyProjectWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
	CorneringStiffness = 250.f;
	FrictionForceMultiplier = 2.f;
	MaxWheelspinRotation = 25000.f;
	ExternalTorqueCombineMethod = ETorqueCombineMethod::Additive;
}