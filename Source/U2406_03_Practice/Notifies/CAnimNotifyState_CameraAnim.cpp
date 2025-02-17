// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_CameraAnim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"//Player CameraManager를 가지고 있다.
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"


UCAnimNotifyState_CameraAnim::UCAnimNotifyState_CameraAnim()
{
	bMoveEnd = true;
}

FString UCAnimNotifyState_CameraAnim::GetNotifyName_Implementation() const
{
	return "CameraAnim";
}

void UCAnimNotifyState_CameraAnim::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	bMoveEnd = true;
	ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(Owner);

	
	camera = CHelpers::GetComponent<UCameraComponent>(Owner);

	CheckNull(camera);
	InitPosition = camera->GetRelativeLocation();
	InitRotation = camera->GetRelativeRotation();


	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {
		bMoveEnd = false;
	});

	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, StopTime, false);
}


void UCAnimNotifyState_CameraAnim::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	CheckNull(MeshComp);
	CheckNull(camera);
	CheckFalse(camera->IsValidLowLevel());
	
	if (bMoveEnd)
	{
		if (MovePos != FVector::ZeroVector)
			camera->SetRelativeLocation(FMath::VInterpTo(camera->GetRelativeLocation(), MovePos, FrameDeltaTime, MoveTime));
		
		camera->SetRelativeRotation(FMath::RInterpTo(camera->GetRelativeRotation(), MoveRot, FrameDeltaTime, RotateTime));
	}


	else
	{
		camera->SetRelativeLocation(FMath::VInterpTo(camera->GetRelativeLocation(), InitPosition, FrameDeltaTime, MoveEndTime));
		camera->SetRelativeRotation(FMath::RInterpTo(camera->GetRelativeRotation(), InitRotation, FrameDeltaTime, RotateEndTime));
	}

}


void UCAnimNotifyState_CameraAnim::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(Owner);
	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(controller);

	CheckNull(camera);

	camera->SetRelativeLocation(InitPosition);
	camera->SetRelativeRotation(InitRotation);
	
}
