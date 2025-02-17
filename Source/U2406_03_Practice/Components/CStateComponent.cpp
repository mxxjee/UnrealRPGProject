// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CStateComponent.h"

// Sets default values for this component's properties
UCStateComponent::UCStateComponent()
{
	
}


// Called when the game starts
void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();


	
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}


void UCStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetDamagedMode()
{
	ChangeType(EStateType::Damaged);
}

void UCStateComponent::SetRideMode()
{
	ChangeType(EStateType::Ride);
}


void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}


void UCStateComponent::ChangeType(EStateType InType)
{
	//�� ��Ÿ�� ����
	EStateType prevType = Type;

	//���ιٲ�Ÿ������ ����.
	Type = InType;

	//��������Ʈ����� �Լ�call.
	if(OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType,InType);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}


void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}

void UCStateComponent::OnSkillActionMode()
{
	bInSkillActionMode = true;
}


void UCStateComponent::OffSkillActionMode()
{
	bInSkillActionMode = false;
}