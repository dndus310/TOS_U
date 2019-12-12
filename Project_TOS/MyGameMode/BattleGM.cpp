// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGM.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "MyPlayer/SwordPlayer.h"
#include "Json.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "MyController/BattlePC.h"
#include "MyPlayerState/BattlePS.h"
#include "SpawnWorldPoint.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "MainGI.h"

ABattleGM::ABattleGM()
{
	DefaultPawnClass = ASwordPlayer::StaticClass();
	PlayerControllerClass = ABattlePC::StaticClass();
}
void ABattleGM::BeginPlay()
{
	Super::BeginPlay();

	UMainGI* GI = Cast<UMainGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->SetLevelName(UGameplayStatics::GetCurrentLevelName(GetWorld()));

	if (GetNetMode() == ENetMode::NM_Standalone)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnWorldPoint::StaticClass(), SpawnPoints);
	}
	else if (GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		TArray<AActor*> WorldPoints = {};

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnWorldPoint::StaticClass(), WorldPoints);

		for (auto iter : WorldPoints)
		{
			ASpawnWorldPoint* SpawnPoint = Cast<ASpawnWorldPoint>(iter);

			if (SpawnPoint->SpawnType == ESpawnType::Player)
			{
				StartPoints.Add(SpawnPoint->GetActorLocation());
			}
		}
	}
}
void ABattleGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ABattleGM::StartSpawn()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABattleGM::OnSpawnGenerator, 2.f, true);
}
void ABattleGM::EndSpawn()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}
void ABattleGM::NextLevel(FName LevelName)
{
	if (HasAuthority() && (GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_DedicatedServer))
	{
		GetWorld()->ServerTravel(LevelName.ToString());
	}
	else
	{
		UGameplayStatics::OpenLevel(this, LevelName);
	}

}
void ABattleGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//ABattleGS* GS = GetGameState<ABattleGS>();
	//if (GS)
	//{
	//	GS->ConnectCount++;

	//	if (HasAuthority() && GetNetMode() == ENetMode::NM_ListenServer) //Server Execute
	//	{
	//		//UI Update, ListenServer
	//		GS->ConnectCount_OnRep();
	//	}
	//}
}
void ABattleGM::Pause()
{
	UGameplayStatics::SetGamePaused(this, true);
}
void ABattleGM::UnPause()
{
	UGameplayStatics::SetGamePaused(this, false);
}
void ABattleGM::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	//ABattleGS* GS = GetGameState<ABattleGS>();
	//if (GS)
	//{
	//	GS->ConnectCount--;

	//	if (HasAuthority()) //Server Execute
	//	{
	//		GS->ConnectCount_OnRep();
	//	}
	//}
}

FVector ABattleGM::GetAutoStartPoint()
{
	FVector Result = StartPoints[CurrentCount];

	if (CurrentCount < StartPoints.Num())
	{
		++CurrentCount;
	}
	else
	{
		CurrentCount = 0;
	}
	return Result;
}

void ABattleGM::OnSpawnGenerator()
{
	for (auto Location : SpawnPoints)
	{
		ASpawnWorldPoint* SpawnPoint = Cast<ASpawnWorldPoint>(Location);

		if (SpawnPoint)
		{
			if (RequestGroup == SpawnPoint->SpawnGroup)
			{
				if (SpawnPoint->SpawnType == ESpawnType::Melee)
				{
					GetWorld()->SpawnActor<AMeleeMonster>(MeleeMonsterClass, Location->GetTransform());
				}
				else if (SpawnPoint->SpawnType == ESpawnType::Ranged)
				{
					GetWorld()->SpawnActor<ARangedMonster>(RangedMonsterClass, Location->GetTransform());
				}
			}
		}
	}
	if ((uint8)RequestGroup < (uint8)ESpawnGroup::GroupEnd)
	{
		uint8 Add = (uint8)RequestGroup;
		RequestGroup = (ESpawnGroup)(Add + 1);
	}
	else
	{
		uint8 Zero = (uint8)ESpawnGroup::GroupA;
		RequestGroup = (ESpawnGroup)Zero;
	}
}
void ABattleGM::CheckCount()
{
	if (GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_DedicatedServer)	
	{
		int AliveCount = 0;

		for (TActorIterator<AActor> iter(GetWorld()); iter; ++iter)
		{
			AActor* Actor = *iter;
			if (Actor->ActorHasTag(FName(TEXT("Player"))))
			{
				ASwordPlayer* PlayerActor = Cast<ASwordPlayer>(*iter);
				FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

				if (PlayerActor && PlayerActor->PlayerPS->CurrentHP > 0.f)
				{
					++AliveCount;
				}
			}
		}
		//UE_LOG(LogClass, Error, TEXT("Log message Player AliveCount ID(%d)"), AliveCount);

		if (AliveCount <= 1)
		{
			NextLevel(TEXT("Scene01"));
		}
	}
}