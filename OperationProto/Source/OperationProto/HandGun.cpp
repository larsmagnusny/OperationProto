// Fill out your copyright notice in the Description page of Project Settings.

#include "HandGun.h"

UHandGun::UHandGun()
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
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleLoader01(TEXT("ParticleSystem'/Game/FirstPerson/ParticleEffects/Hit_System.Hit_System'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleLoader02(TEXT("ParticleSystem'/Game/FirstPerson/ParticleEffects/Hit_System_Wood.Hit_System_Wood'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleLoader03(TEXT("ParticleSystem'/Game/FirstPerson/ParticleEffects/Hit_System_Metal.Hit_System_Metal'"));

	// Load misc
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshLoader(TEXT("SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialLoader(TEXT("Material'/Game/FirstPerson/FPWeapon/Materials/M_FPGun.M_FPGun'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> FireSoundLoader(TEXT("SoundCue'/Game/FirstPerson/Audio/GunCue.GunCue'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> EmptySoundLoader(TEXT("SoundWave'/Game/FirstPerson/Audio/GunClick.GunClick'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimationLoader(TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'"));

	Mesh = MeshLoader.Object;
	Material = MaterialLoader.Object;
	FireSound = FireSoundLoader.Object;
	EmptySound = EmptySoundLoader.Object;
	FireAnimation = FireAnimationLoader.Object;

	decal = DecalLoader.Object;
	particleSystems = new UParticleSystem*[3];

	particleSystems[0] = ParticleLoader01.Object;
	particleSystems[1] = ParticleLoader02.Object;
	particleSystems[2] = ParticleLoader03.Object;

	GunSoundPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("HandGunSound"));
	GunSoundPlayer->bAutoActivate = false;
	GunSoundPlayer->SetupAttachment(this);

	if (GunSoundPlayer->IsValidLowLevelFast())
	{
		GunSoundPlayer->SetSound(FireSound);
	}
}

void UHandGun::BeginPlay()
{
	Super::BeginPlay();

	SetSkeletalMesh(Mesh, true);
	SetMaterial(0, Material);
}

void UHandGun::GetPointingAt(FHitResult & hit)
{
	float Pitch = Cast<AOperationProtoCharacter>(GetOwner())->Mesh1P->GetComponentRotation().Pitch;
	float Yaw = GetComponentRotation().Yaw + 90;

	FRotator Rot = FRotator(Pitch, Yaw, 0);

	FVector Start = FP_MuzzleLocation->GetComponentLocation();
	FVector End = Start + Rot.Vector() * 100000;

	GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECollisionChannel::ECC_Visibility);
}

void UHandGun::Update(float DeltaTime)
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

void UHandGun::Fire(bool& canFireAfter)
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
			// Get the physics Material name, and spawn a fitting particlesystem based off that...

			UParticleSystem* particleSystem = nullptr;

			if (Hit.GetActor()->ActorHasTag(FName("Wood")))
			{
				particleSystem = particleSystems[1];
			}
			else if (Hit.GetActor()->ActorHasTag(FName("Metal")))
			{
				particleSystem = particleSystems[2];

				// Also play the sound
				UAudioComponent* Comp = Cast<UAudioComponent>(Hit.GetActor()->GetComponentByClass(UAudioComponent::StaticClass()));

				if (Comp)
				{
					if(!Comp->IsPlaying())
						Comp->Play();
				}
			}
			else
			{
				particleSystem = particleSystems[0];
			}

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
		canFireAfter = false;
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

bool UHandGun::ReadyToFire()
{
	return !hasFired;
}

int UHandGun::GetAmmoCount()
{
	return ammoCount;
}

int UHandGun::GetMax()
{
	return maxAmmo;
}

void UHandGun::AddAmmo(int amount)
{
	if (ammoCount + amount <= maxAmmo)
		ammoCount += amount;
	else
		ammoCount = maxAmmo;
}
