// Fill out your copyright notice in the Description page of Project Settings.


#include "BV_Door.h"
#include "Engine/Public/Model.h"
#include "MyPlayer/SwordPlayer.h"
#include "MyGameMode/BattleGM.h"
#include "Kismet/GameplayStatics.h"

ABV_Door::ABV_Door()
{
	
}
void ABV_Door::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ABV_Door::BeginOverlap);
}
#if WITH_EDITOR
void ABV_Door::LoadedFromAnotherClass(const FName & OldClassName)
{

}
void ABV_Door::PostEditChangeChainProperty(FPropertyChangedChainEvent & PropertyChangedEvent)
{

}
#endif // WITH_Edi


void ABV_Door::BeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		//UE_LOG(LogClass, Error, TEXT("Log EndOverlap"));
		ASwordPlayer* Player = Cast<ASwordPlayer>(OtherActor);
		Player->SetCollisionPresset(TEXT("PlayerPresset"));
		OnActorBeginOverlap.Clear();

		if (GetNetMode() == ENetMode::NM_Standalone)
		{
			ABattleGM* BttleGM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(this));
			BttleGM->StartSpawn();
		}
	}
}
