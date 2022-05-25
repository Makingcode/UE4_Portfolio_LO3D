// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GunEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API AGunEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGunEnemyAIController();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void AIFire();

	bool IsDead() const;

	class AMainCharacter* EnemyPawn;

private:
	//�����̺�� Ʈ��
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* EnemyBT;


private:


	//AI Perception �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float AISightRadius = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float AISightAge = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float AILoseSightRadius = AISightRadius + 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float AIFieldOfView = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UAISenseConfig_Hearing* HearingConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UAISenseConfig_Damage* DamageConfig;
	



};
