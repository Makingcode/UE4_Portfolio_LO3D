// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Item.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BulletHitInterface.h"
#include "Enemy.h"
#include "LO3DGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Bullet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "GunEnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Missiles.h"


// Sets default values
AMainCharacter::AMainCharacter() :
	bIsScope(false), CrossHairSpreadValue(0.f), CrossHairMoveFactor(0.f), CrossHairJumpFactor(0.f), CrossHairZoomFactor(0.f), CrossHairFireFactor(0.f), bIsCrossHairFire(false), bIsItemTrace(false),
	bCouldFire(true), bFiring(false), bZooming(false), DefaultFOV(0.f), CurrentFOV(0.f), ZoomFOV(50.f), ZoomSpeed(10.f), FindTargetLockOn(false), Health(100.f), MaxHealth(100.f), Stamina(200.f), MaxStamina(200.f),
	CharacterState(ECharacterState::ECS_Normal), bPickUp(false), bSwapping(false), bIsCrouch(false), overlapcount(0), bIsEquipped(false), BaseTurnRate(50.f), BaseLookUpRate(50.f),
	bReloading(false), bIsDead(false), bIsSprint(false),bIsBattle(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//카메라 스프링암 기본설정
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 300.f;
	CameraSpringArm->bUsePawnControlRotation = true;
	CameraSpringArm->SocketOffset = FVector(0.f, 50.f, 50.f);

	//카메라 기본설정
	TPPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
	TPPCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	TPPCamera->bUsePawnControlRotation = false;
	TPPCamera->SetActive(true);

	FPPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPPCamera"));
	FPPCamera->bUsePawnControlRotation = false;
	FPPCamera->SetActive(false);
	FPPCamera->SetWorldScale3D(FVector(0.1f));
	FPPCamera->SetFieldOfView(70.f);


	//컨트롤러 회전값 사용 여부
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//캐릭터가 원래 회전방향으로 움직이는지
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	
	//점프값 설정
	GetCharacterMovement()->JumpZVelocity = 500.f;
	//공중에서 컨트롤할수 있는 시간
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->CrouchedHalfHeight = 60.f;
	

}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetCharacterState(ECharacterState::ECS_Battle);
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		bIsDead = true;
		

		if (bIsDead)
		{
			Dead();
		}
	}
	else
	{
		bIsDead = false;
		Health -= DamageAmount;
	}

	

	return DamageAmount;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (bIsEquipped)
	{
		SetCharacterState(ECharacterState::ECS_Equipped);
	}
	else
	{
		SetCharacterState(ECharacterState::ECS_Normal);
	}
	
	DefaultCameraLocation = TPPCamera->GetComponentLocation();

	if (TPPCamera)
	{
		DefaultFOV = TPPCamera->FieldOfView;
	}
	CurrentFOV = DefaultFOV;
	
	for (int i = 0; i < INVENTORY_CAPACITY; i++)
	{
		MainWeaponSlot.Add(nullptr);
		SubWeaponSlot.Add(nullptr);
		BoomWeaponSlot.Add(nullptr);
		SpecialWeaponSlot.Add(nullptr);
		
	}
	
	
	EquipWeapon(SpawnEnemyWeapon());
	
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(75.f);
	this->GetCapsuleComponent()->SetCapsuleRadius(18.f);
	bZooming = false;

	


}



// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ItemTraceFunc();
	CalculateCrossHair(DeltaTime);
	LockOnFunc();
	CalStamina();
	CalSpringArm(DeltaTime);
	Sprint();
	if (EquippedWeaponGun)
	{
		CurrentBullet = EquippedWeaponGun->GetCurrentBullet();
	}
	
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MousePitch", this, &AMainCharacter::MousePitch);
	PlayerInputComponent->BindAxis("MouseYaw", this, &AMainCharacter::MouseYaw);
	PlayerInputComponent->BindAxis("CloseUp", this, &AMainCharacter::CloseUpFunction);
	PlayerInputComponent->BindAction("Jumping", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jumping", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Firing", IE_Pressed, this, &AMainCharacter::PressedFireButton);
	PlayerInputComponent->BindAction("Firing", IE_Released, this, &AMainCharacter::ReleasedFireButton);
	PlayerInputComponent->BindAction("InteractKey", IE_Pressed, this, &AMainCharacter::InteractActor);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMainCharacter::Zoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMainCharacter::ZoomOut);
	PlayerInputComponent->BindAction("Reloading", IE_Pressed, this, &AMainCharacter::Reload);
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &AMainCharacter::DropWeapon);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintButtonReleased);

	PlayerInputComponent->BindAction("InvenSlot1", IE_Pressed, this, &AMainCharacter::SlotOneKeyPressed);
	PlayerInputComponent->BindAction("InvenSlot2", IE_Pressed, this, &AMainCharacter::SlotTwoKeyPressed);
	PlayerInputComponent->BindAction("InvenSlot3", IE_Pressed, this, &AMainCharacter::SlotThreeKeyPressed);
	PlayerInputComponent->BindAction("InvenSlot4", IE_Pressed, this, &AMainCharacter::SlotFourKeyPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::CharacterCrouch);
}



void AMainCharacter::SetCharacterState(ECharacterState CharState)
{
	CharacterState = CharState;
	SetCharacterStateProperty(CharacterState);
}

void AMainCharacter::SetCharacterStateProperty(ECharacterState CharState)
{
	float Deltasec = GetWorld()->GetDeltaSeconds();
	switch (CharState)
	{
		case ECharacterState::ECS_Normal:
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;
			
			
		}break;
		case ECharacterState::ECS_Equipped:
		{
			bIsBattle = false;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = true;
			
		}break;
		case ECharacterState::ECS_Death:
		{
			
		}break;
		case ECharacterState::ECS_Battle:
		{
			bIsBattle = true;

		}break;
	}
}

void AMainCharacter::CalStamina()
{
	if (!bIsSprint)
	{
		Stamina += 1;
		if (Stamina >= MaxStamina)
		{
			Stamina = MaxStamina;
		}
	}
	else
	{
		Stamina -= 1;
		if (Stamina <= 0.f)
		{
			Stamina = 0.f;
		}
	}
}

AController* AMainCharacter::GetOwnerController() const
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner == nullptr)
	{
		return nullptr;
	}
	return PawnOwner->GetController();
}


void AMainCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0&&!bPickUp)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator{ 0.f,Rotation.Yaw,0.f };
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
		
		if (bIsSprint&&Stamina>0)
		{
			Stamina -= 1.f;
		}

	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0&&!bPickUp)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator{ 0.f,Rotation.Yaw,0.f };
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MousePitch(float Value)
{
	AddControllerPitchInput(Value*BaseLookUpRate*GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::MouseYaw(float Value)
{
	AddControllerYawInput(Value*BaseTurnRate*GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::PressedFireButton()
{
	if (!bIsDead)
	{
		if (EquippedWeaponGun)
		{
			bFiring = true;

			if (bFiring&&bReloading==false&&!bIsSprint)
			{
				AutoFireStart();
			}
		}
	}
	
	
}

void AMainCharacter::ReleasedFireButton()
{
	bFiring = false;
}

void AMainCharacter::CloseUpFunction(float Value)
{
	if (CharacterState==ECharacterState::ECS_Normal)
	{
		if (Controller&&Value!=0)
		{
			CameraSpringArm->TargetArmLength += Value * 1000.f * GetWorld()->GetDeltaSeconds();
			if (CameraSpringArm->TargetArmLength <= 100.f)
			{
				CameraSpringArm->TargetArmLength = 100.f;
			}
			else if (CameraSpringArm->TargetArmLength >= 1000.f)
			{
				CameraSpringArm->TargetArmLength = 1000.f;
			}
		}
	}
}

void AMainCharacter::CharacterCrouch()
{
	if (bIsCrouch == false)
	{
		bIsCrouch = true;
		if (bIsCrouch)
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			GetCharacterMovement()->Crouch(true);
			GetCapsuleComponent()->SetCapsuleRadius(25.f);
			
			
		}
	}
	else
	{
		bIsCrouch = false;
		if (!bIsCrouch)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
			GetCharacterMovement()->UnCrouch(true);
			GetCapsuleComponent()->SetCapsuleRadius(18.f);

			
		}
	}
}

void AMainCharacter::Fire()
{
	UWorld* World = GetWorld();
	if (bIsEquipped)
	{
		AutoCrossHairFireStart();
		Recoil();
		const USkeletalMeshSocket* MuzzleSocket = EquippedWeaponGun->GetItemMesh()->GetSocketByName("GunMuzzleSocket");


		if (MuzzleSocket)
		{
			FTransform MuzzleSocketTransform = MuzzleSocket->GetSocketTransform(EquippedWeaponGun->GetItemMesh());
			FVector MuzzleSocketLocation = MuzzleSocketTransform.GetLocation();
			FHitResult BulletHitResult;
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			if (EquippedWeaponGun->GetMuzzleFlash())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeaponGun->GetMuzzleFlash(), MuzzleSocketTransform);
			}

			bool bBulletEnd = bHitLineTrace(MuzzleSocketLocation, BulletHitResult);

		
				switch (EquippedWeaponGun->GetWeaponType())
				{
				case EWeaponType::EWT_MainWeapon:
				{
					if (EquippedWeaponGun->GetBulletClass())
					{
						FRotator Direction = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation,BulletHitResult.Location);
						Bullet = GetWorld()->SpawnActor<ABullet>(EquippedWeaponGun->GetBulletClass(), MuzzleSocketLocation,Direction, SpawnParams);
					}
					
					
				}break;

				case EWeaponType::EWT_SubWeapon:
				{
					if (EquippedWeaponGun->GetBulletClass())
					{
						FRotator Direction = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, BulletHitResult.Location);
						Bullet = GetWorld()->SpawnActor<ABullet>(EquippedWeaponGun->GetBulletClass(), MuzzleSocketLocation, Direction, SpawnParams);
						if (BulletHitResult.Actor.IsValid())
						{
							IBulletHitInterface* BulletHitInterface = Cast<IBulletHitInterface>(BulletHitResult.Actor.Get());
							if (BulletHitInterface)
							{
								BulletHitInterface->BulletHit_Implementation(BulletHitResult);
							}

							AEnemy* HitEnemy = Cast<AEnemy>(BulletHitResult.Actor.Get());
							if (HitEnemy)
							{
								int32 Damage{};

								if (BulletHitResult.BoneName.ToString() == HitEnemy->GetHeadBoneName())
								{
									//헤드샷
									Damage = Bullet->GetBulletDamage() * 2;
									UGameplayStatics::ApplyDamage(BulletHitResult.Actor.Get(), Damage, GetController(), this, UDamageType::StaticClass());
									HitEnemy->ShowHitNumber(Damage, BulletHitResult.Location, true);
								}
								else
								{
									//바디샷
									Damage = Bullet->GetBulletDamage();
									UGameplayStatics::ApplyDamage(BulletHitResult.Actor.Get(), Damage, GetController(), this, UDamageType::StaticClass());
									HitEnemy->ShowHitNumber(Damage, BulletHitResult.Location, false);
								}
							}
						}
					}
					else
					{
						//기본 히트 이펙트
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeaponGun->GetImpactFlash(), BulletHitResult.Location);
					}
					
				}break;

				case EWeaponType::EWT_BoomWeapon:
				{
					if (EquippedWeaponGun->GetMissileClass())
					{
						FRotator Direction = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, BulletHitResult.Location);
						
						Rocket = World->SpawnActor<AMissiles>(EquippedWeaponGun->GetMissileClass(), MuzzleSocketLocation,Direction, SpawnParams);
						
						//Rocket->GetMissileMovement()->HomingTargetComponent = BulletHitResult.GetComponent();
						
						Rocket->SetLifeSpan(10.f);

					}
					
				}break;
				
				case EWeaponType::EWT_SpecialWeapon:
					if (EquippedWeaponGun->GetMissileClass())
					{
						FRotator Direction = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, BulletHitResult.Location);
						Rocket = World->SpawnActor<AMissiles>(EquippedWeaponGun->GetMissileClass(), MuzzleSocketLocation, Direction, SpawnParams);
					}
				}
			UParticleSystemComponent* BulletTrail = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeaponGun->GetBulletTrail(), MuzzleSocketTransform);
			if (BulletTrail)
			{
				BulletTrail->SetVectorParameter("Target", BulletHitResult.Location);
			}

		}
		if (EquippedWeaponGun->GetFireSound())
		{
			
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquippedWeaponGun->GetFireSound(), EquippedWeaponGun->GetItemMesh()->GetSocketLocation("GunMuzzleSocket"));
			UAISense_Hearing::ReportNoiseEvent(GetWorld(), EquippedWeaponGun->GetItemMesh()->GetSocketLocation("GunMuzzleSocket"), 5.f, UGameplayStatics::GetPlayerPawn(GetWorld(), 0), 1000.f);
			
		}
		UAnimInstance* MainAnimInstance = GetMesh()->GetAnimInstance();
		if (MainAnimInstance)
		{
			MainAnimInstance->Montage_Play(FireMontage);
			MainAnimInstance->Montage_JumpToSection("FireStart", FireMontage);
		}

		EquippedWeaponGun->CalCurrnetBullet(1);
	}

}

void AMainCharacter::SwapWeapon(AWeaponGun* WillEquipWeapon, AWeaponGun* WillSpareWeapon)
{
	EquipWeapon(WillEquipWeapon);
	switch (WillSpareWeapon->GetWeaponType())
	{
		case EWeaponType::EWT_MainWeapon:
		{
			WillSpareWeapon = Cast<AWeaponGun>(MainWeaponSlot[0]);
			WillSpareWeapon->SetItemState(EItemState::EIS_EquipSpare);
		}break;
		case EWeaponType::EWT_SubWeapon:
		{
			WillSpareWeapon = Cast<AWeaponGun>(SubWeaponSlot[0]);
			WillSpareWeapon->SetItemState(EItemState::EIS_PickedUp);
		}break;
		case EWeaponType::EWT_BoomWeapon:
		{
			WillSpareWeapon = Cast<AWeaponGun>(BoomWeaponSlot[0]);
			WillSpareWeapon->SetItemState(EItemState::EIS_PickedUp);
		}break;
		case EWeaponType::EWT_SpecialWeapon:
		{
			WillSpareWeapon = Cast<AWeaponGun>(SpecialWeaponSlot[0]);
			WillSpareWeapon->SetItemState(EItemState::EIS_PickedUp);
		}break;
	}
}

void AMainCharacter::DropWeapon()
{
	bZooming = false;
	if (EquippedWeaponGun == nullptr) return;
	if (EquippedWeaponGun&&!bReloading)
	{
		auto FormerWeapon = Cast<AWeaponGun>(EquippedWeaponGun);
		FPPCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,"FPPCameraSocket");
		EquippedWeaponGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeaponGun->SetItemState(EItemState::EIS_Normal);
		EquippedWeaponGun->ThrowWeapon();
		if (EquippedWeaponGun == MainWeaponSlot[0])
		{
			MainWeaponSlot[0] = nullptr;
		}
		if (EquippedWeaponGun == SubWeaponSlot[0])
		{
			SubWeaponSlot[0] = nullptr;
		}
		if (EquippedWeaponGun == BoomWeaponSlot[0])
		{
			BoomWeaponSlot[0] = nullptr;
		}
		if (EquippedWeaponGun == SpecialWeaponSlot[0])
		{
			SpecialWeaponSlot[0] = nullptr;
		}
		EquippedWeaponGun = nullptr;

		bIsEquipped = false;
		FindTargetLockOn = false;
		SetCharacterState(ECharacterState::ECS_Normal);
		ZoomFunction();

		
		
		
	}
	
}






void AMainCharacter::SprintButtonPressed()
{
	bIsSprint = true;
}

void AMainCharacter::SprintButtonReleased()
{
	bIsSprint = false;
}

void AMainCharacter::Sprint()
{
	if (this == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (bIsSprint)
		{
			if (Stamina > 0)
			{
				GetCharacterMovement()->MaxWalkSpeed = 1000.f;
				if (bIsCrouch)
				{
					GetCharacterMovement()->MaxWalkSpeed = 400.f;
				}

			}
			else
			{
				bIsSprint = false;
			}
		}
		else
		{
			bIsSprint = false;
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
			if (bIsCrouch)
			{
				GetCharacterMovement()->MaxWalkSpeed = 300.f;
			}
		}
	}
}


void AMainCharacter::EquipWeapon(AWeaponGun* WeaponEquip)
{
	bZooming = false;

	if (WeaponEquip)
	{ 
		EquippedWeaponGun = Cast<AWeaponGun>(WeaponEquip);
		WeaponEquip->SetItemRotate(false);
		CurrentBullet = EquippedWeaponGun->GetCurrentBullet();
		CurWeaponCrossHairCenter = EquippedWeaponGun->GetCrossHairCenter();
		CurWeaponCrossHairRight = EquippedWeaponGun->GetCrossHairRight();
		CurWeaponCrossHairLeft = EquippedWeaponGun->GetCrossHairLeft();
		CurWeaponCrossHairUp = EquippedWeaponGun->GetCrossHairUp();
		CurWeaponCrossHairDown = EquippedWeaponGun->GetCrossHairDown();
		bIsScope = EquippedWeaponGun->GetIsScope();
		

		bIsEquipped = true;
		EquippedWeaponGun = WeaponEquip;
		EquippedWeaponGun->SetItemState(EItemState::EIS_Equip);
		SetCharacterState(ECharacterState::ECS_Equipped);

		switch (WeaponEquip->GetWeaponType())
		{
			case EWeaponType::EWT_MainWeapon:
			{
				const USkeletalMeshSocket* WeaponEquipHand = GetMesh()->GetSocketByName("WeaponEquipHand");
				if (WeaponEquipHand)
				{
					WeaponEquipHand->AttachActor(WeaponEquip, GetMesh());
				}
			}break;
			case EWeaponType::EWT_SubWeapon:
			{
				const USkeletalMeshSocket* WeaponEquipHand = GetMesh()->GetSocketByName("PistolEquipHand");
				if (WeaponEquipHand)
				{
					WeaponEquipHand->AttachActor(WeaponEquip, GetMesh());
				}
			}break;
			case EWeaponType::EWT_BoomWeapon:
			{
				const USkeletalMeshSocket* WeaponEquipHand = GetMesh()->GetSocketByName("BoomWeaponEquipHand");
				if (WeaponEquipHand)
				{
					WeaponEquipHand->AttachActor(WeaponEquip, GetMesh());

					WeaponEquip->GetItemMesh()->HideBoneByName("Rocket", EPhysBodyOp::PBO_None);
				}
			}break;
			case EWeaponType::EWT_SpecialWeapon:
			{
				const USkeletalMeshSocket* WeaponEquipHand = GetMesh()->GetSocketByName("SpecialEquipHand");
				if (WeaponEquipHand)
				{
					WeaponEquipHand->AttachActor(WeaponEquip, GetMesh());
				}
			}
		}
	}

}

void AMainCharacter::InteractActor()
{
	if (LineTracedItem)
	{
		auto TraceHitWeaponGun = Cast<AWeaponGun>(LineTracedItem);
		const USkeletalMeshSocket* EquipSpare = GetMesh()->GetSocketByName("EquipWeapon1");
		if (TraceHitWeaponGun)
		{
			if (!bIsEquipped)         //무기 미장착 상태에서의 아이템 줍기
			{
				switch (TraceHitWeaponGun->GetWeaponType())
				{
					case EWeaponType::EWT_MainWeapon:
					{
						if (MainWeaponSlot[0] == nullptr)
						{
							UAnimInstance* MainAnimInstance = GetMesh()->GetAnimInstance();
							if (MainAnimInstance)
							{
								bPickUp = true;
								MainAnimInstance->Montage_Play(PickUpMontage);
								MainAnimInstance->Montage_JumpToSection("PickUpStart", PickUpMontage);
								FTimerHandle PickUpHandle;
								GetWorldTimerManager().SetTimer(PickUpHandle, this, &AMainCharacter::PickUpEnd, 0.71f);
							}
						}
					}break;
					case EWeaponType::EWT_SubWeapon:
					{
						if (SubWeaponSlot[0] == nullptr)
						{
							UAnimInstance* MainAnimInstance = GetMesh()->GetAnimInstance();
							if (MainAnimInstance)
							{
								bPickUp = true;
								MainAnimInstance->Montage_Play(PickUpMontage);
								MainAnimInstance->Montage_JumpToSection("PickUpStart", PickUpMontage);
								FTimerHandle PickUpHandle;
								GetWorldTimerManager().SetTimer(PickUpHandle, this, &AMainCharacter::PickUpEnd, 0.71f);
							}
						}
					}break;
					case EWeaponType::EWT_BoomWeapon:
					{
						if (BoomWeaponSlot[0] == nullptr)
						{
							UAnimInstance* MainAnimInstance = GetMesh()->GetAnimInstance();
							if (MainAnimInstance)
							{
								bPickUp = true;
								MainAnimInstance->Montage_Play(PickUpMontage);
								MainAnimInstance->Montage_JumpToSection("PickUpStart", PickUpMontage);
								FTimerHandle PickUpHandle;
								GetWorldTimerManager().SetTimer(PickUpHandle, this, &AMainCharacter::PickUpEnd, 0.71f);
							}
						}
					}break;
					case EWeaponType::EWT_SpecialWeapon:
					{
						if (SpecialWeaponSlot[0] == nullptr)
						{
							UAnimInstance* MainAnimInstance = GetMesh()->GetAnimInstance();
							if (MainAnimInstance)
							{
								bPickUp = true;
								MainAnimInstance->Montage_Play(PickUpMontage);
								MainAnimInstance->Montage_JumpToSection("PickUpStart", PickUpMontage);
								FTimerHandle PickUpHandle;
								GetWorldTimerManager().SetTimer(PickUpHandle, this, &AMainCharacter::PickUpEnd, 0.71f);
							}
						}
					}break;
				}
				
				
			}
			else       //무기 장착상태에서 아이템 줍기
			{
				switch (TraceHitWeaponGun->GetWeaponType())
				{
				case EWeaponType::EWT_MainWeapon:
				{
					if (MainWeaponSlot[0] == nullptr)
					{
						MainWeaponSlot[0] = TraceHitWeaponGun;
						MainWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
						EquipSpare->AttachActor(MainWeaponSlot[0], GetMesh());
					}
				}break;
				case EWeaponType::EWT_SubWeapon:
				{
					if (SubWeaponSlot[0] == nullptr)
					{
						SubWeaponSlot[0] = TraceHitWeaponGun;
						SubWeaponSlot[0]->SetItemState(EItemState::EIS_PickedUp);
						EquipSpare->AttachActor(SubWeaponSlot[0], GetMesh());
					}
				}break;
				case EWeaponType::EWT_BoomWeapon:
				{
					if (BoomWeaponSlot[0] == nullptr)
					{
						BoomWeaponSlot[0] = TraceHitWeaponGun;
						BoomWeaponSlot[0]->SetItemState(EItemState::EIS_PickedUp);
						EquipSpare->AttachActor(BoomWeaponSlot[0], GetMesh());
					}
				}break;
				case EWeaponType::EWT_SpecialWeapon:
				{
					if (SpecialWeaponSlot[0] == nullptr)
					{
						SpecialWeaponSlot[0] = TraceHitWeaponGun;
						SpecialWeaponSlot[0]->SetItemState(EItemState::EIS_PickedUp);
						EquipSpare->AttachActor(SpecialWeaponSlot[0], GetMesh());
					}
				}break;

				}
			}
		}
	}
}

void AMainCharacter::OverlapItemCount(int8 Count)
{
	if (overlapcount + Count <= 0)
	{
		overlapcount = 0;
		bIsItemTrace = false;
		
	}
	else
	{
		overlapcount += Count;
		bIsItemTrace = true;
	}
}



AWeaponGun* AMainCharacter::SpawnEnemyWeapon()
{
	if (EnemyEquipWeaponClass)
	{
		return GetWorld()->SpawnActor<AWeaponGun>(EnemyEquipWeaponClass);
	}

	return nullptr;
}

float AMainCharacter::SetHealth(float healthsetting)
{
	return Health = healthsetting;
}

void AMainCharacter::SetIsBattle(bool isbattle)
{
	bIsBattle = isbattle;
}

void AMainCharacter::SetIsCrouch(bool iscrouch)
{
	bIsCrouch = iscrouch;
}



bool AMainCharacter::bHitLineTrace(FVector& TraceStartLocation, FHitResult& OutHitResult)
{
	FVector OutLocation;

	//크로스헤어 트레이스 히트 체크
	FHitResult CrosshairHitResult;
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	
	bool bCrossHairHit;
	
	if (this==PlayerPawn)
	{
		bCrossHairHit = TraceUnderCrossHair(CrosshairHitResult, OutLocation);
		if (bCrossHairHit)
		{
			OutLocation = CrosshairHitResult.Location;
		}
	}
	else
	{
		bCrossHairHit = EnemyLinetrace(CrosshairHitResult, OutLocation);
	}

	const FVector WeaponTraceStart{ TraceStartLocation };
	const FVector WeaponTraceEnd{ OutLocation };
	GetWorld()->LineTraceSingleByChannel(OutHitResult, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility);
	if (!OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutLocation;
		return false;
	}
	//DrawDebugLine(GetWorld(), WeaponTraceStart, OutLocation, FColor::Red, true);
	
	return true;


}

void AMainCharacter::ItemTraceFunc()
{
	
	if (bIsItemTrace)
	{
		FHitResult ItemHit;
		FVector ItemHitEndLocation;
		TraceUnderCrossHair(ItemHit, ItemHitEndLocation);

		if (ItemHit.bBlockingHit)
		{
			LineTracedItem = Cast<AWeaponGun>(ItemHit.Actor);
			if (LineTracedItem)
			{
				LineTracedItem->GetPickUpWidget()->SetVisibility(true);
			}

			if (TracedItemValid)
			{
				if (TracedItemValid != LineTracedItem)
				{
					TracedItemValid->GetPickUpWidget()->SetVisibility(false);
				}
			}

			TracedItemValid = LineTracedItem;

			
		}
		else
		{
			if (TracedItemValid)
			{
				TracedItemValid->GetPickUpWidget()->SetVisibility(false);
			}
		}
		


	}

	else
	{
		TracedItemValid = LineTracedItem;
		if (TracedItemValid)
		{
			TracedItemValid->GetPickUpWidget()->SetVisibility(false);
		}
		
		LineTracedItem = nullptr;
		

		
	}
	

}

void AMainCharacter::CalculateCrossHair(float DeltaTime)
{
	CrossHairSpreadValue = 0.5f + CrossHairMoveFactor + CrossHairJumpFactor - CrossHairZoomFactor + CrossHairFireFactor;

	if (bIsEquipped)
	{
		FVector2D MoveSpeedRange(0.f, 600.f);
		FVector2D ClampPerRange(0.f, 1.f);
		CrossHairMoveFactor = FMath::GetMappedRangeValueClamped(MoveSpeedRange, ClampPerRange, GetVelocity().Size());

		bool bJumping = GetCharacterMovement()->IsFalling();
		if (bJumping)
		{
			CrossHairJumpFactor = FMath::FInterpTo(CrossHairJumpFactor, 2.25f, DeltaTime, 2.f);
		}
		else
		{
			CrossHairJumpFactor = FMath::FInterpTo(CrossHairJumpFactor, 0.f, DeltaTime, 2.f);
		}

		if (bIsCrossHairFire)
		{
			CrossHairFireFactor = FMath::FInterpTo(CrossHairFireFactor, 0.5f, DeltaTime, 20.f);
		}
		else
		{
			CrossHairFireFactor = FMath::FInterpTo(CrossHairFireFactor, 0.f, DeltaTime, 20.f);
		}

		if (bZooming)
		{
			CrossHairZoomFactor = FMath::FInterpTo(CrossHairZoomFactor, 0.5f, DeltaTime, 20.f);
		}
		else
		{
			CrossHairZoomFactor = FMath::FInterpTo(CrossHairZoomFactor, 0.f, DeltaTime, 20.f);
		}
	}
}

void AMainCharacter::LockOnFunc()
{
	if (bIsEquipped)
	{
		const USkeletalMeshSocket* MuzzleSocket = EquippedWeaponGun->GetItemMesh()->GetSocketByName("GunMuzzleSocket");
		if (MuzzleSocket)
		{
			FHitResult FindHitLockOnTarget;
			FVector EndLocation;
			TraceUnderCrossHair(FindHitLockOnTarget, EndLocation);
			if (EquippedWeaponGun->GetWeaponType() == EWeaponType::EWT_BoomWeapon)
			{
				if (FindHitLockOnTarget.bBlockingHit)
				{
					LockOnEnemy = Cast<AEnemy>(FindHitLockOnTarget.Actor);
					if (LockOnEnemy&&bZooming)
					{
						FindTargetLockOn = true;
					}
					else
					{
						FindTargetLockOn = false;
					}
				}
			}
		}

	}
}

void AMainCharacter::CalSpringArm(float DeltaTime)
{
	float CurrentArmLength=CameraSpringArm->TargetArmLength;
	
	if (CharacterState == ECharacterState::ECS_Equipped)
	{
		CurrentArmLength = FMath::FInterpTo(CurrentArmLength, 200.f, DeltaTime, 10.f);
	}
	if (CharacterState == ECharacterState::ECS_Normal)
	{
		CurrentArmLength = FMath::FInterpTo(CurrentArmLength, 400.f, DeltaTime, 10.f);
	}
	CameraSpringArm->TargetArmLength = CurrentArmLength;

}



void AMainCharacter::AutoFireStart()
{
	if (!bIsDead)
	{
		if (bCouldFire)
		{
			if (bIsEquipped && EquippedWeaponGun->GetCurrentBullet() > 0 && bReloading == false)
			{
				Fire();
				bCouldFire = false;
				FTimerHandle AutoFireHandle;
				GetWorldTimerManager().SetTimer(AutoFireHandle, this, &AMainCharacter::AutoFireEnd, EquippedWeaponGun->GetFirePerSecond());
			}
		}
	}
}

void AMainCharacter::AutoFireEnd()
{
	bCouldFire = true;
	if (bFiring&&bReloading==false)
	{
		AutoFireStart();
	}
	
}

void AMainCharacter::AutoCrossHairFireStart()
{
	bIsCrossHairFire = true;
	FTimerHandle CrossHairHandle;
	GetWorldTimerManager().SetTimer(CrossHairHandle, this, &AMainCharacter::AutoCrossHairFireEnd, 0.05f);
}

void AMainCharacter::AutoCrossHairFireEnd()
{
	bIsCrossHairFire = false;
}

void AMainCharacter::Zoom()
{
	if (bIsEquipped)
	{
		bZooming = true;
		
	}
	else
	{
		bZooming = false;
	}
	ZoomFunction();

}

void AMainCharacter::ZoomOut()
{
	if (bIsEquipped)
	{
		bZooming = false;
		ZoomFunction();
	}

}


void AMainCharacter::ZoomFunction()
{
	if (!bIsDead)
	{
		if (bIsEquipped)
		{
			if(!bReloading)
			{
				if (bZooming)
				{
					if (EquippedWeaponGun->GetIsScope())
					{
						FPPCamera->AttachToComponent(EquippedWeaponGun->GetItemMesh(), FAttachmentTransformRules::KeepRelativeTransform, "SniperADS");
						FPPCamera->SetFieldOfView(15.f);
						TPPCamera->SetActive(false);
						FPPCamera->SetActive(true);
						bUseControllerRotationPitch = true;
						GetMesh()->HideBoneByName("Left Wrist", EPhysBodyOp::PBO_None);
					}
					else
					{
						FPPCamera->AttachToComponent(EquippedWeaponGun->GetItemMesh(), FAttachmentTransformRules::KeepRelativeTransform, "ADSCameraSocket");
						FPPCamera->SetFieldOfView(70.f);
						TPPCamera->SetActive(false);
						FPPCamera->SetActive(true);
						bUseControllerRotationPitch = true;
						GetMesh()->HideBoneByName("Left Wrist", EPhysBodyOp::PBO_None);
					}
				}
				else
				{
					this->SetActorRotation(FRotator(0, 90, 0));
					FPPCamera->SetActive(false);
					FPPCamera->SetFieldOfView(70.f);
					TPPCamera->SetActive(true);
					bUseControllerRotationPitch = false;
					GetMesh()->UnHideBoneByName("Left Wrist");
				}
			}
			else
			{
				this->SetActorRotation(FRotator(0, 90, 0));
				FPPCamera->SetActive(false);
				FPPCamera->SetFieldOfView(70.f);
				TPPCamera->SetActive(true);
				bUseControllerRotationPitch = false;
				GetMesh()->UnHideBoneByName("Left Wrist");
			}
		}
		else
		{
			FPPCamera->SetActive(false);
			TPPCamera->SetActive(true);
			bUseControllerRotationPitch = false;
			GetMesh()->UnHideBoneByName("Left Wrist");
		}
	}
	else
	{
		FPPCamera->SetActive(false);
		TPPCamera->SetActive(true);
		bUseControllerRotationPitch = false;
		GetMesh()->UnHideBoneByName("Left Wrist");
	}
}

void AMainCharacter::BulletHitEffect(AActor* DamageCauser)
{
	if (BloodParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, DamageCauser->GetTransform());
	}
}

void AMainCharacter::Reload()
{
	if (EquippedWeaponGun)
	{
		bZooming = false;
		if (bIsEquipped&&EquippedWeaponGun->GetBulletMagazineCount()!=EquippedWeaponGun->GetCurrentBullet())
		{
			bReloading = true;
			TPPCamera->SetActive(true);
			FPPCamera->SetActive(false);
			UAnimInstance* MainAnimInstance = GetMesh()->GetAnimInstance();
			if (MainAnimInstance && ReloadMontage)
			{
				if (bReloading)
				{
					switch (GetEquippedWeapon()->GetWeaponType())
					{
					case EWeaponType::EWT_MainWeapon:
					{
						MainAnimInstance->Montage_Play(ReloadMontage);
						MainAnimInstance->Montage_JumpToSection("Reload", ReloadMontage);
						FTimerHandle RealoadHandle;
						GetWorldTimerManager().SetTimer(RealoadHandle, this, &AMainCharacter::ReloadEnd, GetEquippedWeapon()->GetReloadTime());
					}break;
					case EWeaponType::EWT_SubWeapon:
					{
						MainAnimInstance->Montage_Play(ReloadMontage);
						MainAnimInstance->Montage_JumpToSection("Pistol Reload", ReloadMontage);
						FTimerHandle RealoadHandle;
						GetWorldTimerManager().SetTimer(RealoadHandle, this, &AMainCharacter::ReloadEnd, GetEquippedWeapon()->GetReloadTime());
					}break;
					case EWeaponType::EWT_BoomWeapon:
					{
						MainAnimInstance->Montage_Play(ReloadMontage);
						MainAnimInstance->Montage_JumpToSection("Rocket Reload", ReloadMontage);
						FTimerHandle RealoadRocketHandle;
						GetWorldTimerManager().SetTimer(RealoadRocketHandle, this, &AMainCharacter::ReloadEnd, GetEquippedWeapon()->GetReloadTime());
					}break;
					case EWeaponType::EWT_SpecialWeapon:
					{
						MainAnimInstance->Montage_Play(ReloadMontage);
						MainAnimInstance->Montage_JumpToSection("Reload", ReloadMontage);
						FTimerHandle RealoadHandle;
						GetWorldTimerManager().SetTimer(RealoadHandle, this, &AMainCharacter::ReloadEnd, GetEquippedWeapon()->GetReloadTime());
					}break;
					}
				}
			}
		}
	}
	else
	{
		bReloading = false;
	}
}

void AMainCharacter::ReloadFunction(int32 CurrentBulletCount)
{
	if (!bReloading)
	{
		if (EquippedWeaponGun->GetMaxBullet() > 0)
		{
			//탄창을 다채우기 위해 필요한 수량
			int32 ReloadBulletCount = EquippedWeaponGun->GetBulletMagazineCount() - EquippedWeaponGun->GetCurrentBullet();
		
			//가지고 있는 모든 총알 수가 탄창을 다 못채우는 경우
			if (ReloadBulletCount>EquippedWeaponGun->GetMaxBullet())
			{
				CurrentBulletCount = EquippedWeaponGun->GetCurrentBullet() + EquippedWeaponGun->GetMaxBullet();
			}

			else if (ReloadBulletCount <= EquippedWeaponGun->GetMaxBullet())
			{
				CurrentBulletCount = EquippedWeaponGun->GetCurrentBullet() +ReloadBulletCount;
			}
			EquippedWeaponGun->CalMaxBullet(ReloadBulletCount);

			EquippedWeaponGun->SetCurrentBullet(CurrentBulletCount);

		}

		if (EquippedWeaponGun->GetMaxBullet() <= 0)
		{
			EquippedWeaponGun->SetMaxBullet(0);
		}
	}
}
void AMainCharacter::ReloadEnd()
{
	if (!bIsDead)
	{
		if (EquippedWeaponGun)
		{
			bReloading = false;
			ReloadFunction(EquippedWeaponGun->GetCurrentBullet());
		}
	}
}



void AMainCharacter::PickUpEnd()
{
	AWeaponGun* guns = nullptr;
	PickUpFunc(guns);
}

void AMainCharacter::PickUpFunc(AWeaponGun* gun)
{
	const USkeletalMeshSocket* EquipSpare = GetMesh()->GetSocketByName("EquipWeapon1");
	const USkeletalMeshSocket* EquipSpare2 = GetMesh()->GetSocketByName("EquipWeapon2");
	const USkeletalMeshSocket* EquipSpare3 = GetMesh()->GetSocketByName("EquipWeapon3");
	const USkeletalMeshSocket* EquipSpare4 = GetMesh()->GetSocketByName("EquipWeapon4");
	gun = Cast<AWeaponGun>(LineTracedItem);

	if (gun == nullptr)return;
	if (gun)
	{
		if (bPickUp)
		{
			bPickUp = false;
			switch (gun->GetWeaponType())
			{
				case EWeaponType::EWT_MainWeapon:
				{
					MainWeaponSlot[0] = gun;
					MainWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
					EquipSpare->AttachActor(MainWeaponSlot[0], GetMesh());
				}break;
				case EWeaponType::EWT_SubWeapon:
				{
					SubWeaponSlot[0] = gun;
					SubWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
					EquipSpare2->AttachActor(SubWeaponSlot[0], GetMesh());
				}break;
				case EWeaponType::EWT_BoomWeapon:
				{
					BoomWeaponSlot[0] = gun;
					BoomWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
					EquipSpare3->AttachActor(BoomWeaponSlot[0], GetMesh());
				}break;
				case EWeaponType::EWT_SpecialWeapon:
				{
					SpecialWeaponSlot[0] = gun;
					SpecialWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
					EquipSpare4->AttachActor(SpecialWeaponSlot[0], GetMesh());
				}break;
			}
		}
	}
}

void AMainCharacter::SlotOneKeyPressed()
{
	if (MainWeaponSlot[0] != nullptr)
	{
		bZooming = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			bSwapping = true;
			FTimerHandle SwapHandle;
			if (GetEquippedWeapon() == nullptr)
			{
				AnimInstance->Montage_Play(SwapMontage);
				AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
				GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapMainWeapon, 0.8f);
			}
			else
			{
				switch (GetEquippedWeapon()->GetWeaponType())
				{
					case EWeaponType::EWT_MainWeapon:
					{
						
					}break;
					case EWeaponType::EWT_SubWeapon:
					{
						AnimInstance->Montage_Play(SwapMontage);
						AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
						GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapMainWeapon, 0.8f);
					}break;
					case EWeaponType::EWT_BoomWeapon:
					{
						AnimInstance->Montage_Play(LauncherSwapMontage);
						AnimInstance->Montage_JumpToSection("EquipLauncher", LauncherSwapMontage);
						GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapMainWeapon, 0.69f);
					}break;
					case EWeaponType::EWT_SpecialWeapon:
					{
						AnimInstance->Montage_Play(SpecialSwapMontage);
						AnimInstance->Montage_JumpToSection("EquipSpecial", SpecialSwapMontage);
						GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapMainWeapon, 0.8f);
					}break;
				}
			}
			
		}
	}
	
}

void AMainCharacter::SlotTwoKeyPressed()
{
	if (SubWeaponSlot[0] != nullptr)
	{
		bZooming = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			bSwapping = true;
			FTimerHandle SwapHandle;

			if (GetEquippedWeapon() == nullptr)
			{
				AnimInstance->Montage_Play(SwapMontage);
				AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
				GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSubWeapon, 0.8f);
			}
			else
			{
				switch (GetEquippedWeapon()->GetWeaponType())
				{
					case EWeaponType::EWT_MainWeapon:
					{
						AnimInstance->Montage_Play(SwapMontage);
						AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
						GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSubWeapon, 0.8f);
					}break;
					case EWeaponType::EWT_SubWeapon:
					{
					}break;
					case EWeaponType::EWT_BoomWeapon:
					{
						AnimInstance->Montage_Play(LauncherSwapMontage);
						AnimInstance->Montage_JumpToSection("EquipLauncher", LauncherSwapMontage);
						GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSubWeapon, 0.69f);
					}break;
					case EWeaponType::EWT_SpecialWeapon:
					{
						AnimInstance->Montage_Play(SpecialSwapMontage);
						AnimInstance->Montage_JumpToSection("EquipSpecial", SpecialSwapMontage);
						GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSubWeapon, 0.8f);
					}break;
				}
			}
		}
	}
}

void AMainCharacter::SlotThreeKeyPressed()
{
	if (BoomWeaponSlot[0] != nullptr)
	{
		bZooming = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			bSwapping = true;
			FTimerHandle SwapHandle;

			if (GetEquippedWeapon() == nullptr)
			{
				AnimInstance->Montage_Play(SwapMontage);
				AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
				GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapBoomWeapon, 0.8f);
			}
			else
			{
				switch (GetEquippedWeapon()->GetWeaponType())
				{
				case EWeaponType::EWT_MainWeapon:
				{
					AnimInstance->Montage_Play(SwapMontage);
					AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
					GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapBoomWeapon, 0.8f);
				}break;
				case EWeaponType::EWT_SubWeapon:
				{
					AnimInstance->Montage_Play(SwapMontage);
					AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
					GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapBoomWeapon, 0.8f);
				}break;
				case EWeaponType::EWT_BoomWeapon:
				{
				}break;
				case EWeaponType::EWT_SpecialWeapon:
				{
					AnimInstance->Montage_Play(SpecialSwapMontage);
					AnimInstance->Montage_JumpToSection("EquipSpecial", SpecialSwapMontage);
					GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapBoomWeapon, 0.8f);
				}break;
				}
			}
		}
	}
}

void AMainCharacter::SlotFourKeyPressed()
{
	if (SpecialWeaponSlot[0] != nullptr)
	{
		bZooming = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			bSwapping = true;
			FTimerHandle SwapHandle;

			if (GetEquippedWeapon() == nullptr)
			{
				AnimInstance->Montage_Play(SwapMontage);
				AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
				GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSpecialWeapon, 0.8f);
			}
			else
			{
				switch (GetEquippedWeapon()->GetWeaponType())
				{
				case EWeaponType::EWT_MainWeapon:
				{
					AnimInstance->Montage_Play(SwapMontage);
					AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
					GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSpecialWeapon, 0.8f);
				}break;
				case EWeaponType::EWT_SubWeapon:
				{
					AnimInstance->Montage_Play(SwapMontage);
					AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
					GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSpecialWeapon, 0.8f);
				}break;
				case EWeaponType::EWT_BoomWeapon:
				{
					AnimInstance->Montage_Play(LauncherSwapMontage);
					AnimInstance->Montage_JumpToSection("EquipLauncher", LauncherSwapMontage);
					GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapSpecialWeapon, 0.69f);
				}break;
				case EWeaponType::EWT_SpecialWeapon:
				{
				}break;
				}
			}

		}
	}
}

void AMainCharacter::SwapMainWeapon()
{
	if (bSwapping)
	{
		bZooming = false;
		const USkeletalMeshSocket* EquipSpare2 = GetMesh()->GetSocketByName("EquipWeapon2");
		const USkeletalMeshSocket* EquipSpare3 = GetMesh()->GetSocketByName("EquipWeapon3");
		const USkeletalMeshSocket* EquipSpare4 = GetMesh()->GetSocketByName("EquipWeapon4");

		if (MainWeaponSlot[0] != nullptr)
		{
			EquipWeapon(MainWeaponSlot[0]);
			if (SubWeaponSlot[0] != nullptr)
			{
				SubWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare2->AttachActor(SubWeaponSlot[0], GetMesh());
			}
			if (BoomWeaponSlot[0] != nullptr)
			{
				BoomWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare3->AttachActor(BoomWeaponSlot[0], GetMesh());
			}
			if (SpecialWeaponSlot[0] != nullptr)
			{
				SpecialWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare4->AttachActor(SpecialWeaponSlot[0], GetMesh());
			}
		}
		bSwapping = false;
	}
}

void AMainCharacter::SwapSubWeapon()
{
	if (bSwapping)
	{
		bZooming = false;
		const USkeletalMeshSocket* EquipSpare = GetMesh()->GetSocketByName("EquipWeapon1");
		const USkeletalMeshSocket* EquipSpare3 = GetMesh()->GetSocketByName("EquipWeapon3");
		const USkeletalMeshSocket* EquipSpare4 = GetMesh()->GetSocketByName("EquipWeapon4");
		if (SubWeaponSlot[0] != nullptr)
		{
			EquipWeapon(SubWeaponSlot[0]);
			if (MainWeaponSlot[0] != nullptr)
			{
				MainWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare->AttachActor(MainWeaponSlot[0], GetMesh());
			}
			if (BoomWeaponSlot[0] != nullptr)
			{
				BoomWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare3->AttachActor(BoomWeaponSlot[0], GetMesh());
			}
			if (SpecialWeaponSlot[0] != nullptr)
			{
				SpecialWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare4->AttachActor(SpecialWeaponSlot[0], GetMesh());
			}
		}
		bSwapping = false;
	}
}

void AMainCharacter::SwapBoomWeapon()
{
	if (bSwapping)
	{
		bZooming = false;
		const USkeletalMeshSocket* EquipSpare = GetMesh()->GetSocketByName("EquipWeapon1");
		const USkeletalMeshSocket* EquipSpare2 = GetMesh()->GetSocketByName("EquipWeapon2");
		const USkeletalMeshSocket* EquipSpare4 = GetMesh()->GetSocketByName("EquipWeapon4");
		if (BoomWeaponSlot[0] != nullptr)
		{
			EquipWeapon(BoomWeaponSlot[0]);
			if (MainWeaponSlot[0] != nullptr)
			{
				MainWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare->AttachActor(MainWeaponSlot[0], GetMesh());
			}
			if (SubWeaponSlot[0] != nullptr)
			{
				SubWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare2->AttachActor(SubWeaponSlot[0], GetMesh());
			}
			if (SpecialWeaponSlot[0] != nullptr)
			{
				SpecialWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare4->AttachActor(SpecialWeaponSlot[0], GetMesh());
			}
		}
		bSwapping = false;
	}
}

void AMainCharacter::SwapSpecialWeapon()
{
	if (bSwapping)
	{
		bZooming = false;
		const USkeletalMeshSocket* EquipSpare = GetMesh()->GetSocketByName("EquipWeapon1");
		const USkeletalMeshSocket* EquipSpare2 = GetMesh()->GetSocketByName("EquipWeapon2");
		const USkeletalMeshSocket* EquipSpare3 = GetMesh()->GetSocketByName("EquipWeapon3");
		if (SpecialWeaponSlot[0] != nullptr)
		{
			EquipWeapon(SpecialWeaponSlot[0]);
			if (MainWeaponSlot[0] != nullptr)
			{
				MainWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare->AttachActor(MainWeaponSlot[0], GetMesh());
			}
			if (SubWeaponSlot[0] != nullptr)
			{
				SubWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare2->AttachActor(SubWeaponSlot[0], GetMesh());
			}
			if (BoomWeaponSlot[0] != nullptr)
			{
				BoomWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
				EquipSpare3->AttachActor(BoomWeaponSlot[0], GetMesh());
			}
		}
		bSwapping = false;
	}
}

void AMainCharacter::UnEquip(AWeaponGun* Weapon)
{
	Weapon->SetItemState(EItemState::EIS_PickedUp);
}

void AMainCharacter::Recoil()
{
	//플레이어만 총기반동 적용
	if (this == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		float PitchValue = FMath::FRandRange(-0.01f, -0.1f);
		float YawValue = FMath::FRandRange(-0.1f, 0.1f);
		this->AddControllerPitchInput(PitchValue);
		this->AddControllerYawInput(YawValue);
	}
}

void AMainCharacter::Dead()
{

	SetCharacterState(ECharacterState::ECS_Death);
	AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (this!=Main)
	{
		DropWeapon();
	}

	UAnimInstance* MainAnimInstance = GetMesh()->GetAnimInstance();
	if (MainAnimInstance)
	{
		MainAnimInstance->Montage_Play(DeathMontage);
		MainAnimInstance->Montage_JumpToSection("Death", DeathMontage);
	}
	ALO3DGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALO3DGameModeBase>();

	if (GameMode != nullptr)
	{
		GameMode->CharacterKilled(this);
	}

	//죽었을시 사운드
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	}

	//컨트롤러 제거
	DetachFromControllerPendingDestroy();
	//콜리전 제거
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}




bool AMainCharacter::TraceUnderCrossHair(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	//뷰포트의 사이즈를 먼저 구한다
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	//라인트레이스를 어느지점부터 어느지점까지 할껀지 구하기 (시작지점 끝지점 구하기)
	FVector2D CrossHairLocation{ ViewportSize.X / 2.f, ViewportSize.Y / 2.f };	//스크린을 가로 1 세로 1이라 가정할때 길이를 반으로 나누면 화면 정가운데 지점이 된다
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;
	//DeprojectScreenToWorld는 2D상의 좌표를 3D로 변환 해주는 역할을 한다
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrossHairLocation, CrossHairWorldPosition, CrossHairWorldDirection);

	if (bScreenToWorld)
	{
		const FVector Start{ CrossHairWorldPosition };
		const FVector End{ Start + CrossHairWorldDirection * 50000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		//히트하는 경우
		if (OutHitResult.bBlockingHit)
		{
			//라인트레이싱 끝부분을 히트한 부분 위치로 변환
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

bool AMainCharacter::EnemyLinetrace(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	const FVector Start = EquippedWeaponGun->GetItemMesh()->GetSocketLocation("GunMuzzleSocket");
	const FVector End = (EquippedWeaponGun->GetItemMesh()->GetForwardVector() * 50000.f) + Start;
	OutHitLocation = End;

	GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);


	

	return true;
}



