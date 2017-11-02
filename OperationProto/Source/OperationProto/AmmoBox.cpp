// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoBox.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "OperationProtoCharacter.h"

AAmmoBox::AAmmoBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AAmmoBox::OnPickup);
}

void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->bGenerateOverlapEvents = true;
}

void AAmmoBox::OnPickup(AActor* OurActor, AActor * OtherActor)
{
	// Find the character and give him the ammo this box contains...
	AOperationProtoCharacter* Character = Cast<AOperationProtoCharacter>(GetWorld()->GetFirstPlayerController()->GetControlledPawn());

	if (Character)
	{
		if (Character->GetAmmoCount() + ammoAmount <= Character->GetMax())
		{
			Character->AddAmmo(ammoAmount);
			Destroy(this); // CRASH?!
		}
		else
		{
			int delta = Character->GetMax() - Character->GetAmmoCount();
			Character->AddAmmo(delta);
			ammoAmount -= delta;
		}
	}
}
