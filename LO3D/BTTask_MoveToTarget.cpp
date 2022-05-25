// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToTarget.h"
#include "MainCharacter.h"
#include "GunEnemyAIController.h"
#include "kismet/GameplayStatics.h"


UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
    NodeName = "MoveToTarget";
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

   

    Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    EnemyAIController = Cast<AGunEnemyAIController>(OwnerComp.GetAIOwner()->GetPawn()->GetController());
    Enemy = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    EnemyAIController->MoveToActor(Main, AcceptanceRadius);

    if (Main == nullptr)
    {
        return EBTNodeResult::Failed;
    }


    return EBTNodeResult::Succeeded;
}
