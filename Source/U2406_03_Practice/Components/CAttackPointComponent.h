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
	//���Ͱ� �ǰݵɋ����� ȣ��Ǵ� �Լ�, Attackpoint����
	void SetAttackPoint(float InAmount);

public:
	/*��Ƽ���̸� ���� ȣ��*/
	void Begin_AwakeMonatge();
	void End_AwakeMontage();
public:	
	FORCEINLINE bool CanAwake() { return (bCanAwake == true); }
	FORCEINLINE bool IsAwake() { return (bAwake == true); }
	FORCEINLINE float GetCurrentAttackPoint() { return AttackPoint; }
	FORCEINLINE float GetMaxCurrentAttackPoint() { return MaxAttackPoint; }

public:
	bool bAwakening = false;//���� ��Ÿ�ֽ��࿩��
private:
	float AttackPoint;	//���� Ÿ������Ʈ
	bool bCanAwake=false;	//�������ɿ���
	bool bAwake = false;	//���� ��������(true�� ���������, false�� �������ƴ�)

	float Value = 0;
};
