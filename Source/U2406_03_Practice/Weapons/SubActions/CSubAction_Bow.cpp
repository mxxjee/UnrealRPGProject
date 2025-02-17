// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "CSubAction_Bow.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Characters/CPlayer.h"

UCSubAction_Bow::UCSubAction_Bow()
{
	//CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/Weapons/Bow/CurveVector_Aim.CurveVector_Aim'");
}

void UCSubAction_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	
	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!bow)
		Bending = bow->GetAnimInstance_Bending();


	player = Cast<ACPlayer>(Owner);

}

void UCSubAction_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	Timeline.TickTimeline(DeltaTime);	//call 해줘야 tick마다 timeline함수가 실행됨
	Timeline.SetPlayRate(PlayRate);//200배 빠르게 -> 0.1초

	if (State->IsSubActionMode())
	{
		CheckNull(player);
		player->CrossHairLineTrace();
	}
		
}
void UCSubAction_Bow::Pressed()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Pressed();

	State->OnSubActionMode();

	Owner->bUseControllerRotationYaw = true;

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	/*시야각 줄이기*/
	//Camera->FieldOfView = 45;
	CheckNull(Owner);
	CheckNull(player);
	PostProcessVolume = player->GetWorldPostProcessVolume();

	Timeline.PlayFromStart();

	
}

void UCSubAction_Bow::Released()
{
	CheckNull(SpringArm);
	CheckNull(Camera);
	Super::Released();

	State->OffSubActionMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	/*시야각  다시되돌리기*/
	//Camera->FieldOfView = 90;
	Timeline.Reverse();	//다시 45부터 90
}


void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;
	
	if (!!Bending)
		*Bending = Output.Y;
	CheckNull(PostProcessVolume);
	OnVignetting(PostProcessVolume,Output.Z);

	
}