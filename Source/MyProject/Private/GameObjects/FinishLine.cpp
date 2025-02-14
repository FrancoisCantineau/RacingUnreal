
#include "GameObjects/FinishLine.h"

#include "Management/MyProjectGameMode.h"
#include "Player/MyProjectPawn.h"


AFinishLine::AFinishLine()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	FinishTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FinishTrigger"));
	FinishTrigger->SetupAttachment(RootComponent);
	
	
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
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Enter");

		AMyProjectGameMode* GM = Cast<AMyProjectGameMode>(GetWorld()->GetAuthGameMode());
		

		if (GM->AllTrue())
		{
			GM->AddLap();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Current Lap: %d"), GM->GetCurrentLap()));
			if (GM->GetCurrentLap() >= GM->GetTotalLap() && GM->AllTrue())
			{
				//GM->StopRace(OtherActor);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ca switch"));
				GM->UpdateBarriers();
				GM->UpdateCheckPoint();
				GM->ResetCheckpoint();
				
			}
		}
		
	}
}
