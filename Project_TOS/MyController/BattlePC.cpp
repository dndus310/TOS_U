// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePC.h"
#include "MyUI/BattleWidgetBase.h"
#include "MyUI/PlayerInfoWidgetBase.h"
#include "MyPlayer/SwordPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerState/BattlePS.h"
#include "Components/CanvasPanel.h"
#include "MyGameMode/BattleGM.h"
#include "TimerManager.h"

ABattlePC::ABattlePC()
{
	PrimaryActorTick.bCanEverTick = true;

	BattleWidgetClass = UBattleWidgetBase::StaticClass();

	PlayerInfoWidgetClass = UPlayerInfoWidgetBase::StaticClass();
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

		PlayerInfoWidgetBase = CreateWidget<UPlayerInfoWidgetBase>(this, PlayerInfoWidgetClass);
		PlayerInfoWidgetBase->AddToViewport();

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

	InputComponent->BindAction(TEXT("PlayerInfo"), IE_Pressed, this, &ABattlePC::ShowPlayerInfo);
}
void ABattlePC::ShowPlayerInfo()
{
	if (GetNetMode() == ENetMode::NM_Standalone)
	{
		if (PlayerInfoWidgetBase->InfoPanel->Visibility == ESlateVisibility::Hidden)
		{
			ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(this));

			if (GM)
			{
				GM->Pause();

				bShowMouseCursor = true;
				PlayerInfoWidgetBase->InfoPanel->SetVisibility(ESlateVisibility::Visible);
				SetInputMode(FInputModeUIOnly());
			}
		}
	}
}
void ABattlePC::ShowManualKey()
{
	BattleWidgetBase->DrawAnimation();
}
