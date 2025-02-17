// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/Sword/CSkillAction_Sword_Parry.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"

#include "Weapons/CAttachment.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UCSkillAction_Sword_Parry::UCSkillAction_Sword_Parry()
{
	
}

void UCSkillAction_Sword_Parry::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);


}


void UCSkillAction_Sword_Parry::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

}

void UCSkillAction_Sword_Parry::DoSkillAction()
{
	Super::DoSkillAction();
	CheckNull(Owner);
	
	ignores.Add(Owner);
	ignores.Add(Attachment);

}

void UCSkillAction_Sword_Parry::OnBoxTrace()
{
	Super::OnBoxTrace();
	
	CheckTrue(bBlocking);
	AActor* attachment = Cast<AActor>(Attachment);
	USkeletalMeshComponent* mesh = CHelpers::GetComponent<USkeletalMeshComponent>(attachment);

	FVector start = mesh->GetSocketLocation("Start_Guard");
	FVector end = mesh->GetSocketLocation("end");


	UKismetSystemLibrary::BoxTraceSingle(Owner->GetWorld(), start, end, FVector(extentX, extentY, extentZ), attachment->GetActorRotation(), ETraceTypeQuery::TraceTypeQuery5, false, ignores, EDrawDebugTrace::None, hitResult, true);
	if (hitResult.bBlockingHit)
	{
		CLog::Log(hitResult.GetActor()->GetName());

		ACAttachment* enemy_Sword = Cast<ACAttachment>(hitResult.GetActor());
		CheckNull(enemy_Sword);
		CheckNull(enemy_Sword->GetOwner());
		
		ACEnemy_AI* enemy = Cast<ACEnemy_AI>(enemy_Sword->GetOwner());
		CheckNull(enemy);


		enemy_Sword->OffCollisions();
		enemyweapon = enemy_Sword;

		bBlocking = true;

		/*enemy 밀기*/
		FVector start_enemy = enemy->GetActorLocation();
		FVector target = Owner->GetActorLocation();	//플레이어

		FVector direction = target - start_enemy;
		direction.Normalize();

		//그 반대로 밀어야하므로 -곱함
		enemy->LaunchCharacter(-direction * HitData.Launch, false, false);
		enemy->SetbParry(true);

		//바라보기
		enemy->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start_enemy, target));
		enemy->PlayAnimMontage(HitData.Montage, HitData.PlayRate);


		UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), Particle, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation(), EffectScale);
		UNiagaraComponent* effect_Slash = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), Particle_Slash, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation(), EffectScale);
		
		HitData.PlaySoundWave(Owner);
		ACPlayer* player = Cast<ACPlayer>(Owner);
		CheckNull(player);

	

		player->OffCollision_Sword();

		FTimerHandle timerhandle;
		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda([=] {

			for (ACharacter* characater : characters)
			{

				characater->CustomTimeDilation = 1.f;
			}
		});

		Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, 1.f, false);
	}
}

void UCSkillAction_Sword_Parry::SlowTime()
{

	for (AActor* actor : Owner->GetWorld()->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);
		if (!!character)
		{
			//멈춰준다.
			character->CustomTimeDilation = TimeDilation;
			characters.Add(character);
		}

	}

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	//다시 시간되돌려주는 람다식(서서히 시간돌아오도록)
	timerDelegate.BindLambda([=] {

		for (ACharacter* characater : characters)
		{

			characater->CustomTimeDilation = 1.f;
		}
	});

	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, ReturnTime, false);
}

void UCSkillAction_Sword_Parry::End_DoSkillAction()
{
	Super::End_DoSkillAction();
	
	CheckNull(enemyweapon);
	
	bBlocking = false;
	enemyweapon->OnCollisions();

	enemyweapon = nullptr;
}

void UCSkillAction_Sword_Parry::SetStopSpawnGhostTrail()
{

	SkillActionData.SetCanSpawnGhostTrail(false);


}