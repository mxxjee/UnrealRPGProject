// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "Components/CParkourComponent.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		class ACHorse* Horse;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bIsRide;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bIsDead;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bFalling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bAirDash;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bGroundDash;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bFixedCamera;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Skill")
		bool bUseSwordAirSkill;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SubAction")
		bool bInSubAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SubAction")
		bool bUseHammerXSkill;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
		bool bIsIdleMode;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Hit")
		bool bHit;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Hit")
		bool bPlayDamageAnim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Jump")
		bool bIsJump;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Hit")
		float HitValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combo")
		bool bAirCombo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Hit")
		bool IsKnockdown;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Hit")
		bool bAirHit;	//Enemy

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bCinematic;	//Enemy_Boss

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bCinematicJump;	//Enemy_Boss

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bBeamBegin;	//Enemy_Boss
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bBow_Aiming;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float ControlRigAlpha;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		EWeaponType WeaponType=EWeaponType::Max;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;
	class UCMovementComponent* Movement;
	class UCStateComponent* State;
	class UCParkourComponent* Parkour;

	FRotator PrevRotation;
	EParkourType ParkourType;
};
