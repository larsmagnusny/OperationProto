// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OperationProtoCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "OperationProtoHUD.h"
#include "HandGun.h"
#include "MachineGun.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AOperationProtoCharacter

AOperationProtoCharacter::AOperationProtoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun here...

	FP_Gun.Add(CreateDefaultSubobject<UHandGun>(TEXT("FP_Gun")));
	//FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun[0]->SetupAttachment(RootComponent);
	FP_Gun.Add(CreateDefaultSubobject<UMachineGun>(TEXT("FP_MachineGun")));
	FP_Gun[0]->SetupAttachment(RootComponent);
}

void AOperationProtoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	for(int i = 0; i < NumWeapons; i++)
		FP_Gun[i]->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AOperationProtoCharacter::Tick(float DeltaTime)
{
	// Calculate firecooldown
	FHitResult Hit;

	if (FP_Gun[CurrentWeapon]->ReadyToFire() && firePressed)
		FP_Gun[CurrentWeapon]->Fire(firePressed);

	FP_Gun[CurrentWeapon]->Update(DeltaTime);
	FP_Gun[CurrentWeapon]->GetPointingAt(Hit);

	FVector2D retPos;

	if (Hit.GetActor())
	{
		//DrawDebugLine(GetWorld(), Start + FirstPersonCameraComponent->GetForwardVector()*200, Hit.Location, FColor(255, 0, 0), false, -1.f, 0, 3);
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), Hit.Location, retPos, false);

		// Update reticle position
		AOperationProtoHUD* h = Cast<AOperationProtoHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		h->XPos = retPos.X;
		h->YPos = retPos.Y;
	}
}

void AOperationProtoCharacter::SwitchWeaponUp()
{
	if (CurrentWeapon < NumWeapons - 1)
		CurrentWeapon++;
	else if (CurrentWeapon == NumWeapons - 1)
		CurrentWeapon = 0;
}

void AOperationProtoCharacter::SwitchWeaponDown()
{
	if (CurrentWeapon > 0)
		CurrentWeapon--;
	else if (CurrentWeapon == 0)
		CurrentWeapon = NumWeapons - 1;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOperationProtoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AOperationProtoCharacter::FireDown);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AOperationProtoCharacter::FireUp);

	PlayerInputComponent->BindAction("SwitchWeaponUp", IE_Pressed, this, &AOperationProtoCharacter::SwitchWeaponUp);
	PlayerInputComponent->BindAction("SwitchWeaponDown", IE_Pressed, this, &AOperationProtoCharacter::SwitchWeaponDown);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOperationProtoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOperationProtoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AOperationProtoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AOperationProtoCharacter::LookUpAtRate);
}

void AOperationProtoCharacter::FireDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Down"));
	firePressed = true;
}

void AOperationProtoCharacter::FireUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Up"));
	firePressed = false;
}

void AOperationProtoCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AOperationProtoCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AOperationProtoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AOperationProtoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

int AOperationProtoCharacter::GetAmmoCount()
{
	return FP_Gun[CurrentWeapon]->GetAmmoCount();
}

int AOperationProtoCharacter::GetMax()
{
	return FP_Gun[CurrentWeapon]->GetMax();
}

void AOperationProtoCharacter::AddAmmo(int amount)
{
	if(FP_Gun[CurrentWeapon])
		FP_Gun[CurrentWeapon]->AddAmmo(amount);
}
