// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CSkillAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CManaPointComponent.h"
#include "Components/CAttackPointComponent.h"
#include "Components/CWeaponComponent.h"

UCSkillAction::UCSkillAction()
{
	
}
void UCSkillAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Owner = InOwner;
	Attachment = InAttachment;
	

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
	ManaPoint = CHelpers::GetComponent<UCManaPointComponent>(Owner);
	AttackPoint= CHelpers::GetComponent<UCAttackPointComponent>(Owner);
	Weapon=CHelpers::GetComponent<UCWeaponComponent>(Owner);

	InitSkillCoolDownTime = SkillActionData.CoolTime;
}



void UCSkillAction::Pressed()
{
	//CheckTrue(bInSkill);
	

	DoSkillAction();
	
	
	/*if (OnPressed.IsBound())
		OnPressed.Broadcast();*/
}

void UCSkillAction::Released()
{
	

}

void UCSkillAction::Cancel_Skill()
{
	
	bInSkill = false;

}

void UCSkillAction::DoSkillAction()
{
	CheckFalse(ManaPoint->CanUseSkill(SkillActionData.Mana));
	CheckFalse(State->IsIdleMode());
	
	SkillActionData.DoSkillAction(Owner);

	CheckTrue(bIsChargeSkill);
	bInSkill = true;


	ManaPoint->SetCurrentManaPoint(SkillActionData.Mana);
	if (SkillActionData.bFixedCamera)
		Movement->EnableFixedCamera();
	
	State->SetActionMode();
	
	if(SkillActionData.bCanMove==false)
		Movement->Stop();

	CheckNull(AttackPoint);

	if (AttackPoint->IsAwake())
		SkillActionData.CoolTime = InitSkillCoolDownTime - 2;

	else
		SkillActionData.CoolTime = InitSkillCoolDownTime;
	
	//쿨타임 초 후 되돌려주기
	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		bInSkill = false;
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, SkillActionData.CoolTime, false);


}

void UCSkillAction::Begin_DoSkillAction()
{
	bBeginAction = true;
}

void UCSkillAction::End_DoSkillAction()
{
	bBeginAction = false;

	if (SkillActionData.bFixedCamera)
		Movement->DisableFixedCamera();

	CheckNull(State);
	State->OffSkillActionMode();
	State->SetIdleMode();

	CheckNull(Movement);
	Movement->Move();
}