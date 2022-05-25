// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponType.h"
#include "WeaponGun.generated.h"



UCLASS()
class LO3D_API AWeaponGun : public AItem
{
	GENERATED_BODY()

public:
	AWeaponGun();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scope")
	bool bIsScope;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles", meta=(AllowPrivateAccess=true))
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = true))
	class UParticleSystem* ImpactFlash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = true))
	class UParticleSystem* BulletTrail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* SniperScope;

	//»ç¿îµå
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* WeaponImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* BulletTypeImg;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* IconBackGround;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	int32 MaxBullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	int32 BaseMaxBullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	int32 CurrentBullet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	int32 BulletMagazineCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	float FirePerSecond = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperty", meta = (AllowPrivateAccess = true))
	FName WeaponName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemProperty", meta = (AllowPrivateAccess = true))
	EItemState ItemStates;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemProperty", meta=(AllowPrivateAccess=true))
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess=true))
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponProperty", meta=(AllowPrivateAccess=true))
	float ReloadTime;
	
	FTimerHandle ThrowWeaponTimer;

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	void SetItemStateProperty(EItemState State);

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperty", meta = (AllowPrivateAccess = true))
	float HeadShotDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* CrossHair_right;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* CrossHair_left;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* CrossHair_up;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* CrossHair_down;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperty", meta = (AllowPrivateAccess = true))
	class UTexture2D* CrossHair_center;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<class AMissiles> MissileClass;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<class ABullet> BulletClass;

	bool bFalling;
	
public:
	void ThrowWeapon();

	UFUNCTION()
	void StopFalling();

public:
	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }
	FORCEINLINE int32 GetMaxBullet() const { return MaxBullet; }
	FORCEINLINE int32 GetCurrentBullet() const { return CurrentBullet; }
	FORCEINLINE int32 GetBulletMagazineCount() const { return BulletMagazineCount; }
	FORCEINLINE float GetFirePerSecond() const { return FirePerSecond; }
	FORCEINLINE FName GetWeaponName() const { return WeaponName; }
	void SetItemState(EItemState State);
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	void SetWeaponType(EWeaponType weaponType);
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE UParticleSystem* GetImpactFlash() const { return ImpactFlash; }
	FORCEINLINE UParticleSystem* GetBulletTrail() const { return BulletTrail; }
	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }
	FORCEINLINE EItemState GetItemState() const { return ItemStates; }
	FORCEINLINE int32 GetBaseMaxBullet() const { return BaseMaxBullet; }
	
	int32 SetCurrentBullet(int32 useBullet);
	int32 SetMaxBullet(int32 useBullet);
	int32 CalCurrnetBullet(int32 useBullet);
	int32 CalMaxBullet(int32 useBullet);

	FORCEINLINE UTexture2D* GetWeaponImage() const { return WeaponImage; }
	FORCEINLINE UTexture2D* GetBulletTypeImage() const { return BulletTypeImg; }
	FORCEINLINE UTexture2D* GetCrossHairRight() const { return CrossHair_right; }
	FORCEINLINE UTexture2D* GetCrossHairLeft() const { return CrossHair_left; }
	FORCEINLINE UTexture2D* GetCrossHairUp() const { return CrossHair_up; }
	FORCEINLINE UTexture2D* GetCrossHairDown() const { return CrossHair_down; }
	FORCEINLINE UTexture2D* GetCrossHairCenter() const { return CrossHair_center; }
	
	FORCEINLINE TSubclassOf<AMissiles> GetMissileClass() const { return MissileClass; }
	FORCEINLINE TSubclassOf<ABullet> GetBulletClass() const { return BulletClass; }
	FORCEINLINE float GetReloadTime() const { return ReloadTime; }
	FORCEINLINE bool GetIsScope() const { return bIsScope; }
	
};
