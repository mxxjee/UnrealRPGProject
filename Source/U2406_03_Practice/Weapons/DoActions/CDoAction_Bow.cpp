// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Weapons/AddOns/CArrow.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Weapons/CEquipment.h"
#include "Components/CStateComponent.h"
#include "Components/CAttackPointComponent.h"
#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"
#include "Sound/SoundCue.h"
#include "Characters/CEnemy_AI_Boss.h"


UCDoAction_Bow::UCDoAction_Bow()
{

}

void UCDoAction_Bow::BeginPlay(ACharacter* InOwner,ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	Super::BeginPlay(InOwner, InAttachment, InEquipment,InDoActionDatas, InHitDatas);

	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	PoseableMesh= CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

	bEquipped = InEquipment->GetbEquipped();
	OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);
	
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!bow)
		Bending = bow->GetAnimInstance_Bending();

	Attack = CHelpers::GetComponent<UCAttackPointComponent>(OwnerCharacter);
}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	//CLog::Log(Arrows.Num());

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	CheckFalse(*bEquipped);
	CheckFalse(bAttachedString);

	//�ٴ���
	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);

}

void UCDoAction_Bow::CreateArrow()
{
	CheckTrue(World->bIsTearingDown);//���ӳ����� ������Ȯ��
	CheckNull(ArrowClass);

	FTransform transform;
	ACArrow* arrow=World->SpawnActorDeferred<ACArrow>(ArrowClass, transform,nullptr,nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	CheckNull(arrow);
	
	arrow->AddIngoreActor(OwnerCharacter);


	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	arrow->SetActorRotation(FQuat(OwnerCharacter->GetMesh()->GetSocketRotation("Hand_Bow_Right_Arrow")));

	Arrows.Add(arrow);
	//Ȯ��(��ġ)

	UGameplayStatics::FinishSpawningActor(arrow, transform);

	CheckNull(Attack);
	if (Attack->IsAwake())
		arrow->SetArrowMaterial(FLinearColor(0.081419f, 0, 1, 1));

	else
		arrow->SetArrowMaterial(FLinearColor(0, 0.6f, 1, 1));
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();
	CreateArrow();
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	//�� ��ġ �ٽõǵ�����
	PoseableMesh->SetBoneLocationByName("bow_string_mid",OriginLocation, EBoneSpaces::ComponentSpace);

	//�����迭�� ������ Ȱ���Ͽ� ����
	for (int32 i = Arrows.Num() - 1;i >= 0;--i)
	{
		ACArrow* arrow = Arrows[i];

		if (!!arrow)
		{
			if (!!arrow->GetAttachParentActor())//Attach�Ǿ������� not null��ȯ
			{
				Arrows.Remove(arrow);//�迭���� ����
				
				arrow->Destroy();//��Ʈ����
			}
		}
	}
}

void UCDoAction_Bow::DoAction()
{
	
	CheckFalse(State->IsIdleMode());
	CheckFalse(State->IsSubActionMode());

	Super::DoAction();

	
	DoActionDatas[0].DoAction(OwnerCharacter);

}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	bAttachedString = false;
	*Bending = 0.0f;

	ACArrow* arrow = GetArrowAttached();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	
	arrow->OnHitArrow.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);//����
	arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnEndPlay);//����

	


	if (Attack->IsAwake())
	{
		FVector forward = UGameplayStatics::GetPlayerCameraManager(OwnerCharacter->GetWorld(), 0)->GetActorForwardVector();
		arrow->ShootAwake(forward);
		return;
	}

	FVector forward = UGameplayStatics::GetPlayerCameraManager(OwnerCharacter->GetWorld(),0)->GetActorForwardVector();
	arrow->Shoot(forward);
	

}
 
void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();
	CreateArrow();
}


void UCDoAction_Bow::AbortAction()
{
	Super::AbortAction();
	OwnerCharacter->bUseControllerRotationYaw = true;
	
}


ACArrow* UCDoAction_Bow::GetArrowAttached()
{
	for (ACArrow* arrow : Arrows)
	{
		if (arrow->GetAttachParentActor())
			return arrow;
	}

	return nullptr;
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);
	
	Arrows.Remove(Cast<ACArrow>(InCauser));

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitresult;
	//Enemy���� �����ߴٸ�//

	ACEnemy* enemy = Cast<ACEnemy>(InOtherCharacter);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

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
	

	UGameplayStatics::PlaySoundAtLocation(InOtherCharacter->GetWorld(), Hitcue, InOtherCharacter->GetActorLocation());
	
	HitDatas[0].SendDamage(OwnerCharacter,InCauser, InOtherCharacter);

}

void UCDoAction_Bow::SetArrowClass()
{
	ACArrow* arrow = GetArrowAttached();

	
	if (!!arrow)
		arrow->SetArrowMaterial(FLinearColor(0.081419f, 0, 1, 1));
}

void UCDoAction_Bow::InitArrowClass()
{
	ACArrow* arrow = GetArrowAttached();

	if (!!arrow)
		arrow->SetArrowMaterial(FLinearColor(0, 0.6f, 1, 1));
}
void UCDoAction_Bow::OnEndPlay(ACArrow* InDestroyActor)
{
	Arrows.Remove(InDestroyActor);
}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100.0f;
	bAttachedString = true;
}