#include "GameObjects/MyBlockingBarrier.h"
#include "GameObjects/MyBlockingBarrier.h"


AMyBlockingBarrier::AMyBlockingBarrier()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}


void AMyBlockingBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMyBlockingBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

