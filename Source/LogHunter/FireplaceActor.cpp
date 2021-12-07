// Fill out your copyright notice in the Description page of Project Settings.

#include "FireplaceActor.h"
#include "LogHunterPlayerState.h"
#include "Kismet/GameplayStatics.h"

AFireplaceActor::AFireplaceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireplaceActor::OnItemEnter);
}

void AFireplaceActor::OnItemEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AItem* Item = Cast<AItem>(OtherActor);
		
		if (Item)
		{
			ALogHunterCharacter* PlayerCharacter = Cast<ALogHunterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

			HitTheMark(PlayerCharacter);
			Item->Destroy();

			PlayerCharacter->Inventory.RemoveAt(0);
		}
	}
}

void AFireplaceActor::HitTheMark(ALogHunterCharacter* Player)
{
	if (Player)
	{
		ALogHunterPlayerState* PState = Player->GetPlayerState<ALogHunterPlayerState>();

		if (PState)
		{
			PState->IncrementPoints(1);

			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, FString::Printf(TEXT("You have %d POINTS! You still need %d points to win!"), PState->GetPoints(), (PState->PointsToWin - PState->GetPoints())));
		}
	}
}

void AFireplaceActor::BeginPlay()
{
	Super::BeginPlay();

}

void AFireplaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

