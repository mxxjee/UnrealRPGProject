// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Hammer/CSkillAction_Hammer_C.h"
#include "Global.h"
#include "Weapons/CDoAction.h"
#include "Characters/CPlayer.h"
#include "Components/CManaPointComponent.h"
#include "Characters/CEnemy.h"


void UCSkillAction_Hammer_C::DoSkillAction()
{
	CheckNull(Weapon);
	CheckNull(Weapon->GetDoAction());

	InitDoActionDatas = Weapon->GetDoActionDatas();
	InitHitDatas = Weapon->GetHitDatas();

	CheckFalse(ManaPoint->CanUseSkill(SkillActionData.Mana));
	CheckFalse(State->IsIdleMode());

	bInSkill = true;
	Weapon->GetDoAction()->SetDatas(NewDoActionDatas, NewHitDatas);
	SkillActionData.Montage = NewDoActionDatas[0].Montage;
	//SkillActionData.DoSkillAction(Owner);
	Weapon->GetDoAction()->DoAction();
	ManaPoint->SetCurrentManaPoint(SkillActionData.Mana);

	CheckNull(Owner);
	ACPlayer* player = Cast<ACPlayer>(Owner);

	CheckNull(player);
	player->SetCurrentSkillType(SKillType);

	//쿨타임 초 후 되돌려주기
	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		bInSkill = false;
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, SkillActionData.CoolTime, false);

}

void UCSkillAction_Hammer_C::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();
	
}



void UCSkillAction_Hammer_C::End_DoSkillAction()
{
	Super::End_DoSkillAction();


	Weapon->GetDoAction()->SetDatas(InitDoActionDatas, InitHitDatas);
}



void UCSkillAction_Hammer_C::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);
	CheckNull(InOther);

	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);

	TArray<AActor*> ignores;
	ignores.Add(InAttacker);

	FHitResult hitresult;

	CheckTrue(HitData.Power == 0);
	ACEnemy* Enemy = Cast<ACEnemy>(InOther);
	CheckNull(Enemy);



	FVector Start = InAttackCauser->GetActorLocation();
	FVector End = Enemy->GetActorLocation();


	UKismetSystemLibrary::LineTraceSingle(Enemy->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitresult, true);
	Enemy->SetCollisionPoint(hitresult.ImpactPoint);
	Enemy->SetCollisionPointNormal(hitresult.ImpactNormal.Rotation());

	HitData.SendDamage(InAttacker, InAttackCauser, InOther);
}

void UCSkillAction_Hammer_C::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();
}