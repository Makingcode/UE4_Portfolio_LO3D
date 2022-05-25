// Fill out your copyright notice in the Description page of Project Settings.


#include "GunEnemy.h"

// Sets default values
AGunEnemy::AGunEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGunEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGunEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

