// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "MyUI/BattleWidgetBase.h"
#include "MyPlayer/SwordPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerState/BattlePS.h"

ABattlePC::ABattlePC()
{
	PrimaryActorTick.bCanEverTick = true;

	BattleWidgetClass = UBattleWidgetBase::StaticClass();
}

void ABattlePC::BeginPlay()
{
	Super::BeginPlay();
	uint32 ControllerID = UGameplayStatics::GetPlayerControllerID(this);

	//UE_LOG(LogClass, Error, TEXT("The value of 'ControllerID' is: %i"), ControllerID);

	if (IsLocalPlayerController())
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		BattleWidgetBase = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
		BattleWidgetBase->AddToViewport();
	}
}

void ABattlePC::Tick(float DeltaSeconds)
{
	if (IsLocalPlayerController())
	{
		ABattlePS* PS = GetPlayerState<ABattlePS>();

		if (BattleWidgetBase && PS)
		{
			BattleWidgetBase->SetHpBar(PS->CurrentHP);
		}
	}
	if (HasAuthority())
	{
		//ASwordPlayer* Player = Cast<ASwordPlayer>(GetPawn());

		//if (Player)
		//{
		//	uint32 ControllerID = UGameplayStatics::GetPlayerControllerID(this);
		//	
		//	//UE_LOG(LogTemp, Error, TEXT("The value of 'Player(%d) is: %f"), ControllerID, Player->Status->GetCurrentHp());
		//}

	}
}

void ABattlePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Manualkey"), IE_Pressed, this, &ABattlePC::ShowManualKey);
}
void ABattlePC::ShowManualKey()
{
	BattleWidgetBase->DrawAnimation();
}
