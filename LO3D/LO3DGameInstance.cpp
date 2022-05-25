// Fill out your copyright notice in the Description page of Project Settings.


#include "LO3DGameInstance.h"

ULO3DGameInstance::ULO3DGameInstance()
{

}

void ULO3DGameInstance::GameFinishs()
{
	if (EnemyDeathCount == 5)
	{
		UE_LOG(LogTemp, Warning, TEXT("You Killed 5 Enemy"));
	}
}