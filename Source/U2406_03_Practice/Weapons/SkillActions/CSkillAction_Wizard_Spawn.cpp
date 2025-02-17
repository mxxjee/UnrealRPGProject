// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CSkillAction_Wizard_Spawn.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/CSpawnLoc.h"
#include "Weapons/AddOns/CObstacle.h"


void UCSkillAction_Wizard_Spawn::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);

	SetSpawnLocs();

}


void UCSkillAction_Wizard_Spawn::DoSkillAction()
{
	CheckFalse(State->IsIdleMode());

	State->SetActionMode();
	State->OnSkillActionMode();

	bInAction = true;
	SkillActionData.DoSkillAction(Owner);
}


void UCSkillAction_Wizard_Spawn::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	
}

void UCSkillAction_Wizard_Spawn::End_DoSkillAction()
{
	Super::End_DoSkillAction();
	bInAction = false;
}

void UCSkillAction_Wizard_Spawn::ShowDecals()
{
	CheckTrue(spawns.Num() < 1);

	for (ACSpawnLoc* item : spawns)
	{
		item->ShowDecals();
	}

}

void UCSkillAction_Wizard_Spawn::SpawnObstacles()
{
	CheckTrue(SpawnLocs.Num() < 1);
	CheckTrue(ObStaclesClass.Num() < 1);
	CheckNull(Owner);
	
	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(Owner);
	CheckNull(boss);

	
	for (FVector Loc : SpawnLocs)
	{
		FActorSpawnParameters params;
		params.Owner = Owner;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform transform;
		transform.SetLocation(Loc);
		transform.SetRotation(FQuat(FRotator::ZeroRotator));


		ACObstacle* obstacle = Owner->GetWorld()->SpawnActor<ACObstacle>(ObStaclesClass[0], transform, params);

	}

	boss->Camerashake();

}

void UCSkillAction_Wizard_Spawn::SetSpawnLocs()
{
	CheckNull(Owner);
	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(Owner);
	CheckNull(boss);

	boss->SetSpawnLocs();
	spawns = boss->GetSpawnLocs();

	CheckTrue(spawns.Num() < 1);

	for (AActor* item : spawns)
	{
		SpawnLocs.Add(item->GetActorLocation());
	}

	for (FVector Loc : SpawnLocs)
		CLog::Log(Loc);
}

