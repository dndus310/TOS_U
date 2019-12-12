// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGI.h"
#include "Http.h"
#include "Json.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "JsonObjectConverter.h"
#include "JsonUtilities.h"


UMainGI::UMainGI() : 
	Http(NULL), Userid(FString()), Userpwd(FString())
{
	
}
template<typename StructType>
StructType UMainGI::JsonToStructLoadData(StructType& OutStruct, const FString FileName)
{
	FString Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + "/" + FileName + ".json";
	FString JsonData = "";
	FFileHelper::LoadFileToString(JsonData, *Path);

	// LoadJSON
	if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonData, &OutStruct, 0, 0))
	{
		UE_LOG(LogClass, Log, TEXT("Json Load Success"));
	}
	return OutStruct;
}
template<typename StructType>
void UMainGI::StructToJsonSaveData(const StructType & InStruct, const FString NewFileName)
{
	FString Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + "/" + NewFileName + ".json";
	FString JsonData = "";

	// SaveJSON
	if (!FJsonObjectConverter::UStructToJsonObjectString(InStruct, JsonData, 0, 0))
	{
		UE_LOG(LogClass, Log, TEXT("Json Save Success"));
	}
	FFileHelper::SaveStringToFile(*JsonData, *Path);
}
FString UMainGI::ServerLoadData(const FString TableName, const FString UserId, const FString Password)
{
	return FString();
}

void UMainGI::ServerSaveData(const FString TableName, const FString JsonData, FHttpRequestCompleteDelegate ProcessRequestComplete)
{
}
void UMainGI::HTTPPost(const FString URL, const FString UserId, const FString Password, FHttpRequestCompleteDelegate ProcessRequestComplete)
{
	if (UserId == "\0" || Password == "\0")
	{
		return;
	}

	Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete() = ProcessRequestComplete;

	FString PostParameters = FString::Printf(TEXT("userid=%s"), *UserId) + 
		FString::Printf(TEXT("&userpwd=%s"), *Password);
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}
void UMainGI::HTTPResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		/*TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
*/

	}
}

void UMainGI::SaveAccount(const FText id, const FText pwd)
{
	Userid = id.ToString();
	Userpwd = pwd.ToString();
}
void UMainGI::SetLevelName(const FString LevelName)
{
	CurrentLevelName = LevelName;
}
void UMainGI::NextLevel()
{
	//FString GetLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	//GetLevelName = GetLevelName.Reverse();
	//char LevelCountName = GetLevelName[0];
	//int LevelCount = atoi(&LevelCountName);

	//if (LevelCount >= MaxLevelCount)
	//{
	//	return;
	//}
	//LevelCount += 1;
	//FString Result = FString::FromInt(LevelCount);
	//GetLevelName[0] = Result[0];
	//GetLevelName = GetLevelName.Reverse();

	/*UGameplayStatics::OpenLevel(GetWorld(), TEXT("Scene02"));*/
}

FString UMainGI::GetCurrentLevelName() const
{
	return CurrentLevelName;
}
