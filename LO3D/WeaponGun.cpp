// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/Texture2D.h"
#include "Camera/CameraComponent.h"
#include "kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"

//생성자
AWeaponGun::AWeaponGun() :
	SniperScope(nullptr),MaxBullet(0), BaseMaxBullet(0), CurrentBullet(0), BulletMagazineCount(0), WeaponName("Default"),ItemStates(EItemState::EIS_Normal),
	WeaponType(EWeaponType::EWT_MainWeapon), SlotIndex(0),ReloadTime(3.f), bFalling(false)
{
	

	SniperScope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperScope"));
	SniperScope->SetupAttachment(ItemMesh);
	SniperScope->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SniperScope->AttachToComponent(GetItemMesh(), FAttachmentTransformRules::KeepRelativeTransform, "SnipeScope");

}

void AWeaponGun::BeginPlay()
{
	Super::BeginPlay();

	SetItemStateProperty(ItemStates);
	
	

	

}

void AWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}


	
}

void AWeaponGun::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == nullptr) return;

	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));

		if (OtherActor == Main)
		{
			Main->OverlapItemCount(1);
		}
		else
		{
			return;
		}
	}

}

void AWeaponGun::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor == nullptr) return;
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (OtherActor==Main)
		{
			Main->OverlapItemCount(-1);
		}
		else
		{
			return;
		}
	}

}

void AWeaponGun::SetItemStateProperty(EItemState State)
{
	switch (State)
	{
		//평상시 상태(아이템 미장착)
		/*
			아이템을 미장착한 상태에서는
			라인트레이싱을 할때 라인이 콜리전에 부딫혀 아이템 정보를 얻어야 한다
		*/
	case EItemState::EIS_Normal:
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		//아이템을 탐지하는 거리 콜리전 정보
		CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		//쿼리(레이캐스트, 스윕, 오버랩)만 받는다
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		//아이템 콜리전에 대한 정보
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

		//아이템 장착 상태
		/*
			아이템이 장착되면 장착한 아이템의 위젯과 콜리전들이 전부 사라지고 아이템 모양만 남으면 된다
		*/
	case EItemState::EIS_Equip:
		bItemRotate = false;
		PickUpItemWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_PickedUp:
		PickUpItemWidget->SetVisibility(false);
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);




		CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Falling:
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		CollisionSphere->SetSimulatePhysics(true);
		CollisionSphere->SetEnableGravity(true);
		CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_EquipSpare:
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	

		CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bItemRotate = false;
		break;

	}



}

void AWeaponGun::SetWeaponType(EWeaponType weaponType)
{
	WeaponType = weaponType;
}

int32 AWeaponGun::SetCurrentBullet(int32 useBullet)
{
	CurrentBullet = useBullet;
	return CurrentBullet;
}

int32 AWeaponGun::SetMaxBullet(int32 useBullet)
{
	MaxBullet = useBullet;
	return MaxBullet;
}

int32 AWeaponGun::CalCurrnetBullet(int32 useBullet)
{
	CurrentBullet -= useBullet;
	return CurrentBullet;
}

int32 AWeaponGun::CalMaxBullet(int32 useBullet)
{
	MaxBullet -= useBullet;
	return MaxBullet;
}

void AWeaponGun::ThrowWeapon()
{
	FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw,0.f };
	bFalling = true;
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeaponGun::StopFalling, 0.7f);



}

void AWeaponGun::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Normal);
}

void AWeaponGun::SetItemState(EItemState State)
{
	ItemStates = State;
	SetItemStateProperty(State);
}







