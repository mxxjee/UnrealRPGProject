// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTService_MiddleBoss.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CAttachment.h"
#include "Components/CAIBehaviorComponent.h"
#include "GameFramework/Character.h"

UCBTService_MiddleBoss::UCBTService_MiddleBoss()
{
	NodeName = "MiddleBoss";

	Interval = 0.1f;
	RandomDeviation = 0.0f;

}

void UCBTService_MiddleBoss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());


	//ai�� behavior������Ʈ ��������
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	if (weapon->GetAttachment() == nullptr)
	{
		weapon->SetSwordMode();
		behavior->SetWaitMode();

		return;
	}

	if (state->IsDeadMode())
	{
		controller->StopMovement();
		behavior->SetDeadMode();
		return;
	}

	if (state->IsDamagedMode() && ai->GetbAirSkillDamaged())
	{
		behavior->SetHittedMode();
		behavior->SetHitTypeAirMode();
		return;
	}

	if (state->IsDamagedMode() && ai->GetbAirSkillDamaged() == false)
	{
		behavior->SetHittedMode();
		behavior->SetHitTypeNormalMode();
		return;
	}




	if (ai->getbParry())
	{
		behavior->SetBlockMode();
		return;
	}



	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
	{
		
		behavior->SetPatrolMode();//������� ����
		return;
	}

	//������ �Ȼ�Ȳ�̶��,
	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)//�����ȿ� �ִٸ�,���ݸ��
	{
		behavior->SetActionMode();
		return;
	}

	//�����ȿ����ٸ�, �Ѿư���
	behavior->SetApproachMode();

	

	






}