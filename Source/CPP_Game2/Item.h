// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

enum class EItemState : uint8
{
	EIS_Equipped,
	EIS_UnEquipped
};

UCLASS()

class CPP_GAME2_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	EItemState ItemState = EItemState::EIS_UnEquipped;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* RewardSpawnNiagaraEffect;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;
	
	float RunningTime = 0.f;
	UPROPERTY(EditAnywhere, Category = "0_Settings");
	float Amplitude = .2f;
	UPROPERTY(EditAnywhere, Category = "0_Settings");
	float TimeConstant = 2.f;
	UPROPERTY(EditAnywhere, Category = "0_Settings");
	float RotationSpeed = 35.f;
};
