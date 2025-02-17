

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"


UCLASS()
class U2406_03_PRACTICE_API UCDoAction : public UObject
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool GetInAction() { return bInAction; }
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
public:
	UCDoAction();

	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	);

	virtual void Tick(float InDeltaTime) {};
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();
	virtual void AbortAction();

public:
	UFUNCTION()
		virtual void OnBeginEquip() {};
	UFUNCTION()
		virtual void OnUnequip() {};
public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() {}

	UFUNCTION()
		virtual void OnAttachmentEndCollision() {}

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult,UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) {};

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {};


public:
	void SetDatas(const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas);

	void InitDatas();

	virtual void SetArrowClass() {};
	virtual void InitArrowClass() {};
protected:
	class ACharacter* OwnerCharacter;
	class UWorld* World;
	
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;

protected:
	TArray<FDoActionData> InitDoActionDatas;
	TArray<FHitData> InitHitDatas;
	bool bBeginAction;
	bool bInAction;
};
