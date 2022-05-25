// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Stand.h"
#include "MainCharacter.h"
#include "GunEnemyAIController.h"
#include "kismet/kismetMathLibrary.h"

UBTTaskNode_Stand::UBTTaskNode_Stand()
{
    NodeName = "Stand";
}

EBTNodeResult::Type UBTTaskNode_Stand::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AMainCharacter* EnemyCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        EBTNodeResult::Failed;
    }

    if (EnemyCharacter->GetIsCrouch() == true)
    {
        EnemyCharacter->SetIsCrouch(false);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Type();
}
