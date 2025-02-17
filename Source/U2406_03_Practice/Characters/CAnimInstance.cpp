// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Components/CMovementComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Weapons/CSubAction.h"
#include "Characters/CHorse.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_C.h"
#include "Weapons/SkillActions/Hammer/CSkillAction_Hammer_X.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

	if (!!Weapon)
		Weapon->OnWeaponChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
	
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);
	CheckNull(State);


	
	bIsIdleMode = State->IsIdleMode();
	Speed = OwnerCharacter->GetVelocity().Size2D();
	//Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();		//
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);

	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 20);
	Direction = PrevRotation.Yaw;
	
	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);


	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	
	ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
	bInSubAction = State->IsSubActionMode();

	if (enemy)
	{
		//enemy의  AirSkillDamaged여부와 IsDead여부를 가져온다.
		bAirHit = enemy->GetbAirSkillDamaged();
		bIsDead = CHelpers::GetComponent<UCHealthPointComponent>(enemy)->IsDead();
	}

	CheckNull(Weapon);

	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming = true;
		bBow_Aiming &= WeaponType == EWeaponType::Bow;
		bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}

	CheckNull(Movement);

	bAirDash = Movement->GetbAirDash();
	bGroundDash = Movement->GetbGroundDash();

	bFixedCamera = Movement->GetFixedCamera();

	ACEnemy_AI_Boss* Boss = Cast<ACEnemy_AI_Boss>(OwnerCharacter);
	if (!!Boss)
	{
		bCinematic = Boss->GetCinematic();
		bCinematicJump = Boss->GetCinematicJump();
		bBeamBegin = Boss->GetbBeamBegin();
		return;
	}

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	bIsRide = State->IsRideMode();

	bHit = player->GetHit();
	bPlayDamageAnim = player->GetPlayDamageAnim();
	HitValue = player->GetHitValue();
	bIsJump = player->GetIsJump();
	IsKnockdown = player->GetIsKnockdown();

	if (bIsRide)
		Horse = player->HorseToRide;

	else
		Horse = nullptr;

	if (Weapon->IsSwordMode())
	{
		CheckNull(Weapon->GetSkillAction(2));
		UCSkillAction_Sword_C* skill_C = Cast<UCSkillAction_Sword_C>(Weapon->GetSkillAction(2));
		CheckNull(skill_C);

		bUseSwordAirSkill = skill_C->GetbUseSwordSkill();
	}

	else if (Weapon->IsHammerMode())
	{
		CheckNull(Weapon->GetSkillAction(1));
		UCSkillAction_Hammer_X* skill_X = Cast<UCSkillAction_Hammer_X>(Weapon->GetSkillAction(1));
		CheckNull(skill_X);

		bUseHammerXSkill = skill_X->GetbUseHammerXSkill();
	}
	CheckNull(State);
	CheckNull(Parkour);

	if(WeaponType==EWeaponType::Max)
		ParkourType = Parkour->GetParkourType();
	
	if (WeaponType == EWeaponType::Max&& ParkourType ==EParkourType::Max && !bFalling && player->bRide)
		ControlRigAlpha = 1.0f;

	else
		ControlRigAlpha = 0.0f;



}


void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{

	WeaponType = InNewType;
}
