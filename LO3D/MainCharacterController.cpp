// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "Blueprint/UserWidget.h"
#include "kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Sound/SoundCue.h"

void AMainCharacterController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);


	if(bIsWinner)
	{
		auto MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		UUserWidget* WinWidget = CreateWidget(this, WinWidgetClass);

		if (WinWidget != nullptr)
		{
			WinWidget->AddToViewport();
			UGameplayStatics::PlaySound2D(GetWorld(), MainChar->GetWinSound());
			 

			FTimerHandle EndHandle;
			GetWorldTimerManager().SetTimer(EndHandle, this, &AMainCharacterController::MenuOpen, 5.f);
		}
	}

	else
	{
		UUserWidget* LoseWidget = CreateWidget(this, LoseWidgetClass);

		if (LoseWidget != nullptr)
		{
			LoseWidget->AddToViewport();
			FTimerHandle RestartHandle;
			GetWorldTimerManager().SetTimer(RestartHandle, this, &APlayerController::RestartLevel, 3.f);
		}
	}
	


}

void AMainCharacterController::MenuOpen()
{
	UGameplayStatics::OpenLevel(this, "MenuLevel");

}
