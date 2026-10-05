// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverlay.h"
#include "Components/ProgressBar.h"

void UGameOverlay::SetHealthBarPercentage(float Percentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percentage);
	}
}
