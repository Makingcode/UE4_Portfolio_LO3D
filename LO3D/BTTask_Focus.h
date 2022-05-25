// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Focus.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UBTTask_Focus : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Focus();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	class AMainCharacter* FocusCharacter;
	class AGunEnemyAIController* AIController;
	
};
