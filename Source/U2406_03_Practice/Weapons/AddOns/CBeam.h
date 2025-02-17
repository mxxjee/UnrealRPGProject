// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBeam.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACBeam : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UNiagaraComponent* Niagara;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UCapsuleComponent* Capsule;


public:	
	ACBeam();

protected:
	virtual void BeginPlay() override;

public:
	void SetOwnerCharacter(class ACharacter* InOwner) { OwnerCharacter = InOwner; }

public:
	UFUNCTION(BlueprintCallable)
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
		FHitData hitdata;

	UPROPERTY(EditAnywhere)
		float Launch;
private:
	class ACharacter* OwnerCharacter;

};
