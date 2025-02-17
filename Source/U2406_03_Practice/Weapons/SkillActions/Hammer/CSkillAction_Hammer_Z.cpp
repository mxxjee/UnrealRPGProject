// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Hammer/CSkillAction_Hammer_Z.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CManaPointComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Weapons/CAttachment.h"
#include "Components/CWeaponComponent.h"

void UCSkillAction_Hammer_Z::DoSkillAction()
{
	Super::DoSkillAction();

	State->SetActionMode();
	State->OnSkillActionMode();

	CheckNull(Owner);
	player = Cast<ACPlayer>(Owner);

	CheckNull(player);
	player->SetCurrentSkillType(SKillType);

	
	
	bDoChargeAnim = true;

	
}

void UCSkillAction_Hammer_Z::Pressing()
{

	CheckFalse(State->IsSkillActionMode());
	CheckFalse(bDoChargeAnim);
	Power = FMath::Clamp(Power,0, 30);
	Power += 1.f;

	Movement->Stop();

	
	CLog::Print(Power,15000);



	CheckNull(player);



}

void UCSkillAction_Hammer_Z::Released()
{
	CheckNull(player);


	CheckFalse(bDoChargeAnim);
	if (Power < 10)
		AddPower = 5;

	else if (Power >= 10 && Power <= 20)
		AddPower = 10;
	
	else if(Power==30)
		AddPower = 15;

	HitData.Power += AddPower;

	CheckTrue(bInSkill);
	CheckNull(FullChargeSkillMontage);

	ManaPoint->SetCurrentManaPoint(SkillActionData.Mana);

	CheckNull(Weapon->GetAttachment());

	ACAttachment* attachment = Weapon->GetAttachment();
	attachment->SetCollisionSize(200, 200);

	if(Power<11)
		Owner->PlayAnimMontage(LowChargeSkillMontage);

	else if(Power>=11.01&&Power<=29.9)
		Owner->PlayAnimMontage(MidChargeSkillMontage);

	else if(Power>=30)
		Owner->PlayAnimMontage(FullChargeSkillMontage);

	
	
	bInSkill = true;

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		bInSkill = false;
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, SkillActionData.CoolTime, false);

}


void UCSkillAction_Hammer_Z::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	Movement->Stop();
	bInSkill = true;
}

void UCSkillAction_Hammer_Z::End_DoSkillAction()
{
	Super::End_DoSkillAction();

	//초기화
	Power = 0;
	AddPower = 0;
	bDoChargeAnim = false;

	CheckNull(Weapon->GetAttachment());

	ACAttachment* attachment = Weapon->GetAttachment();
	attachment->SetCollisionSize(60, 60);
}

void UCSkillAction_Hammer_Z::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);
	CheckNull(InOther);

	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);

	CheckTrue(HitData.Power == 0);

	HitData.SendDamage(InAttacker, InAttackCauser, InOther);
}


void UCSkillAction_Hammer_Z::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();
}

void UCSkillAction_Hammer_Z::Cancel_Skill()
{
	Super::Cancel_Skill();


	bInSkill = false;
	State->SetDamagedMode();
	State->OffSkillActionMode();
	

	Power = 0;
	AddPower = 0;
	bDoChargeAnim = false;

	ACAttachment* attachment = Weapon->GetAttachment();
	CheckNull(attachment);
	attachment->SetCollisionSize(60, 60);

	CheckNull(player);
	CheckNull(SkillActionData.Montage);
	player->StopAnimMontage(SkillActionData.Montage);
}