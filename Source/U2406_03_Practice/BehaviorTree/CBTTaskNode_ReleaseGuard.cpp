// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_ReleaseGuard.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CWeaponComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Weapons/SubActions/CSubAction_Sword_Enemy.h"

UCBTTaskNode_ReleaseGuard::UCBTTaskNode_ReleaseGuard()
{
	NodeName = "ReleaseGuard";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_ReleaseGuard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


void UCBTTaskNode_ReleaseGuard::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCSubAction_Sword_Enemy* subAction = Cast< UCSubAction_Sword_Enemy>(weapon->GetSubAction());

	subAction->Released();

	if(state->IsSubActionMode()==false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}

EBTNodeResult::Type UCBTTaskNode_ReleaseGuard::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	return EBTNodeResult::Succeeded;


}
