// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "Pawn/MyCharacter.h"


void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		//TODO Add coin to character
		Destroy();
	}
}
