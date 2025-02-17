// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Animation/AnimMontage.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CAttackPointComponent.h"

UCMovementComponent::UCMovementComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Characters/Curves/CB_Dash.CB_Dash'");
	
}



// Called when the game starts
void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	FOnTimelineFloat timeline;
	timeline.BindUFunction(this, "OnDashMax");

	Timeline.AddInterpFloat(Curve, timeline);
}



// Called every frame
void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Player상태에서만 Tick실행


	CheckNull(Cast<ACPlayer>(OwnerCharacter));
	Timeline.TickTimeline(DeltaTime);
	Timeline.SetPlayRate(3);

	if (bSprint_Max)
	{
		TargetMaxSpeed = Speed[(int32)ESpeedType::Sprint_Max];
	}

	bMovePress = bMovePressW || bMovePressS || bMovePressA || bMovePressD;
	CheckTrue(bGroundDash);
	
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);
	
	if (state->IsSubActionMode())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 200;
		return;
	}
	//하나라도 눌려있으면 
	if (bMovePress && state->IsIdleMode())
	{
		
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(
			OwnerCharacter->GetCharacterMovement()->GetMaxSpeed(),
			TargetMaxSpeed,
			0.05f
		);

	}

}

void UCMovementComponent::OnJump()
{
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(weapon);
	
	bCanMove = false;
	if (weapon->IsSwordMode())
	{
		OwnerCharacter->Jump();
		

	}
		
}

void UCMovementComponent::PressMoveW()
{
	bMovePressW = true;
	TargetMaxSpeed = Speed[(int32)ESpeedType::Run];

	
}

void UCMovementComponent::PressMoveA()
{
	bMovePressA = true;
	TargetMaxSpeed = Speed[(int32)ESpeedType::Run];

}

void UCMovementComponent::PressMoveS()
{
	bMovePressS = true;
	TargetMaxSpeed = Speed[(int32)ESpeedType::Run];

	
}

void UCMovementComponent::PressMoveD()
{
	bMovePressD = true;
	TargetMaxSpeed = Speed[(int32)ESpeedType::Run];
	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;



}

void UCMovementComponent::ReleaseMoveW()
{
	bMovePressW = false;
	CheckReleaseMove();
}
void UCMovementComponent::ReleaseMoveA()
{
	bMovePressA = false;
	CheckReleaseMove();
}

void UCMovementComponent::ReleaseMoveS()
{
	bMovePressS = false;
	CheckReleaseMove();
}

void UCMovementComponent::ReleaseMoveD()
{
	bMovePressD = false;
	CheckReleaseMove();


}

void UCMovementComponent::CheckReleaseMove()
{
	// 모든 키가 해제되었을 때만 ReleaseMove를 호출
	if (!bMovePressW && !bMovePressS && !bMovePressA && !bMovePressD)
	{
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		CheckNull(state);
		CheckFalse(state->IsIdleMode());
		ReleseMove();
	}
}
void UCMovementComponent::ReleseMove()
{
	

	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 0.1f;

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	});

	//다시되돌려준다.
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, 3, false);

}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];


}


void UCMovementComponent::OnSprint_Max()
{
	TargetMaxSpeed = Speed[(int32)ESpeedType::Sprint_Max];
	//SetSpeed(ESpeedType::Sprint_Max);
}


void UCMovementComponent::OnSprint()
{
	

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	if (!!weapon && !!state)
	{
		/*칼 장착 시 에어대쉬*/
		if (weapon->IsSwordMode())
		{
			CheckTrue(state->IsSubActionMode());//Subacton모드에는 달리기X
			if (OwnerCharacter->GetCharacterMovement()->IsFalling() == false)
				TargetMaxSpeed = Speed[(int32)ESpeedType::Sprint];

			else
			{
				if (bAirDash == false)
					OnAirDash();
			}
		}

		else
		{
			if (bSprint_Max == false)
				TargetMaxSpeed = Speed[(int32)ESpeedType::Sprint];

			Timeline.PlayFromStart();
		}


	}
	
	/*칼 장착하지않았을 경우*/
	else
	{
		if(bSprint_Max==false)
			TargetMaxSpeed = Speed[(int32)ESpeedType::Sprint];

		Timeline.PlayFromStart();
	}
		

	
}

void UCMovementComponent::OnAirDash()
{
	bAirDash = true;
	bFixedCamera = true;
	bCanMove = false;
	OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;

	//카메라고정
	if (OnAirDashStart.IsBound())
		OnAirDashStart.Broadcast();
	
	FVector LaunchVelocity = GetMovementInputVector() * AirDashSpeed;

	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true);

	//OwnerCharacter->PlayAnimMontage(Montage);
	

}

void UCMovementComponent::OnAirDash_End()
{

	bFixedCamera = false;
	
	//SetSpeed(ESpeedType::Sprint);

	if (OnAirDashEnd.IsBound())
		OnAirDashEnd.Broadcast();

	OwnerCharacter->GetCharacterMovement()->GravityScale = 7.f;
	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	
	CLog::Log("False-bAirDash");
	

}

void UCMovementComponent::OnGround_Dash()
{
	UCAttackPointComponent* attack = CHelpers::GetComponent<UCAttackPointComponent>(OwnerCharacter);
	CheckNull(attack);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(weapon);

	CheckFalse(weapon->IsSwordMode());
	CheckFalse(attack->IsAwake());
	//CheckFalse(weapon->IsUnarmedMode());
	CheckFalse(GroundDashCoolTimeEnd);

	bGroundDash = true;
	bFixedCamera = true;
	bCanMove = false;

	//쿨타임중
	GroundDashCoolTimeEnd = false;

	// 대쉬할 때 속도 및 마찰력 설정
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 5000.0f;
	OwnerCharacter->GetCharacterMovement()->MaxAcceleration = 3500.0f;
	OwnerCharacter->GetCharacterMovement()->BrakingFriction = 0.0f;
	

	// 카메라 고정 이벤트 호출
	if (OnGroundDashStart.IsBound())
		OnGroundDashStart.Broadcast();

	// 캐릭터의 전방으로 강하게 밀어내기
	FVector LaunchVelocity = OwnerCharacter->GetActorForwardVector();

	// 중력을 무시하지 않으면 지면과 마찰이 생길 수 있음
	OwnerCharacter->LaunchCharacter(LaunchVelocity * GroundDashSpeed, true, true);  // 중력을 무시하도록 설정

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {

		OwnerCharacter->GetCharacterMovement()->BrakingFriction = 70.0f;
	});

	//다시되돌려준다.
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, 0.5f, false);

}

void UCMovementComponent::OnGroundDash_End()
{
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(weapon);

	//CheckFalse(weapon->IsUnarmedMode());
	bFixedCamera = false;
	bCanMove = true;

	//SetSpeed(ESpeedType::Sprint);

	if (OnGroundDashEnd.IsBound())
		OnGroundDashEnd.Broadcast();

	SetSpeed(ESpeedType::Sprint);
	OwnerCharacter->GetCharacterMovement()->MaxAcceleration = 2048.0f;
	OwnerCharacter->GetCharacterMovement()->BrakingFriction = 2.0f;

	FTimerHandle timerhandle;

	//두번째인자에 넣을 델리게이트변수
	FTimerDelegate timerDelegate;
	
	timerDelegate.BindLambda([=] {

		GroundDashCoolTimeEnd = true;
	});

	//다시되돌려준다.
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, GroundDashCoolTime, false);

}
void UCMovementComponent::OnRun()
{
	TargetMaxSpeed = Speed[(int32)ESpeedType::Run];

	if(!!Cast<ACPlayer>(OwnerCharacter))
		Timeline.ReverseFromEnd();


}

void UCMovementComponent::OnWalk()
{
	TargetMaxSpeed = Speed[(int32)ESpeedType::Walk];
}



void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);
	CheckTrue(bGroundDash);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector forwardvector = FQuat(rotator).GetForwardVector();


	OwnerCharacter->AddMovementInput(forwardvector,InAxis);

}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);
	CheckTrue(bGroundDash);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector rightvector = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(rightvector, InAxis);
	
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::Stop()
{
	bCanMove = false;
}

void UCMovementComponent::Move()
{
	bCanMove = true;
}

void UCMovementComponent::OnDashMax(float Output)
{

	if (Output < 1)
	{
		bSprint_Max = false;
		return;
	}

	if (Output >= 1)
	{
		bSprint_Max = true;
		return;
	}
		

}

FVector UCMovementComponent::GetMovementInputVector()
{
	FVector MovementInputVector = OwnerCharacter->GetLastMovementInputVector();

	if (MovementInputVector.Length() > 0.5f)
		return MovementInputVector;

	else
		return UGameplayStatics::GetPlayerCameraManager(OwnerCharacter->GetWorld(),0)->GetActorForwardVector() * FVector(1, 1, 0);
}

void UCMovementComponent::MoveCharacter(float InForce)
{
	CheckTrue(InForce == 0);
	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	FVector LaunchVelocity = OwnerCharacter->GetActorForwardVector();
	OwnerCharacter->GetCharacterMovement()->AddImpulse(LaunchVelocity * InForce);
}


