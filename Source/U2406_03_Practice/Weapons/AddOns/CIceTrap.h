// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "CIceTrap.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitIceTrap, class AActor*, InCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class U2406_03_PRACTICE_API ACIceTrap : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UCapsuleComponent* Capsule;


public:	
	// Sets default values for this actor's properties
	ACIceTrap();

public:
	FHitIceTrap OnHitIceTrap;

public:
	void SetOwnerCharacter(class ACharacter* InOwner) { OwnerCharacter = InOwner; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
		FHitData hitdata;
private:
	class ACharacter* OwnerCharacter;
};
