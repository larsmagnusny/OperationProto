// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

UWeapon::UWeapon()
{

}

void UWeapon::Update(float DeltaTime)
{

}

void UWeapon::GetPointingAt(FHitResult & hit)
{
	hit = FHitResult();
}

void UWeapon::Fire(bool& canFireAfter)
{

}

bool UWeapon::ReadyToFire()
{
	return false;
}

int UWeapon::GetAmmoCount()
{
	return 0;
}

int UWeapon::GetMax()
{
	return 0;
}

void UWeapon::AddAmmo(int amount)
{

}
