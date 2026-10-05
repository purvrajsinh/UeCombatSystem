// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "GameOverlay.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && OverlayClass)
		{
			GameOverlay = CreateWidget<UGameOverlay>(PlayerController, OverlayClass);
			GameOverlay->AddToViewport();
		}
	}
}
