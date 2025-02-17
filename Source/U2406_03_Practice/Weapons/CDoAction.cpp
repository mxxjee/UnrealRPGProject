// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "CAttachment.h"

UCDoAction::UCDoAction()
{

}

void UCDoAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;

	InitDoActionDatas = DoActionDatas;
	InitHitDatas = HitDatas;
}

void UCDoAction::DoAction()
{
	bInAction = true;
	State->SetActionMode();
	Movement->Stop();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;
	bInAction = false;
	State->SetIdleMode();
	Movement->Move();
}

void UCDoAction::AbortAction()
{
	bBeginAction = false;
	bInAction = false;

}



void UCDoAction::SetDatas(const TArray<FDoActionData>& InDoActionDatas,const TArray<FHitData>& InHitDatas)
{
	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
	
}

void UCDoAction::InitDatas()
{
	DoActionDatas = InitDoActionDatas;
	HitDatas = InitHitDatas;
}