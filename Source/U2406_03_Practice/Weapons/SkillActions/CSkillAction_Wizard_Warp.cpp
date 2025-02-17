// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CSkillAction_Wizard_Warp.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

void UCSkillAction_Wizard_Warp::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);

}

void UCSkillAction_Wizard_Warp::DoSkillAction()
{
	CheckFalse(State->IsIdleMode());

	State->SetActionMode();
	State->OnSkillActionMode();

	bInAction = true;
	SkillActionData.DoSkillAction(Owner);
}

void UCSkillAction_Wizard_Warp::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	CheckNull(Behavior);
	FVector origin;
	FVector boxExtent;
	Owner->GetActorBounds(false, origin, boxExtent);



	Owner->SetActorLocation(Behavior->GetEqsLocation() + boxExtent.Z);
}

void UCSkillAction_Wizard_Warp::End_DoSkillAction()
{
	Super::End_DoSkillAction();
	bInAction = false;
}
