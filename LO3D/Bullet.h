// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API ABullet : public AItem
{
	GENERATED_BODY()

public:
	ABullet();
	
	class AMainCharacter* WeaponOwnerCharacter;
	class AWeaponGun* EquippedWeapon;

protected:
	UFUNCTION()
	void BulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

	void DoDamage(AActor* HitActor);


private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BulletProperty", meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* BulletMovement;

	UPROPERTY(EditAnywhere, Category="BulletProperty", meta=(AllowPrivateAccess=true))
	float BulletDamage;

	UPROPERTY(EditAnywhere, Category = "HitEffect", meta = (AllowPrivateAccess = true))
	class UParticleSystem* SandhitEffect;

public:
	FORCEINLINE UProjectileMovementComponent* GetBulletMovement() const { return BulletMovement; }
	FORCEINLINE float GetBulletDamage() { return BulletDamage; }
};
