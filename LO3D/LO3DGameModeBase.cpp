// Copyright Epic Games, Inc. All Rights Reserved.


#include "LO3DGameModeBase.h"
#include "GameFrameWork/Controller.h"
#include "EngineUtils.h"
#include "GunEnemyAIController.h"


void ALO3DGameModeBase::CharacterKilled(APawn* PawnKilled)
{
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());

	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	for (AGunEnemyAIController* Controller : TActorRange<AGunEnemyAIController>(GetWorld()))
	{
		if (!Controller->IsDead())
		{
			return;
		}
	}

	EndGame(true);
}

void ALO3DGameModeBase::EndGame(bool IsPlayerWinner)
{
	//���͵��� ���� ��Ʈ�ѷ� ����ŭ �ݺ�
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == IsPlayerWinner; 
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
		bIsWin = bIsWinner;

	}
}