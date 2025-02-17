// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Widgets/CUserWidget_Enemy.h"
#include "Characters/CAIController.h"
#include "BehaviorTree/CPatrolPath.h"
#include "Weapons/CAttachment.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//widgetcomponent ����
	CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label",GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &TargetWidget, "TargetLabel", GetMesh());

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");


	

	//����������Ʈ�� Ŭ��������
	TSubclassOf<UCUserWidget_Enemy> labelClass;
	CHelpers::GetClass<UCUserWidget_Enemy>(&labelClass, "WidgetBlueprint'/Game/Widgets/WB_Enemy.WB_Enemy_C'");
	LabelWidget->SetWidgetClass(labelClass);
	
	//ũ��,��ġ
	LabelWidget->SetRelativeLocation(FVector(0, 0, 220));
	LabelWidget->SetDrawSize(FVector2D(120, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Enemy> TargetClass;
	CHelpers::GetClass<UCUserWidget_Enemy>(&TargetClass, "WidgetBlueprint'/Game/Widgets/WB_Enemy_Target.WB_Enemy_Target_C'");
	TargetWidget->SetWidgetClass(TargetClass);

	//CHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, "BehaviorTree'/Game/Enemies/Melee/BT_Enemy_Melee.BT_Enemy_Melee'");
	
	
}

void ACEnemy_AI::BeginPlay() 
{
	Super::BeginPlay();

	CheckNull(HealthPoint);
	if (bMiddleBoss)
	{
		HealthPoint->SetMaxHealth(200);
		HealthPoint->InitHealth();
	}

	HealthPoint->OnHealthPointChanged.AddDynamic(this, &ACEnemy_AI::OnHealthPointChanged);
	LabelWidget->InitWidget();

	UCUserWidget_Enemy* label=Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
	
	label->UpdateName(GetName());
	
	label->UpdateControllerName(GetController()->GetName());
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Tick_LabelREnderScale();
	ShowTargetUI();

}

void ACEnemy_AI::OnHealthPointChanged(float InHealth, float InMaxHealth)
{
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
}

void ACEnemy_AI::StartKillMode()
{
	CLog::Log("ACEnemy_AI_StartKillMode");
	PlayAnimMontage(HideKillMontage);

	HealthPoint->Damage(100);
	Behavior->SetDeadMode();
}

void ACEnemy_AI::Tick_LabelREnderScale()
{
	//1.�� ��ü������
	//2.���� player�� ī�޶���ġ
	//3.enemy�� ī�޶���ġ
	//4.�� ��ġ�� ���� ���Ѵ�.
	//5.���̿� ���� �����������Ѵ�.
	
	CheckNull(State);

	CheckTrue(State->IsDeadMode());

	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	UCUserWidget_Enemy* Target = Cast<UCUserWidget_Enemy>(TargetWidget->GetUserWidgetObject());

	CheckNull(label);
	CheckNull(Target);

	label->UpdateLabelRotation(LabelWidget,this);
	

	APlayerCameraManager* cameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	FVector CameraLocation = cameraManager->GetCameraLocation();


	FVector EnemyLocation = GetActorLocation();


	float Distance = FVector::Distance(CameraLocation, EnemyLocation);

	if (Distance > MaxTargetLabelDistance)
	{
		Target->SetVisibility(ESlateVisibility::Hidden);
	}
	if (Distance > MaxLabelDistance)
	{
		label->SetVisibility(ESlateVisibility::Hidden);

		return;
	}

	else
	{
		label->SetVisibility(ESlateVisibility::Visible);

	}

}

void ACEnemy_AI::Damaged()
{
	Super::Damaged();

	PlayDamageCameraShake();
	CheckTrue(State->IsDeadMode());

	Behavior->SetHittedMode();	//������ Ű�� ������
	State->OffSubActionMode();
	if (bAirSkillDamaged)
		Behavior->SetHitTypeAirMode();
	
	ACAIController* controller = Cast<ACAIController>(GetController());
	CheckNull(controller);

	controller->StopMovement();

}

void ACEnemy_AI::DamagedAirAttack()
{
	Super::DamagedAirAttack();

	ACAIController* controller = Cast<ACAIController>(GetController());
	CheckNull(controller);

	controller->StopMovement();
	Behavior->SetHittedMode();
	Behavior->SetHitTypeAirMode();

}

void ACEnemy_AI::End_Damaged()
{
	Super::End_Damaged();

	Behavior->SetWaitMode();
}

void ACEnemy_AI::End_AirDamaged()
{
	Super::End_AirDamaged();

	if (bAirSkillDamaged)
		bAirSkillDamaged = false;

	State->SetIdleMode();
	Behavior->SetWaitMode();
	
}


void ACEnemy_AI::ShowTargetUI()
{
	UCUserWidget_Enemy* Target = Cast<UCUserWidget_Enemy>(TargetWidget->GetUserWidgetObject());
	CheckNull(Target);

	if (HealthPoint->IsDead())
	{
		Target->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (bShowTargetUI)
	{
		Target->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	else
	{
		Target->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
}

void ACEnemy_AI::StopMovement()
{
	ACAIController* controller = Cast<ACAIController>(GetController());
	CheckNull(controller);

	controller->StopMovement();
}


void ACEnemy_AI::OffCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACEnemy_AI::OnCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACEnemy_AI::Dead()
{

	Super::Dead();


	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	UCUserWidget_Enemy* Target = Cast<UCUserWidget_Enemy>(TargetWidget->GetUserWidgetObject());

	CheckNull(label);
	CheckNull(Target);

	Target->SetVisibility(ESlateVisibility::Hidden);
	label->SetVisibility(ESlateVisibility::Hidden);

	ACAIController* controller = Cast<ACAIController>(GetController());
	CheckNull(controller);

	controller->StopMovement();
	controller->ClearFocus(EAIFocusPriority::Gameplay);
	Behavior->SetDeadMode();


	ACAttachment* attachment = Weapon->GetAttachment();
	CheckNull(attachment);

	attachment->SetLifeSpan(3);
}