// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CHorse.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMovementComponent.h"
#include "Characters/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACHorse::ACHorse()
{
 
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/HorseAnimsetPro/Meshes/Horses/SK_Horse.SK_Horse'");

	GetMesh()->SetSkeletalMesh(mesh);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 400;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, "/Script/Engine.AnimBlueprint'/Game/ABP_Horse.ABP_Horse_C'");
	GetMesh()->SetAnimClass(Anim);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}


void ACHorse::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun();

	
}

// Called every frame
void ACHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACHorse::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACHorse::OffSprint);

	PlayerInputComponent->BindAction("Ride", EInputEvent::IE_Pressed, this, &ACHorse::DisMount);
}

void ACHorse::DisMount()
{
	CheckNull(OwnerCharacter);

	OwnerCharacter->OffRideHorse();
	
	GetController()->Possess(OwnerCharacter);
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	OwnerCharacter->DisMountHorse();
	FActorSpawnParameters spawnParams;
	GetWorld()->SpawnActor<AActor>(DisMountEffect, (GetActorLocation() + LocationOffSet), GetActorRotation(), spawnParams);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DisMountSound, GetActorLocation());
	Destroy();
}

void ACHorse::OnSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 800;
}

void ACHorse::OffSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}