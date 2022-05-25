// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoverHealth.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"

ARecoverHealth::ARecoverHealth():
	RecoverHealthAmount(30.f)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh->SetupAttachment(GetRootComponent());

	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ARecoverHealth::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == nullptr) return;
	Main = Cast<AMainCharacter>(OtherActor);

	if (OtherActor == Main)
	{
		RecoverHealthCalculate(Main);
		Destroy();
	}
	else
	{
		return;
	}

}

void ARecoverHealth::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);



}

void ARecoverHealth::RecoverHealthCalculate(AMainCharacter* mainchar)
{
	if (mainchar)
	{
		if (mainchar->GetMaxHealth() > mainchar->GetHealth())
		{
			float RecoverAmount = RecoverHealthAmount + mainchar->GetHealth();
			
			if (mainchar->GetMaxHealth() <= RecoverAmount)
			{
				RecoverAmount = mainchar->GetMaxHealth();
				mainchar->SetHealth(RecoverAmount);
			}
			else
			{
				mainchar->SetHealth(RecoverAmount);
			}

		}
	}
}
