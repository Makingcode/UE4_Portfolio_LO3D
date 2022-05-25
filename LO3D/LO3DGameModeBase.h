// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LO3DGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API ALO3DGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void CharacterKilled(APawn* PawnKilled);
	void EndGame(bool IsPlayerWinner);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameSetting", meta=(AllowPrivateAccess=true))
	bool bIsWin;

public:
	FORCEINLINE bool GetIsWin() const { return bIsWin; }
};
