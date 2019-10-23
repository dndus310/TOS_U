// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "MainGI.generated.h"

/**
 * 
 */

 /**
  * Comment
  */



UCLASS()
class PROJECT_TOS_API UMainGI : public UGameInstance
{
	GENERATED_BODY()
public:
	UMainGI();

	void HTTPPost(FString URL, FString UserId, FString Password, FHttpRequestCompleteDelegate ProcessRequestComplete);
	void HTTPResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	class FHttpModule* Http;
private:
	FString Userid;
	FString Userpwd;
	FString CurrentLevelName;

	int MaxLevelCount = 3;
public:
	void SaveAccount(FText id, FText pwd);
	void SetLevelName(FString LevelName);

	UFUNCTION(BlueprintCallable)
	void NextLevel();
public:
	FString GetCurrentLevelName() const;
};
