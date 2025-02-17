// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Hammer_Z.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Hammer_Z : public UCSkillAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* FullChargeSkillMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* LowChargeSkillMontage;


	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* MidChargeSkillMontage;

private:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

	void Pressing() override;
	void Released() override;

	void Cancel_Skill() override;
public:
	void OnAttachmentEndCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;

private:
	bool bIsChargeEnd;
	float AddPower;
	bool bDoChargeAnim;

private:
	class ACPlayer* player;
};
