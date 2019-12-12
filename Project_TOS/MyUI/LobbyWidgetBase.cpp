// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "MainGI.h"
#include "Kismet/GameplayStatics.h"
#include "Http.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "MainGI.h"
#include "MyGameMode/LobbyGM.h"


void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	AddressTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("AddressTextBox")));
	UseridText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UseridText")));
	UserpwdText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserpwdText")));
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelButton")));
	SingleStartButton = Cast<UButton>(GetWidgetFromName(TEXT("SingleStartButton")));
	MultiStartButton = Cast<UButton>(GetWidgetFromName(TEXT("MultiStartButton")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
	MakeServerButton = Cast<UButton>(GetWidgetFromName(TEXT("MakeServerButton")));
	ConnectServerButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectServerButton")));
	CancelServerButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelServerButton")));

	LoginPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("LoginPanel")));
	ValuesPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("ValuesPanel")));
	LoadingPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("LoadingPanel")));
	LevelPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("LevelPanel")));


	if (LevelPanel)
	{
		LevelPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (LoginPanel)
	{
		LoginPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (MultiStartButton)
	{
		MultiStartButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OpenLoginPanel);
	}
	if (SingleStartButton)
	{
		SingleStartButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::SingleGameStart);
	}
	if(ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::ExitGame);
	}
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::Connect);
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::LoginPanelCancel);
	}
	if (MakeServerButton)
	{
		MakeServerButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::MakeServer);
	}
	if (ConnectServerButton)
	{
		ConnectServerButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::ConnectServer);
	}
	if (CancelServerButton)
	{
		CancelServerButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::LevelPanelCancel);
	}
}
void ULobbyWidgetBase::Connect()
{
	UMainGI* GI = Cast<UMainGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	GetWorld()->GetTimerManager().SetTimer(FConnectTimeHandle, this, &ULobbyWidgetBase::CompleteDelayConnect, 0.5f, false);
	ValuesPanel->SetVisibility(ESlateVisibility::Collapsed);
	LoadingPanel->SetVisibility(ESlateVisibility::Visible);
	if (GI)
	{
		FString url = AddressTextBox->GetText().ToString() + "/login";
		FString id = UseridText->GetText().ToString();
		FString pwd = UserpwdText->GetText().ToString();

		GI->HTTPPost(url, id, pwd, FHttpRequestCompleteDelegate::CreateUObject(this, &ULobbyWidgetBase::ConnectResult));
	}
}
void ULobbyWidgetBase::OpenLoginPanel()
{
	if (LoginPanel->GetVisibility() != ESlateVisibility::Visible)
	{
		LoginPanel->SetVisibility(ESlateVisibility::Visible);
		LoadingPanel->SetVisibility(ESlateVisibility::Collapsed);
		ValuesPanel->SetVisibility(ESlateVisibility::Visible);
	}
}
void ULobbyWidgetBase::ExitGame()
{
#if UE_GAME
	FGenericPlatformMisc::RequestExit(false);
#endif // UE_GAME
}
void ULobbyWidgetBase::SingleGameStart()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->NextLevel();
}
void ULobbyWidgetBase::MakeServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Scene03")), true, TEXT("listen"));
}
void ULobbyWidgetBase::ConnectServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1"));
}
void ULobbyWidgetBase::ConnectResult(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString Result = Response->GetContentAsString();
		if (Result.Contains("Success"))
		{
			//UE_LOG(LogTemp, Error, TEXT("Log Success"));
			IsLoginResult = true;
		}
		if(Result.Contains("Fail"))
		{
			//UE_LOG(LogTemp, Error, TEXT("Log Fail"));
			IsLoginResult = false;
		}
	}

}
void ULobbyWidgetBase::CompleteDelayConnect()
{
	GetWorld()->GetTimerManager().ClearTimer(FConnectTimeHandle);

	if (IsLoginResult)
	{
		LoginPanel->SetVisibility(ESlateVisibility::Collapsed);
		LevelPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LoadingPanel->SetVisibility(ESlateVisibility::Collapsed);
		ValuesPanel->SetVisibility(ESlateVisibility::Visible);
	}

}
void ULobbyWidgetBase::LoginPanelCancel()
{
	LoginPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidgetBase::LevelPanelCancel()
{
	LevelPanel->SetVisibility(ESlateVisibility::Collapsed);
}
