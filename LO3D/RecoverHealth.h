// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "RecoverHealth.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API ARecoverHealth : public AItem
{
	GENERATED_BODY()

public:
	ARecoverHealth();
	class AMainCharacter* Main;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* ItemStaticMesh;

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere, Category="Recovery", meta=(AllowPrivateAccess=true))
	float RecoverHealthAmount;

public:
	FORCEINLINE float GetRecoverHealthAmount() const { return RecoverHealthAmount; }
	void RecoverHealthCalculate(AMainCharacter* mainchar);
};
