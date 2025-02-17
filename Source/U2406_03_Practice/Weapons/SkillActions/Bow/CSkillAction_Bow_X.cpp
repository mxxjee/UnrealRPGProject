// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Bow/CSkillAction_Bow_X.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CMovementComponent.h"
#include "Components/CManaPointComponent.h"
#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/AddOns/CArrow.h"
#include "Characters/CEnemy.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/AddOns/CIceTrap.h"
#include "Components/CAttackPointComponent.h"
#include "Characters/CEnemy_AI_Boss.h"

void UCSkillAction_Bow_X::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	CheckNull(Owner);
	player = Cast<ACPlayer>(Owner);

	Attack = CHelpers::GetComponent<UCAttackPointComponent>(Owner);
}

void UCSkillAction_Bow_X::DoSkillAction()
{
	Super::DoSkillAction();

	InitCameraPos = Camera->GetRelativeLocation();
	InitCameraRot = Camera->GetRelativeRotation();

	CheckNull(player);
	player->SetCurrentSkillType(SKillType);


	Camera->SetRelativeLocation(MovePos);
	Camera->SetRelativeRotation(MoveRot);
	//camera->SetRelativeRotation(FMath::RInterpTo(camera->GetRelativeRotation(), MoveRot, FrameDeltaTime, RotateTime));

}



void UCSkillAction_Bow_X::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();

	Movement->Stop();

	ACArrow* arrow = GetArrowAttached();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));


	arrow->OnHitArrow.AddDynamic(this, &UCSkillAction_Bow_X::OnArrowHit);//연결
	arrow->OnEndPlay.AddDynamic(this, &UCSkillAction_Bow_X::OnEndPlay);//연결




	FVector forward = UGameplayStatics::GetPlayerCameraManager(Owner->GetWorld(), 0)->GetActorForwardVector();
	
	if (Attack->IsAwake())
	{
		arrow->ShootAwake(forward);
		UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
		TArray<ACArrow*>& Arrows = bowAction->GetArrows();

		Arrows.Remove(Cast<ACArrow>(arrow));


		CreateArrow();
		return;
	}

	arrow->SkillIceShoot(forward, AddSpeed);
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(arrow));


	CreateArrow();
	
	

	/*else
	{
		FVector Start = player->GetActorLocation();
		FVector End = target->GetActorLocation();

		FVector Direction = End - Start;
		Direction.Normalize();
		arrow->SkillShoot(Direction, AddSpeed);

		CreateArrow();
		return;
	}*/
}

void UCSkillAction_Bow_X::End_DoSkillAction()
{
	Super::End_DoSkillAction();
	//CreateArrow();

	CheckNull(player);
	player->bUseControllerRotationYaw = true;

	Camera->SetRelativeLocation(InitCameraPos);
	Camera->SetRelativeRotation(InitCameraRot);
}

void UCSkillAction_Bow_X::LaunchPlayer()
{
	FVector LaunchVelocity = Owner->GetActorUpVector();
	Owner->GetCharacterMovement()->GravityScale = LaunchGravityScale;
	
	Owner->LaunchCharacter(LaunchVelocity * LaunchSpeed, false, false);
}

void UCSkillAction_Bow_X::CreateArrow()
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

	CLog::Print(Arrows.Num(),15000,2);

	//확정(배치)
	UGameplayStatics::FinishSpawningActor(arrow, transform);

	if (Attack->IsAwake())
		arrow->SetArrowMaterial(FLinearColor(0.081419f, 0, 1, 1));

	else
		arrow->SetArrowMaterial(FLinearColor(0, 0.6f, 1, 1));
}

void UCSkillAction_Bow_X::CreateArrowAndShot()
{


	ACArrow* arrow = GetArrowAttached();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));


	arrow->OnHitArrow.AddDynamic(this, &UCSkillAction_Bow_X::OnArrowHit);//연결
	arrow->OnFloorHitArrow.AddDynamic(this, &UCSkillAction_Bow_X::OnHitFloorArrow);
	arrow->OnEndPlay.AddDynamic(this, &UCSkillAction_Bow_X::OnEndPlay);//연결




	FVector forward = UGameplayStatics::GetPlayerCameraManager(Owner->GetWorld(), 0)->GetActorForwardVector();
	
	
	if (Attack->IsAwake())
	{
		arrow->ShootAwake(forward);
		UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
		TArray<ACArrow*>& Arrows = bowAction->GetArrows();

		Arrows.Remove(Cast<ACArrow>(arrow));

		CreateArrow();
		return;
	}

	arrow->SkillIceShoot(forward, AddSpeed);
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(arrow));


	CreateArrow();


	

	
}


ACArrow* UCSkillAction_Bow_X::GetArrowAttached()
{
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNullResult(bowAction, nullptr);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	for (ACArrow* arrow : Arrows)
	{
		if (!!arrow)
		{
			if (arrow->GetAttachParentActor()==Owner)
				return arrow;
		}
		
	}

	return nullptr;
}

void UCSkillAction_Bow_X::StopLaunchPlayer()
{
	//좀더 빨리떨어지게하기위해 5로설정, JumpEnd노티파이에서 다시 1로설정해줌.
	Owner->GetCharacterMovement()->GravityScale = RecoverGravityScale;

}

void UCSkillAction_Bow_X::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckTrue(HitData.Power == 0);

	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(InCauser));


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

void UCSkillAction_Bow_X::OnHitFloorArrow(AActor* InCauser)
{
	/*UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(InCauser));*/

	if (bPlaySoundEffect)
	{
		UGameplayStatics::SpawnSoundAtLocation(Owner->GetWorld(), Icesound, InCauser->GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(Owner->GetWorld(), EarthqaukeSound, InCauser->GetActorLocation());
		bPlaySoundEffect = false;
	}

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		bPlaySoundEffect = true;
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, 5, false);
	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	FTransform transform;
	transform.SetLocation(InCauser->GetActorLocation());
	transform.SetRotation(FQuat(FRotator::ZeroRotator));

	ACIceTrap* icetrap = Owner->GetWorld()->SpawnActor<ACIceTrap>(IceTrapClass, transform, params);
	

}

void UCSkillAction_Bow_X::OnEndPlay(ACArrow* InDestroyActor)
{
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	Arrows.Remove(InDestroyActor);
}


