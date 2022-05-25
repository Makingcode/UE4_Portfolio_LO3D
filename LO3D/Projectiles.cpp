// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"

AProjectiles::AProjectiles():
	MissileDamage(100.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjMovement"));
	ProjMovement->SetUpdatedComponent(RootComponent);
	ProjMovement->InitialSpeed = 1500.f;
	ProjMovement->bRotationFollowsVelocity = false;
	ProjMovement->bShouldBounce = false;
	ProjMovement->bIsHomingProjectile = false;

	BombImpact = CreateDefaultSubobject<UParticleSystem>(TEXT("BombImpact"));
	
	BoomSound = CreateDefaultSubobject<USoundCue>(TEXT("BoomSound"));
	

	//매개변수가 맞아야 델리게이트가 가능
	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectiles::MissileHit);
}

void AProjectiles::BeginPlay()
{
	Super::BeginPlay();

	
}

void AProjectiles::MissileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComponent == nullptr)return;

	TArray<AActor*> ignoredActors;

	UWorld* World = GetWorld();

	if (HitComponent)
	{
		if (Hit.bBlockingHit)
		{
			
			UGameplayStatics::SpawnEmitterAtLocation(World, BombImpact, Hit.Location,FRotator(0),FVector(10.f,10.f,10.f));
			UGameplayStatics::ApplyRadialDamage(World, 300.f, Hit.Location, 500.f, DamageType, ignoredActors, this, nullptr, false, ECollisionChannel::ECC_Pawn);
			UGameplayStatics::PlaySound2D(World, BoomSound, 5.f);

			Destroy();
		}
	}
}
