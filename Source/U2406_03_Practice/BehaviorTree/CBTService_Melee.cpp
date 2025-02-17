

#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CAttachment.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.0f;

}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	
	//ai의 behavior컴포넌트 가져오기
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

	if (state->IsDamagedMode() && ai->GetbAirSkillDamaged()==false)
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
		state->OffSubActionMode();
		behavior->SetPatrolMode();//정찰모드 설정
		ai->GetMesh()->SetOverlayMaterial(nullptr);
		return;
	}

	//감지된 상황이라면,
	//범위안에 들어온상황이라면 공격모드전환
	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)
	{
		behavior->SetActionMode();
		return;
	}

	if (distance > ActionRange && distance < ReadyRange)
	{
		state->OffSubActionMode();
		behavior->SetReadyMode();
		
		ai->GetMesh()->SetOverlayMaterial(nullptr);
		return;
	}

	//ApproachRange
	if(distance<ApproachRange && distance>ReadyRange)
	{
		if (state->IsIdleMode())
		{
			state->OffSubActionMode();
			behavior->SetApproachMode();
			ai->GetMesh()->SetOverlayMaterial(nullptr);
		}
	}
	





}