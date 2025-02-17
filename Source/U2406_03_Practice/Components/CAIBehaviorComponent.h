#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CWeaponComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid,Block,Ready, Beam,Dead,Max,
};

UENUM(BlueprintType)
enum class EAIHitType : uint8
{
	Normal=0, Air, Max
};


UENUM(BlueprintType)
enum class EAIAttackType : uint8
{
	Attack,Guard, Max
};

UENUM(BlueprintType)
enum class EPhase : uint8
{
	OnePhase,
	TwoPhase,
	ThreePhase,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U2406_03_PRACTICE_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName PatrolLocationKey = "PatrolLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName EWeaponTypeKey = "WeaponType";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName EqsLocationKey = "EqsLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName HitTypeKey = "HitType";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName AttackTypeKey = "AttackType";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName CurrentPhaseKey = "CurrentPhase";

public:	
	UCAIBehaviorComponent();

private:
	EAIStateType GetType();

protected:
	virtual void BeginPlay() override;

public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsBlockMode();
	bool IsDeadMode();

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }


public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();
	void SetBlockMode();
	void SetReadyMode();
	void SetBeamMode();
	void SetWeaponType(EWeaponType InType);

	void SetHitTypeNormalMode();
	void SetHitTypeAirMode();

public:
	void SetOnePhase();
	void SetTwoPhase();
	void SetThreePhase();

public:
	EPhase CurrentPhaseType();
public:
	class ACharacter* GetTarget();

	//블랙보드에 값 설정/블랙보드 키 값가져오기
	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);

	FVector GetEqsLocation();
	void SetEqsLocation(const FVector& InLocation);


	EWeaponType GetWeaponType();

	void SetAttackType(EAIAttackType InType);
private:
	void ChangeType(EAIStateType InType);

public:
	FAIStateTypeChanged OnAIStateTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
};
