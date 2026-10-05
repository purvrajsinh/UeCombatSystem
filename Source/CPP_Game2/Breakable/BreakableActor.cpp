// Fill out your copyright notice in the Description page of Project Settings.

#include "BreakableActor.h"
#include "CPP_Game2/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);

	GeometryCollection->SetGenerateOverlapEvents(true);
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::GetHit_Implementation(const FVector& InpactPoint, AActor* Hitter)
{
	if (bBroken) return;
	bBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 115.f;
		
		const int32 RandomSelection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[RandomSelection], SpawnLocation, GetActorRotation());
		Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
