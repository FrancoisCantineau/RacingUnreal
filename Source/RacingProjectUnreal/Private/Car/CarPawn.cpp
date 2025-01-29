#include "Car/CarPawn.h"
#include "Components/BoxComponent.h"

ACarPawn::ACarPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	BaseMesh->SetupAttachment(BoxComp);

	FrontLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front left wheel mesh"));
	FrontLeftWheel->SetupAttachment(BaseMesh);
	
	FrontRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front right wheel mesh"));
	FrontRightWheel->SetupAttachment(BaseMesh);

	BackLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back left wheel mesh"));
	BackLeftWheel->SetupAttachment(BaseMesh);

	BackRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back right wheel mesh"));
	BackRightWheel->SetupAttachment(BaseMesh);
	

}

void ACarPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

