// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h"
#include "CUserWidget_Player.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCUserWidget_Player : public UUserWidget
{
	GENERATED_BODY()

		
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdateWeapon(EWeaponType InType);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateCrossHairVisibility(bool bInVisible);

	UFUNCTION(BlueprintImplementableEvent)
		void OnPressDash();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPressDashCoolDown(float DeltaTime,float DashCoolTime);

	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthPointChanged(float CurrentHp, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void OnAttackPointChanged(float CurrentAttackPoint, float MaxAttackPoint);

	UFUNCTION(BlueprintImplementableEvent)
		void OnManaPointChanged(float CurrentMana, float MaxMana);


	//Skill

	UFUNCTION(BlueprintImplementableEvent)
		void OnPressZSkill();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPressXSkill();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPressCSkill();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPressSpecialSkill();


	UFUNCTION(BlueprintImplementableEvent)
		void OnZSkillCoolDown(float DeltaTime, float SkillCoolTime, bool InbSkill);

	UFUNCTION(BlueprintImplementableEvent)
		void OnXSkillCoolDown(float DeltaTime, float SkillCoolTime, bool InbSkill);

	UFUNCTION(BlueprintImplementableEvent)
		void OnCSkillCoolDown(float DeltaTime, float SkillCoolTime, bool InbSkill);

	UFUNCTION(BlueprintImplementableEvent)
		void OnSpecialSkillCoolDown(float DeltaTime, float SkillCoolTime, bool InbSkill);
	
	//SpecialSkillUI
	UFUNCTION(BlueprintImplementableEvent)
		void OnShowSpecialSkill();

	UFUNCTION(BlueprintImplementableEvent)
		void OffShowSpecialSkill();
	
	//ChargeSkillUI

	UFUNCTION(BlueprintImplementableEvent)
		void OnShowChargeUI();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPressingCharge(float ChargeValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnHideChargeUI();

	//crosshair
	UFUNCTION(BlueprintImplementableEvent)
		void SetCrossHairColorRed();

	UFUNCTION(BlueprintImplementableEvent)
		void SetCrossHairColorWhite();

	//LockOnText
	UFUNCTION(BlueprintImplementableEvent)
		void ShowLockOnText();

	UFUNCTION(BlueprintImplementableEvent)
		void HideLockOnText();

	//Damage
	UFUNCTION(BlueprintImplementableEvent)
		void PlayDamageEffect();

	//Cinematic 
	UFUNCTION(BlueprintImplementableEvent)
		void HideUI();

	UFUNCTION(BlueprintImplementableEvent)
		void ShowUI();
};
