// Fill out your copyright notice in the Description page of Project Settings.

#include "Waypoint.h"

AWaypoint::AWaypoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaypoint::OnPlayerEnter);
}

void AWaypoint::BeginPlay()
{
	Super::BeginPlay();
}

void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaypoint::OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AGhostCharacter* GhostCharacter = Cast<AGhostCharacter>(OtherActor);

		if (GhostCharacter)
		{
			GhostCharacter->NextWaypoint = NextWaypoint;
		}
	}
}
