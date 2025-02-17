// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_BeamAction.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Characters/CAIController.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/CSkillAction_Wizard_Beam.h"

UCBTTaskNode_BeamAction::UCBTTaskNode_BeamAction()
{
	NodeName = "BeamSkill";
	bNotifyTick = true;	//tick실행여부
}

EBTNodeResult::Type UCBTTaskNode_BeamAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNullResult(state, EBTNodeResult::Failed);

	CheckTrueResult(weapon->IsUnarmedMode(), EBTNodeResult::Failed);
	CheckTrueResult(state->IsDeadMode(), EBTNodeResult::Failed);
	CheckFalseResult(state->IsIdleMode(), EBTNodeResult::Failed);

	controller->StopMovement();

	CheckNullResult(weapon->GetSkillAction(2), EBTNodeResult::Failed);
	weapon->GetSkillAction(2)->DoSkillAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_BeamAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	CheckNull(controller);
	CheckNull(ai);
	CheckNull(weapon);
	CheckNull(state);
	CheckNull(weapon->GetSkillAction(2));

	bool bCheck = true;
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetSkillAction(2)->GetInAction() == false);

	if (bCheck)
	{

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

