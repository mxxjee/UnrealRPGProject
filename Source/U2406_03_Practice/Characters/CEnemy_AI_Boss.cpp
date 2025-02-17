// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy_AI_Boss.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CHealthPointComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/CUserWidget_Enemy.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetComponent.h"

ACEnemy_AI_Boss::ACEnemy_AI_Boss()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ACEnemy_AI_Boss::BeginPlay()
{
	/*시네마틱 on (off밑에 주석처리 후 바로밑줄 주석제거)*/
	bCinematic = true;
	

	

	Super::BeginPlay();

	CheckNull(HealthPoint);

	HealthPoint->SetMaxHealth(300);
	HealthPoint->InitHealth();

	

	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	
	//boss  HP는 뷰포트에 추가해서 표시
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	label->AddToViewport();

	label->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());

	label->SetVisibility(ESlateVisibility::Hidden);
	
	label->UpdateHealthColor_OnePhase();
	/*시네마틱  off시*/
	/*bCinematic = false;
	SetUI();*/

	
}


void ACEnemy_AI_Boss::Tick(float DeltaTime)
{
	Enemy_DeltaTime = DeltaTime;


}
void ACEnemy_AI_Boss::SetUI()
{
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	label->SetVisibility(ESlateVisibility::Visible);

	SetEnbaleRunBT();
}

void ACEnemy_AI_Boss::SetEnbaleRunBT()
{
	ACAIController* controller = Cast<ACAIController>(GetController());
	CheckNull(controller);

	controller->setUpdateTree(true);
}

void ACEnemy_AI_Boss::LaunchBoss()
{
	GetCharacterMovement()->GravityScale = CinematicGravityScale;
	FVector LaunchVelocity = GetActorUpVector() + GetActorForwardVector();
	LaunchCharacter(LaunchVelocity * LaunchPower, false, false);

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindUFunction(this, "StopLaunchBoss");

	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, 3, false);
}

void ACEnemy_AI_Boss::StopLaunchBoss()
{
	GetCharacterMovement()->GravityScale = CinematicRecoverGravityScale;

	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	timerDelegate.BindLambda([=] {
		GetCharacterMovement()->GravityScale = 1;
	});

	Owner->GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, 1, false);
}

void ACEnemy_AI_Boss::UpdateHealthColor_TwoPhase()
{
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);

	label->UpdateHealthColor_TwoPhase();
}

void ACEnemy_AI_Boss::UpdateHealthColor_ThreePhase()
{
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);

	label->UpdateHealthColor_ThreePhase();
}
