// Copyright Epic Games, Inc. All Rights Reserved.

#include "LogHunterCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "LogHunterPlayerState.h"
#include "Containers/Array.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ALogHunterCharacter

ALogHunterCharacter::ALogHunterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.5, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ALogHunterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Raycast();

	ALogHunterPlayerController* PlayerControler = Cast<ALogHunterPlayerController>(GetController());
	if (PlayerControler != nullptr)
	{
		PlayerControler->HandlePlayerState();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALogHunterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALogHunterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALogHunterCharacter::MoveRight);

	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &ALogHunterCharacter::PickupItem);
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &ALogHunterCharacter::DropEquippedItem);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ALogHunterCharacter::HandleInventoryInput);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALogHunterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALogHunterCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALogHunterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALogHunterCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALogHunterCharacter::OnResetVR);
}

void ALogHunterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Initializing our reference
	LastItemSeen = nullptr;

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ALogHunterCharacter::OnGhostOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ALogHunterCharacter::OnGhostOverlapEnd);
}

void ALogHunterCharacter::OnResetVR()
{
	// If LogHunter is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in LogHunter.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALogHunterCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALogHunterCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALogHunterCharacter::PickupItem()
{
	if (LastItemSeen)
	{
		uint32 AvailableSlot = Inventory.Num();

		if (AvailableSlot < MAX_INVENTORY_ITEMS)
		{
			Inventory.Emplace(LastItemSeen);
			LastItemSeen->Destroy();
		}
		else GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("You can't carry any more items!")));
	}
}

void ALogHunterCharacter::DropEquippedItem()
{
	if (Inventory.Num() > 0)
	{
		FVector DropLocation = GetActorLocation() + (GetActorForwardVector() * 200);

		FTransform Transform; Transform.SetLocation(DropLocation);

		FActorSpawnParameters SpawnParams;

		AItem* LastItem = Inventory.Last();

		if (LastItem)
		{
			AItem* PickupToSpawn = GetWorld()->SpawnActor<AItem>(LastItem->GetClass(), Transform, SpawnParams);

			if (PickupToSpawn)
			{
				Inventory.RemoveAt(0);
			}
		}
	}
}

void ALogHunterCharacter::Raycast()
{
	FVector StartLocation = GetActorLocation();
	StartLocation.Z = StartLocation.Z - 80.0f;

	FVector ForwardVector = CameraBoom->GetForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * RaycastRange);

	FHitResult RaycastHit;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this->GetOwner());

	GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CQP);

	AItem* Item = Cast<AItem>(RaycastHit.GetActor());
	 
	if (LastItemSeen && LastItemSeen != Item)
	{
		LastItemSeen->SetGlowEffect(false);
	}
	if (Item)
	{
		LastItemSeen = Item;
		Item->SetGlowEffect(true);
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, FString::Printf(TEXT("Press E to pickup Log")));
	}
	else
		LastItemSeen = nullptr;
}


void ALogHunterCharacter::HandleInventoryInput()
{
	ALogHunterPlayerController* PlayerControler = Cast<ALogHunterPlayerController>(GetController());
	if (PlayerControler != nullptr)
	{
		PlayerControler->HandleInventoryInput();
	}
}

void ALogHunterCharacter::OnGhostOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AGhostCharacter* GhostCharacter = Cast<AGhostCharacter>(OtherActor);
		if (GhostCharacter != nullptr)
		{
			ALogHunterPlayerState* PState = this->GetPlayerState<ALogHunterPlayerState>();

			if (PState)
			{
				GhostNameWhoKilledThePlayer = GhostCharacter->GetFName();
				PState->SetPlayerCaught(true);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LogHunterPlayerState is not set"));
			}
		}
	}
}

void ALogHunterCharacter::OnGhostOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ALogHunterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALogHunterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALogHunterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALogHunterCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

