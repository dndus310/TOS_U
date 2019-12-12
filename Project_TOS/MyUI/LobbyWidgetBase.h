// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TOS_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEditableTextBox* AddressTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEditableTextBox* UseridText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEditableTextBox* UserpwdText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* ConnectButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* CancelButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* SingleStartButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* MultiStartButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* ExitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCanvasPanel* LoginPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCanvasPanel* ValuesPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCanvasPanel* LoadingPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCanvasPanel* LevelPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* MakeServerButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* ConnectServerButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UButton* CancelServerButton;

private:
	FTimerHandle FConnectTimeHandle;
	bool IsLoginResult;
public:
	UFUNCTION(BlueprintCallable)
	void Connect();

	UFUNCTION(BlueprintCallable)
	void LoginPanelCancel();

	UFUNCTION(BlueprintCallable)
	void LevelPanelCancel();

	UFUNCTION(BlueprintCallable)
	void OpenLoginPanel();

	UFUNCTION(BlueprintCallable)
	void ExitGame();

	UFUNCTION(BlueprintCallable)
	void SingleGameStart();

	UFUNCTION(BlueprintCallable)
	void MakeServer();

	UFUNCTION(BlueprintCallable)
	void ConnectServer();

private:
	void ConnectResult(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void CompleteDelayConnect();
public:
	bool GetLoginResult() const { return IsLoginResult; }
};
