// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Components/CapsuleComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CAttackPointComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CWeaponStructures.h"

#include "Animation/AnimMontage.h"
#include "Sound/SoundWave.h"
#include "Particles/ParticleSystem.h"
#include "Weapons/CSkillAction.h"

#include "Characters/CPlayer.h"

// Sets default values
ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "HealthPoint");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	//�ִ�Ŭ���� ����
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character_Sword.ABP_Character_Sword_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);



}

// Called when the game starts or when spawned
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	//�����ϸ� �Ȱ�����.
	GetCharacterMovement()->MaxWalkSpeed = 200;

	//���̳��� instacne ���׸���
	for (int i = 0;i < GetMesh()->GetMaterials().Num();i++)
	{
		UMaterialInterface* material = GetMesh()->GetMaterials()[i];
		GetMesh()->SetMaterial(i, UMaterialInstanceDynamic::Create(material, this));


	}

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
	Change_Color(OriginColor);
}

void ACEnemy::Tick(float DeltaTime)
{
	Enemy_DeltaTime = DeltaTime;


}

void ACEnemy::Change_Color(const FLinearColor& InColor)
{
	//���̳��� ���׸���
	for (UMaterialInterface* material : GetMesh()->GetMaterials())
	{
		UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic>(material);

		if (!!instance)
			instance->SetVectorParameterValue("Color", InColor);


	}
}


float ACEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//�θ𿡼� �����������ϼ��������Ƿ� ����
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	int RandomCritical = FMath::RandRange(0, 4);
	if (RandomCritical == 4)
		DamageData.bCritical = true;
	else
		DamageData.bCritical = false;


	int NewDamage = FMath::RandRange(Damage, Damage + 6);

	if (DamageData.bCritical)
		NewDamage += 5;

	ACPlayer* player = Cast<ACPlayer>(Cast<ACharacter>(EventInstigator->GetPawn()));
	if (!!player)
	{
		if (player->IsAwakeMode())
		{
			CLog::Log("Awake+Damage");
			NewDamage += FMath::RandRange(5, 10);
		}
			
	}

	DamageData.Power = NewDamage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	//25%�� ũ��Ƽ��
	
	if (State->IsDeadMode())
		return Damage;

	State->SetDamagedMode();

	


	return Damage;
}

void ACEnemy::RestoreColor()
{
	Change_Color(OriginColor);
}



void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Damaged:
			Damaged();
			break;

		case EStateType::Dead:
			Dead();
			break;
	}
}

void ACEnemy::Damaged()
{
	
	//Apply Damage
	HealthPoint->Damage(DamageData.Power);

	if (DamageData.bCritical == false)
		SpawnDamageAmountUI(DamageData.Power+230);

	else
		SpawnCriticalDamageAmountUI(DamageData.Power+ 330);


	/*Attacker�� ��������Ʈ����*/
	ACPlayer* player = Cast<ACPlayer>(DamageData.Attacker);


	if (!!player)
	{
		/*���� ��ų�� �ƴҶ���*/
		if (player->IsAwakeMode()==false)
		{
			CLog::Log("NoAwake+Damage");
			if (player->GetCurrentSKillType() != ESkillType::Sword_X_Skill)
			{
				float Half = DamageData.Power * 0.5f;
				float AP = FMath::Clamp(Half, 0.0f, 50.0f);


				player->TakeAttackPoint(AP);
				DamageData.Power = 0.0f;	//��������Ƿ� �ʱ�ȭ
			}
		}
		
		
	}

	

	{
		Change_Color(FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");
		//�ٽõǵ�����
		GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle, timerDelegate, 0.2f, false);
	}

	//�Ϲݰ������� ���ع޾������

	if (bSkillDamaged == false)
	{
		DamagedNormalAttack();

	}
	
	//��ų�������� ���ع޾��� ���
	else if(bSkillDamaged==true)
	{
		//��ų���� �� air�����̶��
		if (bAirSkillDamaged==true)
			DamagedAirAttack();
		else
			DamagedSkillAttack();
	}


	
	CheckNull(DamageData.Attacker);
	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;

	DamageData.Event->HitData = nullptr;
	DamageData.Event->SkillHitData = nullptr;

	DamageData.Event = nullptr;
	

	bSkillDamaged = false;
	
	if (!bAirSkillDamaged)
	{
		if (HealthPoint->IsDead())
		{

			State->SetDeadMode();
			return;
		}
	}
	

	

	
	
	
}


void ACEnemy::DamagedNormalAttack()
{
	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* hitData = DamageData.Event->HitData;

		UAnimMontage* montage = nullptr;//hitData->Montage;
		montage = hitData->Montage;
		float playRate = hitData->PlayRate;


		if (montage == nullptr)
		{
			int RandomIdx = FMath::RandRange(0, DamagedMontage.Num() - 1);
			montage = DamagedMontage[RandomIdx];
			playRate = 1.5f;
		}

		PlayAnimMontage(montage, playRate);

		
		//hitData->PlayNiagaraEffect(GetWorld(), CollisionPoint, CollisionRotator);
		hitData->PlayEffect(GetWorld(), CollisionPoint, CollisionRotator);
		hitData->PlayHitStop(this, GetWorld());		//��Ʈ��ž
		hitData->PlaySoundWave(this);	//�÷��̻���

		
		

		//����������� ó��
		if (HealthPoint->IsDead() == false)
		{
			CheckTrue((hitData->Launch == 0));
			FVector start = GetActorLocation();
			//eventInstagtor->�÷��̾��� controller��ȯ
			FVector target = DamageData.Attacker->GetActorLocation();	//�÷��̾�

			FVector direction = target - start;
			direction.Normalize();

			//�� �ݴ�� �о���ϹǷ� -����
			LaunchCharacter(-direction * hitData->Launch, false, false);


			FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);


			lookAtRotation.Pitch = 0.0f;
			lookAtRotation.Roll = 0.0f; // Roll�� �ʿ����� �����Ƿ� 0���� ����
			//�ٶ󺸱�
			SetActorRotation(lookAtRotation);
		}
	}
}

void ACEnemy::DamagedSkillAttack()
{
	if (!!DamageData.Event && !!DamageData.Event->SkillHitData)
	{
		FSkillHitData* hitData = DamageData.Event->SkillHitData;

		UAnimMontage* montage = hitData->Montage;
		float playRate = hitData->PlayRate;

		if (montage == nullptr)
		{
			int RandomIdx = FMath::RandRange(0, DamagedMontage.Num() - 1);
			montage = DamagedMontage[RandomIdx];
			playRate = 1.5f;
		}

		PlayAnimMontage(montage, playRate);


		hitData->PlayHitStop(this, GetWorld(), Enemy_DeltaTime);		//��Ʈ��ž
		hitData->PlayEffect(GetWorld(), CollisionPoint, CollisionRotator);
		hitData->PlaySoundWave(this);

		//����������� ó��
		if (HealthPoint->IsDead() == false)
		{
			CheckTrue((hitData->Launch == 0));
			FVector start = GetActorLocation();
			//eventInstagtor->�÷��̾��� controller��ȯ
			FVector target = DamageData.Attacker->GetActorLocation();	//�÷��̾�

			FVector direction = target - start;
			direction.Normalize();

			
			//�� �ݴ�� �о���ϹǷ� -����
			LaunchCharacter(-direction * hitData->Launch, false, false);

			FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

			
			lookAtRotation.Pitch = 0.0f;
			lookAtRotation.Roll = 0.0f; // Roll�� �ʿ����� �����Ƿ� 0���� ����
			//�ٶ󺸱�
			SetActorRotation(lookAtRotation);


		}
	}
}

void ACEnemy::DamagedAirAttack()
{
	
	if (!!DamageData.Event && !!DamageData.Event->SkillHitData)
	{
		FSkillHitData* hitData = DamageData.Event->SkillHitData;

		hitData->PlayHitStop(this, GetWorld(), Enemy_DeltaTime);
		hitData->PlaySoundWave(this);
		hitData->PlayEffect(GetWorld(), CollisionPoint, CollisionRotator);

		
	}
}

void ACEnemy::End_Damaged()
{
	State->SetIdleMode();

	
}


void ACEnemy::End_AirDamaged()
{
	
		

	CheckTrue(HealthPoint->IsDead());

	State->SetIdleMode();
	if (bAirSkillDamaged)
		bAirSkillDamaged = false;



}

void ACEnemy::End_Parried()
{


	CheckTrue(HealthPoint->IsDead());
	if (bParry)
		bParry = false;


}


void ACEnemy::LockOn()
{
	CheckNull(LockOnMaterial);
	GetMesh()->SetOverlayMaterial(LockOnMaterial);
}

void ACEnemy::LockOff()
{
	GetMesh()->SetOverlayMaterial(nullptr);
}

void ACEnemy::Dead()
{
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!bAirSkillDamaged)
	{
		int RandomIdx = FMath::RandRange(0, DeadMontage.Num()-1);
		PlayAnimMontage(DeadMontage[RandomIdx]);
	}
}

void ACEnemy::End_Dead()
{
	Destroy();
}