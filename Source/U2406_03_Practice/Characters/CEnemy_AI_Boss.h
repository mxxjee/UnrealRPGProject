// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_Boss.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API ACEnemy_AI_Boss : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACEnemy_AI_Boss();

protected:
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, Category = "LaunchBoss")
		float CinematicGravityScale;

	UPROPERTY(EditAnywhere, Category = "LaunchBoss")
		float CinematicRecoverGravityScale;

	UPROPERTY(EditAnywhere, Category = "LaunchBoss")
		float LaunchPower=500;

protected:
	void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void SetCinematic(bool bInCinematic) { bCinematic = bInCinematic; }
	bool GetCinematic() { return bCinematic; }

	UFUNCTION(BlueprintCallable)
		void SetCinematicJump(bool bInCinematicJump) { bCinematicJump = bInCinematicJump; }
	bool GetCinematicJump() { return bCinematicJump; }

public:
	UFUNCTION(BlueprintCallable)
		void SetUI();

	UFUNCTION(BlueprintCallable)
		void SetEnbaleRunBT();

public:
	/*SkillAction_Spawn에서 호출*/
	UFUNCTION(BlueprintNativeEvent)
		void SetSpawnLocs();
	void SetSpawnLocs_Implementation() {};

	TArray<class ACSpawnLoc*> GetSpawnLocs() { return SpawnLocs; }

public:
	void LaunchBoss();

	UFUNCTION()
		void StopLaunchBoss();

	UFUNCTION(BlueprintNativeEvent)
		void Camerashake();
	void Camerashake_Implementation() {};


public:
	void SetbBeamBegin(bool bInbBeamBegin) { bBeamBegin = bInbBeamBegin; }
	bool GetbBeamBegin() { return bBeamBegin; }

public:
	UFUNCTION(BlueprintNativeEvent)
		void SpawnInvincibleUI();
	virtual void SpawnInvincibleUI_Implementation() {};

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class ACSpawnLoc*> SpawnLocs;

public:
	void UpdateHealthColor_TwoPhase();
	void UpdateHealthColor_ThreePhase();
private:
	//Beam공격
	bool bBeamBegin = false;;
private:
	bool bCinematic;
	bool bCinematicJump;
};
