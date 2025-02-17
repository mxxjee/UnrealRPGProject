// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CSkillAction_Bow_Z.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CMovementComponent.h"
#include "Components/CManaPointComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/AddOns/CArrow.h"
#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CAttackPointComponent.h"
#include "Sound/SoundCue.h"

UCSkillAction_Bow_Z::UCSkillAction_Bow_Z()
{
	//CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/Weapons/Bow/CurveVector_Aim.CurveVector_Aim'");
}

void UCSkillAction_Bow_Z::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);

	Bow = Cast<ACAttachment_Bow>(InAttachment);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);


	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);

	if (!!Bow)
		Bending = Bow->GetAnimInstance_Bending();


	CheckNull(Owner);
	player = Cast<ACPlayer>(Owner);

	Attack = CHelpers::GetComponent<UCAttackPointComponent>(Owner);
}

void UCSkillAction_Bow_Z::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	Timeline.TickTimeline(InDeltaTime);	//call 해줘야 tick마다 timeline함수가 실행됨
	Timeline.SetPlayRate(PlayRate);//200배 빠르게 -> 0.1초
}
void UCSkillAction_Bow_Z::DoSkillAction()
{
	CheckTrue(State->IsSubActionMode());
	Super::DoSkillAction();

	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);
	
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	State->SetActionMode();
	State->OnSkillActionMode();
	State->OffSubActionMode();


	CheckNull(player);
	player->SetCurrentSkillType(SKillType);

	if(PostProcessVolume==nullptr)
		PostProcessVolume = player->GetWorldPostProcessVolume();

	UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), PulledCue, Owner->GetActorLocation());
	StartZoom();

	bDoChargeAnim = true;

	player->CrossHairLineTrace_Skill_Z();
}

void UCSkillAction_Bow_Z::Pressing()
{

	CheckFalse(State->IsSkillActionMode());
	CheckFalse(bDoChargeAnim);
	Power = FMath::Clamp(Power, 0, 30);
	Power += 1.f;

	Movement->Stop();


	player->CrossHairLineTrace_Skill_Z();
}

void UCSkillAction_Bow_Z::Released()
{


	CheckFalse(bDoChargeAnim);
	if (Power < 10)
	{
		AddPower = 5;
		AddVelocity = 100;
		OverlapCount = 1;
	}

	else if (Power >= 10 && Power <= 20)
	{
		AddPower = 10;
		AddVelocity = 500;
		OverlapCount = 2;
	}

	else if (Power >=30)
	{
		AddPower = 15;
		AddVelocity = 1000;
		OverlapCount = 3;
	}

	HitData.Power += AddPower;

	CheckTrue(bInSkill);
	CheckNull(ShootMontage);

	Owner->PlayAnimMontage(ShootMontage);

	ManaPoint->SetCurrentManaPoint(SkillActionData.Mana);

	CheckNull(Weapon->GetAttachment());

	ACAttachment* attachment = Weapon->GetAttachment();
	
	bInSkill = true;

	ReleaseZoom();
	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		bInSkill = false;
	});

	//다시되돌려준다.
	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, SkillActionData.CoolTime, false);

}

void UCSkillAction_Bow_Z::StartZoom()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	Owner->bUseControllerRotationYaw = true;

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	/*시야각 줄이기*/
	//Camera->FieldOfView = 45;
	CheckNull(Owner);
	CheckNull(player);
	//PostProcessVolume = player->GetWorldPostProcessVolume();

	Timeline.PlayFromStart();
}

void UCSkillAction_Bow_Z::ReleaseZoom()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	/*시야각  다시되돌리기*/
	//Camera->FieldOfView = 90;
	Timeline.Reverse();	//다시 45부터 90
}

void UCSkillAction_Bow_Z::CreateArrow()
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

	CLog::Print(Arrows.Num(), 15000,2);
	UGameplayStatics::FinishSpawningActor(arrow, transform);

	if (Attack->IsAwake())
		arrow->SetArrowMaterial(FLinearColor(0.081419f, 0, 1, 1));

	else
		arrow->SetArrowMaterial(FLinearColor(0, 0.6f, 1, 1));


}
void UCSkillAction_Bow_Z::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();
	ACArrow* arrow = GetArrowAttached();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));


	arrow->OnHitArrow.AddDynamic(this, &UCSkillAction_Bow_Z::OnArrowHit);//연결
	arrow->OnEndPlay.AddDynamic(this, &UCSkillAction_Bow_Z::OnEndPlay);//연결



	//카메라 방향으로 shoot

	FVector forward = UGameplayStatics::GetPlayerCameraManager(Owner->GetWorld(), 0)->GetActorForwardVector();


	if (Attack->IsAwake())
	{
		arrow->ShootAwake(forward);
		UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
		TArray<ACArrow*>& Arrows = bowAction->GetArrows();

		Arrows.Remove(Cast<ACArrow>(arrow));

		return;
	}

	arrow->Shoot(forward, AddVelocity, OverlapCount );
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(arrow));

}

void UCSkillAction_Bow_Z::End_DoSkillAction()
{
	Super::End_DoSkillAction();

	Power = 0;
	AddPower = 0;
	bDoChargeAnim = false;
	AddVelocity = 0;
	OverlapCount = 0;

	CreateArrow();

}
void UCSkillAction_Bow_Z::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;

	if (!!Bending)
		*Bending = Output.Y;
	
	CheckNull(PostProcessVolume);
	OnVignetting(PostProcessVolume, Output.Z);

	//CheckNull()

}

ACArrow* UCSkillAction_Bow_Z::GetArrowAttached()
{
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNullResult(bowAction,nullptr);

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

void UCSkillAction_Bow_Z::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckTrue(HitData.Power==0);
	/*UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	TArray<ACArrow*>& Arrows = bowAction->GetArrows();

	Arrows.Remove(Cast<ACArrow>(InCauser));*/


	TArray<AActor*> ignores;
	ignores.Add(Owner);

	FHitResult hitresult;
	//Enemy에게 명중했다면//

	ACEnemy* enemy = Cast<ACEnemy>(InOtherCharacter);
	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(enemy);

	if(!!boss)
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

void UCSkillAction_Bow_Z::OnEndPlay(ACArrow* InDestroyActor)
{
	UCDoAction_Bow* bowAction = Cast<UCDoAction_Bow>(Weapon->GetDoAction());
	CheckNull(bowAction);

	TArray<ACArrow*>& Arrows = bowAction->GetArrows();
	Arrows.Remove(InDestroyActor);
}
