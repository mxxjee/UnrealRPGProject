// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Bow_C.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Bow_C : public UCSkillAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> SkillArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> NormalArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		float AddSpeed;


private:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;
private:
	void DoSkillAction() override;
	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;


private:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnArrowFloorHit(class AActor* InCauser, class ACharacter* InOtherCharacter);
	
	UFUNCTION()
		void OnEndPlay(class ACArrow* InDestroyActor);

private:
	class ACPlayer* player;

public:
	//연속 생성과 shoot을 위함
	void CreateArrowAndShot();

private:
	void CreateArrow();
	void CreateNormalArrow();
	class ACArrow* GetArrowAttached();

private:
	class UCAttackPointComponent* Attack;
};
