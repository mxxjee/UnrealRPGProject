// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"


ACAttachment_Bow::ACAttachment_Bow()
{
	

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> bowAnimInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&bowAnimInstance, "AnimBlueprint'/Game/Weapons/Bow/ABP_Bow.ABP_Bow_C'");

	//bow애님클래스 적용
	SkeletalMesh->SetAnimInstanceClass(bowAnimInstance);

	
	


}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");
	SkeletalMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

}



void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();
	AttachTo("Hand_Bow_Left");

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller);

	OriginViewPitchRange.X=controller->PlayerCameraManager->ViewPitchMin;
	OriginViewPitchRange.Y=controller->PlayerCameraManager->ViewPitchMax;

	controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
	controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;

}

void ACAttachment_Bow::OnUnEquip_Implementation()
{
	Super::OnUnEquip_Implementation();
	AttachTo("Holster_Bow");


	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller);

	controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
	controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
}

float* ACAttachment_Bow::GetAnimInstance_Bending()
{
	UCAnimInstance_Bow* bow = Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance());
	
	if (!!bow)
		return bow->GetBending();

	return nullptr;
}