// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"


USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;//장착 몽타주
	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;	//장착 중 이동가능여부

	UPROPERTY(EditAnywhere)
		bool bUsedControlRotation = true;//회전방향으로 이동여부

	UPROPERTY(EditAnywhere)
		class UAnimMontage* UnEquipMontage;//장착 몽타주
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;//장착 몽타주
	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;	//공격 중 이동가능여부

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;	//공격 중 카메라고정

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

private:
	class ACGhostTrail* GhostTrail;
public:
	void DoAction(class ACharacter* InOwner);
	void Destroy_GhostTrail();
	
};

USTRUCT()
struct FSkillActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;//실행
	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;	//공격 중 이동가능여부

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;	//공격 중 카메라고정

	UPROPERTY(EditAnywhere)
		float CoolTime;	//쿨타임

	UPROPERTY(EditAnywhere)
		float Mana;		//소요할 마나포인트

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

	UPROPERTY(EditAnywhere)
		float SpawnGhostTrailTime;

private:
	class ACGhostTrail* GhostTrail;
	class TArray<ACGhostTrail*> GhostTrails;

	bool bCanSpawnGhostTrail=true;
public:
	void SetCanSpawnGhostTrail(bool InbCanSpawn) { bCanSpawnGhostTrail = InbCanSpawn; }
	void DoSkillAction(class ACharacter* InOwner);
	void Spawn_GhostTrail(ACharacter* InOwner);
	void Destroy_GhostTrail();
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		float Power;//데미지

	UPROPERTY(EditAnywhere)
		float Launch = 100;	//밀릴 정도 100=1m

	UPROPERTY(EditAnywhere)
		float StopTime;	//hitstop 시간

	UPROPERTY(EditAnywhere)
		class USoundWave* Sound;
	
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;


	UPROPERTY(EditAnywhere, Category = "Particle")
		class UNiagaraSystem* Particle;

public:
	void SendDamage(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOther);
	void PlayHitStop(ACharacter* InOwner,class UWorld* InWorld);
	void PlaySoundWave(ACharacter* InOwner);

	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);
	void PlayNiagaraEffect(UWorld* InWorld, const FVector& InLocation, const FRotator InRotator);
};

USTRUCT()
struct FSkillHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		float Power;//데미지

	UPROPERTY(EditAnywhere)
		float Launch = 100;	//밀릴 정도 100=1m

	UPROPERTY(EditAnywhere)
		float StopTime;	//hitstop 시간

	UPROPERTY(EditAnywhere)
		class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere)
		float InterpSpeed;
	UPROPERTY(EditAnywhere)
		float ResetTime;
public:
	void SendDamage(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOther);
	void PlayHitStop(ACharacter* InOwner, class UWorld* InWorld, float DeltaTime);
	void PlaySoundWave(ACharacter* InOwner);
	void PlaySoundWave(ACharacter* InOwner,float InVolumeMultipler);

	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);
};


USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData;
	FSkillHitData* SkillHitData;
};



UCLASS()
class U2406_03_PRACTICE_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
