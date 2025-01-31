#include "Car/PlayerCar.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

APlayerCar::APlayerCar()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void APlayerCar::BeginPlay()
{
	Super::BeginPlay();

	PlayerCarController = Cast<APlayerController>(GetController());
	if (PlayerCarController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCarController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}
}
void APlayerCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void APlayerCar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (CarMovements)
		{
			EnhancedInputComponent->BindAction(CarMovements, ETriggerEvent::Triggered, this, &APlayerCar::Move);
		}
		if (CarBoost)
		{
			EnhancedInputComponent->BindAction(CarBoost, ETriggerEvent::Triggered, this, &APlayerCar::Boost);
		}
	}
}
void APlayerCar::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();
	
	if (Controller)
	{
		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation.X = MovementVector.Y*carSpeed*UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalOffset(DeltaLocation, true);

		FRotator DeltaRotation = FRotator::ZeroRotator;
		DeltaRotation.Yaw = MovementVector.X * turnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalRotation(DeltaRotation, true);
	}
}
void APlayerCar::Boost()
{
	
}