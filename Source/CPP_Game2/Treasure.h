// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GAME2_API ATreasure : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold = 25;
};
