// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CSkillAction_Wizard_Beam.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/CBeam.h"

void UCSkillAction_Wizard_Beam::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);

}

void UCSkillAction_Wizard_Beam::DoSkillAction()
{
	CheckFalse(State->IsIdleMode());

	State->SetActionMode();
	State->OnSkillActionMode();

	bInAction = true;
	SkillActionData.DoSkillAction(Owner);
}

void UCSkillAction_Wizard_Beam::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();


}

void UCSkillAction_Wizard_Beam::End_DoSkillAction()
{
	Super::End_DoSkillAction();
	bInAction = false;
}


void UCSkillAction_Wizard_Beam::SpawnBeam()
{
	CheckNull(BeamClass);
	CheckNull(Owner);

	FActorSpawnParameters params;
	params.Owner = Owner;

	USkeletalMeshComponent* mesh = Owner->GetMesh();
	FTransform transform = mesh->GetSocketTransform("Beam");

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ACBeam* Beam = Owner->GetWorld()->SpawnActor<ACBeam>(BeamClass, transform, params);

	CheckNull(Beam);
	Beam->AttachToComponent(mesh, rule, "Beam");


}
