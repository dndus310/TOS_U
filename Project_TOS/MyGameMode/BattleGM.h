// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnWorldPoint.h"
#include "BattleGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ABattleGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABattleGM();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController * NewPlayer) override;
	virtual void Logout(AController * Exiting) override;
public:
	void Pause();
	void UnPause();
	void StartSpawn();
	void EndSpawn();
	void NextLevel(FName LevelName);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AMeleeMonster> MeleeMonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ARangedMonster> RangedMonsterClass;


	FTimerHandle PauseHandle;
	FTimerHandle SpawnTimer;
	TArray<AActor*> SpawnPoints;
	ESpawnGroup RequestGroup = ESpawnGroup::GroupA;

	int CurrentCount = 0;
	TArray<FVector> StartPoints;
public:
	FVector GetAutoStartPoint();
	void OnSpawnGenerator();
	void CheckCount();
};
