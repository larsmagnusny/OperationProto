// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetableActor.h"

ATargetableActor::ATargetableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

int ATargetableActor::GetHP()
{
	return HP;
}

void ATargetableActor::ApplyDamage(int d)
{
	HP -= d;
}

bool ATargetableActor::isDead()
{
	return HP <= 0;
}
