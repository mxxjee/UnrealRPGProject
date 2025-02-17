// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTService_ActionType.h"
#include "Global.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

UCBTService_ActionType::UCBTService_ActionType()
{
	NodeName = "ActionType";

	Interval = 0.1f;
	RandomDeviation = 0.0f;


}

void UCBTService_ActionType::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNull(behavior);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(state);

	AttackType = FMath::RandRange(1, 4);

	if (state->IsSubActionMode()==false)
	{
		if (AttackType < 2)
		{

			behavior->SetAttackType(EAIAttackType::Guard);
			return;
		}


		else
		{

			if (state->IsSubActionMode() == false)
			{
				behavior->SetAttackType(EAIAttackType::Attack);
				return;
			}
		}
	}
	
}