// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "WinGameWidget.h"
#include "GameOverWidget.h"
#include "LogHunterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LOGHUNTER_API ALogHunterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UInventoryWidget* InventoryWidgetRef;

	UWinGameWidget* WinGameWidgetRef;

	UGameOverWidget* GameOverWidgetRef;

	bool bIsInventoryOpen;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWinGameWidget> WinWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameOverWidget> GameOverWidgetBP;

public:
	virtual void OnPossess(APawn* InPawn) override;

	void HandleInventoryInput();

	void HandlePlayerState();
};
