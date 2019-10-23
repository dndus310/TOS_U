// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SpawnWorldPoint.generated.h"


UENUM()
enum class ESpawnGroup : uint8
{
	GroupA = 0 UMETA(DisplayName = "GroupA"),
	GroupB = 1 UMETA(DisplayName = "GroupB"),
	GroupC = 2 UMETA(DisplayName = "GroupC"),
	GroupD = 3 UMETA(DisplayName = "GroupD"),
	GroupEnd = 4,
};
UENUM()
enum class ESpawnType : uint8
{
	Melee = 0 UMETA(DisplayName = "Melee"),
	Ranged = 1 UMETA(DisplayName = "Ranged"),
	Item = 2 UMETA(DisplayName = "Item"),
	Boss = 3 UMETA(DisplayName = "Boss"),
	Player = 4 UMETA(DisplayName = "Player"),
};
/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ASpawnWorldPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ASpawnWorldPoint();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnType SpawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnGroup SpawnGroup;
	
};
