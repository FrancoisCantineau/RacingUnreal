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

	Pivot_Wheel_FL = CreateDefaultSubobject<USceneComponent>(TEXT("FrontLeft"));
	Pivot_Wheel_FL->SetupAttachment(BaseMesh);

	Pivot_Wheel_FR = CreateDefaultSubobject<USceneComponent>(TEXT("FrontRight"));
	Pivot_Wheel_FR->SetupAttachment(BaseMesh);

	Pivot_Wheel_RL = CreateDefaultSubobject<USceneComponent>(TEXT("RearLeft"));
	Pivot_Wheel_RL->SetupAttachment(BaseMesh);

	Pivot_Wheel_RR = CreateDefaultSubobject<USceneComponent>(TEXT("RearRight"));
	Pivot_Wheel_RR->SetupAttachment(BaseMesh);
	
	Wheel_FL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front left wheel mesh"));
	Wheel_FL->SetupAttachment(BaseMesh);
	
	Wheel_FR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front right wheel mesh"));
	Wheel_FR->SetupAttachment(BaseMesh);

	Wheel_RL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rear left wheel mesh"));
	Wheel_RL->SetupAttachment(BaseMesh);

	Wheel_RR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rear right wheel mesh"));
	Wheel_RR->SetupAttachment(BaseMesh);
	

}

void ACarPawn::BeginPlay()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hello Unreal!"));
	
	Super::BeginPlay();

	Pivot_Wheel_FL->SetWorldLocation(Wheel_FL->GetComponentLocation());
	Pivot_Wheel_FR->SetWorldLocation(Wheel_FR->GetComponentLocation());
	Pivot_Wheel_RL->SetWorldLocation(Wheel_RL->GetComponentLocation());
	Pivot_Wheel_RR->SetWorldLocation(Wheel_RR->GetComponentLocation());

	Wheel_FL->AttachToComponent(Pivot_Wheel_FL, FAttachmentTransformRules::KeepWorldTransform);
	Wheel_FR->AttachToComponent(Pivot_Wheel_FR, FAttachmentTransformRules::KeepWorldTransform);
	Wheel_RL->AttachToComponent(Pivot_Wheel_RL, FAttachmentTransformRules::KeepWorldTransform);
	Wheel_RR->AttachToComponent(Pivot_Wheel_RR, FAttachmentTransformRules::KeepWorldTransform);
}

void ACarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

