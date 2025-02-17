// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Sword/CSkillAction_Sword_X.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CAttackPointComponent.h"
#include "Weapons/CAttachment.h"


void UCSkillAction_Sword_X::DoSkillAction()
{
	Super::DoSkillAction();


	CheckNull(Owner);
	Owner->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;

	ACPlayer* player = Cast<ACPlayer>(Owner);

	CheckNull(player);
	player->SetCurrentSkillType(SKillType);


}

void UCSkillAction_Sword_X::LaunchPlayer()
{
	// ĳ������ �������� ���ϰ� �о��
	FVector LaunchVelocity = Owner->GetActorForwardVector();
	Owner->GetCharacterMovement()->AddImpulse(LaunchVelocity * LaunchSpeed);
	//Owner->LaunchCharacter(LaunchVelocity * LaunchSpeed, false, false);

	
}

void UCSkillAction_Sword_X::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);
	CheckNull(InOther);

	if (!Hitted.IsEmpty())
		Hitted.Empty();

	//���� �÷��̾ ���� ��ų�� ���ٸ� �Ʒ��Ž���(��ų���� �ٸ� ��������Ʈ�� �������ֱ�����)
	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(EObjectTypeQuery::ObjectTypeQuery3);

	Attacker = InAttacker;
	AttackCauser = InAttackCauser;

	UKismetSystemLibrary::SphereTraceMultiForObjects(player->GetWorld(), player->GetActorLocation(), player->GetActorLocation(),SphereDistance, types,false, ignores,EDrawDebugTrace::None, hitResults,true);

	CheckTrue(HitData.Power == 0);

	for (FHitResult hit : hitResults)
	{
		ACharacter* hitted = Cast<ACharacter>(hit.GetActor());
		Hitted.AddUnique(hitted);
		
	}
		
	UCAttackPointComponent* Attack = CHelpers::GetComponent<UCAttackPointComponent>(Owner);
	CheckNull(Attack);

	if(player->IsAwakeMode() == false)
		Attack->SetAttackPoint(Hitted.Num() * 20.f);

}

void UCSkillAction_Sword_X::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{
	Super::OnAttachmentEndOverlap(InAttacker, InOther);

}

void UCSkillAction_Sword_X::OnAttachmentEndCollision()
{

	Super::OnAttachmentEndCollision();

	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);
	
	hitResults.Empty();

}

/// <summary>
/////��Ƽ���̷� ���ؼ� ȣ��Ǵ��Լ�//////////////
/// </summary>

void UCSkillAction_Sword_X::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();
	

	// �뽬�� �� �ӵ� �� ������ ����

	// �߷��� �������� ������ ����� ������ ���� �� ����
	//Owner->LaunchCharacter(LaunchVelocity * 1500.0f, true, false);  // �߷��� �����ϵ��� ����


}

void UCSkillAction_Sword_X::End_DoSkillAction()
{
	Super::End_DoSkillAction();

	//SkillActionData.Destroy_GhostTrail();
	if (!Hitted.IsEmpty())
		Hitted.Empty();
	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	player->SetCurrentSkillType(ESkillType::MAX);
}

void UCSkillAction_Sword_X::TakeDamage()
{
	CheckNull(Attacker);
	CheckNull(AttackCauser);
	CheckTrue(Hitted.Num() < 1);

	for (ACharacter* hitted : Hitted)
	{
		if (hitted != nullptr)
		{
			HitData.SendDamage(Attacker, AttackCauser, hitted);
			
		}
		
	}

	Hitted.Empty();
	Attacker = nullptr;
	AttackCauser = nullptr;
}


void UCSkillAction_Sword_X::SetStopSpawnGhostTrail()
{
	
	SkillActionData.SetCanSpawnGhostTrail(false);


}