// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLance.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACLance : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float LifeSpan = 4;

	UPROPERTY(EditAnywhere)
		FHitData hitdata;

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class USphereComponent* Sphere;

private:
	UFUNCTION(BlueprintCallable)
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FORCEINLINE void AddIngoreActor(AActor* InActor) { Ignores.Add(InActor); }


public:	
	ACLance();

protected:
	virtual void BeginPlay() override;


private:
	TArray<AActor*> Ignores;


};
