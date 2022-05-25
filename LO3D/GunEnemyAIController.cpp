// Fill out your copyright notice in the Description page of Project Settings.


#include "GunEnemyAIController.h"
#include "MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Kismet/KismetMathLibrary.h"


AGunEnemyAIController::AGunEnemyAIController()
{
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	DamageConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));

	SightConfig->SightRadius = AISightRadius;       //�þ߹���
	SightConfig->LoseSightRadius = AILoseSightRadius;     
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;  //�ֺ��þ߰���
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	
	


	HearingConfig->SetMaxAge(10.f);

	//�׳� PerceptionComponent�� ����� ���� ������� ǥ�ð� �ȵǱ� ������ ����
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	GetPerceptionComponent()->ConfigureSense(*DamageConfig);

	
	
}


void AGunEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	EnemyPawn = Cast<AMainCharacter>(this->GetPawn());
	
	const FVector WorldPatrolPoint1 = UKismetMathLibrary::TransformLocation(EnemyPawn->GetActorTransform(), EnemyPawn->PatrolPoint);
	const FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation(EnemyPawn->GetActorTransform(), EnemyPawn->PatrolPoint2);
	const FVector WorldPatrolPoint3 = UKismetMathLibrary::TransformLocation(EnemyPawn->GetActorTransform(), EnemyPawn->PatrolPoint3);
	const FVector WorldHidePoint = UKismetMathLibrary::TransformLocation(EnemyPawn->GetActorTransform(), EnemyPawn->HidePoint);


	if (EnemyBT != nullptr)
	{
		RunBehaviorTree(EnemyBT);

		GetBlackboardComponent()->SetValueAsObject("Target", PlayerPawn);
		GetBlackboardComponent()->SetValueAsVector("StartLocation", GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector("PatrolPoint1", WorldPatrolPoint1);
		GetBlackboardComponent()->SetValueAsVector("PatrolPoint2", WorldPatrolPoint2);
		GetBlackboardComponent()->SetValueAsVector("PatrolPoint3", WorldPatrolPoint3);
		GetBlackboardComponent()->SetValueAsVector("HidePoint", WorldHidePoint);
	}



	

}

void AGunEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GetBlackboardComponent()->SetValueAsFloat("EnemyHP", EnemyPawn->GetHealth());
}

void AGunEnemyAIController::AIFire()
{
	EnemyPawn = Cast<AMainCharacter>(this->GetPawn());
	
	
}

bool AGunEnemyAIController::IsDead() const
{
	AMainCharacter* ControlledEnemy = Cast<AMainCharacter>(GetPawn());
	if (ControlledEnemy != nullptr)
	{
		return ControlledEnemy->GetIsDead();
	}
	

	return true;
}





