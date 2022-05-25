// Fill out your copyright notice in the Description page of Project Settings.


#include "Missiles.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "MainCharacter.h"

// Sets default values
AMissiles::AMissiles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileHitColliision = CreateDefaultSubobject<USphereComponent>(TEXT("MissileHitCollision"));
	SetRootComponent(MissileHitColliision);
	MissileHitColliision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	MissileSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MissileSkelMesh"));
	MissileSkeletalMesh->SetupAttachment(RootComponent);

	MissileStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileStaticMesh"));
	MissileStaticMesh->SetupAttachment(RootComponent);


	MissileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MissileMovement"));
	MissileMovement->SetUpdatedComponent(RootComponent);
	MissileMovement->bIsHomingProjectile = false;
	MissileMovement->bShouldBounce = false;
	MissileMovement->InitialSpeed = 1500.f;

	BoomEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("BoomEffect"));

	BoomSound = CreateDefaultSubobject<USoundCue>(TEXT("BoomSound"));
	
	

}

// Called when the game starts or when spawned
void AMissiles::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Spawn!!"));
}




// Called every frame
void AMissiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

