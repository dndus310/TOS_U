// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeMonster.h"

// Sets default values
AMeleeMonster::AMeleeMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeleeMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMeleeMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

