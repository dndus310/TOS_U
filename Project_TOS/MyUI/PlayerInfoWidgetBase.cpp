// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidgetBase.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "MyGameMode/BattleGM.h"
#include "Kismet/GameplayStatics.h"
#include "MyController/BattlePC.h"

void UPlayerInfoWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	InfoPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("InfoPanel")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UPlayerInfoWidgetBase::OnExit);
	}

	InfoPanel->SetVisibility(ESlateVisibility::Hidden);

}

void UPlayerInfoWidgetBase::OnAnimationFinished_Implementation(const UWidgetAnimation * Animation)
{
}

void UPlayerInfoWidgetBase::OnExit()
{
	if (InfoPanel->Visibility == ESlateVisibility::Visible)
	{
		ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(this));
		ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(this, 0));

		if (GM && PC && GM->GetNetMode() == ENetMode::NM_Standalone)
		{
			GM->UnPause();
			InfoPanel->SetVisibility(ESlateVisibility::Hidden);
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}
