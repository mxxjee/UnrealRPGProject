// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CSkillAction_Wizard_Wall.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

void UCSkillAction_Wizard_Wall::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);

	

}


void UCSkillAction_Wizard_Wall::DoSkillAction()
{
	CheckFalse(State->IsIdleMode());

	State->SetActionMode();
	State->OnSkillActionMode();

	bInAction = true;
	SkillActionData.DoSkillAction(Owner);
}

void UCSkillAction_Wizard_Wall::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();


}

void UCSkillAction_Wizard_Wall::End_DoSkillAction()
{
	Super::End_DoSkillAction();
	bInAction = false;
}

void UCSkillAction_Wizard_Wall::SpawnIceWalls()
{
	CheckNull(IceWallClass)
	CheckNull(Owner);

	FActorSpawnParameters params;
	params.Owner = Owner;

	USkeletalMeshComponent* mesh = Owner->GetMesh();
	
	FTransform transform;
	transform.SetLocation(mesh->GetSocketLocation("IceWall"));
	transform.SetRotation(FQuat(FRotator::ZeroRotator));
	transform.SetScale3D(FVector(10, 10, 10));

	AActor* Icewall = Owner->GetWorld()->SpawnActor<AActor>(IceWallClass, transform, params);
	Icewall->SetActorScale3D(FVector(10, 10, 10));
}
