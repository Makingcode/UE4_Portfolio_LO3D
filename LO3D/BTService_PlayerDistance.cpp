// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDistance.h"
#include "MainCharacter.h"
#include "kismet/GameplayStatics.h"
#include "GunEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerDistance::UBTService_PlayerDistance()
{
	NodeName = "PlayerDistance";
}

void UBTService_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AMainCharacter* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	auto EnemyChar = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());


	if (MainChar)
	{
		if (EnemyChar)
		{
		
			Distance = (MainChar->GetActorLocation() - EnemyChar->GetActorLocation()).Size();
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Distance", Distance);
		}
	}

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}




}
