// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSpawnLoc.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACSpawnLoc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSpawnLoc();

protected:
	/*UPROPERTY(VisibleAnywhere)
		class UDecalComponent* Decal;*/

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UParticleSystemComponent* Particle;


	UPROPERTY(EditAnywhere, Category = "Options")
		float AfterSecond=5;
public:
	void ShowDecals();

};
