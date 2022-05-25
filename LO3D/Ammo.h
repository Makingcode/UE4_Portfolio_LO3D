// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS()
class LO3D_API AAmmo : public AItem
{
	GENERATED_BODY()

public:
	AAmmo();

	class AMainCharacter* Main;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* ItemStaticMesh;
	
protected:

	/* Overlap */
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/*ÃÑ¾Ë °è»ê*/
	void AmmoCalculate(AMainCharacter* mainchar);

private:
	UPROPERTY(EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = true))
	int32 Ammo;

public:
	FORCEINLINE int32 GetAmmo() const { return Ammo; }

};
