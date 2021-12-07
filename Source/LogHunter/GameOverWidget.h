// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/UMG/Public/Components/TextBlock.h>
#include <Runtime/UMG/Public/Components/Button.h>
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGHUNTER_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString GhostWhoKilledThePlayer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextBlock* GameOverText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextBlock* WhichGhostKilledAPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UButton* Retly;
};
