// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/UMG/Public/Components/TextBlock.h>
#include <Runtime/UMG/Public/Components/Button.h>
#include "WinGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGHUNTER_API UWinGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextBlock* WinText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UButton* GotoNextLevel;
};
