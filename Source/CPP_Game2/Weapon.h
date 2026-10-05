// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


class UBoxComponent;
/**
 * 
 */
UCLASS()
class CPP_GAME2_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InShoketName,AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSoket(USceneComponent* InParent,const FName InShoketName);
	TArray<AActor*> IgnoreActors;
	
protected:
	void BeginPlay() override;
	void ExecuteGetHit(FHitResult OutHit);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DeactiveRewardSpawnNiagaraEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	
private: 
	void BoxTrace(FHitResult& HitResult);
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowDebugBox;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	USceneComponent* BoxTraceEnd;
	
	UPROPERTY(VisibleAnywhere)
	float Damage = 20.f;
	
	
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const {return WeaponBox;}
	
};
