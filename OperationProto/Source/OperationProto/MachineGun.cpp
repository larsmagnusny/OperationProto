// Fill out your copyright notice in the Description page of Project Settings.

#include "MachineGun.h"
#include "OperationProtoCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/DecalComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

UMachineGun::UMachineGun()
{
	SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	bCastDynamicShadow = false;
	CastShadow = false;

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(this);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.0f, 100.0f, 10.6f));

	// Load Decal
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalLoader(TEXT("MaterialInterface'/Game/FirstPerson/Textures/Decals/BulletHoledecal.BulletHoledecal'"));
	// Load Particle System
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleLoader(TEXT("ParticleSystem'/Game/FirstPerson/ParticleEffects/Hit_System.Hit_System'"));

	// Load misc
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshLoader(TEXT("SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialLoader(TEXT("Material'/Game/FirstPerson/FPWeapon/Materials/M_FPGun.M_FPGun'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> FireSoundLoader(TEXT("SoundCue'/Game/FirstPerson/Audio/MachineGun.MachineGun'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> EmptySoundLoader(TEXT("SoundWave'/Game/FirstPerson/Audio/GunClick.GunClick'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimationLoader(TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'"));

	Mesh = MeshLoader.Object;
	Material = MaterialLoader.Object;
	FireSound = FireSoundLoader.Object;
	EmptySound = EmptySoundLoader.Object;
	FireAnimation = FireAnimationLoader.Object;

	decal = DecalLoader.Object;
	particleSystem = ParticleLoader.Object;

	GunSoundPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("MachineGunSound"));
	GunSoundPlayer->bAutoActivate = false;
	GunSoundPlayer->SetupAttachment(this);

	if (GunSoundPlayer->IsValidLowLevelFast())
	{
		GunSoundPlayer->SetSound(FireSound);
	}
}

void UMachineGun::BeginPlay()
{
	Super::BeginPlay();

	SetSkeletalMesh(Mesh, true);
	SetMaterial(0, Material);
}

void UMachineGun::GetPointingAt(FHitResult & hit)
{
	float Pitch = Cast<AOperationProtoCharacter>(GetOwner())->Mesh1P->GetComponentRotation().Pitch;
	float Yaw = GetComponentRotation().Yaw + 90;

	FRotator Rot = FRotator(Pitch, Yaw, 0);

	FVector Start = FP_MuzzleLocation->GetComponentLocation();
	FVector End = Start + Rot.Vector() * 100000;

	GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECollisionChannel::ECC_Visibility);
}

void UMachineGun::Update(float DeltaTime)
{
	if (hasFired)
	{
		if (FireTimer < FireCooldown)
		{
			FireTimer += DeltaTime;
		}
		else
		{
			FireTimer = 0.f;
			hasFired = false;
		}
	}
}

void UMachineGun::Fire(bool & canFireAfter)
{
	if (ammoCount > 0)
	{
		// Raycast to object you are aiming at...
		FHitResult Hit;

		float Pitch = Cast<AOperationProtoCharacter>(GetOwner())->Mesh1P->GetComponentRotation().Pitch;
		float Yaw = GetComponentRotation().Yaw + 90;

		FRotator Rot = FRotator(Pitch, Yaw, 0);

		FVector Start = FP_MuzzleLocation->GetComponentLocation();
		FVector End = Start + Rot.Vector() * 100000;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility);


		if (Hit.Actor != nullptr)
		{
			// Spawn a decal on the surface you hit, and maybe play a partice effect?

			UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decal, FVector(10, 10, 10), Hit.Location, Hit.ImpactNormal.ToOrientationRotator());
			DecalComponent->SetFadeScreenSize(0.0001f);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleSystem, Hit.Location, Hit.ImpactNormal.ToOrientationRotator(), true);
			UE_LOG(LogTemp, Warning, TEXT("Spawned Decal"));
		}

		// try and play the sound if specified
		if (FireSound != NULL)
		{
			GunSoundPlayer->Play();
		}

		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Cast<AOperationProtoCharacter>(GetOwner())->Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		ammoCount--;
	}
	else
	{
		if (EmptySound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, EmptySound, GetOwner()->GetActorLocation());
		}

		canFireAfter = false;
	}

	hasFired = true;
}

bool UMachineGun::ReadyToFire()
{
	return !hasFired;
}

int UMachineGun::GetAmmoCount()
{
	return ammoCount;
}

int UMachineGun::GetMax()
{
	return maxAmmo;
}

void UMachineGun::AddAmmo(int amount)
{
	if (ammoCount + amount <= maxAmmo)
		ammoCount += amount;
	else
		ammoCount = maxAmmo;
}
