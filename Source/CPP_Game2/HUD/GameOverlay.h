// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverlay.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class CPP_GAME2_API UGameOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthBarPercentage(float Percentage);
	
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
};
