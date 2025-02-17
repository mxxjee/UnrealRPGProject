
#include "Components/CAIBehaviorComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"


UCAIBehaviorComponent::UCAIBehaviorComponent()
{
	
}


// Called when the game starts
void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

EAIStateType UCAIBehaviorComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}

ACharacter* UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

FVector UCAIBehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}



void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey,InLocation);
}



FVector UCAIBehaviorComponent::GetEqsLocation()
{
	return Blackboard->GetValueAsVector(EqsLocationKey);
}

void UCAIBehaviorComponent::SetEqsLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(EqsLocationKey, InLocation);
}


bool UCAIBehaviorComponent::IsWaitMode()
{
	return GetType() == EAIStateType::Wait;
}

bool UCAIBehaviorComponent::IsApproachMode()
{
	return GetType() == EAIStateType::Approach;
}

bool UCAIBehaviorComponent::IsActionMode()
{
	return GetType() == EAIStateType::Action;
}

bool UCAIBehaviorComponent::IsPatrolMode()
{
	return GetType() == EAIStateType::Patrol;
}

bool UCAIBehaviorComponent::IsHittedMode()
{
	return GetType() == EAIStateType::Hitted;
}

bool UCAIBehaviorComponent::IsAvoidMode()
{
	return GetType() == EAIStateType::Avoid;
}

bool UCAIBehaviorComponent::IsDeadMode()
{
	return GetType() == EAIStateType::Dead;
}

bool UCAIBehaviorComponent::IsBlockMode()
{
	return GetType() == EAIStateType::Block;
}



void UCAIBehaviorComponent::SetWaitMode()
{
	ChangeType(EAIStateType::Wait);
}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);
}

void UCAIBehaviorComponent::SetActionMode()
{
	ChangeType(EAIStateType::Action);
}

void UCAIBehaviorComponent::SetPatrolMode()
{
	ChangeType(EAIStateType::Patrol);
}

void UCAIBehaviorComponent::SetHittedMode()
{
	ChangeType(EAIStateType::Hitted);
}

void UCAIBehaviorComponent::SetAvoidMode()
{
	ChangeType(EAIStateType::Avoid);
}

void UCAIBehaviorComponent::SetDeadMode()
{
	ChangeType(EAIStateType::Dead);
}

void UCAIBehaviorComponent::SetBlockMode()
{
	ChangeType(EAIStateType::Block);
}

void UCAIBehaviorComponent::SetReadyMode()
{
	ChangeType(EAIStateType::Ready);
}

void UCAIBehaviorComponent::SetBeamMode()
{
	ChangeType(EAIStateType::Beam);
}



void UCAIBehaviorComponent::SetHitTypeNormalMode()
{
	Blackboard->SetValueAsEnum("HitType", (uint8)EAIHitType::Normal);
}

void UCAIBehaviorComponent::SetHitTypeAirMode()
{
	Blackboard->SetValueAsEnum("HitType", (uint8)EAIHitType::Air);
}

void UCAIBehaviorComponent::SetWeaponType(EWeaponType InType)
{
	Blackboard->SetValueAsEnum("WeaponType", (uint8)InType);
}

void UCAIBehaviorComponent::SetAttackType(EAIAttackType InType)
{
	Blackboard->SetValueAsEnum(AttackTypeKey, (uint8)InType);
}

void UCAIBehaviorComponent::SetOnePhase()
{
	Blackboard->SetValueAsEnum("CurrentPhase", (uint8)EPhase::OnePhase);
}

void UCAIBehaviorComponent::SetTwoPhase()
{
	Blackboard->SetValueAsEnum("CurrentPhase", (uint8)EPhase::TwoPhase);
}

void UCAIBehaviorComponent::SetThreePhase()
{
	Blackboard->SetValueAsEnum("CurrentPhase", (uint8)EPhase::ThreePhase);
}

EPhase UCAIBehaviorComponent::CurrentPhaseType()
{
	return (EPhase)Blackboard->GetValueAsEnum(CurrentPhaseKey);
}

void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);

	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}

EWeaponType UCAIBehaviorComponent::GetWeaponType()
{
	return (EWeaponType)Blackboard->GetValueAsEnum(EWeaponTypeKey);
}