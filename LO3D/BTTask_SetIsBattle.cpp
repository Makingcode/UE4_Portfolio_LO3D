// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetIsbattle.h"
#include "MainCharacter.h"
#include "GunEnemyAIController.h"


UBTTask_SetIsbattle::UBTTask_SetIsbattle()
{
    NodeName = "SetIsbattle";
}

EBTNodeResult::Type UBTTask_SetIsbattle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AMainCharacter* EnemyCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    
    EnemyCharacter->SetIsBattle(IsBattle);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    
    return EBTNodeResult::Succeeded;
}
