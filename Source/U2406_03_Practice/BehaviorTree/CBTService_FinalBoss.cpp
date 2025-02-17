// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTService_FinalBoss.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_FinalBoss::UCBTService_FinalBoss()
{
	NodeName = "FinalBoss";

	Interval = 0.1f;
	RandomDeviation = 0.0f;

}

void UCBTService_FinalBoss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);



	//ai의 behavior컴포넌트 가져오기
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(behavior);
	CheckNull(weapon);
	CheckNull(state);

	if (weapon->GetAttachment() == nullptr)
	{
		weapon->SetBowMode();
		behavior->SetWaitMode();

		return;
	}

	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();
		start.Z -= 25.0f;

		FVector end = start;
		end.Z += 50.0f;

		DrawDebugCylinder(ai->GetWorld(), start, end, AvoidRange, 10, FColor::Red, false, Interval);
	}



	if (state->IsDamagedMode())
	{
		behavior->SetHittedMode();
		return;
	}

	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		behavior->SetWaitMode();
		return;
	}


	//감지되었다면 타겟을바라보게.
	controller->SetFocus(target);



	//범위안에 들어온상황이라면 공격모드전환
	float distance = ai->GetDistanceTo(target);

	CLog::Print(distance,15100,2,FColor::Green);
	if (distance < AvoidRange)
	{
		behavior->SetAvoidMode();
		return;
	}


	//범위안에있다면 바로공격
	behavior->SetActionMode();





}
