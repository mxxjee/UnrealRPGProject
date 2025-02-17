#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponComponent.generated.h"



UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist,Sword,Hammer,Warp,Bow,Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS()
class U2406_03_PRACTICE_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

	UPROPERTY(EditAnywhere, Category = "Settings")
		float RInterpTime = 5.0f;


	UPROPERTY(EditAnywhere, Category = "Settings")
		float FinishAngle = 2.0f;
public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE EWeaponType GetType() { return Type; }
public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCSubAction* GetSubAction();
	class UCSkillAction* GetSkillAction(int InSkillNum);
	int GetSkillActionsNum();

	TArray<FDoActionData> GetDoActionDatas();
	TArray<FHitData> GetHitDatas();

	bool GetSkillActionIsChargetAttack(int InSkillNum);
private:
	bool IsIdleMode();
	


public:	

	UCWeaponComponent();

protected:

	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetFistMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetWarpMode();
	void SetBowMode();

	void SetUnarmedMode();
	void DoAction();

	bool IsSwordMode();
	bool IsHammerMode();
	bool IsBowMode();
private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);
	
public:
	void SubAction_Pressed();
	void SubAction_Released();

public:
	void Skill_Pressed(int InSkillNum);
	void Skill_Repeat(int InSkillNum);
	void Skill_Released(int InSkillNum);
	void Skill_Canceled(int InSkillNum);


public:
	FWeaponChanged OnWeaponChanged;
private:
	EWeaponType Type = EWeaponType::Max;
		
private:
	class ACharacter* OwnerCharacter;

private:
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::Max];
};
