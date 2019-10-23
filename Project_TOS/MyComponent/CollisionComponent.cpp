// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "MyInterface/UnitInterface.h"
#include "UnrealNetwork.h"
#include "GameFramework/Pawn.h"
// Sets default values for this component's properties
UCollisionComponent::UCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	bReplicates = true;
}
// Called when the game starts
void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPawn = Cast<APawn>(GetOwner());
	// ...
}
void UCollisionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCollisionComponent, IsCollisionCheck);
}
// Called every frame
void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SwordCollision();

	// ...
}
void UCollisionComponent::InitCollisionSocket(TArray<FName> socketNames)
{
	this->SocketNames = socketNames;
}

void UCollisionComponent::SwordCollision()
{
	bool Result = false;
	if (IsCollisionCheck) //특정 애니메이션 프레임일때
	{
		IsCollisionStart = true;

		TArray<AActor*> IgnoreObjects = {};
		TArray<FHitResult> OutHits = {};
		//UE_LOG(LogClass, Error, TEXT("Log Start"));

		TMap<FName, FVector>::TRangedForIterator iter = SocketLocations.begin();
		TArray<FVector> Sockets;
		FName SocketName = "";

		for (; iter != SocketLocations.end(); ++iter)
		{
			FVector Start = (*iter).Value;
			FVector End = IUnitInterface::Execute_GetUnitSwordLocation(GetOwner(), (*iter).Key);
			SocketName = (*iter).Key;

			Result = UKismetSystemLibrary::LineTraceMultiForObjects(this, Start, End, ObjectTypes,
				bTraceComlex, IgnoreObjects, (EDrawDebugTrace::Type)DrawDebugTrace, OutHits,
				bIgnoreSelf, FLinearColor::Red, FLinearColor::Green, DrawTime);

			if (Result)
			{
				for (int i = 0; i < OutHits.Num(); ++i) // 충돌된 오브젝트를 한번씩만 받아온다
				{
					if (HitObjects.Find(OutHits[i].GetActor()) == INDEX_NONE) // 충돌 한번만 하기위해
					{
						HitObjects.Emplace(OutHits[i].GetActor());
						SaveHits.Emplace(OutHits[i]);
						IsCollisionStart = true;

						if (!OutHits[i].GetActor()->ActorHasTag(TEXT("Player")))
						{
							OnHitEvent.ExecuteIfBound(OutHits[i]);
						}
						else
						{
							if (OutHits[i].GetActor()->HasAuthority())
							{
								ExcuteHitEvent();
							}
						}
					}
				}
			}
		}
	}
	else if (!IsCollisionCheck && IsCollisionStart)
	{
		//CtoS_SwordCollision();
		IsCollisionStart = false;
	}
	else
	{
		//SocketLocationsArray.Empty();
		HitObjects.Empty();
		SaveHits.Empty();
	}
	for (int i = 0; i < SocketNames.Num(); ++i) // 소켓 포지션을 갱신시킨다.
	{
		SocketLocations.Emplace(SocketNames[i], IUnitInterface::Execute_GetUnitSwordLocation(GetOwner(), SocketNames[i]));
	}
}
void UCollisionComponent::CtoS_SwordCollision_Implementation()
{

}
void UCollisionComponent::ExcuteHitEvent()
{
	/*for (int i = 0; i < SaveHits.Num(); ++i)
	{
		OnHitEvent.ExecuteIfBound(SaveHits[i]);
	}*/
	CtoS_ExcuteHitEvent();
}
void UCollisionComponent::CtoS_ExcuteHitEvent_Implementation()
{
	for (int i = 0; i < SaveHits.Num(); ++i)
	{
		OnHitEvent.ExecuteIfBound(SaveHits[i]);
	}
}
void UCollisionComponent::SetCollisionCheck(bool Result)
{
	CtoS_SetCollisionCheck(Result);
}
void UCollisionComponent::CtoS_SetCollisionCheck_Implementation(bool Result)
{
	StoA_SetCollisionCheck(Result);
}

void UCollisionComponent::StoA_SetCollisionCheck_Implementation(bool Result)
{
	IsCollisionCheck = Result;
}
