// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Characters/CGhostTrail.h"
#include "Components/CapsuleComponent.h"
#include "Characters/CEnemy.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		movement->Stop();
	}

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

	if (!!GhostTrailClass)
	{
		FVector location = InOwner->GetActorLocation();
		location.Z -= InOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FActorSpawnParameters params;
		params.Owner = InOwner;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	//충돌체가있어도 무조건 생성

		FTransform transform;
		transform.SetTranslation(location);

		GhostTrail = InOwner->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, params);
	}
}

void FDoActionData::Destroy_GhostTrail()
{
	if (!!GhostTrail)
		GhostTrail->Destroy();
}
///////////////////////////////////////////////////////////////////////////////
void FSkillActionData::DoSkillAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	bCanSpawnGhostTrail = true;
	/*if (!!movement)
	{
		movement->Stop();
	}*/

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);


	if (!!GhostTrailClass)
	{

		FTimerHandle TimerHandle;
		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda([=]()
		{
			CheckFalse(bCanSpawnGhostTrail);
			Spawn_GhostTrail(InOwner);
		});

		
		InOwner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, SpawnGhostTrailTime, true);

	}
	
}

void FSkillActionData::Spawn_GhostTrail(ACharacter* InOwner)
{
	if (!!GhostTrailClass)
	{
		FVector location = InOwner->GetActorLocation();
		location.Z -= InOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FActorSpawnParameters params;
		params.Owner = InOwner;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	//충돌체가있어도 무조건 생성

		FTransform transform;
		transform.SetTranslation(location);

		GhostTrail = InOwner->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, params);
		GhostTrails.Add(GhostTrail);
	}

}
void FSkillActionData::Destroy_GhostTrail()
{
	CheckFalse(GhostTrails.Num() > 0);
	CheckNull(GhostTrails[GhostTrails.Num()-1]);

	if (GhostTrails.Num() > 0)
	{
		for (int i = GhostTrails.Num() - 1; i >= 0;--i)
		{
			if(!!GhostTrails[i])
				GhostTrails[i]->Destroy();

		}
	}
}
///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;

	ACEnemy* enemy = Cast<ACEnemy>(InOther);

	if (!!enemy)
		enemy->SetbSkillDamaged(false);

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);

	
}

void FHitData::PlayHitStop(class ACharacter* InOwner,class UWorld* InWorld)
{
	//InTime이 0이라면, 굳이 Hitstop을할필요가없다.
	//FMath::IsNearlyZero
	CheckTrue(FMath::IsNearlyZero(StopTime));

	//현재 레벨에서의 character들을 가져온다.
	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);
		if (!!character)
		{
			//멈춰준다.
			character->CustomTimeDilation = 1e-3f;
			characters.Add(character);
		}

	}

	FTimerHandle timerhandle;
	//두번째인자에 넣을 델리게이트변수
	FTimerDelegate timerDelegate;

	//다시 시간되돌려주는 람다식(서서히 시간돌아오도록)
	timerDelegate.BindLambda([=] {

		for (ACharacter* characater : characters)
		{
			
			
			if (!!characater)
				characater->CustomTimeDilation = 1.0f;
			

		}
	});

	FTimerHandle timerhandle2;
	FTimerDelegate timerDelegate2;

	//다시 시간되돌려주는 람다식(서서히 시간돌아오도록)
	timerDelegate2.BindLambda([=] {

		//PlaySoundWave(InOwner);
	});

	//다시되돌려준다.
	InWorld->GetTimerManager().SetTimer(timerhandle, timerDelegate, StopTime, false);
	InWorld->GetTimerManager().SetTimer(timerhandle2, timerDelegate2, StopTime, false);
}

void FHitData::PlaySoundWave(class ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);

	transform.AddToTranslation(InLocation);	//EffectLocation + InLocation

	UGameplayStatics::SpawnEmitterAtLocation(InWorld, Effect, transform);

}
void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation);
	transform.SetRotation(InRotator.Quaternion());
	transform.SetScale3D(EffectScale);


	UGameplayStatics::SpawnEmitterAtLocation(InWorld, Effect, transform);

}

void FHitData::PlayNiagaraEffect(UWorld* InWorld, const FVector& InLocation, const FRotator InRotator)
{
	
	CheckNull(Particle);

	FTransform transform;
	transform.SetLocation(InLocation);
	transform.SetScale3D(EffectScale);

	
	
	UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, Particle, InLocation, InRotator, EffectScale);
	effect->SetWorldRotation(InRotator);

}

/////////////////////////////////////////////////////////////////////////////
void FSkillHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.SkillHitData = this;

	ACEnemy* enemy = Cast<ACEnemy>(InOther);
	
	if(!!enemy)
		enemy->SetbSkillDamaged(true);

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);

	
}

void FSkillHitData::PlayHitStop(class ACharacter* InOwner, class UWorld* InWorld, float DeltaTime)
{

	//InTime이 0이라면, 굳이 Hitstop을할필요가없다.
	//FMath::IsNearlyZero
	CheckTrue(FMath::IsNearlyZero(StopTime));

	//현재 레벨에서의 character들을 가져온다.
	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);
		if (!!character)
		{
			//멈춰준다.
			character->CustomTimeDilation = 1e-3f;
			characters.Add(character);
		}

	}

	FTimerHandle timerhandle;

	//두번째인자에 넣을 델리게이트변수
	FTimerDelegate timerDelegate;

	//다시 시간되돌려주는 람다식(서서히 시간돌아오도록)
	timerDelegate.BindLambda([=] {

		for (ACharacter* characater : characters)
		{


			characater->CustomTimeDilation = FMath::FInterpTo(characater->CustomTimeDilation, 1.0f, DeltaTime, InterpSpeed);


		}
	});


	FTimerHandle timerhandle2;
	FTimerDelegate timerDelegate2;
	timerDelegate2.BindLambda([=] {

		for (ACharacter* characater : characters)
		{


			characater->CustomTimeDilation = 1.0f;
			//PlaySoundWave(InOwner,0.5f);

		}
	});


	//다시되돌려준다.
	InWorld->GetTimerManager().SetTimer(timerhandle, timerDelegate, StopTime, false);
	InWorld->GetTimerManager().SetTimer(timerhandle2, timerDelegate2, ResetTime, false);
}

void FSkillHitData::PlaySoundWave(class ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FSkillHitData::PlaySoundWave(ACharacter* InOwner, float InVolumeMultipler)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location,FRotator::ZeroRotator,InVolumeMultipler);
}

void FSkillHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);

	transform.AddToTranslation(InLocation);	//EffectLocation + InLocation

	UGameplayStatics::SpawnEmitterAtLocation(InWorld, Effect, transform);

}
void FSkillHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation + InRotator.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);


	UGameplayStatics::SpawnEmitterAtLocation(InWorld, Effect, transform);

}

