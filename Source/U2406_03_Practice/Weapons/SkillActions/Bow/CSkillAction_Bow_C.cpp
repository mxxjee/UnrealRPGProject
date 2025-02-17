// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Bow/CSkillAction_Bow_C.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Weapons/AddOns/CArrow.h"
#include "Characters/CEnemy.h"
#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Components/CAttackPointComponent.h"
#include "Characters/CEnemy_AI_Boss.h"

void UCSkillAction_Bow_C::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);
	CheckNull(Owner);
	player = Cast<ACPlayer>(Owner);

	Attack = CHelpers::GetComponent<UCAttackPointComponent>(Owner);
}

void UCSkillAction_Bow_C::DoSkillAction()
{
	Super::DoSkillAction();

	CheckNull(player);
	player->SetCurrentSkillType(SKillType);
}
void UCSkillAction_Bow_C::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	ACArrow* arrow = GetArrowAttached();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));


	arrow->OnHitArrow.AddDynamic(this, &UCSkillAction_Bow_C::OnArrowHit);//연결
	arrow->OnEndPlay.AddDynamic(this, &UCSkillAction_Bow_C::OnEndPlay);//연결

	FVector forward = UGameplayStatics::GetPlayerCameraManager(Owner->GetWorld(), 0)->GetActorForwardVector();

	if (Attack->IsAwake())
	{
		arrow->ShootAwake(forward);
		UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
		TArray<ACArrow*>& Arrows = bowAction->GetArrows();

		Arrows.Remove(Cast<ACArrow>(arrow));

		return;
	}

	arrow->SkillShoot(forward, AddSpeed);
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(arrow));

}

void UCSkillAction_Bow_C::End_DoSkillAction()
{
	Super::End_DoSkillAction();

	CreateNormalArrow();
}


void UCSkillAction_Bow_C::CreateArrowAndShot()
{
	CreateArrow();

	ACArrow* arrow = GetArrowAttached();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));


	arrow->OnHitArrow.AddDynamic(this, &UCSkillAction_Bow_C::OnArrowHit);//연결
	arrow->OnEndPlay.AddDynamic(this, &UCSkillAction_Bow_C::OnEndPlay);//연결


	FVector forward = UGameplayStatics::GetPlayerCameraManager(Owner->GetWorld(), 0)->GetActorForwardVector();
	
	if (Attack->IsAwake())
	{
		arrow->ShootAwake(forward);
		UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
		TArray<ACArrow*>& Arrows = bowAction->GetArrows();

		Arrows.Remove(Cast<ACArrow>(arrow));

		return;
	}

	arrow->SkillShoot(forward, AddSpeed);
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(arrow));

	
}

void UCSkillAction_Bow_C::CreateArrow()
{
	CheckTrue(Owner->GetWorld()->bIsTearingDown);//게임끝나는 중인지확인
	CheckNull(SkillArrowClass);

	FTransform transform;
	ACArrow* arrow = Owner->GetWorld()->SpawnActorDeferred<ACArrow>(SkillArrowClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIngoreActor(Owner);



	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(Owner->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	arrow->SetActorRotation(FQuat(Owner->GetMesh()->GetSocketRotation("Hand_Bow_Right_Arrow")));

	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	Arrows.Add(arrow);

	UGameplayStatics::FinishSpawningActor(arrow, transform);

	if (Attack->IsAwake())
		arrow->SetArrowMaterial(FLinearColor(0.081419f, 0, 1, 1));

	else
		arrow->SetArrowMaterial(FLinearColor(0, 0.6f, 1, 1));
}

void UCSkillAction_Bow_C::CreateNormalArrow()
{
	CheckTrue(Owner->GetWorld()->bIsTearingDown);//게임끝나는 중인지확인
	CheckNull(NormalArrowClass);

	FTransform transform;
	ACArrow* arrow = Owner->GetWorld()->SpawnActorDeferred<ACArrow>(NormalArrowClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIngoreActor(Owner);



	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(Owner->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	arrow->SetActorRotation(FQuat(Owner->GetMesh()->GetSocketRotation("Hand_Bow_Right_Arrow")));

	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	Arrows.Add(arrow);

	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACArrow* UCSkillAction_Bow_C::GetArrowAttached()
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
void UCSkillAction_Bow_C::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	
	CheckTrue(HitData.Power == 0);

	//UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	//TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	//Arrows.Remove(Cast<ACArrow>(InCauser));

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

void UCSkillAction_Bow_C::OnArrowFloorHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	/*UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(InCauser));*/

}


void UCSkillAction_Bow_C::OnEndPlay(ACArrow* InDestroyActor)
{
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	Arrows.Remove(InDestroyActor);
}
