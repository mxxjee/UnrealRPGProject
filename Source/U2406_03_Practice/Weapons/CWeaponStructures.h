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
		class UAnimMontage* Montage;//���� ��Ÿ��
	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;	//���� �� �̵����ɿ���

	UPROPERTY(EditAnywhere)
		bool bUsedControlRotation = true;//ȸ���������� �̵�����

	UPROPERTY(EditAnywhere)
		class UAnimMontage* UnEquipMontage;//���� ��Ÿ��
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;//���� ��Ÿ��
	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;	//���� �� �̵����ɿ���

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;	//���� �� ī�޶����

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
		class UAnimMontage* Montage;//����
	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;	//���� �� �̵����ɿ���

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;	//���� �� ī�޶����

	UPROPERTY(EditAnywhere)
		float CoolTime;	//��Ÿ��

	UPROPERTY(EditAnywhere)
		float Mana;		//�ҿ��� ��������Ʈ

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
		float Power;//������

	UPROPERTY(EditAnywhere)
		float Launch = 100;	//�и� ���� 100=1m

	UPROPERTY(EditAnywhere)
		float StopTime;	//hitstop �ð�

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
		float Power;//������

	UPROPERTY(EditAnywhere)
		float Launch = 100;	//�и� ���� 100=1m

	UPROPERTY(EditAnywhere)
		float StopTime;	//hitstop �ð�

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
