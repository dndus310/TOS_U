// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePS.h"
#include "UnrealNetwork.h"
#include "MyPlayer/SwordPlayer.h"
#include "MyComponent/StatusComponent.h"
#include "Kismet/GameplayStatics.h"

ABattlePS::ABattlePS()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}
void ABattlePS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
void ABattlePS::BeginPlay()
{
	Super::BeginPlay();

	//Player = GetPawn<ASwordPlayer>();


}
void ABattlePS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogClass, Error, TEXT("The value of 'CurrentHP' is: %f"), CurrentHP);


}
void ABattlePS::SetPlayer(ASwordPlayer * Value)
{
	Player = Value;
}
void ABattlePS::SetCurrentHP(float Value)
{
	//UE_LOG(LogClass, Error, TEXT("The value of 'Value' is: %f"), Value);

	CurrentHP = Value;

	//UE_LOG(LogClass, Error, TEXT("The value of 'CurrentHP' is: %f"), CurrentHP);

	StoA_SetCurrentHP(Value);
}

void ABattlePS::CtoS_SetCurrentHP_Implementation(float Value)
{
	StoA_SetCurrentHP(Value);
}

void ABattlePS::StoA_SetCurrentHP_Implementation(float Value)
{
	if (Player)
	{
		CurrentHP = Value;

		if (CurrentHP <= 0.f)
		{
			Player->Status->ExcuteDeadEvent();
			
		}
	}

	//if (Player)
	//{
	//	CurrentHP = Value;
	//}
}
