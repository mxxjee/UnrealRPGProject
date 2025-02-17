// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/PlayerController.h"
#include "MatineeCameraShake.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/CPlayer.h"

UCSubAction_Sword::UCSubAction_Sword()
{
	
	CHelpers::GetAsset<USoundWave>(&Sound, "SoundWave'/Game/Sounds/Sword_Hits_1.Sword_Hits_1'");
	CHelpers::GetAsset<UNiagaraSystem>(&Particle, "NiagaraSystem'/Game/Effect/NS_Impact_1.NS_Impact_1'");
}

void UCSubAction_Sword::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	ignores.Add(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);

	InitTargetArmLength = SpringArm->TargetArmLength;
	InitSocketOffSet = SpringArm->SocketOffset;
}

void UCSubAction_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckNull(State);
	if (State->IsSubActionMode())
	{
		if (bCanMove == false)
		{
			CheckNull(Movement);
			Movement->SetbCanMove(false);
		}

		AActor* attachment = Cast<AActor>(Attachment);
		USkeletalMeshComponent* mesh = CHelpers::GetComponent<USkeletalMeshComponent>(attachment);

		//"Start_Guard","end"
		FVector start = mesh->GetSocketLocation(StartSocketName);
		FVector end = mesh->GetSocketLocation(EndSocketName);
		
		UKismetSystemLibrary::BoxTraceSingle(Owner->GetWorld(), start, end, FVector(extentX,extentY,extentZ),attachment->GetActorRotation(),ETraceTypeQuery::TraceTypeQuery4, false, ignores, EDrawDebugTrace::None, hitResult, true);

		if (!!Camera)
		{
			if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, ZoomTargetArmLength))
			{
				SpringArm->TargetArmLength = ZoomTargetArmLength;
				SpringArm->SocketOffset.Z = ZoomTargetArmLength / (2.5f);
			}

			SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ZoomTargetArmLength, DeltaTime, FovInterpTime);
			SpringArm->SocketOffset.Z = FMath::FInterpTo(SpringArm->SocketOffset.Z, ZoomTargetArmLength/(2.5f), DeltaTime, FovInterpTime);
		
		}
		//guard를 누르고 10초 이후면 50%확률로 가드해제

		//CLog::Print(PressCount, 1111,10.0f,FColor::Red);

		if (PressCount>= MaxPressCount)
		{

			double Random = FMath::RandRange(1, 4);

			if (Random >= 3)
			{
				Cancel_Guard();
				PressCount = 0;
				MaxPressCount = FMath::RandRange(7, 10);
			}

			else
			{
				PressCount = 0;
				MaxPressCount = FMath::RandRange(7, 10);
			}
				
				
		}
	}

	else
	{
		if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, InitTargetArmLength))
			return;
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, InitTargetArmLength, DeltaTime, FovInterpTime);
		SpringArm->SocketOffset.Z = FMath::FInterpTo(SpringArm->SocketOffset.Z,
			InitSocketOffSet.Z,
			DeltaTime, FovInterpTime);
		
	}

}

void UCSubAction_Sword::Pressed()
{

	Super::Pressed();
	State->OnSubActionMode();

	if (bCanMove==false)
	{
		CheckNull(Movement);
		Movement->SetbCanMove(false);
	}

	PressCount = 0;
	MaxPressCount = FMath::RandRange(7, 10);

	timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		++PressCount;

	});


	Owner->GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 1.f, true);
}

void UCSubAction_Sword::Released()
{
	
	Super::Released();
	State->OffSubActionMode();
	Owner->GetWorld()->GetTimerManager().ClearTimer(timerHandle);

	if (bCanMove == false)
	{
		CheckNull(Movement);
		Movement->SetbCanMove(true);
	}
}

void UCSubAction_Sword::Cancel_Guard()
{
	Super::Cancel_Guard();
	State->OffSubActionMode();

	CLog::Print("Cancel_Guard", 15000, 1, FColor::Green);
}

void UCSubAction_Sword::PlayParrySound()
{

	CheckNull(Sound);

	FVector location = Owner->GetActorLocation();
	UGameplayStatics::SpawnSoundAtLocation(Owner->GetWorld(), Sound, location);

	PlayCameraShake();
	PlayBlockEffect();
}

void UCSubAction_Sword::PlayEffect(UWorld* InWorld)
{

	CheckNull(Particle);
	CheckFalse(hitResult.bBlockingHit);

	FTransform transform;
	transform.SetLocation(hitResult.ImpactPoint);
	transform.SetScale3D(EffectScale);

	//UGameplayStatics::SpawnEmitterAtLocation(InWorld, ImpactParticle, transform);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, Particle, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation());
	//CLog::Log(hitResult.ImpactNormal.Rotation());

}

void UCSubAction_Sword::PlayCameraShake()
{
	APlayerController* controller = Owner->GetController<APlayerController>();
	CheckNull(controller);

	controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);

}

void UCSubAction_Sword::PlayBlockEffect()
{
	CheckNull(Owner);
	ACPlayer* player = Cast<ACPlayer>(Owner);

	CheckNull(player);

	player->SetBlockSuccess();
}