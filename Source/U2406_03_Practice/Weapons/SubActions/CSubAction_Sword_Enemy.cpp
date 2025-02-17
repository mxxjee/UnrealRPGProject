// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Sword_Enemy.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Weapons/CAttachment.h"
#include "Components/CAIBehaviorComponent.h"

void UCSubAction_Sword_Enemy::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	OwnerCharacter = Cast<ACEnemy_AI>(Owner);


}


void UCSubAction_Sword_Enemy::Pressed()
{
	CheckNull(OwnerCharacter);
	CheckNull(State);
	
	bInAction = true;
	
	State->OnSubActionMode();
	State->SetActionMode();

	OwnerCharacter->GetMesh()->SetOverlayMaterial(GuardMaterial);

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "Cancel");

	Owner->GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, FMath::RandRange(5,8), false);
}

void UCSubAction_Sword_Enemy::Released()
{
	CheckNull(OwnerCharacter);
	bInAction = false;

	State->OffSubActionMode();
	State->SetIdleMode();
	OwnerCharacter->GetMesh()->SetOverlayMaterial(nullptr);
}


void UCSubAction_Sword_Enemy::Tick(float DeltaTime)
{
}

void UCSubAction_Sword_Enemy::Cancel()
{

	bInAction = false;
	State->OffSubActionMode();
	State->SetIdleMode();

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(OwnerCharacter);
	behavior->SetAttackType(EAIAttackType::Attack);
	OwnerCharacter->GetMesh()->SetOverlayMaterial(nullptr);
}

void UCSubAction_Sword_Enemy::Cancel_Guard()
{
	Super::Cancel_Guard();

	bInAction = false;
	State->OffSubActionMode();
	State->SetIdleMode();

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(OwnerCharacter);
	behavior->SetAttackType(EAIAttackType::Attack);
	OwnerCharacter->GetMesh()->SetOverlayMaterial(nullptr);
}

void UCSubAction_Sword_Enemy::PlayParrySound()
{
}

void UCSubAction_Sword_Enemy::PlayEffect(UWorld* InWorld)
{
	CheckNull(Effect);

	USkeletalMeshComponent* Meshcomp = CHelpers::GetComponent<USkeletalMeshComponent>(Attachment);
	FTransform Spawntransform = Meshcomp->GetSocketTransform("Impact");


	FTransform transform;
	transform.SetLocation(Spawntransform.GetLocation());
	transform.SetScale3D(Scale);

	UGameplayStatics::SpawnEmitterAtLocation(InWorld, Effect, transform);
}


