// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetIsbattle.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UBTTask_SetIsbattle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetIsbattle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool IsBattle;
};
