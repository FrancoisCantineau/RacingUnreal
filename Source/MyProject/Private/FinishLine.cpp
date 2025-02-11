#include "FinishLine.h"

#include "MyProjectGameMode.h"
#include "MyProjectPawn.h"


AFinishLine::AFinishLine()
{
	PrimaryActorTick.bCanEverTick = true;

	FinishTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FinishTrigger"));
	RootComponent = FinishTrigger;
	
	FinishTrigger->SetGenerateOverlapEvents(true);
	
}

void AFinishLine::BeginPlay()
{
		
	Super::BeginPlay();
	FinishTrigger->OnComponentBeginOverlap.AddDynamic(this,&AFinishLine::OverlapBegin);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Begin");
	
}

void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishLine::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if the actor hit is a car
	if (AMyProjectPawn* Car = Cast<AMyProjectPawn>(OtherActor))
	{

		AMyProjectGameMode* GM = Cast<AMyProjectGameMode>(GetWorld()->GetAuthGameMode());
		if (GM->AllTrue())
		{
			GM->AddLap();
			if (GM->GetEndGame())
			{
				GM->StopRace(OtherActor);
			}
		}
		
		
		
		
	}
}
