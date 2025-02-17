// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/State/CAnimNotify_EndParry.h"
#include "Global.h"
#include "Components/CParkourComponent.h"
#include "Components/CWeaponComponent.h"
#include "Characters/CPlayer.h"
#include "Weapons/CSkillAction.h"

FString UCAnimNotify_EndParry::GetNotifyName_Implementation() const
{
	return "EndParry";
}

void UCAnimNotify_EndParry::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	CheckTrue(weapon->IsUnarmedMode());
	
	UCSkillAction* skillaction = weapon->GetSkillAction(SkillNumber);
	CheckNull(skillaction);

	skillaction->End_DoSkillAction();
	


}
