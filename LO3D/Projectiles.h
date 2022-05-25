// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Projectiles.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API AProjectiles : public AItem
{
	GENERATED_BODY()
	
public:
	AProjectiles();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void MissileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProjectileProperty", meta=(AllowPrivateAccess=true))
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category="ProjectileProperty", meta=(AllowPrivateAccess=true))
	class UProjectileMovementComponent* ProjMovement;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperty", meta = (AllowPrivateAccess = true))
	class UParticleSystem* MissileMuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperty", meta = (AllowPrivateAccess = true))
	class UParticleSystem* BombImpact;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperty", meta = (AllowPrivateAccess = true))
	float MissileDamage;
	
	UPROPERTY(EditAnywhere, Category = "ProjectileProperty", meta = (AllowPrivateAccess = true))
	class USoundCue* BoomSound;

public:
	FORCEINLINE UProjectileMovementComponent* GetProjMovement() const { return ProjMovement; }
	FORCEINLINE UParticleSystem* GetMissileMuzzleFlash() const { return MissileMuzzleFlash; }
	FORCEINLINE UParticleSystem* GetBombImpact() const { return BombImpact; }

};
