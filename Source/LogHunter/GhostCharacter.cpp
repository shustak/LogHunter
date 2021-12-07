// Fill out your copyright notice in the Description page of Project Settings.

#include "GhostCharacter.h"
#include "LogHunterCharacter.h"

AGhostCharacter::AGhostCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
}

void AGhostCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGhostCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGhostCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

