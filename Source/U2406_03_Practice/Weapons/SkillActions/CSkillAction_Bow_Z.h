// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "Components/TimelineComponent.h"
#include "Weapons/SubActions/CSubAction_Bow.h"
#include "CSkillAction_Bow_Z.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Bow_Z : public UCSkillAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		class APostProcessVolume* PostProcessVolume;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;

	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* ShootMontage;


	UPROPERTY(EditDefaultsOnly, Category = "Soundcueue")
		class USoundCue* PulledCue;

private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;
	
	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		float PlayRate = 200;
	
public:
	UCSkillAction_Bow_Z();


private:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;
	void Tick(float InDeltaTime) override;
private:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

	void Pressing() override;
	void Released() override;

private:
	UFUNCTION()
		void OnAiming(FVector Output);

private:
	void CreateArrow();
	class ACArrow* GetArrowAttached();
private:
	void StartZoom();
	void ReleaseZoom();

private:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnEndPlay(class ACArrow* InDestroyActor);

protected:
	UFUNCTION(BlueprintNativeEvent)
		void OnVignetting(APostProcessVolume* InPostProcess, float Z);
	void OnVignetting_Implementation(APostProcessVolume* InPostProcess, float Z) {};
private:
	class ACAttachment_Bow* Bow;
	bool bIsChargeEnd;

	float AddPower;
	float AddVelocity;
	int OverlapCount;

	bool bDoChargeAnim;

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	FTimeline Timeline;
	FAimData OriginData;
private:
	float* Bending;

	class UCAttackPointComponent* Attack;
	
private:
	class ACPlayer* player;
};
