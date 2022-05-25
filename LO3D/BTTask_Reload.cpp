// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Reload.h"
#include "MainCharacter.h"
#include "GunEnemyAIController.h"

UBTTask_Reload::UBTTask_Reload()
{
	NodeName = "Reload";
}

EBTNodeResult::Type UBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyPawn = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	


	if (EnemyPawn->EquippedWeaponGun->GetCurrentBullet() == 0)
	{
		EnemyPawn->Reload();
		return EBTNodeResult::InProgress;
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}

}
