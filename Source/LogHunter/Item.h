// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Item.generated.h"

UCLASS()
class LOGHUNTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PickupSM;

	void SetGlowEffect(bool Status);

	FORCEINLINE UTexture2D* GetPickupTexture() { return PickupTexture; }

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "ItemProperties")
	UTexture2D* PickupTexture;

	UPROPERTY(EditAnywhere, Category = "ItemProperties")
	FString ItemName;
};
