// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
	
public:
	UCDoAction_Bow();

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Soundcueue")
		class USoundCue* Hitcue;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;
private:
	void CreateArrow();
	class ACArrow* GetArrowAttached();
public:
	void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class  UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	) override;

	virtual void Tick(float InDeltaTime) override;

public:
	void OnBeginEquip() override;
	void OnUnequip() override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void AbortAction() override;
private:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnEndPlay(class ACArrow* InDestroyActor);

public:
	void End_BowString();
private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	void SetArrowClass() override;
	void InitArrowClass() override;
public:
	FORCEINLINE TArray<ACArrow*>& GetArrows() { return Arrows; }
private:
	TArray<ACArrow*> Arrows;


	
private:
	FVector OriginLocation;
private:
	const bool* bEquipped;//equipment¿« bequipped¿˙¿Â

private:
	bool bAttachedString = true;
	float* Bending;

private:
	class UCAttackPointComponent* Attack;
};
