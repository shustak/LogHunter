// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "LogHunterGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ALogHunterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALogHunterGameMode();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "GameState")
	EGameState GetCurrentState() const;

	void SetCurrentState(EGameState NewState);

private:
	EGameState CurrentState;
};



