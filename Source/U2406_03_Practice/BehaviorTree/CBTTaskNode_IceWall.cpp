// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_IceWall.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Characters/CAIController.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/CSkillAction_Wizard_Wall.h"

UCBTTaskNode_IceWall::UCBTTaskNode_IceWall()
{
	NodeName = "SpawnIceWall";
	bNotifyTick = true;	//tick���࿩��
}

EBTNodeResult::Type UCBTTaskNode_IceWall::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	CheckNullResult(weapon->GetSkillAction(3), EBTNodeResult::Failed);
	weapon->GetSkillAction(3)->DoSkillAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_IceWall::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	CheckNull(controller);
	CheckNull(ai);
	CheckNull(weapon);
	CheckNull(state);
	CheckNull(weapon->GetSkillAction(1));

	bool bCheck = true;
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetSkillAction(3)->GetInAction() == false);

	if (bCheck)
	{

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
