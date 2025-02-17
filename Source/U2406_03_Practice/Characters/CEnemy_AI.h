// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "GenericTeamAgentInterface.h"
#include "CEnemy_AI.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "HideKill")
		class UAnimMontage* HideKillMontage;


	UPROPERTY(EditAnywhere, Category = "Option")
		bool bMiddleBoss;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
		class UBehaviorTree* BehaviorTree;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float MaxLabelDistance=1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float MaxTargetLabelDistance = 1000.0f;

	UPROPERTY(EditAnyWhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;


public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

public:
	void SetbShowUI(bool InbShowUI) {bShowUI = InbShowUI;}
public:
	ACEnemy_AI();

public:
	void StopMovement();
public:
	void SetbShowTargetUI(bool bInShowTargetUI) { bShowTargetUI = bInShowTargetUI; }
	void ShowTargetUI();
private:
	bool bShowUI;
	bool bShowTargetUI = false;
private:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;
	UPROPERTY(VisibleAnywhere)
		class UCAIBehaviorComponent* Behavior;
protected:
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* LabelWidget;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* TargetWidget;



protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		virtual void OnHealthPointChanged(float InHealth, float InMaxHealth);

private:
	void Tick_LabelREnderScale();//거리에 따른 scale조정


public:
	void StartKillMode() override;
protected:
	UFUNCTION(BlueprintNativeEvent)
		void PlayDamageCameraShake();
	void PlayDamageCameraShake_Implementation() {};
public:
	void OffCollision();
	void OnCollision();

protected:
	void DamagedAirAttack() override;
public:
	void Damaged() override;	//실제 데미지처리 함수
	void End_Damaged() override;
	virtual void End_AirDamaged() override;

	void Dead() override;
};
