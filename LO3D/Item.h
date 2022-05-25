// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Equip UMETA(DisplayName="Equip"),
	EIS_Normal UMETA(DisplayName="Normal"),
	EIS_PickedUp UMETA(DisplayName="PickedUp"),
	EIS_Falling UMETA(DisplayName="Falling"),
	EIS_EquipSpare UMETA(DisplayName="EquipSpare")
};

UCLASS()
class LO3D_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = true))
	class USceneComponent* Root;


	//ÄÝ¸®Àü sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item", meta = (AllowPrivateAccess = true))
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = true))
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemWidget", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* PickUpItemWidget;
	

protected:
	UPROPERTY(EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = true))
	bool bItemRotate;
	UPROPERTY(EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = true))
	float RotationRate;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE UWidgetComponent* GetPickUpWidget() const { return PickUpItemWidget; }
	bool SetItemRotate(bool IsItemRotate);
	
};
