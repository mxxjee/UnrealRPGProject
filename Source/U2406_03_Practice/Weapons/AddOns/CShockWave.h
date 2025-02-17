// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "CShockWave.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACShockWave : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		FHitData hitdata;
private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float LifeSpan = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Option")
		float Launch = 500;
private:
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UProjectileMovementComponent* Projectile;

public:	
	ACShockWave();


public:
	FORCEINLINE void AddIngoreActor(AActor* InActor) { Ignores.Add(InActor); }

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void Shoot(const FVector& InForward);

private:
	UFUNCTION(BlueprintCallable)
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	TArray<AActor*> Ignores;
};
