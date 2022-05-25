// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponType.h"
#include "WeaponGun.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Normal UMETA(DisplayName = "Normal"),
	ECS_Equipped UMETA(DisplayName = "Equipped"),
	ECS_Death UMETA(DisplayName = "Death"),
	ECS_Battle UMETA(DisplayName="Battle")
};



UCLASS()
class LO3D_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	//받는 데미지 계산
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MyWeapons")
	class AWeaponGun* EquippedWeaponGun;
	
	class AGunEnemyAIController* EnemyController;

	//AI 정찰지점 및 엄폐지점 
	UPROPERTY(EditAnywhere, meta = (MakeEditwidget = true))
	FVector PatrolPoint;
	UPROPERTY(EditAnywhere, meta = (MakeEditwidget = true))
	FVector PatrolPoint2;
	UPROPERTY(EditAnywhere, meta = (MakeEditwidget = true))
	FVector PatrolPoint3;
	UPROPERTY(EditAnywhere, meta = (MakeEditwidget = true))
	FVector HidePoint;

private:

	//카메라 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = true))
	class UCameraComponent* TPPCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = true))
	class UCameraComponent* FPPCamera;


	FVector DefaultCameraLocation;

	//스나이퍼 스코프
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = true))
	bool bIsScope;

	//사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = true))
	class USoundCue* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = true))
	class USoundCue* WinSound;

	
	//애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire", meta = (AllowPrivateAccess = true))
	class UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire", meta = (AllowPrivateAccess = true))
	class UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = true))
	class UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = true))
	class UAnimMontage* DashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = true))
	class UAnimMontage* PickUpMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = true))
	class UAnimMontage* SwapMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = true))
	class UAnimMontage* LauncherSwapMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = true))
	class UAnimMontage* SpecialSwapMontage;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CrossHair", meta = (AllowPrivateAccess = true))
	class UTexture2D* CurWeaponCrossHairCenter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CrossHair", meta = (AllowPrivateAccess = true))
	class UTexture2D* CurWeaponCrossHairLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CrossHair", meta = (AllowPrivateAccess = true))
	class UTexture2D* CurWeaponCrossHairRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CrossHair", meta = (AllowPrivateAccess = true))
	class UTexture2D* CurWeaponCrossHairUp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CrossHair", meta = (AllowPrivateAccess = true))
	class UTexture2D* CurWeaponCrossHairDown;

	//파티클
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Particles", meta=(AllowPrivateAccess=true))
	class UParticleSystem* BloodParticle;

	//크로스헤어
	UPROPERTY(VisibleAnywhere, Category = "CrossHair", meta = (AllowPrivateAccess = true))
	float CrossHairSpreadValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, CAtegory = "CrossHair", meta = (AllowPrivateAccess = true))
	float CrossHairMoveFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, CAtegory = "CrossHair", meta = (AllowPrivateAccess = true))
	float CrossHairJumpFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, CAtegory = "CrossHair", meta = (AllowPrivateAccess = true))
	float CrossHairZoomFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, CAtegory = "CrossHair", meta = (AllowPrivateAccess = true))
	float CrossHairFireFactor;

	bool bIsCrossHairFire;
	bool bIsItemTrace;
	
	bool TraceUnderCrossHair(FHitResult& OutHitResult, FVector& OutHitLocation);
	bool EnemyLinetrace(FHitResult& OutHitResult, FVector& OutHitLocation);

	bool bCouldFire;
	bool bFiring;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Zoom", meta=(AllowPrivateAccess=true))
	bool bZooming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
	float DefaultFOV;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
	float CurrentFOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
	float ZoomFOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
	float ZoomSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LockOn", meta=(AllowPrivateAccess=true))
	bool FindTargetLockOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State", meta=(AllowPrivateAccess=true))
	ECharacterState CharacterState;

	bool bPickUp;
	bool bSwapping;

	AController* GetOwnerController() const;

	UPROPERTY(EditAnywhere, Category="State", meta=(AllowPrivateAccess=true))
	bool bIsCrouch;

	
	

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
	int32 BaseMaxBullet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
	int32 CurrentBullet;

	//인벤토리 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AWeaponGun*> MainWeaponSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AWeaponGun*> SubWeaponSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AWeaponGun*> BoomWeaponSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AWeaponGun*> SpecialWeaponSlot;

	//인벤 배열 최대 한계치
	int32 INVENTORY_CAPACITY = 1;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MousePitch(float Value);
	void MouseYaw(float Value);
	void PressedFireButton();
	void ReleasedFireButton();

	void CloseUpFunction(float Value);

	//앉기
	void CharacterCrouch();
	
	//달리기
	void SprintButtonPressed();
	void SprintButtonReleased();
	void Sprint();

	//무기 스왑, 드랍
	void SwapWeapon(AWeaponGun* WillEquipWeapon, AWeaponGun* WillSpareWeapon);
	void DropWeapon();

	//라인트레이스 계산
	bool bHitLineTrace(FVector& TraceStartLocation, FHitResult& OutHitResult);
	void ItemTraceFunc();
	void CalculateCrossHair(float DeltaTime);
	void LockOnFunc();
	void CalSpringArm(float DeltaTime);

	

	//발사시 크로스헤어
	void AutoCrossHairFireStart();
	UFUNCTION()
	void AutoCrossHairFireEnd();

	//줌
	void Zoom();
	void ZoomOut();
	void ZoomFunction();


	//아이템 줍기
	UFUNCTION()
	void PickUpEnd();
	void PickUpFunc(AWeaponGun* gun);

	//슬롯 번호 작동
	void SlotOneKeyPressed();
	void SlotTwoKeyPressed();
	void SlotThreeKeyPressed();
	void SlotFourKeyPressed();

	//무기 스왑
	void SwapMainWeapon();
	void SwapSubWeapon();
	void SwapBoomWeapon();
	void SwapSpecialWeapon();

	void UnEquip(AWeaponGun* Weapon);

	//총기반동
	void Recoil();


	//발사
	void Fire();

	//캐릭터 상태
	void SetCharacterState(ECharacterState CharState);
	void SetCharacterStateProperty(ECharacterState CharState);

	

	//스태미나 계산
	void CalStamina();

public:
	void EquipWeapon(AWeaponGun* WeaponEquip);
	void InteractActor();
	void OverlapItemCount(int8 Count);
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int8 overlapcount;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess=true))
	bool bIsEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess=true))
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	float BaseLookUpRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "weapon", meta = (AllowPrivateAccess = true))
	bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dead", meta = (AllowPrivateAccess = true))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsSprint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsBattle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemTrace", meta = (AllowPrivateAccess = true))
	class AWeaponGun* LineTracedItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemTrace", meta = (AllowPrivateAccess = true))
	class AWeaponGun* TracedItemValid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemTrace", meta = (AllowPrivateAccess = true))
	class AEnemy* LockOnEnemy;

	class AMissiles* Rocket;
	class ABullet* Bullet;

	//FVector CalBulletDirection;

	UPROPERTY(EditAnywhere, Category="EnemyEquipWeapon", meta=(AllowPrivateAccess=true))
	TSubclassOf<class AWeaponGun> EnemyEquipWeaponClass;

	class AWeaponGun* SpawnEnemyWeapon();
	


public:
	//죽음
	void Dead();

	//총알 충돌시 이펙트
	void BulletHitEffect(AActor* DamageCauser);

	//연사기능
	void AutoFireStart();
	UFUNCTION()
	void AutoFireEnd();

	//리로드
	void Reload();
	void ReloadFunction(int32 CurrentBulletCount);
	UFUNCTION()
	void ReloadEnd();

	
	FORCEINLINE bool GetZooming() const { return bZooming; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetBaseMaxBullet() const { return BaseMaxBullet; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCrossHairSpreadValue() const { return CrossHairSpreadValue; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	float SetHealth(float healthsetting);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE AWeaponGun* GetEquippedWeapon() const { return EquippedWeaponGun; }
	//FORCEINLINE FVector GetCalBulletDirection() const { return CalBulletDirection; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentBullet() const { return CurrentBullet; }
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE bool GetIsCrouch() const { return bIsCrouch; }
	FORCEINLINE bool GetIsBattle() const { return bIsBattle; }
	void SetIsBattle(bool isbattle);
	void SetIsCrouch(bool iscrouch);
	FORCEINLINE USoundCue* GetWinSound() const { return WinSound; }
};
