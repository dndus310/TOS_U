// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "Kismet/GameplayStatics.h"
#include "MainGI.h"
#include "Engine/World.h"

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	UMainGI* GI = Cast<UMainGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->SetLevelName(UGameplayStatics::GetCurrentLevelName(GetWorld()));
}

void ALobbyGM::NextLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Scene02"));
}
