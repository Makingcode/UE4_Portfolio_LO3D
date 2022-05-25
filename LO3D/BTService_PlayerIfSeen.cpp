// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerIfSeen.h"
#include "kismet/GamePlayStatics.h"
#include "GunEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerIfSeen::UBTService_PlayerIfSeen()
{
	NodeName = "PlayerIfSeen";
}

void UBTService_PlayerIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr)
	{
		return;
	}

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}

	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}

}
