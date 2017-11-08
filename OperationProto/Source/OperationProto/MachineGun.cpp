// Fill out your copyright notice in the Description page of Project Settings.

#include "MachineGun.h"
#include "Enemy.h"

UMachineGun::UMachineGun()
{
	FireCooldown = 0.08f;
	ammoCount = 142;
	maxAmmo = 142;
	SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	bCastDynamicShadow = false;
	CastShadow = false;

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(this);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.0f, 100.0f, 10.6f));

	Mesh = Cast<USkeletalMesh>(Loader.GetObject("SK_FPGun"));
	Material = Cast<UMaterialInstance>(Loader.GetObject("M_FPGun"));
	FireSound = Cast<USoundCue>(Loader.GetObject("MachineGun"));
	EmptySound = Cast<USoundBase>(Loader.GetObject("GunClick"));
	FireAnimation = Cast<UAnimMontage>(Loader.GetObject("FirstPersonFire_Montage"));

	decals = new UMaterialInterface*[numDecals];

	decals[0] = Cast<UMaterialInterface>(Loader.GetObject("BulletHoledecal"));
	decals[1] = Cast<UMaterialInterface>(Loader.GetObject("BloodSplatter"));


	particleSystems = new UParticleSystem*[numParticleSystems];

	particleSystems[0] = Cast<UParticleSystem>(Loader.GetObject("Hit_System"));
	particleSystems[1] = Cast<UParticleSystem>(Loader.GetObject("Hit_System_Wood"));
	particleSystems[2] = Cast<UParticleSystem>(Loader.GetObject("Hit_System_Metal"));
	particleSystems[3] = Cast<UParticleSystem>(Loader.GetObject("BloodSplat"));

	GunSoundPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("HandGunSound"));
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

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.bTraceComplex = true;

	GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECollisionChannel::ECC_Visibility, Params);
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

		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.bTraceComplex = true;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, Params);

		FRotator ParticleSystemRotation = Hit.ImpactNormal.ToOrientationRotator();

		if (Hit.Actor != nullptr)
		{
			// Spawn a decal on the surface you hit, and maybe play a partice effect?
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
					if (!Comp->IsPlaying())
						Comp->Play();
				}
			}
			else if (Hit.GetActor()->IsA(AEnemy::StaticClass()))
			{
				AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor());

				if (Hit.BoneName == FName("neck_01"))
					Enemy->ApplyDamage(100);
				if (Hit.BoneName == FName("spine_03"))
					Enemy->ApplyDamage(34);
				if (Hit.BoneName == FName("pelvis") || Hit.BoneName == FName("spine_01") || Hit.BoneName == FName("spine_02"))
					Enemy->ApplyDamage(10);
				else
					Enemy->ApplyDamage(5);

				particleSystem = particleSystems[3];

				ParticleSystemRotation = Rot;
			}
			else
			{
				particleSystem = particleSystems[0];
			}

			UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decals[0], FVector(10, 10, 10), Hit.Location, Hit.ImpactNormal.ToOrientationRotator());
			DecalComponent->SetFadeScreenSize(0.0001f);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleSystem, Hit.Location, ParticleSystemRotation, true);
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
