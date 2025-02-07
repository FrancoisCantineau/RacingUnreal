// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UMyProjectWheelFront::UMyProjectWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
	CorneringStiffness = 200.f;
	FrictionForceMultiplier = 1.8f;
	MaxSteerAngle = 65.f;
}