// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "LogHunter.h"
#include "Engine.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupSM"));
	PickupTexture = CreateDefaultSubobject<UTexture2D>(TEXT("ItemTexture"));

	RootComponent = PickupSM;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::SetGlowEffect(bool Status)
{
	PickupSM->SetRenderCustomDepth(Status);
}

