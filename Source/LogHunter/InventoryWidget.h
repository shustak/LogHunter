// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOGHUNTER_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = Animations)
	void Show();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AItem*> ItemsArray;
};
