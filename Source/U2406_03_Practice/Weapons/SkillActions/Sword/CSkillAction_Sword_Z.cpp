// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Sword/CSkillAction_Sword_Z.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Weapons/CAttachment.h"

void UCSkillAction_Sword_Z::DoSkillAction()
{
	Super::DoSkillAction();

	CheckNull(Owner);
	ACPlayer* player = Cast<ACPlayer>(Owner);

	CheckNull(player);
	player->SetCurrentSkillType(SKillType);

}

void UCSkillAction_Sword_Z::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);
	CheckNull(InOther);


	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);

	for (ACharacter* hitted : Hitted)
	{
		HitData.SendDamage(InAttacker, InAttackCauser, InOther);
		CheckTrue(hitted == InOther);
	}
		

	Hitted.AddUnique(InOther);

	//CLog::Print("UCSkillAction_Sword_Z->OnAttachmentBeginOverlap()", 15000, 1.f);
	CheckTrue(HitData.Power == 0);

	HitData.SendDamage(InAttacker, InAttackCauser, InOther);

	ACEnemy* Enemy = Cast<ACEnemy>(InOther);
	CheckNull(Enemy);

	TArray<AActor*> ignores;
	ignores.Add(InAttacker);

	FHitResult hitresult;

	FVector Start = InAttackCauser->GetActorLocation();
	FVector End = Enemy->GetActorLocation();

	UKismetSystemLibrary::LineTraceSingle(Enemy->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitresult, true);
	Enemy->SetCollisionPoint(hitresult.ImpactPoint);
	Enemy->SetCollisionPointNormal(hitresult.ImpactNormal.Rotation());

}

void UCSkillAction_Sword_Z::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);


	CheckNull(candidate);
	APlayerController* controller = Cast<APlayerController>(Owner->GetController());
	CheckNull(controller);
	FRotator controlRotation = Owner->GetControlRotation();

	//보간제한(어느정도 값에 도달하면 고정시키고 보간멈추기)
	if (controlRotation.Equals(target, FinishAngle))
	{

		controller->SetControlRotation(target);
		candidate = nullptr;


		return;
	}

	FRotator Degree = controlRotation - OwnerToTarget;


	float Pitch = controlRotation.Pitch;

	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation, target, InDeltaTime, RInterpTime);

	controller->SetControlRotation(result);

}

void UCSkillAction_Sword_Z::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{
	Super::OnAttachmentEndOverlap(InAttacker, InOther);

	
}

void UCSkillAction_Sword_Z::OnAttachmentEndCollision()
{

	Super::OnAttachmentEndCollision();

	CLog::Print("UCSkillAction_Sword_Z->OnAttachmentEndOverlap()", 15000, 1.f);
	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);

	float angle = -2.0f;


	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - Owner->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = Owner->GetActorForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if (dot < 0.75f || dot < angle)
			continue;

		angle = dot;
		candidate = hitted;


	}

	if (!!candidate)
	{
		OwnerToTarget = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), candidate->GetActorLocation());



		APlayerController* controller = Owner->GetController<APlayerController>();
		if (!!controller)
		{
			FRotator controlRotation = controller->GetControlRotation();
			target = FRotator(controlRotation.Pitch, OwnerToTarget.Yaw, controlRotation.Roll);


		}

	}

	Hitted.Empty();
	player->SetCurrentSkillType(ESkillType::MAX);
}

void UCSkillAction_Sword_Z::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	CheckNull(Owner);
	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	State->SetActionMode();
	State->OnSkillActionMode();
	player->SetCurrentSkillType(SKillType);


}