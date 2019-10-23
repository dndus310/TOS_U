// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboSystemComponent.h"

// Sets default values for this component's properties
UComboSystemComponent::UComboSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}
bool UComboSystemComponent::InitComboStateMap(TMap<FString, FString> DataInfoMap)
{
	//json 으로 받을때 사용함.
	if (DataInfoMap.Num() == 0)
	{
		return false;
	}
	TMap<FString, FString>::TRangedForIterator iter = DataInfoMap.begin();
	for (; iter != DataInfoMap.end(); ++iter)
	{
		ManualKeyMap.Emplace((*iter).Key, FComboStateInfo((*iter).Value));
	}
	return false;
}
bool UComboSystemComponent::InitComboStateMap(TArray<FString> SectionNameArray, TArray<FString> ManualKeyArray)
{
	if ((SectionNameArray.Max() == 0) && (ManualKeyArray.Max() == 0) &&
		SectionNameArray.Max() != ManualKeyArray.Max())
	{
		return false;
	}
	for (int i = 0; i < SectionNameArray.Max(); ++i)
	{
		ManualKeyMap.Emplace(ManualKeyArray[i], FComboStateInfo(SectionNameArray[i]));
	}
	return true;
}
bool UComboSystemComponent::AnyKey()
{
	TMap<FString, FComboStateInfo>::TRangedForIterator iter = ManualKeyMap.begin();
	for (; iter != ManualKeyMap.end(); ++iter)
	{
		if ((*iter).Value.IsState)
		{
			return true;
		}
	}
	return false;
}
bool UComboSystemComponent::CheckManualKey(FString ManualKey)
{
	return ManualKeyMap.Contains(ManualKey);
}
FString UComboSystemComponent::EnableKey(FString ManualKey)
{
	CurrentManualKey += ManualKey + "\0";

	//UE_LOG(LogTemp, Log, TEXT("%s"), *CurrentManualKey);

	if (CheckManualKey(CurrentManualKey))
	{
		ManualKeyMap[CurrentManualKey].IsState = true;
		return ManualKeyMap.Find(CurrentManualKey)->SectionName;
	}
	return FString();
}
void UComboSystemComponent::ClearKey()
{
	if (CurrentManualKey != "\0")
	{
		CurrentManualKey = "\0";
	}
	TMap<FString, FComboStateInfo>::TRangedForIterator iter = ManualKeyMap.begin();
	for (; iter != ManualKeyMap.end(); ++iter)
	{
		(*iter).Value.IsState = false;
	}
}
// Called when the game starts
void UComboSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

// Called every frame
void UComboSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

