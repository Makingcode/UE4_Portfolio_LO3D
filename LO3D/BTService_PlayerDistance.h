// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API UBTService_PlayerDistance : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_PlayerDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float Distance;

};
