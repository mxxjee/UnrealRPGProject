// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Sword/CSkillAction_Sword_C.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

void UCSkillAction_Sword_C::DoSkillAction()
{
	Super::DoSkillAction();

	CheckNull(Owner);
	player = Cast<ACPlayer>(Owner);

	bUseSwordCSkill = true;
	CheckNull(player);
	player->SetCurrentSkillType(SKillType);
}

void UCSkillAction_Sword_C::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	if (Index == -1)
		++Index;

	Movement->Stop();
	
	CheckNull(player);
	player->SetIsJump(true);
}

void UCSkillAction_Sword_C::LaunchPlayer()
{
	// 캐릭터를 띄우기

	FVector LaunchVelocity = Owner->GetActorUpVector();
	Owner->GetCharacterMovement()->GravityScale = LaunchGravityScale;

	Owner->LaunchCharacter(LaunchVelocity * LaunchSpeed, false, false);
}

void UCSkillAction_Sword_C::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);
	CheckNull(InOther);

	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);

	Hitted.AddUnique(InOther);
	Attacker = InAttacker;
	AttackCauser = InAttackCauser;

	if (!Hitted.IsEmpty() && bLaunchEnemy)
	{
		for (ACharacter* hitted : Hitted)
		{
			if (hitted != nullptr)
			{
				ACEnemy* enemy = Cast<ACEnemy>(hitted);

				bool bAirHit = enemy->GetbAirSkillDamaged();
				/*최초 1회 몽타주재생*/
				if (bAirHit)
					HitData.SendDamage(Attacker, AttackCauser, hitted);

		

			}

		}
	}
}

void UCSkillAction_Sword_C::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{
	Super::OnAttachmentEndOverlap(InAttacker, InOther);

}

void UCSkillAction_Sword_C::OnAttachmentEndCollision()
{

	Super::OnAttachmentEndCollision();

	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);



}

void UCSkillAction_Sword_C::TakeLaunch()
{
	CheckNull(Attacker);
	CheckNull(AttackCauser);
	CheckTrue(Hitted.Num() < 1);

	for (ACharacter* hitted : Hitted)
	{
		if (hitted != nullptr)
		{
			FVector LaunchVelocity = hitted->GetActorUpVector();
			hitted->GetCharacterMovement()->GravityScale = EnemyLaunchGravityScale;
			hitted->LaunchCharacter(LaunchVelocity * LaunchSpeed, false, false);
			hitted->PlayAnimMontage(DamagedMontage, 1.0f);

			UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(hitted);
			if (state)
				state->SetDamagedMode();
			
			bLaunchEnemy = true;
		}

	}

	
	
}

void UCSkillAction_Sword_C::StopLaunchEnemy()
{
	
	CheckTrue(Hitted.Num() < 1);

	//좀더 빨리떨어지게 하기 위해 변수설정, End_DoActionSkill에서 다시 1로해줌.
	for (ACharacter* hitted : Hitted)
	{
		if (hitted != nullptr)
		{
			hitted->GetCharacterMovement()->GravityScale = RecoverEnemyGravityScale;
			
			//살짝밀어주기
			FVector start = hitted->GetActorLocation();
			FVector target = Owner->GetActorLocation();	//플레이어

			FVector direction = target - start;
			direction.Normalize();

			//그 반대로 밀어야하므로 -곱함
			hitted->LaunchCharacter(-direction * FinalLaunch, false, false);
		}

	}

	
}

void UCSkillAction_Sword_C::StopLaunchPlayer()
{
	//좀더 빨리떨어지게하기위해 5로설정, JumpEnd노티파이에서 다시 1로설정해줌.
	Owner->GetCharacterMovement()->GravityScale = RecoverGravityScale;

}

void UCSkillAction_Sword_C::End_DoSkillAction()
{
	Super::End_DoSkillAction();

	State->SetActionMode();	//착지 이후 공격가능하게해야하므로 다시 action으로 해주고, 노티파이로 idle로 변경.

	//SkillActionData.Destroy_GhostTrail();

	if (Hitted.Num() > 0)
	{
		for (ACharacter* hitted : Hitted)
		{
			if (hitted != nullptr)
			{
				hitted->GetCharacterMovement()->GravityScale = 1.f;
			}

		}
	}
	

	Hitted.Empty();
	Movement->Stop();

	CheckNull(player);
	Attacker = nullptr;
	AttackCauser = nullptr;
	bLaunchEnemy = false;
	player->SetIsJump(false);

	player->SetCurrentSkillType(ESkillType::MAX);
}

void UCSkillAction_Sword_C::SetStopSpawnGhostTrail()
{

	SkillActionData.SetCanSpawnGhostTrail(false);


}