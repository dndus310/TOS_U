// Fill out your copyright notice in the Description page of Project Settings.

#include "GM_ProjectTOS.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "SwordPlayer.h"
#include "Json.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

AGM_ProjectTOS::AGM_ProjectTOS()
{

}
void AGM_ProjectTOS::GetComboData()
{
	
}

void AGM_ProjectTOS::NextLevel(AActor* actor)
{
	FString GetLevelName = GetWorld()->GetMapName();
	GetLevelName.RemoveFromStart(TEXT("UEDPIE_0_"));
	GetLevelName = GetLevelName.Reverse();
	char LevelCountName = GetLevelName[0];
	int LevelCount = atoi(&LevelCountName);
	
	if (LevelCount >= MaxLevelCount)
	{
		return;
	}
	LevelCount += 1;
	FString Result = FString::FromInt(LevelCount);
	GetLevelName[0] = Result[0];
	GetLevelName = GetLevelName.Reverse();

	UGameplayStatics::OpenLevel(actor,FName(*GetLevelName));
}