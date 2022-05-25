// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"
#include "WeaponGun.h"
#include "Kismet/GameplayStatics.h"

AAmmo::AAmmo()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh->SetupAttachment(GetRootComponent());

	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

/* Overlap */
void AAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == nullptr) return;

	//플레이어가 overlap을 하는 경우에만 작동
	Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));

	if (OtherActor == Main)
	{
		if (Main->EquippedWeaponGun != nullptr)
		{
			AmmoCalculate(Main);
			Destroy();
		}
	}
	else
	{
		return;
	}

}

void AAmmo::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor == nullptr) return;

}

/* 총알 계산 */
void AAmmo::AmmoCalculate(AMainCharacter* mainchar)
{
	if (mainchar)
	{
		//충전되는 총알 갯수가 현재 장착한 총이 최대 가질수 있는 총알 갯수를 넘어가는 경우
		if (mainchar->EquippedWeaponGun->GetBaseMaxBullet() <= mainchar->EquippedWeaponGun->GetMaxBullet() + GetAmmo())
		{
			mainchar->EquippedWeaponGun->SetMaxBullet(mainchar->EquippedWeaponGun->GetBaseMaxBullet());
		}
		else
		{
			int32 AmmoReChargeCount = mainchar->EquippedWeaponGun->GetMaxBullet() + GetAmmo();
			mainchar->EquippedWeaponGun->SetMaxBullet(AmmoReChargeCount);
		}
	}
}
