// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API UPlayerInfoWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

	UFUNCTION(BlueprintCallable)
	void OnExit();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCanvasPanel* InfoPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UButton* ExitButton;
};
