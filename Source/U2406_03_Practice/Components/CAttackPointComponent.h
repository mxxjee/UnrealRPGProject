// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttackPointComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackPointChanged, float, InAttackPoint, float, InMaxAttackPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginAwake);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndAwake);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginAwakening);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAwakening, float, Value);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U2406_03_PRACTICE_API UCAttackPointComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "AttackPoint")
		float MaxAttackPoint = 100;

public:	
	UCAttackPointComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

public:
	FOnAttackPointChanged OnAttackPointchanged;
	
	/*BeginAwakening->OnAwakening->BeginAwake*/
	FOnBeginAwakening OnBeginAwakening;
	FOnBeginAwake OnBeginAwaked;
	FOnAwakening OnAwekening;

	
	FOnEndAwake OnEndAwaked;
public:
	void Awake();
	void EndAwake();

public:
	//몬스터가 피격될떄마다 호출되는 함수, Attackpoint세팅
	void SetAttackPoint(float InAmount);

public:
	/*노티파이를 통해 호출*/
	void Begin_AwakeMonatge();
	void End_AwakeMontage();
public:	
	FORCEINLINE bool CanAwake() { return (bCanAwake == true); }
	FORCEINLINE bool IsAwake() { return (bAwake == true); }
	FORCEINLINE float GetCurrentAttackPoint() { return AttackPoint; }
	FORCEINLINE float GetMaxCurrentAttackPoint() { return MaxAttackPoint; }

public:
	bool bAwakening = false;//각성 몽타주실행여부
private:
	float AttackPoint;	//현재 타격포인트
	bool bCanAwake=false;	//각성가능여부
	bool bAwake = false;	//현재 각성여부(true면 각성모드중, false면 각성모드아님)

	float Value = 0;
};
