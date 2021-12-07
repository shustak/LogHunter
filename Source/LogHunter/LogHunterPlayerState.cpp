// Fill out your copyright notice in the Description page of Project Settings.


#include "LogHunterPlayerState.h"
#include "Net/UnrealNetwork.h"

ALogHunterPlayerState::ALogHunterPlayerState()
{
	Points = 0;
}

void ALogHunterPlayerState::IncrementPoints(uint16 Value)
{
	Points += Value;
}

int32 ALogHunterPlayerState::GetPoints()
{
	return Points;
}

void ALogHunterPlayerState::SetPlayerCaught(bool bPlayerCought)
{
	bPlayerCaught = bPlayerCought;
}

bool ALogHunterPlayerState::IsPlayerCought()
{
	return bPlayerCaught;
}
