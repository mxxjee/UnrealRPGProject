// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SkillActions/CSkillAction_Wizard_Final.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/AddOns/CShockWave.h"
#include "Weapons/AddOns/CLance.h"

void UCSkillAction_Wizard_Final::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment)
{
	Super::BeginPlay(InOwner, InAttachment);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);

}

void UCSkillAction_Wizard_Final::DoSkillAction()
{
	CheckFalse(State->IsIdleMode());

	State->SetActionMode();
	State->OnSkillActionMode();

	bInAction = true;
	SkillActionData.DoSkillAction(Owner);
}

void UCSkillAction_Wizard_Final::Begin_DoSkillAction()
{
	Super::Begin_DoSkillAction();


}

void UCSkillAction_Wizard_Final::End_DoSkillAction()
{
	Super::End_DoSkillAction();
	bInAction = false;
}

void UCSkillAction_Wizard_Final::SpawnShockWave()
{
	CheckNull(ShockWaveClass);
	CheckNull(Owner);

	FActorSpawnParameters params;
	params.Owner = Owner;
	
	TArray<AActor*> Toignore;
	Toignore.Add(Owner);
	FHitResult hitResult;

	FVector start = Owner->GetActorLocation();
	FVector end = start + FVector(0, 0, -1000);

	//惯关俊 积己
	UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery7,
		false,
		Toignore,
		EDrawDebugTrace::None,
		hitResult,
		true
	);

	if (hitResult.bBlockingHit)
	{
		FTransform transform;
		transform.SetLocation(hitResult.Location);
		transform.SetRotation(FQuat(FRotator::ZeroRotator));

		ACShockWave* shockwave= Owner->GetWorld()->SpawnActor<ACShockWave>(ShockWaveClass, transform, params);
		CheckNull(shockwave);

		shockwave->AddIngoreActor(Owner);
		CheckNull(Behavior);

		ACharacter* target = Behavior->GetTarget();
		CheckNull(target);

		FVector StartPoint = Owner->GetActorLocation();
		FVector EndPoint = target->GetActorLocation();

		FVector direction = EndPoint - StartPoint;
		direction.Normalize();

		shockwave->Shoot(direction);

	}
}

void UCSkillAction_Wizard_Final::SetLanceLocation()
{
	CheckNull(Behavior);
	CheckNull(Decal);

	ACharacter* target = Behavior->GetTarget();
	CheckNull(target);

	LanceLocation = target->GetActorLocation();

	//敲饭捞绢 惯关俊 单漠积己
	TArray<AActor*> Toignore;
	Toignore.Add(target);
	FHitResult hitResult;

	FVector start = target->GetActorLocation();
	FVector end = start + FVector(0, 0, -1000);

	//惯关俊 积己
	UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		Toignore,
		EDrawDebugTrace::None,
		hitResult,
		true
	);

	if (hitResult.bBlockingHit)
	{
		FActorSpawnParameters params;
		FTransform transform;

		transform.SetLocation(hitResult.Location);
		transform.SetRotation(FQuat(FRotator::ZeroRotator));

		AActor* decal = Owner->GetWorld()->SpawnActor<AActor>(Decal, transform, params);
	}
}

void UCSkillAction_Wizard_Final::SpawnLance()
{
	CheckNull(lanceClass);
	CheckNull(Owner);

	FActorSpawnParameters params;
	params.Owner = Owner;

	FTransform transform;
	transform.SetLocation(LanceLocation);
	transform.SetRotation(FQuat(FRotator::ZeroRotator));

	ACLance* lance = Owner->GetWorld()->SpawnActor<ACLance>(lanceClass, transform, params);
	lance->AddIngoreActor(Owner);

}