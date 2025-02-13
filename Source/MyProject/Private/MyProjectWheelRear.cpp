// Copyright Epic Games, Inc. All Rights Reserved.;


#include "MyProjectWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UMyProjectWheelRear::UMyProjectWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
	FrictionForceMultiplier = 2.6f; 
    CorneringStiffness = 270.f;
	MaxWheelspinRotation = 20000.f;
	ExternalTorqueCombineMethod = ETorqueCombineMethod::Additive;
}