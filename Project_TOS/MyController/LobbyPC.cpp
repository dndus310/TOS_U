// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "MyUI/LobbyWidgetBase.h"

ALobbyPC::ALobbyPC()
{
	LobbyWidgetClass = ULobbyWidgetBase::StaticClass();
}
void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	//#include "Path", 경로, UE4 Asset, BP
	FSoftClassPath LobbyWidgetClass_BP_PATH(TEXT("WidgetBlueprint'/Game/_TOS/BluePrints/StandardBP/UI/Lobby/UI/BP_LobbyWidget.BP_LobbyWidget_C'"));

	//#include "" 실행
	UClass* LobbyWidgetClass_BP = LobbyWidgetClass_BP_PATH.TryLoadClass<ULobbyWidgetBase>();

	if (IsLocalPlayerController())
	{
		if (LobbyWidgetClass_BP)
		{
			LobbyWidgetBase = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass_BP);

			if (LobbyWidgetBase)
			{
				LobbyWidgetBase->AddToViewport();

				SetInputMode(FInputModeUIOnly());
				bShowMouseCursor = true;
			}
		}
	}
}
