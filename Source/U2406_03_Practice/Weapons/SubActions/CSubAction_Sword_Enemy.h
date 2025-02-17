// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Sword_Enemy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSubAction_Sword_Enemy : public UCSubAction
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere)
		UMaterialInstance* GuardMaterial;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FVector Scale;
public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	void Tick(float DeltaTime) override;

public:
	void Pressed() override;
	void Released() override;

	void Cancel_Guard() override;

	UFUNCTION()
		void Cancel();

	void PlayParrySound() override;
	void PlayEffect(UWorld* InWorld) override;
private:
	class ACEnemy_AI* OwnerCharacter;
};
