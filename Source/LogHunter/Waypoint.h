// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GhostCharacter.h"
#include "WayPoint.generated.h"

UCLASS()
class LOGHUNTER_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaypoint();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWaypoint* NextWaypoint;

protected:
	virtual void BeginPlay() override;
};
