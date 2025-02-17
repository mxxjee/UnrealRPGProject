// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Sword.generated.h"


UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "Options")
		bool bCanMove;

	UPROPERTY(EditAnywhere, Category = "Options")
		FName StartSocketName;

	UPROPERTY(EditAnywhere, Category = "Options")
		FName EndSocketName;

	UPROPERTY(EditAnywhere, Category = "value")
		float ZoomTargetArmLength;

	float InitTargetArmLength;
	FVector InitSocketOffSet;

	UPROPERTY(EditAnywhere, Category = "value")
		float FovInterpTime;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundWave* Sound;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UNiagaraSystem* Particle;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "CameraClass")
		TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Animation")
		float PlayRate=1.f;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float extentX;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float extentY;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float extentZ;

public:
	UCSubAction_Sword();

public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* ISnAttachment, class UCDoAction* InDoAction) override;
	void Tick(float DeltaTime)override;

public:
	void Pressed() override;
	void Released() override;

	void PlayParrySound() override;
	void PlayEffect(UWorld* InWorld) override;
	void PlayCameraShake();
	void PlayBlockEffect();

	void Cancel_Guard() override;

private:
	
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	
	float MaxPressCount;
	float PressCount;
	FHitResult hitResult;
	TArray<AActor*> ignores;

	class UCameraComponent* Camera;
	class USpringArmComponent* SpringArm;
};
