// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_BulletCount.generated.h"


/* �Ѿ� ��� �ൿƮ���� ���� */

UCLASS()
class LO3D_API UBTService_BulletCount : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_BulletCount();

protected:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	
private:
	int32 CurrentBullet;
};
