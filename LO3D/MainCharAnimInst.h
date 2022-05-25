// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponType.h"
#include "MainCharAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UMainCharAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;

private:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimProperty(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	bool bIsAccelerating;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	float YawValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	float PitchValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	bool bIsCrouch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	bool bIsBattle;


	class AMainCharacter* MainCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimProperty", meta = (AllowPrivateAccess = true))
	bool IsZooming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AnimProperty", meta=(AllowPrivateAccess=true))
	EWeaponType EquippedWeaponType;

public:
};
