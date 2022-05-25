// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetMaxSpeed.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UBTTask_SetMaxSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetMaxSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "WalkSpeed", meta = (AllowPrivateAccess = true))
	float MaxSpeed;
};
