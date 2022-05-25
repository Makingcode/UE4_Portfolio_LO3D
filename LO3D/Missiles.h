// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missiles.generated.h"

UCLASS()
class LO3D_API AMissiles : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissiles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	class USphereComponent* MissileHitColliision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* MissileSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* MissileStaticMesh;

	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess=true))
	class UProjectileMovementComponent* MissileMovement;

	UPROPERTY(EditAnywhere, Category = "Particle", meta = (AllowPrivateAccess = true))
	class UParticleSystem* BoomEffect;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = true))
	class USoundCue* BoomSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = true))
	float MissileDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess = true))
	float MissileEffectRange;

	UPROPERTY(EditDefaultsOnly, Category = "Property", meta=(AllowPrivateAccess=true))
	TSubclassOf<UDamageType> DamageType;

public:
	FORCEINLINE UProjectileMovementComponent* GetMissileMovement() const { return MissileMovement; }


};
