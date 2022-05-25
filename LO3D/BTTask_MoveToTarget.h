// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UBTTask_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToTarget();

	class AMainCharacter* Main;
	class AMainCharacter* Enemy;
	class AGunEnemyAIController* EnemyAIController;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, meta= (AllowPrivateAccess=true))
	float AcceptanceRadius = 1000.f;

};
