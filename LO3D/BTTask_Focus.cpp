// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Focus.h"
#include "MainCharacter.h"
#include "kismet/GameplayStatics.h"
#include "GunEnemyAIController.h"


UBTTask_Focus::UBTTask_Focus()
{
	NodeName = "Focus";
}

EBTNodeResult::Type UBTTask_Focus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FocusCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AIController = Cast<AGunEnemyAIController>(OwnerComp.GetAIOwner()->GetPawn()->GetController());

	if (FocusCharacter == nullptr)
	{
		EBTNodeResult::Failed;
	}

	AIController->SetFocus(FocusCharacter);
	

	return EBTNodeResult::Succeeded;
}
