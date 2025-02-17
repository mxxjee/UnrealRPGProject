// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "CMovementComponent.generated.h"



UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk = 0,
	Run,
	Sprint,
	Sprint_Max,
	Evade,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAirDash);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAirDash_End);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGroundDash);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGroundDash_End);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U2406_03_PRACTICE_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	//speed[3] 배열
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::Max] = { 200,400,600,800,800 };

private:
	UPROPERTY(EditAnywhere, Category = "EvadeSpeed")
		float EvadeForce = 1000.f;

	UPROPERTY(EditAnywhere, Category = "AirDash")
		float AirDashSpeed=1000.f;

	UPROPERTY(EditAnywhere, Category = "AirDash")
		float AirDashReleaseTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Dash")
		class UCurveFloat* Curve;

	//카메라 마우스감도
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float VerticalLook = 45;

	UPROPERTY(EditAnywhere, Category = "GroundDash")
		float GroundDashSpeed = 1000.f;


	UPROPERTY(EditAnywhere, Category = "GroundDash")
		bool GroundDashCoolTimeEnd = true;	
	//false:쿨타임중,true:쿨타임끝(실행가능)

public:
	UPROPERTY(EditAnywhere, Category = "GroundDash")
		float GroundDashCoolTime = 10.0f;
	
public:
	FORCEINLINE bool CanMove() { return bCanMove; }

	FORCEINLINE void SetCanMove(bool bInMove) { bCanMove = bInMove; }

	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }

	FORCEINLINE bool GetGroundDashCoolTimeEnd() { return GroundDashCoolTimeEnd; }

	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }
	FORCEINLINE float GetSprintMaxSpeed() { return Speed[(int32)ESpeedType::Sprint_Max]; }
public:	
	// Sets default values for this component's properties
	UCMovementComponent();

private:
	class ACharacter* OwnerCharacter;

private:
	void CheckReleaseMove();
public:
	void PressMoveW();
	void PressMoveA();
	void PressMoveS();
	void PressMoveD();

	void ReleaseMoveW();
	void ReleaseMoveA();
	void ReleaseMoveS();
	void ReleaseMoveD();


	

	void ReleseMove();
public:
	
	void SetSpeed(ESpeedType InType);

public:
	void MoveCharacter(float InForce);

public:
	//이동기능
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	//마우스회전
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void OnAirDash();
	void OnSprint_Max();
	void OnSprint();
	void OnRun();
	void OnWalk();
	

	void OnJump();

	void OnAirDash_End();

	void OnGround_Dash();
	void OnGroundDash_End();


public:
	//카메라 회전을 따라 캐릭터회전
	void EnableControlRotation();
	void DisableControlRotation();

public:
	void Stop();
	void Move();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool GetbAirDash() { return bAirDash; }
	void SetbAirDash(bool Inbool) { bAirDash = Inbool; }

	bool GetbGroundDash() { return bGroundDash; }
	void SetbGroundDash(bool Inbool) { bGroundDash = Inbool; }

	void SetbCanMove(bool Inbool) { bCanMove = Inbool; }

private:
	UFUNCTION()
		void OnDashMax(float Output);
	FVector GetMovementInputVector();

public:
	FOnAirDash OnAirDashStart;
	FOnAirDash_End OnAirDashEnd;

	FOnGroundDash OnGroundDashStart;
	FOnGroundDash_End OnGroundDashEnd;

private:
	FTimeline Timeline;
private:
	bool bCanMove = true;
	bool bFixedCamera;

	bool bAirDash = false;

	bool bGroundDash = false;

	float TargetMaxSpeed;

private:
	bool bMovePress;
	bool bMovePressW;
	bool bMovePressS;
	bool bMovePressA;
	bool bMovePressD;


private:
	bool bPreviousMovePress = false;

private:
	UPROPERTY(EditAnywhere)
		bool bSprint_Max = false;

	UPROPERTY(EditAnywhere)
		float InterpSpeed;

};
