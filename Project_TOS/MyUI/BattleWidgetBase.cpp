// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidgetBase.h"
#include "Components/ProgressBar.h"
#include "Components/EditableText.h"
#include "Components/CanvasPanel.h"
#include "LobbyWidgetBase.h"
#include "Animation/UMGSequencePlayer.h"

void UBattleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ManuPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("ManuPanel")));
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	StaminaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));
	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ExpBar")));
	LevelCount = Cast<UEditableText>(GetWidgetFromName(TEXT("LevelCount")));

	
	GetAnimations(WidgetAnimations);
	DrawAnim = *(WidgetAnimations.Find("DrawAnimation"));
	ExpBar->Percent = 0.f;
}
void UBattleWidgetBase::OnAnimationFinished_Implementation(const UWidgetAnimation * Animation)
{
	if (Animation->GetClass() == DrawAnim->GetClass())
	{
		IsDrawAnim = false;
	}
}
void UBattleWidgetBase::GetAnimations(TMap<FString, UWidgetAnimation*>& OutResults)
{
	OutResults.Empty();

	UProperty* Property = GetClass()->PropertyLink;
	while (Property != nullptr)
	{
		if (Property->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property);

			if (ObjectProperty->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Object = ObjectProperty->GetObjectPropertyValue_InContainer(this);
				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Object);

				if (WidgetAnimation != nullptr)
				{
					FString Name = WidgetAnimation->GetMovieScene()->GetFName().ToString();
					OutResults.Add(Name, WidgetAnimation);
				}
			}
		}

		Property = Property->PropertyLinkNext;
	}
}
void UBattleWidgetBase::OnDrawAnimFinished(UUMGSequencePlayer& Player)
{

}
void UBattleWidgetBase::DrawAnimation()
{
	if (IsDrawAnim)
	{
		return;
	}
	switch (PlayMode)
	{
	case EUMGSequencePlayMode::Forward:
		PlayMode = EUMGSequencePlayMode::Reverse;
		PlayAnimation(DrawAnim, 0.f, 1, PlayMode, 1.f);
		break;
	case EUMGSequencePlayMode::Reverse:
		PlayMode = EUMGSequencePlayMode::Forward;
		PlayAnimation(DrawAnim, 0.f, 1, PlayMode, 1.f);
		break;
	}
	IsDrawAnim = true;
}

void UBattleWidgetBase::SetHpBar(float Value)
{
	HpBar->SetPercent(Value * 0.01f);
}
