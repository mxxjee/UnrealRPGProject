// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Guard.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CWeaponComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Weapons/SubActions/CSubAction_Sword_Enemy.h"

UCBTTaskNode_Guard::UCBTTaskNode_Guard()
{
	NodeName = "Guard";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Guard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	
	UCSubAction_Sword_Enemy* subAction = Cast< UCSubAction_Sword_Enemy>(weapon->GetSubAction());
	CheckNullResult(subAction, EBTNodeResult::Failed);
	
	controller->StopMovement();

	controller->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Guard::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCSubAction_Sword_Enemy* subAction = Cast< UCSubAction_Sword_Enemy>(weapon->GetSubAction());

	CheckNull(subAction);
	CheckNull(state);

	subAction->Pressed();

	if(state->IsSubActionMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
}

EBTNodeResult::Type UCBTTaskNode_Guard::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCSubAction_Sword_Enemy* subAction = Cast< UCSubAction_Sword_Enemy>(weapon->GetSubAction());

	CheckNullResult(ai, EBTNodeResult::Failed);
	CheckNullResult(behavior, EBTNodeResult::Failed);
	CheckNullResult(weapon, EBTNodeResult::Failed);
	CheckNullResult(subAction, EBTNodeResult::Failed);
	
	subAction->Released();
	return EBTNodeResult::Failed;


}