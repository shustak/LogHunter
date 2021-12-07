// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LogHunterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LOGHUNTER_API ALogHunterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ALogHunterPlayerState();

	void IncrementPoints(uint16 Value);

	int32 GetPoints();

	void SetPlayerCaught(bool bPlayerCought);

	bool IsPlayerCought();

	const int32 PointsToWin = 12;

protected:

	UPROPERTY(EditDefaultsOnly)
	int32 Points;

	bool bPlayerCaught = false;
};
