// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Http.h"
#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "Define.h"
#include "JsonObjectConverter.h"

#include "MainGI.generated.h"

/**
 * 
 */

 /**
  * Comment
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
	template<typename StructType> StructType JsonToStructLoadData(StructType& OutStruct, const FString FileName);
	template<typename StructType> void StructToJsonSaveData(const StructType& InStruct, const FString NewFileName);
	FString ServerLoadData(const FString TableName, const FString UserId, const FString Password);
	void ServerSaveData(const FString TableName, const FString JsonData, FHttpRequestCompleteDelegate ProcessRequestComplete);

	void HTTPPost(const FString URL, const FString UserId, const FString Password, FHttpRequestCompleteDelegate ProcessRequestComplete);
	void HTTPResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	class FHttpModule* Http;
private:
	FString Userid;
	FString Userpwd;
	FString CurrentLevelName;
	int MaxLevelCount = 3;
	FStageData StageData;
public:
	void SaveAccount(const FText id, const FText pwd);
	void SetLevelName(const FString LevelName);

	UFUNCTION(BlueprintCallable)
	void NextLevel();
public:
	FString GetCurrentLevelName() const;
};



