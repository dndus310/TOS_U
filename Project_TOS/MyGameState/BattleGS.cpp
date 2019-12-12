// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGS.h"
#include "Define.h"

ABattleGS::ABattleGS()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ABattleGS::BeginPlay()
{
	Super::BeginPlay();
}
void ABattleGS::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
