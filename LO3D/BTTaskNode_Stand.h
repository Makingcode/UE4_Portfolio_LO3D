// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Stand.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UBTTaskNode_Stand : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_Stand();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
