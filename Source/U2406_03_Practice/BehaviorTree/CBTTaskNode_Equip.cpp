// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Weapons/CEquipment.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";
	bNotifyTick = true;	//tick실행여부

}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());


	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	//장착하려는 무기와, 장착되어있는 무기가 같으면,성공리턴
	if (Type == weapon->GetType())
		return EBTNodeResult::Succeeded;

	//Max를 선택했다면, 장착x
	CheckTrueResult(Type == EWeaponType::Max, EBTNodeResult::Failed);

	

	switch (Type)
	{

		case EWeaponType::Sword:
			weapon->SetSwordMode();
			break;

		case EWeaponType::Warp:
			weapon->SetWarpMode();
			break;

		case EWeaponType::Bow:
			weapon->SetBowMode();
			break;

	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	//장착완료여부 가져오기.
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	const bool* bEquipped = weapon->GetEquipment()->GetbEquipped();


	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	if (*bEquipped && state->IsIdleMode())
	{
		//장착완료
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}





}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	
	bool bBeginEquip = weapon->GetEquipment()->GetBeginEquip();

	if (bBeginEquip == false)
		weapon->GetEquipment()->Begin_Equip();

	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;
}