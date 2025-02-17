// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "Characters/IStatable.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	IIStatable* statable = Cast<IIStatable>(MeshComp->GetOwner());
	CheckNull(statable);

	if (bParryHitted)
	{
		statable->End_Parried();
		return;
	}
	if (bAirDamagedEnd)
	{
		statable->End_AirDamaged();
		return;
	}
		
	switch (StateType)
	{
		case EStateType::Evade:
			statable->End_Backstep();
			break;
			
		case EStateType::Dead:
			statable->End_Dead();
			break;

		case EStateType::Damaged:
			statable->End_Damaged();
			break;


	}

	

}
