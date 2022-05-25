// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) override;
	void MenuOpen();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseWidgetClass;



};
