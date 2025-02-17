// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Hammer/CSkillAction_Hammer_X.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CEnemy.h"
#include "Weapons/CAttachment.h"


void UCSkillAction_Hammer_X::DoSkillAction()
{

	Super::DoSkillAction();

	CLog::Log("DoSkillAction_Hammer_X");
	
	CheckNull(Owner);
	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);

	player->SetCurrentSkillType(SKillType);
	player->SetIsJump(true);
	

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		StopLaunchPlayer();
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, StopTime, false);
}

void UCSkillAction_Hammer_X::LaunchPlayer()
{
	CheckNull(Owner);
	ACPlayer* player = Cast<ACPlayer>(Owner);

	CheckNull(player);
	

	player->OffCollision();

	ACAttachment* attach = Weapon->GetAttachment();
	CheckNull(attach);
	attach->SetCollisionSize(100, 100);

	ACharacter* candidate = player->GetCandidate();
	bool bFinish;
	FVector start;
	FVector target;

	FVector LaunchVelocity;
	if (candidate == nullptr)
	{
		LaunchVelocity = Owner->GetActorUpVector() + Owner->GetActorForwardVector();
		Owner->GetCharacterMovement()->GravityScale = LaunchGravityScale;
		Owner->LaunchCharacter(LaunchVelocity * 600, false, false);
		return;
	}

	else
	{

		player->bUseControllerRotationYaw = false;
		start = Owner->GetActorLocation();
		target = candidate->GetActorLocation();

		Distance = FVector::Distance(start, target);
		bFinish = Owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

		
	}

	CLog::Log(player->GetCandidate());
	CLog::Log(Distance);

	if (Distance > MinDistance && Distance>MidDistance)
	{
		if (bFinish)
		{

			LaunchVelocity = Owner->GetActorUpVector() + Owner->GetActorForwardVector();
			Owner->GetCharacterMovement()->GravityScale = LaunchGravityScale;
			Owner->LaunchCharacter(LaunchVelocity * LaunchSpeed, false, false);
			
			return;
	
		}

	}

	//최소거리 이하라면 그냥 위로 띄워주기.
	else if(Distance<MinDistance)
	{
		LaunchVelocity = Owner->GetActorUpVector();
		Owner->GetCharacterMovement()->GravityScale =1.f;
		Owner->LaunchCharacter(LaunchVelocity * 600, false, false);
		return;
	}

	else if (Distance > MinDistance && Distance < MidDistance)
	{
		if (bFinish)
		{

			LaunchVelocity = Owner->GetActorUpVector() + Owner->GetActorForwardVector();
			Owner->GetCharacterMovement()->GravityScale = LaunchGravityScale;
			Owner->LaunchCharacter(LaunchVelocity * LaunchSpeed/2, false, false);

			return;

		}
	}
	
}


void UCSkillAction_Hammer_X::StopLaunchPlayer()
{
	//좀더 빨리떨어지게하기위해 5로설정, JumpEnd노티파이에서 다시 1로설정해줌.
	Owner->GetCharacterMovement()->GravityScale = RecoverGravityScale;

	

	

}

void UCSkillAction_Hammer_X::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();
	bUseHammerXSkill = true;

}

void UCSkillAction_Hammer_X::End_DoSkillAction()
{
	Super::End_DoSkillAction();

	CheckNull(Owner);
	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	player->SetIsJump(false);

	Owner->GetCharacterMovement()->GravityScale = 1;
	Owner->bUseControllerRotationYaw = true;
	bUseHammerXSkill = false;


	FRotator playerRot = player->GetActorRotation();
	player->SetActorRotation(FRotator(0, playerRot.Yaw, playerRot.Roll));
	player->OnCollision();

	ACAttachment* attach = Weapon->GetAttachment();
	CheckNull(attach);
	attach->SetCollisionSize(60, 60);
}

void UCSkillAction_Hammer_X::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, InResult, OverlappedComponent, OtherComp);

	CheckNull(InOther);

	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);
	CheckFalse(player->GetCurrentSKillType() == SKillType);

	
	//Beginoverlap된 적을 띄운다.

	ACEnemy* enemy = Cast<ACEnemy>(InOther);
	CheckNull(enemy);

	enemy->SetbAirSkillDamaged(true);

	TArray<AActor*> ignores;
	ignores.Add(InAttacker);

	FHitResult hitresult;

	FVector Start = InAttackCauser->GetActorLocation();
	FVector End = enemy->GetActorLocation();


	UKismetSystemLibrary::LineTraceSingle(enemy->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitresult, true);

	FVector start = Owner->GetActorLocation();
	FVector target = InOther->GetActorLocation();

	FVector direction = target - start;
	direction.Normalize();

	FVector LaunchVelocity = InOther->GetActorUpVector()+ direction;
	InOther->GetCharacterMovement()->GravityScale = EnemyGravityScale;
	InOther->LaunchCharacter(LaunchVelocity * EnemyLaunchSpeed, false, false);

	enemy->PlayAnimMontage(HitData.Montage);
	enemy->SetCollisionPoint(hitresult.ImpactPoint);
	enemy->SetCollisionPointNormal(hitresult.ImpactNormal.Rotation());

	HitData.SendDamage(InAttacker, InAttackCauser, InOther);

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		InOther->GetCharacterMovement()->GravityScale = EnemyRecoverGravityScale;
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, EnemyStopTime, false);

	FTimerHandle timerhandle2;
	FTimerDelegate timerDelegate2;

	timerDelegate2.BindLambda([=] {

		InOther->GetCharacterMovement()->GravityScale = 1.f;
	});

	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle2, timerDelegate2, EnemyStopTime, false);
}
