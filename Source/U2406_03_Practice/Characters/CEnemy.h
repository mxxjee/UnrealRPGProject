// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/IStatable.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACEnemy
	: public ACharacter
	, public IIStatable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		class UMaterialInstance* LockOnMaterial;
private:
	UPROPERTY(EditAnywhere, Category = "Color")
		FLinearColor OriginColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		TArray<class UAnimMontage*> DamagedMontage;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		TArray<class UAnimMontage*> DeadMontage;
protected:
	UPROPERTY(VisibleAnywhere)
		class UCHealthPointComponent* HealthPoint;

private:
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

protected:
	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

public:
	// Sets default values for this character's properties
	ACEnemy();
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime)override;

protected:
	void DamagedNormalAttack();
	void DamagedSkillAttack();
	virtual void DamagedAirAttack();

public:
	void LockOn();
	void LockOff();

private:
	void Change_Color(const FLinearColor& InColor);

public:
	void SetbSkillDamaged(bool InbSkillDamaged) { bSkillDamaged = InbSkillDamaged; }
	void SetbAirSkillDamaged(bool InbAirSkillDamaged) {bAirSkillDamaged = InbAirSkillDamaged;}
	bool GetbAirSkillDamaged() {return bAirSkillDamaged;}

public:
	void SetCollisionPoint(FVector InVector) { CollisionPoint = InVector; }
	void SetCollisionPointNormal(FRotator InRotator) { CollisionRotator = InRotator; }
private:
	UFUNCTION()
		void RestoreColor();

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:
	virtual void Damaged();	//실제 데미지처리 함수
	virtual void End_Damaged() override;
	virtual void End_AirDamaged() override;
	virtual void End_Parried() override;

public:
	virtual void Dead();
	void End_Dead() override;


protected:
	UFUNCTION(BlueprintNativeEvent)
		void SpawnDamageAmountUI(float Damage);
	virtual void SpawnDamageAmountUI_Implementation(float Damage) {};

	UFUNCTION(BlueprintNativeEvent)
		void SpawnCriticalDamageAmountUI(float Damage);
	virtual void SpawnCriticalDamageAmountUI_Implementation(float Damage) {};

protected:
	//SetTimer에 넣을 변수
	FTimerHandle ChangeColor_TimerHandle;

	float Enemy_DeltaTime;

	bool bSkillDamaged;	//true일경우 skill공격으로 피해받음, false일 경우 일반공격으로 피해받음.

	bool bAirSkillDamaged;	//true일경우 공중콤보 skill공격으로 피해받음.

	FVector CollisionPoint;//슬래쉬Effect spawn위치
	FRotator CollisionRotator;

private:
	bool bParry;	//Parry당했으면 true

public:
	FORCEINLINE bool getbParry() { return bParry; }
	void SetbParry(bool InbParry) { bParry = InbParry; }
private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
		bool bCritical;
	} DamageData;
};
