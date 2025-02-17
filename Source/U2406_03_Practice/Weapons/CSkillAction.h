// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CSkillAction.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZSkillPressed);

UENUM(BlueprintType)
enum class ESkillNum : uint8
{
	Z=0,X,C
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Sword_Z_Skill,
	Sword_X_Skill,
	Sword_C_Skill,
	Hammer_Z_Skill,
	Hammer_X,Skill,
	Hammer_C_Skill,
	Bow_Z_Skill,
	Bow_X_Skill,
	Bow_C_Skill,
	Bow_SpecialSkill,
	MAX
};
UCLASS()
class U2406_03_PRACTICE_API UCSkillAction : public UObject
{
	GENERATED_BODY()
	
public:
	/*각 스킬마다의 액션데이터와 데미지 데이터*/

	UPROPERTY(EditAnywhere)
		FSkillActionData SkillActionData;

	UPROPERTY(EditAnywhere)
		FSkillHitData HitData;
	UPROPERTY(EditAnywhere)
		ESkillType SKillType;

public:
	FORCEINLINE bool GetbInSkill() { return bInSkill; }
	FORCEINLINE float GetSkillCoolTime() { return SkillActionData.CoolTime; }
	FORCEINLINE float GetChargePower() { return Power; }

public:
	UCSkillAction();

public:
	FORCEINLINE bool GetInAction() { return bInAction; }
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment);

	virtual void Tick(float InDeltaTime) {};
	
	virtual void DoSkillAction();
	virtual void Begin_DoSkillAction();
	virtual void End_DoSkillAction();


public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() {}

	UFUNCTION()
		virtual void OnAttachmentEndCollision() {}

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) {};

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {};


public:


	virtual void Pressed();
	virtual void Pressing() {};
	virtual void Released();
	virtual void Cancel_Skill();
public:
	virtual void OnBoxTrace() {};
	virtual void SlowTime() {};

protected:
	bool bInSkill;	//pressed시 true가 되고 
	//쿨타임카운트가 시작. false상태일때만 스킬을 사용할 수 있다.

	
private:
	float InitSkillCoolDownTime;

protected:
	bool bBeginAction;
	bool bInAction;

	class ACharacter* Owner;
	class ACAttachment* Attachment;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
	class UCManaPointComponent* ManaPoint;
	class UCAttackPointComponent* AttackPoint;
	class UCWeaponComponent* Weapon;

public:
	UPROPERTY(EditAnywhere, Category = "Option")
		bool bIsChargeSkill;

	FORCEINLINE bool GetIsChargeSkill() { return bIsChargeSkill; }

protected:
	float Power;

};
