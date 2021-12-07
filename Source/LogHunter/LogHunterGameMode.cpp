// Copyright Epic Games, Inc. All Rights Reserved.

#include "LogHunterGameMode.h"
#include "LogHunterCharacter.h"
#include "LogHunterPlayerState.h"
#include "UObject/ConstructorHelpers.h"

ALogHunterGameMode::ALogHunterGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EGameState::EPlaying;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALogHunterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALogHunterCharacter* PlayerCharacter = Cast<ALogHunterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (PlayerCharacter)
	{
		ALogHunterPlayerState* PState = PlayerCharacter->GetPlayerState<ALogHunterPlayerState>();

		if (PState)
		{
			if (PState->GetPoints() >= PState->PointsToWin)
			{
				SetCurrentState(EGameState::EWon);
			}
			else if (PState->IsPlayerCought())
			{
				SetCurrentState(EGameState::EGameOver);
			}
		}
	}
}

void ALogHunterGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EGameState::EPlaying);
}

EGameState ALogHunterGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ALogHunterGameMode::SetCurrentState(EGameState NewState)
{
	CurrentState = NewState;
}
