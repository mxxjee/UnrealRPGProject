// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMagicBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitMagicBall, AActor*, InCauser, ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMagicBallEndPlay, class ACMagicBall*, InDestroyActor);


UCLASS()
class U2406_03_PRACTICE_API ACMagicBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACMagicBall();
private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float LifeSpanAfterCollision = 1;//충돌 후 사라질시간
private:
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* Projectile;


public:
	FORCEINLINE void AddIngoreActor(AActor* InActor) { Ignores.Add(InActor); }


public:
	void Shoot(const FVector& InForward);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	FHitMagicBall OnHitMagicBall;
	FMagicBallEndPlay OnMagicBallEndPlay;
private:
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:
	TArray<AActor*> Ignores;



};
