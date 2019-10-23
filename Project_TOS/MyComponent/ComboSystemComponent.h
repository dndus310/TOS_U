// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "ComboSystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FComboStateInfo
{
	GENERATED_BODY()

	bool IsState;
	FString SectionName;
public:
	FComboStateInfo() {};
	FComboStateInfo(FString SName, bool State = false)
	{
		IsState = State;
		SectionName = SName;
	}
};
UCLASS(HideCategories = (Mobility, Rendering, LOD), Blueprintable, ClassGroup = Custom, meta = (BlueprintSpawnableComponent))
class PROJECT_TOS_API UComboSystemComponent : public USceneComponent
{
	GENERATED_BODY()
private:
	bool CheckManualKey(FString ManualKey);
public:	
	// Sets default values for this component's properties
	UComboSystemComponent();
	bool InitComboStateMap(TMap<FString, FString> KeyInfo);
	bool InitComboStateMap(TArray<FString> SectionNameArray, TArray<FString> ManualKeyArray);
	bool AnyKey();
	FString EnableKey(FString ManualKey);
	void ClearKey();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame

	// tick 사용체크 X
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	TMap<FString, FComboStateInfo> ManualKeyMap;
	FString CurrentManualKey;
};
