// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"//우리가 정의한 컴포넌트
#include "Characters/IStatable.h"
#include "Components/CWeaponComponent.h"
#include "Components/CParkourComponent.h"
#include "Weapons/CSkillAction.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

USTRUCT(BlueprintType)
struct FAirDashCameraData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float AirDashAxisValue = 300.f;

	UPROPERTY(EditAnywhere)
		float AirDashInterpTime = 10.f;

	UPROPERTY(EditAnywhere)
		float FarFOV;
};

UCLASS()
class U2406_03_PRACTICE_API ACPlayer 
	: public ACharacter,
	public IIStatable,
	public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 1;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ride")
		TSubclassOf<class ACHorse> HorseClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ride")
		class UAnimMontage* HorseCallingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Ride")
		float VisibleTime;
public:
	class ACHorse* HorseToRide;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Block")
		class UMaterialInstance* BlockMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Awake")
		class UMaterialInstance* AwakeMaterial;

	UPROPERTY()
		UMaterialInstanceDynamic* AwakeMaterialDyamic;

	UPROPERTY(EditDefaultsOnly, Category = "Awake")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;
	class ACGhostTrail* GhostTrail;

	UPROPERTY(EditDefaultsOnly, Category = "Awake")
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditDefaultsOnly, Category = "Awake")
		TArray<FHitData> HitDatas;

	UPROPERTY(EditDefaultsOnly, Category = "Awake")
		TArray<FDoActionData> DoActionBowDatas;

	UPROPERTY(EditDefaultsOnly, Category = "Awake")
		TArray<FHitData> HitBowDatas;

	UPROPERTY(EditDefaultsOnly, Category = "Awake")
		class UMaterialInstance* PostProcessMaterial;

	UPROPERTY()
		UMaterialInstanceDynamic* PostProcessMaterialDyamic;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Awake")
		class APostProcessVolume* PostProcessVolume;

public:
	FORCEINLINE APostProcessVolume* GetWorldPostProcessVolume() {return PostProcessVolume;}
	void SetBlockSuccess();
public:
	//Awake 몽타주가 끝나면 호출되는함수
	UFUNCTION()
		void SetAwakeDatas();

	UFUNCTION()
		void SetPostProcessVolumeMaterial();

	UFUNCTION(BlueprintNativeEvent)
		void AwakeDashLine();
	void AwakeDashLine_Implementation() {};

	//Awake몽타주 시작되면 호출되는 함수
	UFUNCTION()
		void SetAwakeMaterial();

	//Awakening 중일때 호출되는함수
	UFUNCTION()
		void SetAwakeMaterialParam(float Value);
	

	//각성상태가 끝나면 호출되는함수.
	UFUNCTION()
		void InitAwakeDatas();
	UFUNCTION()
		void InitAwakeMaterial();

	UFUNCTION(BlueprintNativeEvent)
		void StopDashLine();
	void StopDashLine_Implementation() {};

	UFUNCTION()
		void InitPostProcessVolumeMaterial();

	UFUNCTION()
		void SpawnGhostTrail();

private:

	UPROPERTY(EditAnywhere, Category = "UserInterface")
		TSubclassOf<class UCUserWidget_Player> Uiclass;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		class UAnimMontage* BackStepMontage;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		class UAnimMontage* AwakeMontage;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		class UAnimMontage* EvadeLMontage;



	UPROPERTY(EditAnywhere, Category = "CurrentSkill")
		ESkillType CurrentSkillType= ESkillType::MAX;
private:
	//씬컴포넌트 
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

private:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;
	//액터컴포넌트
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCTargetComponent* Target;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCParkourComponent* Parkour;

public:/*체력,타격,MP*/
	UPROPERTY(VisibleAnywhere)
		class UCHealthPointComponent* HealthPoint;

	UPROPERTY(VisibleAnywhere)
		class UCAttackPointComponent* AttackPoint;

	UPROPERTY(VisibleAnywhere)
		class UCManaPointComponent* ManaPoint;
public:
	// Sets default values for this character's properties
	ACPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }
public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UFUNCTION(BlueprintNativeEvent)
		void PlayCameraAnimation();
	virtual void PlayCameraAnimation_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void AircomboPlayCameraAnimation();
	virtual void AircomboPlayCameraAnimation_Implementation() {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ExplosionPostProcessSetting();
	void ExplosionPostProcessSetting_Implementation() {};

public:
	bool IsAwakeMode();
public:
	void SetCurrentSkillType(ESkillType InSkillTpye) { CurrentSkillType = InSkillTpye; }
	FORCEINLINE ESkillType GetCurrentSKillType() { return CurrentSkillType; }
public:
	void End_Backstep() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "SwordCollision")
		TEnumAsByte<ECollisionChannel> SwordCollisonChannel;
public:
	void OnCollision();
	void OffCollision();

	//적 Sword에 대한 반응
	void OffCollision_Sword();
	void OnCollision_Sword();



private:
	void OnSubAction();
	void OffSubAction();

private:
	UPROPERTY(EditAnywhere, Category = "HideKill")
		class UAnimMontage* HideAttackMontage;

	UPROPERTY(EditAnywhere, Category = "HideKill")
		float MinDistance;

	UPROPERTY(EditAnywhere, Category = "HideKill")
		float MinDot;

	void OnKill();
	void StartKillMode() override;	//Interface
private:
	
	void OnAwake();
private:
	void OnZSkill();
	void OffZSkill();

	void OnXSkill();
	void OffXSkill();

	void OnCSkill();
	void OffCSkil();

	void OnZSkillCharge();

	void OnVSkill();
	void OffVSkill();
	void OnVSkillCharge();

public:
	class ACEnemy* GetLockOnEnemy() { return LockOnEnemy; }

public:
	void OnRideHorse();
	void SpawnHorse();
	void OffRideHorse();
	void DisMountHorse();
private:
	void Landed(const FHitResult& Hit) override;


private:
	//델리게이트연결해야하므로 ufunction
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);


private:
	//AirDash 델리게이트
	UFUNCTION()
		void OnAirDash_Camera();

	UFUNCTION()
		void OnAirDash_Camera_End();



	//OnGroundDash델리게이트
	UFUNCTION()
		void OnGroundDash_Camera();

	UFUNCTION()
		void OnGroundDash_Camera_End();


private:
	UFUNCTION()
		void OnMouseWheelUp_Zoom(float InAxis);

private:
	UFUNCTION()
		void OnPressedSubAction();
	UFUNCTION()
		void OnReleasedSubAction();

	UFUNCTION()
		void OnParry();

private:
	void OnEvade();
	
	void OnEvade_Right();
	void OnEvade_Left();
	void OnEvade_Forward();
	void OnEvade_BackWard();



	void BackStep();	//몽타주플레이
	
	void Damaged();
	void Change_Color();

	UFUNCTION()
		void Restore_Color();

public:
	UFUNCTION(BlueprintCallable)
		void HideUI();

	UFUNCTION(BlueprintCallable)
		void ShowUI();

public:
	/*BP에서 호출*/
	UFUNCTION(BlueprintNativeEvent)
		void Move_Right();
	void Move_Right_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_Forward();
	void Move_Forward_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_Left();
	void Move_Left_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_Backward();
	void Move_Backward_Implementation() {};

	/*Hammer일시 */
	UFUNCTION(BlueprintNativeEvent)
		void Move_HammerRight();
	void Move_HammerRight_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_HammerForward();
	void Move_HammerForward_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_HammerLeft();
	void Move_HammerLeft_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_HammerBackward();
	void Move_HammerBackward_Implementation() {};


	/*Bow일시*/
	UFUNCTION(BlueprintNativeEvent)
		void Move_BowRight();
	void Move_BowRight_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_BowForward();
	void Move_BowForward_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_BowLeft();
	void Move_BowLeft_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Move_BowBackward();
	void Move_BowBackward_Implementation() {};

protected:
		virtual void End_Damaged() override;

private:
	UFUNCTION()/*델리게이트에 연결*/
		void OnHealthPointUpdate(float InHealth, float InMaxHealth);

	UFUNCTION()/*델리게이트에 연결*/
		void OnAttackPointUpdate(float InAttackPoint, float InMaxAttackPoint);

	UFUNCTION()
		void OnManaPointUpdate(float InMana, float InMaxMana);
public:
	void OnAction();

public:
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float GetHitValue() { return HitValue; }
	void SetCollisionPoint(FVector InVector) { CollisionPoint = InVector; }
	bool GetHit() { return bHit; }
	bool GetPlayDamageAnim() { return bPlayDamageAnim; }

	void TakeAttackPoint(float InAmount);

public:
	void CrossHairLineTrace();
	void CrossHairLineTrace_Skill_Z();

private:
	FTimerHandle ChangeColor_TimerHandle;
	int MaterialIndex = 0;
	UPROPERTY(EditAnywhere, Category = "HitMaterial")
		class UMaterialInterface* HitMaterial;

	TArray<class UMaterialInterface*> InitMaterials;
private:
	UPROPERTY(EditAnywhere, Category = "Zoom")
		float MinTargetArmLength = 200;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float MaxTargetArmLength = 400;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float AxisValue=-50.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float InterpTime=5.0f;

private:
	UPROPERTY(EditAnywhere, Category = "SphereOverlap")
		float dotValue;

	UPROPERTY(EditAnywhere, Category = "SphereOverlap")
		float SphereRadius = 200;

	UPROPERTY(EditAnywhere, Category = "SphereOverlap")
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, Category = "SphereOverlap")
		UClass* ActorClassFilter;

	UPROPERTY(EditAnywhere, Category = "SphereOverlap")
		float TargetLabelMaxDistance;
	TArray<AActor*> ActorsIgnore;
	TArray<AActor*> OutActors;

	ACharacter* candidate;

private:
	void EnemyTargeting();
	void SetCandidate();
	void SetTargetLabel(class ACEnemy_AI* Inenemy,bool InbShow);

public:
	FORCEINLINE class ACharacter* GetCandidate() { return candidate; }
private:
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		class UAnimMontage* DamagedMontage;


	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		class UAnimMontage* DeadMontage;

private:
	UPROPERTY(EditAnywhere, Category = "AirDash")
		FAirDashCameraData AirDashCameraData;

	
private:
	float NormalizeAngle(float Angle);
private:

	float ZoomValue;
	float TargetValue;

	float TargetSocketOffSetZValue;

	bool bFixedZoom = false;

	class UCUserWidget_Player* UserInterface;
	
	/*AirDash관련*/
	bool bAirDashZoom = false;
	float InitTargetValue;

	/*4방향 충돌*/
	float HitValue;
	FVector CollisionPoint;
	bool bHit;
	bool IsKnockdown;

	/*groundDash관련*/
	bool bGroundDashZoom = false;

public:
	/*AirCombo관련*/
	bool bAirCombo;

private:
	FHitResult DisMountresult;
	FVector DisMountLocation;

private:
	bool bIsShowTargetUI;

private:
	class ACEnemy* LockOnEnemy;

public:
	void OnJump();
	void SetIsJump(bool InJump) { bIsJump = InJump; }
	FORCEINLINE bool GetIsJump() { return bIsJump; }

public:
	//IsKnockdown
	FORCEINLINE bool GetIsKnockdown() { return IsKnockdown; }
	void SetIsKnockdown(bool bInIsKnockdown);

	void SetGuard(bool bInGuard);

private:
	bool bPlayDamageAnim;
	bool bIsJump;

public:
	bool bRide = false;
private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} DamageData;
};
