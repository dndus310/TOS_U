// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "BattleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UBattleWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCanvasPanel* ManuPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProgressBar* HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProgressBar* StaminaBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProgressBar* ExpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEditableText* LevelCount;

	class UWidgetAnimation* DrawAnim;

	bool IsDrawAnim = false;
private:
	UUserWidget* BattleWidget;
	EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Reverse;
	TMap<FString, UWidgetAnimation*> WidgetAnimations;
	void GetAnimations(TMap<FString, UWidgetAnimation*>& OutResults);
private:
	void OnDrawAnimFinished(UUMGSequencePlayer& Player);
public:
	void DrawAnimation();
	void SetHpBar(float Value);
};
