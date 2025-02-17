// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Bow_X.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Bow_X : public UCSkillAction
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, Category = "sound")
		class USoundWave* Icesound;

	UPROPERTY(EditAnywhere, Category = "sound")
		class USoundWave* EarthqaukeSound;

	UPROPERTY(EditAnywhere, Category = "Camera")
		FRotator MoveRot = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector MovePos = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		float AddSpeed;


	UPROPERTY(EditDefaultsOnly, Category = "IceTrap")
		TSubclassOf<class ACIceTrap> IceTrapClass;

	UPROPERTY(EditAnywhere)
		float LaunchGravityScale = 0.7f;

	UPROPERTY(EditAnywhere)
		float LaunchSpeed;

	UPROPERTY(EditAnywhere)
		float RecoverGravityScale;

private:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;
	void DoSkillAction() override;


	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

public:
	void LaunchPlayer();

public:
	//연속 생성과 shoot을 위함
	void CreateArrowAndShot();
private:
	void CreateArrow();
	class ACArrow* GetArrowAttached();

private:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);


	UFUNCTION()
		void OnEndPlay(class ACArrow* InDestroyActor);

	UFUNCTION()
		void OnHitFloorArrow(class AActor* InCauser);
public:
	void StopLaunchPlayer();
	class UCameraComponent* Camera;
	class ACPlayer* player;

	FVector InitCameraPos;
	FRotator InitCameraRot;

	bool bPlaySoundEffect=true;

	class UCAttackPointComponent* Attack;
};
