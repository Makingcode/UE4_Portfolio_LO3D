// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Fire.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UBTTask_Fire : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Fire();

	class AMainCharacter* GunEnemy;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



};
