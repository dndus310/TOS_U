// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedMonster.h"

// Sets default values
ARangedMonster::ARangedMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARangedMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangedMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARangedMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

