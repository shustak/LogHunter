// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h" 
#include "LogHunterCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AGhostAIController::AGhostAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AGhostAIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AGhostAIController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Something went wrong"));
	}
	
}

void AGhostAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AGhostAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGhostCharacter* GCharacter = Cast<AGhostCharacter>(GetPawn());

	if (DistanceToPlayer > AISightRadius)
	{
		bIsPlayerDetected = false;
	}

	if (GCharacter && bIsPlayerDetected == false)
	{
		if (GCharacter->NextWaypoint != nullptr)
		{
			MoveToActor(GCharacter->NextWaypoint, 5.0f);
		}
	}
	else if (bIsPlayerDetected == true)
	{
		ALogHunterCharacter* Player = Cast<ALogHunterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(Player, 5.0f);
	}
}

FRotator AGhostAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AGhostAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		ALogHunterCharacter* PlayerCharacter = Cast<ALogHunterCharacter>(DetectedPawns[i]);
		if (PlayerCharacter) 
		{
			DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);

			bIsPlayerDetected = true;
		}
	}

}
