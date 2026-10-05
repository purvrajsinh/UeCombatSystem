// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Game2/Interfaces/HitInterface.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetGenerateOverlapEvents(true);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore); 
	
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}

void AWeapon::ExecuteGetHit(FHitResult OutHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(OutHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(OutHit.GetActor(), OutHit.ImpactPoint, GetOwner());
	}
}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"))) return;
	
	FHitResult OutHit;
	BoxTrace(OutHit);
	
	if (OutHit.GetActor())
	{
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"))) return;

		UGameplayStatics::ApplyDamage(OutHit.GetActor(), Damage,GetInstigator()->GetController(),this,UDamageType::StaticClass());
		ExecuteGetHit(OutHit);
		CreateFields(OutHit.ImpactPoint);
	}
 }

void AWeapon::Equip(USceneComponent* InParent, FName InSoket, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSoket(InParent, InSoket);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DeactiveRewardSpawnNiagaraEffect();
}

void AWeapon::DeactiveRewardSpawnNiagaraEffect()
{
	if (RewardSpawnNiagaraEffect)
	{
		RewardSpawnNiagaraEffect->Deactivate();
	}
}

void AWeapon::BoxTrace(FHitResult& HitResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowDebugBox ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
		);
	IgnoreActors.AddUnique(HitResult.GetActor());
}

void AWeapon::AttachMeshToSoket(USceneComponent* InParent, FName InShoketName)
{
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentTransformRules, InShoketName);
}



