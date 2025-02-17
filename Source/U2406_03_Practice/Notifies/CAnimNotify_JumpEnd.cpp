// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_JumpEnd.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_C.h"

FString UCAnimNotify_JumpEnd::GetNotifyName_Implementation() const
{
	return "JumpEnd";
}

void UCAnimNotify_JumpEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	
	Super::Notify(MeshComp, Animation, EventReference);


	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* Owner = Cast<ACPlayer>(MeshComp->GetOwner());
	UCMovementComponent* Movement = CHelpers::GetComponent<UCMovementComponent>(MeshComp->GetOwner());
	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());

	CheckNull(Owner);
	CheckNull(Movement);
	CheckNull(Weapon);
	CheckNull(State);


	switch (Type)
	{
		case EJumpType::NORMAL:
		{
			Movement->SetbAirDash(false);
			Movement->SetbCanMove(true);
			Owner->GetCharacterMovement()->GravityScale = 1.0f;

			break;
		}
		
		case EJumpType::AIRCOMBO:
		{
			Owner->GetCharacterMovement()->GravityScale = 1.0f;
			CheckNull(Weapon->GetSkillAction(2));
			UCSkillAction_Sword_C* skill = Cast<UCSkillAction_Sword_C>(Weapon->GetSkillAction(2));
			CheckNull(skill);

			skill->SetbUseSwordSkill(false);
			Movement->SetbCanMove(true);
			State->SetIdleMode();
			break;
		}
			

	}
	

	

	
}

