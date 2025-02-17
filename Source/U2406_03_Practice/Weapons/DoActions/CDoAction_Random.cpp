// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Random.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CAIController.h"

void UCDoAction_Random::DoAction()
{
	

	CheckTrue(DoActionDatas.Num() < 1);

	CheckFalse(State->IsIdleMode());

	Super::DoAction();

	if(!bUseKnockDownAttack)
		OwnerCharacter->GetMesh()->SetOverlayMaterial(nullptr);

	bInAction = true;


	/*초기화 되지않았을때 (attack중간에 damage입을경우)*/
	if (bUseKnockDownAttack)
	{
		if (AttackCount == MaxAttackCount)
		{
			AttackCount = 0;
			bIsKnockdownAction = false;
		}
	}
	

	Index = FMath::RandRange(0, DoActionDatas.Num() - 2);

	State->SetActionMode();

	if (bUseKnockDownAttack)
	{
		++AttackCount;
		AttackCount = FMath::Clamp(AttackCount, 0, MaxAttackCount);
	}
	
	
	if (bUseKnockDownAttack)
	{
		if (AttackCount == MaxAttackCount)
		{
			bIsKnockdownAction = true;
			Index = 6;
			SetPlayerGuardCancle();

		}
	}
	
	

	DoActionDatas[Index].DoAction(OwnerCharacter);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(OwnerCharacter);
	CheckNull(enemy);

	enemy->StopMovement();
	controller = CHelpers::GetComponent<ACAIController>(enemy);

	OwnerCharacter->GetMesh()->SetOverlayMaterial(nullptr);

}

void UCDoAction_Random::Begin_DoAction()
{
	Super::Begin_DoAction();
	
	CheckNull(controller);

	controller->StopMovement();
}

void UCDoAction_Random::End_DoAction()
{
	Super::End_DoAction();

	CLog::Log("End_DoAction_Random");
	Index = -1;
	bInAction = false;
	State->SetIdleMode();

	CheckFalse(bUseKnockDownAttack);
	if (bIsKnockdownAction)
	{
		AttackCount = 0;
		bIsKnockdownAction = false;
	}
}

void UCDoAction_Random::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);
	CheckNull(InOther);

	CLog::Log(InOther->GetName());

	TArray<AActor*> ignores;
	ignores.Add(InAttacker);

	FHitResult hitresult;

	
	ACEnemy* Enemy = Cast<ACEnemy>(InAttacker);

	if (!!Enemy)
	{
		FVector Start = InAttackCauser->GetActorLocation();
		FVector End = Enemy->GetActorLocation();


		UKismetSystemLibrary::LineTraceSingle(Enemy->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitresult, true);
		Enemy->SetCollisionPoint(hitresult.ImpactPoint);
		Enemy->SetCollisionPointNormal(hitresult.ImpactNormal.Rotation());
	}

	if (Index == -1)
		Index = FMath::RandRange(0, DoActionDatas.Num() - 1);

	ACPlayer* player = Cast<ACPlayer>(InOther);
	CheckNull(player);

	if (bUseKnockDownAttack)
	{
		if (bIsKnockdownAction)
		{
			Index = 6;

			player->SetIsKnockdown(true);

			AttackCount = 0;
			bIsKnockdownAction = false;
		}
	}
	


	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
	
	

}

void UCDoAction_Random::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{
	Super::OnAttachmentEndOverlap(InAttacker, InOther);
}


void UCDoAction_Random::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();
}

void UCDoAction_Random::SetPlayerGuardCancle()
{
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(OwnerCharacter);
	CheckNull(enemy);

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(OwnerCharacter);
	CheckNull(behavior);

	Targetplayer = Cast<ACPlayer>(behavior->GetTarget());

	CheckNull(Targetplayer);
	Targetplayer->SetGuard(false);
}


