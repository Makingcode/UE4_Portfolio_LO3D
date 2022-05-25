// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharAnimInst.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/KismetMathLibrary.h"
#include "WeaponGun.h"

void UMainCharAnimInst::NativeInitializeAnimation()
{
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainCharAnimInst::UpdateAnimProperty(float DeltaTime)
{
	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}

	if (MainCharacter)
	{
		bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();

		FVector CharacterVelocity = MainCharacter->GetVelocity();
		CharacterVelocity.Z = 0.f;
		Speed = CharacterVelocity.Size();

		bIsCrouch = MainCharacter->GetIsCrouch();
		bIsBattle = MainCharacter->GetIsBattle();

		if (MainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		if (MainCharacter->GetEquippedWeapon())
		{
			EquippedWeaponType = MainCharacter->GetEquippedWeapon()->GetWeaponType();
		}


		FRotator AimRotation = MainCharacter->GetBaseAimRotation();
		FRotator XAxisRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());
		YawValue = UKismetMathLibrary::NormalizedDeltaRotator(XAxisRotation,AimRotation).Yaw;
		
		PitchValue = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, MainCharacter->GetActorRotation()).Pitch;

		IsZooming = MainCharacter->GetZooming();

	}
}
