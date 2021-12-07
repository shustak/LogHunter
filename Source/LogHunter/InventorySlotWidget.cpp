// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlotWidget.h"
#include "LogHunter.h"
#include "LogHunterCharacter.h"

void UInventorySlotWidget::SetItemTexture(AItem* Item)
{
	(Item) ? ItemTexture = Item->GetPickupTexture() : ItemTexture = nullptr;
}
