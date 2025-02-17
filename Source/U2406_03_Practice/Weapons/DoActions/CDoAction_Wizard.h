// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Wizard.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCDoAction_Wizard : public UCDoAction
{
	GENERATED_BODY()
	
public:
	UCDoAction_Wizard();

private:
	UPROPERTY(EditDefaultsOnly, Category = "MagicBall")
		TSubclassOf<class ACMagicBall> MagicBallClass;
private:
	class ACMagicBall* CreateMagicBall();
	//class * GetArrowAttached();

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
	//Equipment-OnBeginEquip시 호출(델리게이트)
	void OnBeginEquip() override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

private:
	UFUNCTION()
		void OnMagicBallHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnEndPlay(class ACMagicBall* InDestroyActor);


private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;
};
