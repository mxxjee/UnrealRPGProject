// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Wizard.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Weapons/CEquipment.h"
#include "Components/CStateComponent.h"
#include "Weapons/AddOns/CMagicBall.h"
#include "Characters/CEnemy.h"

UCDoAction_Wizard::UCDoAction_Wizard()
{

}

ACMagicBall* UCDoAction_Wizard::CreateMagicBall()
{
	CheckTrueResult(World->bIsTearingDown,nullptr);//게임끝나는 중인지확인
	CheckNullResult(MagicBallClass,nullptr);

	FTransform transform;
	ACMagicBall* MagicBall = World->SpawnActorDeferred<ACMagicBall>(MagicBallClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	CheckNullResult(MagicBall,nullptr);

	MagicBall->AddIngoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	MagicBall->AttachToComponent(OwnerCharacter->GetMesh(), rule, "SpawnMagicShoot");

	UGameplayStatics::FinishSpawningActor(MagicBall, transform);

	return MagicBall;

}

void UCDoAction_Wizard::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionDatas, InHitDatas);

	//SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	//PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

	//bEquipped = InEquipment->GetbEquipped();
}

void UCDoAction_Wizard::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);


	//CheckFalse(*bEquipped);
}


void UCDoAction_Wizard::OnBeginEquip()
{
	Super::OnBeginEquip();
	
}

void UCDoAction_Wizard::DoAction()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillActionMode());

	Super::DoAction();
	DoActionDatas[0].DoAction(OwnerCharacter);

}

void UCDoAction_Wizard::Begin_DoAction()
{
	Super::Begin_DoAction();

	ACMagicBall* MagicBall = CreateMagicBall();
	CheckNull(MagicBall);

	MagicBall->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MagicBall->OnHitMagicBall.AddDynamic(this, &UCDoAction_Wizard::OnMagicBallHit);//연결
	MagicBall->OnMagicBallEndPlay.AddDynamic(this, &UCDoAction_Wizard::OnEndPlay);//연결

	FVector forward = OwnerCharacter->GetActorForwardVector();
	MagicBall->Shoot(forward);

}

void UCDoAction_Wizard::End_DoAction()
{
	Super::End_DoAction();
	
}

void UCDoAction_Wizard::OnMagicBallHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitresult;
	ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);

	if (!!enemy)
	{
		FVector Start = InCauser->GetActorLocation();
		FVector End = InOtherCharacter->GetActorLocation();


		UKismetSystemLibrary::LineTraceSingle(InOtherCharacter->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitresult, true);
		enemy->SetCollisionPoint(hitresult.ImpactPoint);
		enemy->SetCollisionPointNormal(hitresult.ImpactNormal.Rotation());
	}

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
}

void UCDoAction_Wizard::OnEndPlay(ACMagicBall* InDestroyActor)
{
	
}
