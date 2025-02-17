// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI.h"


void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);
	CheckTrue(State->IsDamagedMode());
	

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());
	
	CheckNull(OwnerCharacter);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	CheckTrue(player->GetIsKnockdown());
	Super::DoAction();
	DoActionDatas[Index].DoAction(OwnerCharacter);
	
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(OwnerCharacter);
	CheckNull(enemy);

	enemy->StopMovement();

}

void UCDoAction_Combo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	//CheckNull(candidate);
	//APlayerController* controller = Cast<APlayerController>(OwnerCharacter->GetController());
	//CheckNull(controller);
	//FRotator controlRotation = OwnerCharacter->GetControlRotation();

	////보간제한(어느정도 값에 도달하면 고정시키고 보간멈추기)
	//if (controlRotation.Equals(target, FinishAngle))
	//{
	//	
	//	controller->SetControlRotation(target);
	//	candidate = nullptr;

	//	
	//	return;
	//}

	//FRotator Degree = controlRotation - OwnerToTarget;
	//
	//
	//float Pitch = controlRotation.Pitch;

	//FRotator result = UKismetMathLibrary::RInterpTo(controlRotation, target, InDeltaTime, RInterpTime);

	//controller->SetControlRotation(result);
	////OwnerCharacter->SetActorRotation(result)


}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	DoActionDatas[Index].Destroy_GhostTrail();
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();
	//DoActionDatas[Index].Destroy_GhostTrail();
	OwnerCharacter->bUseControllerRotationYaw = true;
	Index = 0;
}


void UCDoAction_Combo::AbortAction()
{
	Super::AbortAction();
	OwnerCharacter->bUseControllerRotationYaw = true;
	Index = 0;
}


void UCDoAction_Combo::DestroyGhostTrail()
{
	DoActionDatas[Index].Destroy_GhostTrail();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.Add(InOther);

	CheckTrue(HitDatas.Num() - 1 < Index);

	TArray<AActor*> ignores;
	ignores.Add(InAttacker);

	FHitResult hitresult;


	ACPlayer* Player = Cast<ACPlayer>(InOther);
	ACEnemy* Enemy = Cast<ACEnemy>(InOther);
	
	if (!!Player)
	{

		FVector Start = InAttackCauser->GetActorLocation();
		FVector End = Player->GetActorLocation();

		
		UKismetSystemLibrary::LineTraceSingle(Player->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores,EDrawDebugTrace::None, hitresult, true);
		Player->SetCollisionPoint(hitresult.ImpactPoint);
		
	}

	else if (!!Enemy)
	{
		FVector Start = InAttackCauser->GetActorLocation();
		FVector End = Enemy->GetActorLocation();


		UKismetSystemLibrary::LineTraceSingle(Enemy->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitresult, true);
		Enemy->SetCollisionPoint(hitresult.ImpactPoint);
		Enemy->SetCollisionPointNormal(hitresult.ImpactNormal.Rotation());
	}


	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

	

}

void UCDoAction_Combo::OnAttachmentEndOverlap(ACharacter* InAttacker,ACharacter* InOther)
{
	Super::OnAttachmentEndOverlap(InAttacker, InOther);
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{

    Super::OnAttachmentEndCollision();

	
    float angle = -2.0f;
    

    Hitted.Empty();
	
}