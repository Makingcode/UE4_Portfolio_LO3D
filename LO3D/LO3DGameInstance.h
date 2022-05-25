// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LO3DGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API ULO3DGameInstance : public UGameInstance
{
	GENERATED_BODY()

	ULO3DGameInstance();


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (AllowPrivateAccess = true))
	int32 EnemyDeathCount;
	
public:
	UFUNCTION(BlueprintCallable)
	void GameFinishs();

};
