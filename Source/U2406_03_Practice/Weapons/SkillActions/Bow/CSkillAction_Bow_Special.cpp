// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Bow/CSkillAction_Bow_Special.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI_Boss.h"

#include "Components/CMovementComponent.h"
#include "Components/CManaPointComponent.h"
#include "Components/CAttackPointComponent.h"

#include "Weapons/AddOns/CArrow.h"
#include "Weapons/DoActions/CDoAction_Bow.h"

void UCSkillAction_Bow_Special::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);
	
	CheckNull(Owner);
	player = Cast<ACPlayer>(Owner);

	Attack = CHelpers::GetComponent<UCAttackPointComponent>(Owner);
}

void UCSkillAction_Bow_Special::DoSkillAction()
{
	Super::DoSkillAction();

	State->SetActionMode();
	State->OnSkillActionMode();

	CheckNull(player);
	player->SetCurrentSkillType(SKillType);

	bDoChargeAnim = true;
	CreateParticleActor();
	
}
void UCSkillAction_Bow_Special::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	
	CheckTrue(LockOnEnemys.Num() == 0);

	ACArrow* arrow = GetArrowAttached();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));



	arrow->Shoot(arrow->GetActorForwardVector());
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(arrow));


	
	//SpawnArrowToEnemy();
}

void UCSkillAction_Bow_Special::SpawnArrowToEnemy()
{
	CheckTrue(Owner->GetWorld()->bIsTearingDown);//게임끝나는 중인지확인
	CheckNull(ArrowClass);
	

	for (ACEnemy* enemy : LockOnEnemys)
	{
		FTransform SocketTrans = enemy->GetMesh()->GetSocketTransform("ShootPoint");
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketTrans.GetLocation());
		SpawnTransform.SetRotation(SocketTrans.GetRotation());
	


		ACArrow* arrow = Owner->GetWorld()->SpawnActorDeferred<ACArrow>(ArrowClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

		UGameplayStatics::FinishSpawningActor(arrow, SpawnTransform);

		arrow->OnHitArrow.AddDynamic(this, &UCSkillAction_Bow_Special::OnArrowHit);//연결
		arrow->OnEndPlay.AddDynamic(this, &UCSkillAction_Bow_Special::OnEndPlay);//연결

		if (Attack->IsAwake())
			arrow->SetArrowMaterial(FLinearColor(0.081419f, 0, 1, 1));

		else
			arrow->SetArrowMaterial(FLinearColor(0, 0.6f, 1, 1));

		arrow->ShootSpecialSkill(arrow->GetActorForwardVector());

	}

}
void UCSkillAction_Bow_Special::End_DoSkillAction()
{
	Super::End_DoSkillAction();

	LockOnEnemys.Empty();
	CreateArrow();
}

void UCSkillAction_Bow_Special::Pressing()
{
	CheckFalse(State->IsSkillActionMode());
	CheckFalse(bDoChargeAnim);
	CheckNull(player);

	
	Movement->Stop();

	CheckTrue(LockOnEnemys.Num() >= 5);
	ACEnemy* enemy = player->GetLockOnEnemy();
	CheckNull(enemy);

	CheckNull(LockOnSound);

	if (!LockOnEnemys.Contains(enemy))
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(),
			LockOnSound,
			Owner->GetActorLocation());
		LockOnEnemys.AddUnique(enemy);
	}
	

	//추가한 enemy들의 overlay 메테리얼 변경
	enemy->LockOn();

	
}

void UCSkillAction_Bow_Special::Released()
{
	CheckFalse(bDoChargeAnim);
	CheckTrue(bInSkill);
	bInSkill = true;


	ParticleActor->Destroy();
	//몽타주실행
	Owner->PlayAnimMontage(ShootMontage);
	ManaPoint->SetCurrentManaPoint(SkillActionData.Mana);

	//Enemy들 배열을 순회하며 화살을 생성 후 enemy들 머리 위 소켓을 향해 화살발사.
	for (ACEnemy* enemy : LockOnEnemys)
	{
		if (!!enemy)
			enemy->LockOff();
	}
	
	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		bInSkill = false;
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, SkillActionData.CoolTime, false);
	
}

void UCSkillAction_Bow_Special::CreateArrow()
{
	CheckTrue(Owner->GetWorld()->bIsTearingDown);//게임끝나는 중인지확인
	CheckNull(ArrowClass);

	FTransform transform;
	ACArrow* arrow = Owner->GetWorld()->SpawnActorDeferred<ACArrow>(ArrowClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIngoreActor(Owner);


	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(Owner->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	arrow->SetActorRotation(FQuat(Owner->GetMesh()->GetSocketRotation("Hand_Bow_Right_Arrow")));

	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	Arrows.Add(arrow);
	//확정(배치)

	CLog::Print(Arrows.Num(), 15000, 2);
	UGameplayStatics::FinishSpawningActor(arrow, transform);

	if (Attack->IsAwake())
		arrow->SetArrowMaterial(FLinearColor(0.081419f, 0, 1, 1));

	else
		arrow->SetArrowMaterial(FLinearColor(0, 0.6f, 1, 1));
}


void UCSkillAction_Bow_Special::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckTrue(HitData.Power == 0);
	/*UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(InCauser));*/

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	FHitResult hitresult;
	//Enemy에게 명중했다면//

	ACEnemy* enemy = Cast<ACEnemy>(InOtherCharacter);
	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(enemy);

	if (!!boss)
	{
		UCStateComponent* bossState = CHelpers::GetComponent<UCStateComponent>(boss);
		CheckNull(bossState);

		if (bossState->IsSkillActionMode())
		{
			boss->SpawnInvincibleUI();
			return;
		}

	}

	if (!!enemy)
	{
		FVector Start = InCauser->GetActorLocation();
		FVector End = enemy->GetActorLocation();


		UKismetSystemLibrary::LineTraceSingle(player->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitresult, true);
		enemy->SetCollisionPoint(hitresult.ImpactPoint);
		enemy->SetCollisionPointNormal(hitresult.ImpactNormal.Rotation());
	}



	HitData.SendDamage(Owner, InCauser, InOtherCharacter);

}

void UCSkillAction_Bow_Special::OnEndPlay(ACArrow* InDestroyActor)
{
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	Arrows.Remove(InDestroyActor);
}


ACArrow* UCSkillAction_Bow_Special::GetArrowAttached()
{
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNullResult(bowAction, nullptr);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	for (ACArrow* arrow : Arrows)
	{
		if (!!arrow)
		{
			if (arrow->GetAttachParentActor() == Owner)
				return arrow;
		}
	}

	return nullptr;
}

void UCSkillAction_Bow_Special::CreateParticleActor()
{
	CheckNull(SkillParticle);

	FTransform transform = Owner->GetMesh()->GetSocketTransform("SpawnParticle");

	FActorSpawnParameters spawnParams;

	ParticleActor = Owner->GetWorld()->SpawnActor<AActor>(
		SkillParticle,
		transform.GetLocation(),
		transform.GetRotation().Rotator(),
		spawnParams
		);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ParticleActor->AttachToComponent(Owner->GetMesh(), rule, "SpawnParticle");

	
	
}
