// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Weapons/CDoAction.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";
	bNotifyTick = true;	//tick실행여부
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ai = Cast<ACEnemy_AI>(controller->GetPawn());

	weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	state = CHelpers::GetComponent<UCStateComponent>(ai);

	CheckNullResult(state, EBTNodeResult::Failed);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	

	CheckTrueResult(weapon->IsUnarmedMode(), EBTNodeResult::Failed);
	CheckTrueResult(state->IsDeadMode(), EBTNodeResult::Failed);
	CheckFalseResult(state->IsIdleMode(), EBTNodeResult::Failed);

	//공격전 멈추기
	state->OffSubActionMode();
	controller->StopMovement();
	weapon->DoAction();
	
	

	controller->ClearFocus(EAIFocusPriority::Gameplay);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	CheckNull(controller);
	CheckNull(ai);
	CheckNull(weapon);
	CheckNull(state);

	if (state->IsDeadMode())
	{
		UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
		if (behavior)
			behavior->SetDeadMode();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (state->IsDamagedMode())
	{
		UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
		if (behavior)
			behavior->SetHittedMode();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	if (ai->GetbAirSkillDamaged())
	{
		UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
		if (behavior)
		{
			behavior->SetHittedMode();
			behavior->SetHitTypeAirMode();
		}
			

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	

	bool bCheck = true;
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetDoAction()->GetInAction() == false);

	if (bCheck)
	{

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	
}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	
	CheckNullResult(ai, EBTNodeResult::Failed);
	CheckNullResult(behavior, EBTNodeResult::Failed);

	controller->StopMovement();
	if (ai->GetbAirSkillDamaged())
	{
		
		if (behavior)
		{
			behavior->SetHittedMode();
			behavior->SetHitTypeAirMode();
		}
		
		return EBTNodeResult::Succeeded;
	}
		
	bool bBeginAction = weapon->GetDoAction()->GetBeginAction();

	if (bBeginAction == false)
	{
		CLog::Print("bBeginAction==false",15000);
		weapon->GetDoAction()->Begin_DoAction();
		weapon->GetDoAction()->End_DoAction();

		return EBTNodeResult::Succeeded;
	}

	if (bBeginAction)
	{
		CLog::Print("bBeginAction==true",15000);
		//weapon->GetDoAction()->End_DoAction();
		behavior->SetWaitMode();
		return EBTNodeResult::Succeeded;
		
	}
		

	
	return EBTNodeResult::Succeeded;
	

}