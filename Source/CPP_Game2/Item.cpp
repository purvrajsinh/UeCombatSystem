// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Pawn/MyCharacter.h"
#include "NiagaraComponent.h"
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = ItemMesh;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetupAttachment(GetRootComponent());
	
	RewardSpawnNiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RewardSpawnNiagaraSystem"));
	RewardSpawnNiagaraEffect->SetupAttachment(GetRootComponent());
	
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}	


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->SetOverLappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->SetOverLappingItem(nullptr);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ItemState == EItemState::EIS_UnEquipped)
	{
	RunningTime += DeltaTime;
	float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant); 
	
	AddActorWorldOffset((FVector(0.f,0.f, DeltaZ)));
	AddActorWorldRotation(FRotator(0.f,RotationSpeed * DeltaTime,0.f));
	}
	
}

