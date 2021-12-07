// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGHUNTER_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* ItemTexture;

public:
	UFUNCTION(BlueprintCallable, Category = UI)
	void SetItemTexture(AItem* Item);
};
