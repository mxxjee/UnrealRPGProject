// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"
UENUM()
enum class EStateType : uint8
{
	Idle=0,//아무것도아닌상태
	Evade,//회피
	Equip,//장착
	Damaged,//피격
	Action,//행동(기본공격)
	Ride,	//탑승
	Dead,//죽음
	Max
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class U2406_03_PRACTICE_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsDamagedMode() { return Type == EStateType::Damaged; }
	FORCEINLINE bool IsRideMode() { return Type == EStateType::Ride; }
	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }
	FORCEINLINE bool IsSkillActionMode() { return bInSkillActionMode; }
	FORCEINLINE bool IsEvadeMode() { return Type==EStateType::Evade; }

public:	
	// Sets default values for this component's properties
	UCStateComponent();

private:
	void ChangeType(EStateType InType);

public:
	void SetIdleMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetActionMode();
	void SetDamagedMode();
	void SetRideMode();
	void SetDeadMode();



	void OnSubActionMode();
	void OffSubActionMode();

	void OnSkillActionMode();
	void OffSkillActionMode();
public:
	//타입이 바뀔때마다 델리게이트에 연결된 함수를 실행하도록
	//델리게이트변수
	FStateTypeChanged OnStateTypeChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	EStateType Type;
		
private:
	bool bInSubActionMode;
	bool bInSkillActionMode;
};
