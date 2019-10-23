// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeBullet.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARangeBullet::ARangeBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Explosion = CreateDefaultSubobject<UParticleSystem>(TEXT("Explosion"));

	RootComponent = SphereCollision;

	Particle->SetupAttachment(SphereCollision);

	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->MaxSpeed = 200.0f;
	ProjectileMovement->InitialSpeed = 20000.0f;
	ProjectileMovement->HomingAccelerationMagnitude = 5000.f;

	Particle->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Particle->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void ARangeBullet::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(5.0f);

	OnActorBeginOverlap.AddDynamic(this, &ARangeBullet::OnBeginOverlap);
}

// Called every frame
void ARangeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARangeBullet::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	Destroy();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		Explosion, GetActorLocation());

	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
	
	}
}

