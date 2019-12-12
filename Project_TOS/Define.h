// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Define.generated.h"

/**
 * 
 */
UENUM()
enum class ERelational
{
	OR UMETA(DisplayName = "OR"),
	AND UMETA(DisplayName = "AND")
};
UENUM()
enum class EStartMode
{
	SingleMode = 0,
	MultiMode = 1,
};
USTRUCT()
struct FStageSetting
{
	GENERATED_USTRUCT_BODY()
public:
	float MaxFazeTime;
	int SpawnCount;
};
USTRUCT()
struct FStageData
{
	GENERATED_USTRUCT_BODY()
public:
	TArray<FStageData> StageArray;
};

USTRUCT()
struct FTracePosition
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		TArray<FVector> TracePositions;
	FTracePosition()
	{
		TracePositions.Empty();
	}
};
USTRUCT()
struct FTraceData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		TMap < FString, FTracePosition> TraceMap;
	FTraceData()
	{
		TraceMap.Empty();
	}
};



