// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerBox.h"/////
#include "Item.h"
#include "LogHunterCharacter.h"
#include "FireplaceActor.generated.h"

UCLASS()
class LOGHUNTER_API AFireplaceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFireplaceActor();

	UFUNCTION()
	void OnItemEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	void HitTheMark(ALogHunterCharacter* Player);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
