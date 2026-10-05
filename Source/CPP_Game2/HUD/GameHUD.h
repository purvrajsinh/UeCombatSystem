// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dataflow/DataflowOverlay.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GAME2_API AGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameOverlay> OverlayClass; 
	
	UPROPERTY()
	UGameOverlay* GameOverlay;
	
public:
	FORCEINLINE UGameOverlay* GetGameOverlay() const { return GameOverlay;}
	
};
