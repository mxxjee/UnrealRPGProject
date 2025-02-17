// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_CreateAndShoot.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SkillActions/Bow/CSkillAction_Bow_X.h"
#include "Weapons/SkillActions/Bow/CSkillAction_Bow_C.h"

FString UCAnimNotify_CreateAndShoot::GetNotifyName_Implementation() const
{
	return "CreateAndShoot";
}

void UCAnimNotify_CreateAndShoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	switch (SkillType)
	{

		case ESkillNum::X:
		{
			CheckNull(weapon->GetSkillAction(1));
			UCSkillAction_Bow_X* Bow_X = Cast<UCSkillAction_Bow_X>(weapon->GetSkillAction(1));
			CheckNull(Bow_X);

			Bow_X->CreateArrowAndShot();
			break;

		}


		case ESkillNum::C:
		{
			CheckNull(weapon->GetSkillAction(2));
			UCSkillAction_Bow_C* Bow_C = Cast<UCSkillAction_Bow_C>(weapon->GetSkillAction(2));
			CheckNull(Bow_C);

			Bow_C->CreateArrowAndShot();
			break;
		}


	}
}
