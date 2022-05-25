// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BulletCount.h"
#include "MainCharacter.h"
#include "GunEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_BulletCount::UBTService_BulletCount()
{
	NodeName = "BulletCount";
}

void UBTService_BulletCount::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	

	AMainCharacter* EnemyPawn = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	

	if (EnemyPawn)
	{
		//캐릭터의 총알 개수 구하기
		CurrentBullet = EnemyPawn->GetCurrentBullet();
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), CurrentBullet);
	}

	if (EnemyPawn == nullptr)
	{
		return;
	}

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}


}
