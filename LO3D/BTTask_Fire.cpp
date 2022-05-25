// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"
#include "MainCharacter.h"
#include "GunEnemyAIController.h"
#include "GameFramework/Actor.h"

UBTTask_Fire::UBTTask_Fire()
{
	NodeName = "Fire";
	
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GunEnemy = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());


	FHitResult Hit;
	const FVector Start = GunEnemy->EquippedWeaponGun->GetItemMesh()->GetSocketLocation("GunMuzzleSocket");
	const FVector End = (GunEnemy->EquippedWeaponGun->GetItemMesh()->GetForwardVector() * 50000.f) + Start;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility);


	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (GunEnemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (GunEnemy->GetEquippedWeapon() == nullptr)
	{
		
		return EBTNodeResult::Failed;
	}


	if (GunEnemy->GetEquippedWeapon()->GetCurrentBullet() == 0)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		GunEnemy->AutoFireStart();
		return EBTNodeResult::Succeeded;
	}
	
	




	
}



