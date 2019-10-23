// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnWorldPoint.h"
#include "Components/BillboardComponent.h"
#include "ConstructorHelpers.h"

ASpawnWorldPoint::ASpawnWorldPoint()
{
#if WITH_EDITOR
	GetSpriteComponent()->RelativeScale3D = FVector(1, 1, 1);
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> S_Icon(TEXT("Texture2D'/Game/ParagonMinions/FX/Textures/Misc/T_Jungle_Spawn_DiamondShape_Icon_01_D.T_Jungle_Spawn_DiamondShape_Icon_01_D'"));

	if (S_Icon.Succeeded())
	{
		GetSpriteComponent()->SetSprite(S_Icon.Object);
	}
#endif // WITH_EDITOR
}
