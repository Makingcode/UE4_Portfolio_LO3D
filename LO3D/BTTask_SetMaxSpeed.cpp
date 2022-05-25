// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetMaxSpeed.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GunEnemyAIController.h"

UBTTask_SetMaxSpeed::UBTTask_SetMaxSpeed()
{
	NodeName = "SetMaxSpeed";
}

EBTNodeResult::Type UBTTask_SetMaxSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMainCharacter* EnemyCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	

	if (OwnerComp.GetAIOwner()==nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
