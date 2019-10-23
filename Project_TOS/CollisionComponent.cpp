// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCollisionComponent::UCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
// Called when the game starts
void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
}
// Called every frame
void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
bool UCollisionComponent::LineTraceObjects(FVector Start, FVector End, TArray<AActor*> &IgnoreObjects, TArray<FHitResult> &HitObjects)
{
	bool Result = UKismetSystemLibrary::LineTraceMultiForObjects(this, Start, End, ObjectTypes, 
		bTraceComlex, IgnoreObjects, (EDrawDebugTrace::Type)DrawDebugTrace, HitObjects,
		bIgnoreSelf, FLinearColor::Red, FLinearColor::Green, DrawTime);


	return Result;
}

