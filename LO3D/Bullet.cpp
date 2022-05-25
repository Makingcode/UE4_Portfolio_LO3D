// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "WeaponGun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Controller.h"
#include "Perception/AISense_Damage.h"




ABullet::ABullet()
{
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovement->InitialSpeed = 20000.f;
	BulletMovement->Velocity = FVector(1,0,0);

	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	bItemRotate = false;

}

void ABullet::BulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto Character = Cast<AMainCharacter>(OtherActor);
	if (OtherActor)
	{
		DoDamage(OtherActor);

		if (OtherActor==Character)
		{
			Character->BulletHitEffect(this);
		}

	}
}


void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	
	CollisionSphere->OnComponentHit.AddDynamic(this, &ABullet::BulletHit);
	
	
}

void ABullet::DoDamage(AActor* HitActor)
{
	if (HitActor == nullptr) return;
	auto EnemyChar = Cast<AMainCharacter>(HitActor);
	auto MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	

	if (EnemyChar)
	{
		
		UGameplayStatics::ApplyDamage(EnemyChar, BulletDamage, nullptr, this, UDamageType::StaticClass());
		UAISense_Damage::ReportDamageEvent(GetWorld(), EnemyChar, MainChar, BulletDamage, EnemyChar->GetActorLocation(), EnemyChar->GetActorLocation());
		Destroy();
		
	}

}
