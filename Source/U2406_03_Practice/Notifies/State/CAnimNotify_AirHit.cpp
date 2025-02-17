// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/State/CAnimNotify_AirHit.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
#include "Characters/CEnemy.h"

FString UCAnimNotify_AirHit::GetNotifyName_Implementation() const
{
	return "AirHit";
}

void UCAnimNotify_AirHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	switch (Type)
	{
		case EType::ENEMY:
		{
			ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
			CheckNull(enemy);

			enemy->SetbAirSkillDamaged(true);
			break;
		}
			

		case EType::PLAYER:
			break;
	}
}
