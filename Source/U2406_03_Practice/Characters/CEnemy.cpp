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

	//애님클래스 설정
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character_Sword.ABP_Character_Sword_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);



}

// Called when the game starts or when spawned
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	//시작하면 걷게하자.
	GetCharacterMovement()->MaxWalkSpeed = 200;

	//다이나믹 instacne 메테리얼
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
	//다이나믹 메테리얼
	for (UMaterialInterface* material : GetMesh()->GetMaterials())
	{
		UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic>(material);

		if (!!instance)
			instance->SetVectorParameterValue("Color", InColor);


	}
}


float ACEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//부모에서 데미지가깎일수도있으므로 저장
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

	//25%로 크리티컬
	
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


	/*Attacker의 어택포인트세팅*/
	ACPlayer* player = Cast<ACPlayer>(DamageData.Attacker);


	if (!!player)
	{
		/*광역 스킬이 아닐때만*/
		if (player->IsAwakeMode()==false)
		{
			CLog::Log("NoAwake+Damage");
			if (player->GetCurrentSKillType() != ESkillType::Sword_X_Skill)
			{
				float Half = DamageData.Power * 0.5f;
				float AP = FMath::Clamp(Half, 0.0f, 50.0f);


				player->TakeAttackPoint(AP);
				DamageData.Power = 0.0f;	//사용했으므로 초기화
			}
		}
		
		
	}

	

	{
		Change_Color(FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");
		//다시되돌리기
		GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle, timerDelegate, 0.2f, false);
	}

	//일반공격으로 피해받았을경우

	if (bSkillDamaged == false)
	{
		DamagedNormalAttack();

	}
	
	//스킬공격으로 피해받았을 경우
	else if(bSkillDamaged==true)
	{
		//스킬공격 중 air공격이라면
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
		hitData->PlayHitStop(this, GetWorld());		//히트스탑
		hitData->PlaySoundWave(this);	//플레이사운드

		
		

		//살아있을때만 처리
		if (HealthPoint->IsDead() == false)
		{
			CheckTrue((hitData->Launch == 0));
			FVector start = GetActorLocation();
			//eventInstagtor->플레이어의 controller반환
			FVector target = DamageData.Attacker->GetActorLocation();	//플레이어

			FVector direction = target - start;
			direction.Normalize();

			//그 반대로 밀어야하므로 -곱함
			LaunchCharacter(-direction * hitData->Launch, false, false);


			FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);


			lookAtRotation.Pitch = 0.0f;
			lookAtRotation.Roll = 0.0f; // Roll은 필요하지 않으므로 0으로 고정
			//바라보기
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


		hitData->PlayHitStop(this, GetWorld(), Enemy_DeltaTime);		//히트스탑
		hitData->PlayEffect(GetWorld(), CollisionPoint, CollisionRotator);
		hitData->PlaySoundWave(this);

		//살아있을때만 처리
		if (HealthPoint->IsDead() == false)
		{
			CheckTrue((hitData->Launch == 0));
			FVector start = GetActorLocation();
			//eventInstagtor->플레이어의 controller반환
			FVector target = DamageData.Attacker->GetActorLocation();	//플레이어

			FVector direction = target - start;
			direction.Normalize();

			
			//그 반대로 밀어야하므로 -곱함
			LaunchCharacter(-direction * hitData->Launch, false, false);

			FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

			
			lookAtRotation.Pitch = 0.0f;
			lookAtRotation.Roll = 0.0f; // Roll은 필요하지 않으므로 0으로 고정
			//바라보기
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