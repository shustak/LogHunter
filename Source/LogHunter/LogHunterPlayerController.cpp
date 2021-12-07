// Fill out your copyright notice in the Description page of Project Settings.

#include "LogHunterPlayerController.h"
#include "LogHunter.h"
#include "LogHunterGameMode.h"
#include "LogHunterCharacter.h"

void ALogHunterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InventoryWidgetBP)
	{
		InventoryWidgetRef = CreateWidget<UInventoryWidget>(this, InventoryWidgetBP);
	}

	if (WinWidgetBP)
	{
		WinGameWidgetRef = CreateWidget<UWinGameWidget>(this, WinWidgetBP);
	}

	if (GameOverWidgetBP)
	{
		GameOverWidgetRef = CreateWidget<UGameOverWidget>(this, GameOverWidgetBP);
	}

	bIsInventoryOpen = false;
}

void ALogHunterPlayerController::HandleInventoryInput()
{
	ALogHunterCharacter* PlayerCharacter = Cast<ALogHunterCharacter>(GetPawn());

	if (InventoryWidgetRef)
	{
		if (bIsInventoryOpen)
		{
			bIsInventoryOpen = false;

			InventoryWidgetRef->RemoveFromViewport();
		}
		else
		{
			bIsInventoryOpen = true;

			InventoryWidgetRef->ItemsArray = PlayerCharacter->GetInventory();

			InventoryWidgetRef->Show();
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget is not set"));
}

void ALogHunterPlayerController::HandlePlayerState()
{
	ALogHunterCharacter* PlayerCharacter = Cast<ALogHunterCharacter>(GetPawn());

	if (PlayerCharacter)
	{
		ALogHunterGameMode* GameMode = (ALogHunterGameMode*)GetWorld()->GetAuthGameMode();

		if (GameMode)
		{
			if (GameMode->GetCurrentState() == EGameState::EWon)
			{
				if (WinGameWidgetRef && !WinGameWidgetRef->IsInViewport())
				{
					WinGameWidgetRef->AddToViewport();

					this->Pause();
					this->bShowMouseCursor = true;
					this->bEnableClickEvents = true;
					this->bEnableMouseOverEvents = true;
				}
				else
					UE_LOG(LogTemp, Warning, TEXT("WinGameWidget is not set"));
			}
			else if (GameMode->GetCurrentState() == EGameState::EGameOver)
			{
				if (GameOverWidgetRef && !GameOverWidgetRef->IsInViewport())
				{
					GameOverWidgetRef->GhostWhoKilledThePlayer = PlayerCharacter->GhostNameWhoKilledThePlayer.ToString();
					GameOverWidgetRef->AddToViewport();

					this->Pause();
					this->bShowMouseCursor = true;
					this->bEnableClickEvents = true;
					this->bEnableMouseOverEvents = true;
				}
				else
					UE_LOG(LogTemp, Warning, TEXT("GameOverWidget is not set"));
			}
		}
	}
}
