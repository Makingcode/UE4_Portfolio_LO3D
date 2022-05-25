// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Crouch.h"
#include "MainCharacter.h"
#include "GunEnemyAIController.h"
#include "kismet/kismetMathLibrary.h"

UBTTask_Crouch::UBTTask_Crouch()
{
	NodeName = "Crouch";
}

EBTNodeResult::Type UBTTask_Crouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMainCharacter* EnemyCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (EnemyCharacter->GetIsCrouch()==false)
	{
		EnemyCharacter->SetIsCrouch(true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}