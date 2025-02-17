// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CAnimInstance.h"
#include "Weapons/CSword.h"
#include "Weapons/CAttachment.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"

#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"//우리가 정의한 컴포넌트
#include "Components/CTargetComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CAttackPointComponent.h"
#include "Components/CManaPointComponent.h"

#include "Weapons/CWeaponStructures.h"

#include "Animation/AnimMontage.h"

#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"

#include "Widgets/CUserWidget_Player.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CGhostTrail.h"

#include "Engine/PostProcessVolume.h"
#include  "Characters/CHorse.h"

// Sets default values
ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorsIgnore.Add(this);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	

	//액터컴포넌트 더하기
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
	CHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "HealthPoint");
	CHelpers::CreateActorComponent<UCAttackPointComponent>(this, &AttackPoint, "AttackPoint");
	CHelpers::CreateActorComponent<UCManaPointComponent>(this, &ManaPoint, "ManaPoint");


	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");

	GetMesh()->SetSkeletalMesh(mesh);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 400;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());
	for (int32 i = 0;i < (int32)EParkourArrowType::Max;++i)
	{
		//Enum을 string으로반환
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		CHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);

		switch ((EParkourArrowType)i)
		{
			case EParkourArrowType::Center:
				Arrows[i]->ArrowColor = FColor::Red;
				break;

			case EParkourArrowType::Head:
				Arrows[i]->ArrowColor = FColor::Green;
				Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
				break;

			case EParkourArrowType::Foot:
				Arrows[i]->ArrowColor = FColor::Blue;
				Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
				break;

			case EParkourArrowType::Left:
				Arrows[i]->ArrowColor = FColor::Magenta;
				Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
				break;

			case EParkourArrowType::Right:
				Arrows[i]->ArrowColor = FColor::Magenta;
				Arrows[i]->SetRelativeLocation(FVector(0, 30, 0));
				break;

			case EParkourArrowType::Land:
				//아래를 보도록 회전(Pitch)
				Arrows[i]->ArrowColor = FColor::Yellow;
				Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
				Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
				break;
		}
	}


	//애님클래스지정
	TSubclassOf<UCAnimInstance> Anim;
	CHelpers::GetClass<UCAnimInstance>(&Anim, "AnimBlueprint'/Game/ABP_Character_Player.ABP_Character_Player_C'");
	GetMesh()->SetAnimClass(Anim);

	


	CHelpers::GetAsset<UAnimMontage>(&BackStepMontage, "AnimMontage'/Game/Characters/Montages/BackStep_Montage.BackStep_Montage'");




	CHelpers::GetClass <UCUserWidget_Player>(&Uiclass, "WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");

}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	//부드러운 이동을 사용할것이므로 0속도로 세팅
	GetCharacterMovement()->MaxWalkSpeed = 0;
	Movement->DisableControlRotation();

	FActorSpawnParameters params;
	params.Owner = this;

	HealthPoint->OnHealthPointChanged.AddDynamic(this, &ACPlayer::OnHealthPointUpdate);
	
	AttackPoint->OnAttackPointchanged.AddDynamic(this, &ACPlayer::OnAttackPointUpdate);
	
	
	ManaPoint->OnManaPointChanged.AddDynamic(this, &ACPlayer::OnManaPointUpdate);

	//ui생성
	if (!!Uiclass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		//<UI자료형, 컨트롤러> - 컨트롤러 : 스크린,카메라관련
		UserInterface = CreateWidget<UCUserWidget_Player>(PlayerController, Uiclass);
		UserInterface->AddToViewport();	//뷰포트에 추가함으로써 실제화면에서 보여진다.
		UserInterface->OnUpdateWeapon(EWeaponType::Max);
		UserInterface->UpdateCrossHairVisibility(false);
		UserInterface->OnHealthPointChanged(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
		UserInterface->OnAttackPointChanged(AttackPoint->GetCurrentAttackPoint(), AttackPoint->GetMaxCurrentAttackPoint());
		UserInterface->OnManaPointChanged(ManaPoint->GetMana(), ManaPoint->GetMaxMana());
		UserInterface->HideLockOnText();
	}

	AwakeMaterialDyamic = UMaterialInstanceDynamic::Create(AwakeMaterial, this);

	InitTargetValue = SpringArm->TargetArmLength;

	PostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
	
	//이벤트연결
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	Weapon->OnWeaponChanged.AddDynamic(this, &ACPlayer::OnWeaponTypeChanged);
	

	Movement->OnAirDashStart.AddDynamic(this, &ACPlayer::OnAirDash_Camera);
	Movement->OnAirDashEnd.AddDynamic(this, &ACPlayer::OnAirDash_Camera_End);

	Movement->OnGroundDashStart.AddDynamic(this, &ACPlayer::OnGroundDash_Camera);
	Movement->OnGroundDashEnd.AddDynamic(this, &ACPlayer::OnGroundDash_Camera_End);


	AttackPoint->OnBeginAwaked.AddDynamic(this, &ACPlayer::SetAwakeDatas);
	AttackPoint->OnBeginAwaked.AddDynamic(this, &ACPlayer::SetPostProcessVolumeMaterial);
	

	AttackPoint->OnBeginAwakening.AddDynamic(this, &ACPlayer::SetAwakeMaterial);
	
	AttackPoint->OnAwekening.AddDynamic(this, &ACPlayer::SetAwakeMaterialParam);
	
	AttackPoint->OnEndAwaked.AddDynamic(this, &ACPlayer::InitAwakeDatas);
	AttackPoint->OnEndAwaked.AddDynamic(this, &ACPlayer::InitAwakeMaterial);
	AttackPoint->OnEndAwaked.AddDynamic(this, &ACPlayer::InitPostProcessVolumeMaterial);
	

	for (UMaterialInterface* material : GetMesh()->GetMaterials())
	{
		InitMaterials.Add(material);
	}
	
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetValue != 0)
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetValue, DeltaTime, InterpTime);
		SpringArm->SocketOffset.Z = FMath::FInterpTo(SpringArm->SocketOffset.Z, TargetSocketOffSetZValue, DeltaTime, InterpTime);
	}

	//공중대시 시 bFixedzoom은 true가됨.

	if (Weapon->IsSwordMode())
	{
		

		if (bFixedZoom && bAirDashZoom || bFixedZoom && bGroundDashZoom)
		{
			//SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ZoomValue, DeltaTime, AirDashCameraData.AirDashInterpTime);
			Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, AirDashCameraData.FarFOV, DeltaTime, AirDashCameraData.AirDashInterpTime);
		}

		else if (bAirDashZoom == false || bGroundDashZoom == false)
		{
			//SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, InitTargetValue, DeltaTime, AirDashCameraData.AirDashInterpTime);
			Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, 90, DeltaTime, AirDashCameraData.AirDashInterpTime);
		}
	}
	

	/*Unarmed상태일때*/
	//대쉬 쿨다운
	if (Weapon->IsUnarmedMode())
	{
		CheckNull(Movement);

		if (Movement->GetGroundDashCoolTimeEnd() == false)
		{
			if (!!UserInterface)
				UserInterface->OnPressDashCoolDown(DeltaTime, Movement->GroundDashCoolTime);
		}
	}

	
	/*if (Weapon->IsBowMode())
		CrossHairLineTrace();*/


	if (!!State)
	{
		if (State->IsDamagedMode())
		{
			if(!!Movement)
				Movement->Stop();
		}
			
	}

	EnemyTargeting();
	//Skill CoolDown
	CheckTrue(Weapon->IsUnarmedMode());
	CheckFalse(Weapon->GetSkillActionsNum() > 0);

	if (!!Weapon->GetSkillAction(0))
	{
		if (!!UserInterface)
			UserInterface->OnZSkillCoolDown(DeltaTime, Weapon->GetSkillAction(0)->GetSkillCoolTime(), Weapon->GetSkillAction(0)->GetbInSkill());
	}

	CheckFalse(Weapon->GetSkillActionsNum() > 1);
	if (!!Weapon->GetSkillAction(1))
	{
		if (!!UserInterface)
			UserInterface->OnXSkillCoolDown(DeltaTime, Weapon->GetSkillAction(1)->GetSkillCoolTime(), Weapon->GetSkillAction(1)->GetbInSkill());
	}

	CheckFalse(Weapon->GetSkillActionsNum() > 2);
	if (!!Weapon->GetSkillAction(2))
	{
		if (!!UserInterface)
			UserInterface->OnCSkillCoolDown(DeltaTime, Weapon->GetSkillAction(2)->GetSkillCoolTime(), Weapon->GetSkillAction(2)->GetbInSkill());
	}

	CheckFalse(Weapon->GetSkillActionsNum() > 3);
	if (!!Weapon->GetSkillAction(3))
	{
		if (!!UserInterface)
			UserInterface->OnSpecialSkillCoolDown(DeltaTime, Weapon->GetSkillAction(3)->GetSkillCoolTime(), Weapon->GetSkillAction(3)->GetbInSkill());
	}
}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);

	//movement smooth
	//MoveW
	PlayerInputComponent->BindAction("MoveW", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::PressMoveW);
	PlayerInputComponent->BindAction("MoveW", EInputEvent::IE_Released, Movement, &UCMovementComponent::ReleaseMoveW);
	

	PlayerInputComponent->BindAction("MoveS", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::PressMoveS);
	PlayerInputComponent->BindAction("MoveS", EInputEvent::IE_Released, Movement, &UCMovementComponent::ReleaseMoveS);
	

	PlayerInputComponent->BindAction("MoveD", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::PressMoveD);
	PlayerInputComponent->BindAction("MoveD", EInputEvent::IE_Released, Movement, &UCMovementComponent::ReleaseMoveD);
	

	PlayerInputComponent->BindAction("MoveA", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::PressMoveA);
	PlayerInputComponent->BindAction("MoveA", EInputEvent::IE_Released, Movement, &UCMovementComponent::ReleaseMoveA);
	

	//줌인.줌아웃
	PlayerInputComponent->BindAxis("MouseWheel", this, &ACPlayer::OnMouseWheelUp_Zoom);
	

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);
	

	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnJump);
	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);

	//공격
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	//회피
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	//파쿠르
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSubAction);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffSubAction);

	//v->지면대쉬
	PlayerInputComponent->BindAction("GroundDash", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnGround_Dash);


	//t->타겟팅
	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, Target, &UCTargetComponent::Toggle);
	PlayerInputComponent->BindAction("Target_Left", EInputEvent::IE_Pressed, Target, &UCTargetComponent::MoveLeft);
	PlayerInputComponent->BindAction("Target_Right", EInputEvent::IE_Pressed, Target, &UCTargetComponent::MoveRight);

	/*Z,X,C = 스킬*/
	PlayerInputComponent->BindAction("FirstSkill", EInputEvent::IE_Pressed, this, &ACPlayer::OnZSkill);
	PlayerInputComponent->BindAction("FirstSkill", EInputEvent::IE_Released, this, &ACPlayer::OffZSkill);
	PlayerInputComponent->BindAction("FirstSkill", EInputEvent::IE_Repeat, this, &ACPlayer::OnZSkillCharge);

	PlayerInputComponent->BindAction("SecondSkill", EInputEvent::IE_Pressed, this, &ACPlayer::OnXSkill);
	PlayerInputComponent->BindAction("SecondSkill", EInputEvent::IE_Released, this, &ACPlayer::OffXSkill);

	
	PlayerInputComponent->BindAction("ThirdSkill", EInputEvent::IE_Pressed, this, &ACPlayer::OnCSkill);
	PlayerInputComponent->BindAction("ThirdSkill", EInputEvent::IE_Released, this, &ACPlayer::OffCSkil);

	//V: 궁수모드 시 스페셜스킬
	PlayerInputComponent->BindAction("SpecialSkill", EInputEvent::IE_Pressed, this, &ACPlayer::OnVSkill);
	PlayerInputComponent->BindAction("SpecialSkill", EInputEvent::IE_Repeat, this, &ACPlayer::OnVSkillCharge);
	PlayerInputComponent->BindAction("SpecialSkill", EInputEvent::IE_Released, this, &ACPlayer::OffVSkill);

	/*F->패링, 암살*/
	//PlayerInputComponent->BindAction("Parry", EInputEvent::IE_Pressed, this, &ACPlayer::OnParry);
	PlayerInputComponent->BindAction("Kill", EInputEvent::IE_Pressed, this, &ACPlayer::OnKill);
	/*G->각성*/
	PlayerInputComponent->BindAction("Awake", EInputEvent::IE_Pressed, this, &ACPlayer::OnAwake);

	/*F5->말타기*/
	PlayerInputComponent->BindAction("Ride", EInputEvent::IE_Pressed, this, &ACPlayer::OnRideHorse);

}


void ACPlayer::EnemyTargeting()
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(),
		SphereRadius, ObjectTypes, ActorClassFilter, ActorsIgnore, OutActors);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 32, FColor::Green);

	SetCandidate();
	for (AActor* actor : OutActors)
	{
		ACEnemy_AI* enemy = Cast<ACEnemy_AI>(actor);
		CheckNull(enemy);


		if (bIsShowTargetUI)
		{
			if (enemy == candidate)
				SetTargetLabel(enemy, true);

			else
				SetTargetLabel(enemy, false);
		}
		
		else
		{
			SetTargetLabel(enemy, false);
		}
	}


}


void ACPlayer::OnAction()
{
	
	CheckNull(candidate);
	
	
	CheckTrue(Weapon->IsBowMode());
	CheckFalse(State->IsIdleMode());

	//Candidate를 찾아서 바라보기
	FVector start = GetActorLocation();
	FVector target = candidate->GetActorLocation();



	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

	// Pitch를 제한
	lookAtRotation.Pitch =0.0f;

	// Yaw만 유지하도록 조정
	lookAtRotation.Roll = 0.0f; // Roll은 필요하지 않으므로 0으로 고정

	// 캐릭터 회전 적용
	bUseControllerRotationYaw = false;
	SetActorRotation(lookAtRotation);

	
}

void ACPlayer::SetCandidate()
{
	if (OutActors.Num() == 0)
	{
		candidate = nullptr;
		return;
	}
	CheckFalse(OutActors.Num() > 0);

	float angle = -2.0f;
	
	candidate = nullptr;
	for (AActor* actor : OutActors)
	{
		FVector direction = actor->GetActorLocation() - GetActorLocation();
		direction = direction.GetSafeNormal2D();

		APlayerCameraManager* cameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		CheckNull(cameraManager);
		
		FVector CameraLocation = cameraManager->GetCameraLocation();
		FVector EnemyLocation = actor->GetActorLocation();

		float Distance = FVector::Distance(CameraLocation, EnemyLocation);

		if (Distance > TargetLabelMaxDistance)
		{
			ACEnemy_AI* enemy = Cast<ACEnemy_AI>(actor);
			CheckNull(enemy);
			enemy->SetbShowTargetUI(false);
			return;
		}
		
		
		FVector forward = cameraManager->GetActorForwardVector();
		
		float dot = FVector::DotProduct(direction, forward);

		
		if (dot < dotValue || dot < angle)
			continue;

		angle = dot;
		candidate = Cast<ACharacter>(actor);

	}
}

void ACPlayer::SetTargetLabel(class ACEnemy_AI* Inenemy, bool InbShow)
{
	CheckNull(Inenemy);

	UCStateComponent* Enemystate = CHelpers::GetComponent<UCStateComponent>(Inenemy);
	CheckNull(Enemystate);
	
	if (Enemystate->IsDeadMode())
	{
		Inenemy->SetbShowTargetUI(false);
		return;

	}
	Inenemy->SetbShowTargetUI(InbShow);
}

void ACPlayer::CrossHairLineTrace()
{
	CheckNull(Camera);


	FHitResult hit;

	APlayerCameraManager* cameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	
	FTransform SocketTransform = GetMesh()->GetSocketTransform("crosshair");
	FVector start = SocketTransform.GetLocation();
	

	FVector end = start + (cameraManager->GetActorForwardVector()) * 3000.0f;

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end,ObjectTypes,
		false, ActorsIgnore, EDrawDebugTrace::None, hit, true
	);

	CheckNull(UserInterface);
	if (hit.bBlockingHit)
	{
		ACEnemy* enemy= Cast<ACEnemy>(hit.GetActor());
		if (!!enemy)
		{
			LockOnEnemy = enemy;
			UserInterface->SetCrossHairColorRed();
		}

		else
		{
			LockOnEnemy = nullptr;
			UserInterface->SetCrossHairColorWhite();
		}
			
	}

	else
	{
		LockOnEnemy = nullptr;
		UserInterface->SetCrossHairColorWhite();
	}

}
void ACPlayer::CrossHairLineTrace_Skill_Z()
{
	CheckNull(Camera);


	FHitResult hit;

	APlayerCameraManager* cameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));

	
	FVector start = cameraManager->GetCameraLocation();


	FVector end = start + (cameraManager->GetActorForwardVector()) * 3000.0f;

	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTypes,
		false, ActorsIgnore, EDrawDebugTrace::None, hit, true
	);

	CheckNull(UserInterface);
	if (hit.bBlockingHit)
	{
		ACEnemy* enemy = Cast<ACEnemy>(hit.GetActor());
		if (!!enemy)
			UserInterface->SetCrossHairColorRed();

		else
			UserInterface->SetCrossHairColorWhite();
	}

	else
	{
		UserInterface->SetCrossHairColorWhite();
	}

}
void ACPlayer::OnAwake()
{

	CheckTrue(Weapon->IsUnarmedMode());
	CheckFalse(AttackPoint->CanAwake());

	PlayAnimMontage(AwakeMontage);
	PlayCameraAnimation();
	ExplosionPostProcessSetting();

	UserInterface->OnShowSpecialSkill();

}
float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//부모에서 데미지가깎일수도있으므로 저장
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	
	if (State->IsIdleMode()==false)
	{
		bPlayDamageAnim = false;
		
	}

	else
	{
		//bow모드에서 조준시 hit할 시 애니메이션실행x
		if (Weapon->IsBowMode() && State->IsSubActionMode())
			bPlayDamageAnim = false;

		else
			bPlayDamageAnim = true;
	}

	//BowMode일땐 knockdown 시 startanimation을 ABP에서실행	
	if (Weapon->IsBowMode())
	{
		if (IsKnockdown)
			bPlayDamageAnim = false;
	}
	CheckTrueResult(AttackPoint->bAwakening,0);// 각성몽타주 실행중에는 데미지를받지않음.
	State->SetDamagedMode();

	Movement->SetCanMove(false);
	
	return Damage;
}



//델리게이트에 연결된함수
void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Evade:

			break;

		case EStateType::Damaged:
			Damaged();
			break;
	}
}

void ACPlayer::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	UserInterface->OnUpdateWeapon(InNewType);

	bool bVisible = false;
	if (InNewType == EWeaponType::Bow)
		bIsShowTargetUI = false;

	else
		bIsShowTargetUI = true;
	
	UserInterface->UpdateCrossHairVisibility(bVisible);
}

void ACPlayer::OnAirDash_Camera()
{
	//에어대시 중 카메라 설정
	bFixedZoom = true;
	bAirDashZoom = true;

	ZoomValue = SpringArm->TargetArmLength + (AirDashCameraData.AirDashAxisValue);

}

void ACPlayer::OnAirDash_Camera_End()
{
	//에어대시 중 카메라 설정
	bFixedZoom = false;
	bAirDashZoom = false;


}

void ACPlayer::OnGroundDash_Camera()
{

	UserInterface->OnPressDash();

	//지면대시 중 카메라설정
	bFixedZoom = true;
	bGroundDashZoom = true;

	ZoomValue = SpringArm->TargetArmLength + (AirDashCameraData.AirDashAxisValue);


	
}

void ACPlayer::OnGroundDash_Camera_End()
{
	//지면대시 중 카메라설정
	bFixedZoom = false;
	bGroundDashZoom = false;


}

void ACPlayer::OnMouseWheelUp_Zoom(float InAxis)
{
	
	CheckTrue(bFixedZoom);
	CLog::Print(InAxis, 12000, 5, FColor::Green);
	
	if (InAxis != 0)
	{
		ZoomValue = SpringArm->TargetArmLength + InAxis * (AxisValue);
		TargetValue = FMath::Clamp(ZoomValue, MinTargetArmLength, MaxTargetArmLength);
		
		TargetSocketOffSetZValue = TargetValue / (2.5f);



	}
	
}



void ACPlayer::OnEvade()
{
	//상태가 Idle일때 회피해야함.
	CheckFalse(State->IsIdleMode());
	CheckTrue(Weapon->IsUnarmedMode());
	
	//움직일수있을때만
	CheckFalse(Movement->CanMove());

	//각 이동에 맞춰서 회피동작수행

	if (InputComponent->GetAxisValue("MoveForward") > 0.0f && (InputComponent->GetAxisValue("MoveRight") == 0.0f))
	{
		OnEvade_Forward();
		State->SetEvadeMode();
		return;
	}

	else if (InputComponent->GetAxisValue("MoveRight") > 0.0f && (InputComponent->GetAxisValue("MoveForward") == 0.0f))
	{
		OnEvade_Right();
		State->SetEvadeMode();
		return;
	}

	else if (InputComponent->GetAxisValue("MoveRight") < 0.0f && (InputComponent->GetAxisValue("MoveForward")==0.0f))
	{
		OnEvade_Left();
		State->SetEvadeMode();
		return;
	}


	else if (InputComponent->GetAxisValue("MoveForward") < 0.0f && (InputComponent->GetAxisValue("MoveRight") == 0.0f))
	{
		OnEvade_BackWard();
		State->SetEvadeMode();
		return;
	}

	//모드변경
	
}


void ACPlayer::OnEvade_Right()
{
	CheckTrue(State->IsEvadeMode());

	State->SetEvadeMode();
	Movement->SetCanMove(false);

	if (Weapon->IsSwordMode())
	{
		Move_Right();//BP에서 몽타주 및 이동호출
		return;
	}

	else if (Weapon->IsHammerMode())
	{
		Move_HammerRight();
		return;
	}
	
	else if (Weapon->IsBowMode())
	{
		Move_BowRight();
		return;
	}
	
}

void ACPlayer::OnEvade_Left()
{
	CheckTrue(State->IsEvadeMode());

	State->SetEvadeMode();
	Movement->SetCanMove(false);

	if (Weapon->IsSwordMode())
	{
		Move_Left();//BP에서 몽타주 및 이동호출
		return;
	}

	else if (Weapon->IsHammerMode())
	{
		Move_HammerLeft();
		return;
	}

	else if (Weapon->IsBowMode())
	{
		Move_BowLeft();
		return;
	}
}

void ACPlayer::OnEvade_Forward()
{
	CheckTrue(State->IsEvadeMode());

	State->SetEvadeMode();
	Movement->SetCanMove(false);

	if (Weapon->IsSwordMode())
	{
		Move_Forward();//BP에서 몽타주 및 이동호출
		return;
	}
	
	else if (Weapon->IsHammerMode())
	{
		Move_HammerForward();
		return;
	}

	else if (Weapon->IsBowMode())
	{
		Move_BowForward();
		return;
	}
}

void ACPlayer::OnEvade_BackWard()
{
	CheckTrue(State->IsEvadeMode());

	State->SetEvadeMode();
	Movement->SetCanMove(false);

	if (Weapon->IsSwordMode())
	{
		Move_Backward();//BP에서 몽타주 및 이동호출
		return;
	}
	
	else if (Weapon->IsHammerMode())
	{
		Move_HammerBackward();
		return;
	}

	else if (Weapon->IsBowMode())
	{
		Move_BowBackward();
		return;
	}
}

void ACPlayer::BackStep()
{

	/*if (bEquipped==false)
		Movement->EnableControlRotation();*/

	PlayAnimMontage(BackStepMontage);

}

void ACPlayer::End_Backstep()
{/*
	if (bEquipped == false)
		Movement->EnableControlRotation();*/

	State->SetIdleMode();
	Movement->SetCanMove(true);
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
}

void ACPlayer::OnCollision()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	CheckNull(Capsule);

	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Block);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));
}

void ACPlayer::OffCollision()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	CheckNull(Capsule);

	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	
}

void ACPlayer::OffCollision_Sword()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	CheckNull(Capsule);

	Capsule->SetCollisionResponseToChannel(SwordCollisonChannel, ECR_Ignore);

}

void ACPlayer::OnCollision_Sword()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	CheckNull(Capsule);

	Capsule->SetCollisionResponseToChannel(SwordCollisonChannel, ECR_Overlap);

}

void ACPlayer::OnSubAction()
{
	if (Weapon->IsUnarmedMode())
	{
		//Unarmed일때만 파쿠르 수행가능.
		CheckFalse(State->IsIdleMode());
		Parkour->DoParkour();

		return;
	}


	if(Weapon->GetSubAction()->OnPressed.IsBound()==false)
		Weapon->GetSubAction()->OnPressed.AddDynamic(this, &ACPlayer::OnPressedSubAction);

	//무기를 쥐고있는상황이라면
	Weapon->SubAction_Pressed();
	


}

void ACPlayer::OffSubAction()
{
	CheckTrue(Weapon->IsUnarmedMode());

	if (Weapon->GetSubAction()->OnReleased.IsBound() == false)
		Weapon->GetSubAction()->OnReleased.AddDynamic(this, &ACPlayer::OnReleasedSubAction);

	Weapon->SubAction_Released();
}


void ACPlayer::OnParry()
{
	CheckTrue(Weapon->IsUnarmedMode());
	CheckFalse(State->IsIdleMode());

	if (Weapon->GetSkillAction(3))
	{
		Weapon->Skill_Pressed(3);
		State->OnSkillActionMode();
	}
		
}



void ACPlayer::OnZSkill()
{
	CheckTrue(Weapon->IsUnarmedMode());

	if (IsKnockdown)
	{

		//스킬취소
		Weapon->Skill_Canceled(0);
		UserInterface->OnHideChargeUI();
		return;
	}

	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillActionMode());
	

	CheckTrue(Weapon->GetSkillAction(0)->GetbInSkill());	//skill쿨타임중에는 스킬사용불가
	Weapon->Skill_Pressed(0);
	


	if (Weapon->GetSkillActionIsChargetAttack(0))
	{
		UserInterface->OnShowChargeUI();
		OnPressedSubAction();
		return;

	}
	CheckTrue(Weapon->GetSkillActionIsChargetAttack(0));	
	
	//ChargeAttack이 아닐경우
	State->SetActionMode();
	UserInterface->OnPressZSkill();


	
}

void ACPlayer::OffZSkill()
{

	CheckTrue(Weapon->IsUnarmedMode());
	UserInterface->OnHideChargeUI();

	CheckNull(Weapon->GetAttachment());
	Weapon->GetAttachment()->OffCharge();

	if (IsKnockdown)
	{

		//스킬취소
		Weapon->Skill_Canceled(0);
		return;
	}

	CheckTrue(Weapon->GetSkillAction(0)->GetbInSkill());

	Weapon->Skill_Released(0);

	CheckFalse(Weapon->GetSkillActionIsChargetAttack(0));	//ChargeAttack일때만 아랫줄수행
	UserInterface->OnPressZSkill();

	
	

	
	OnReleasedSubAction();
}

/*차징스킬일때 호출*/
void ACPlayer::OnZSkillCharge()
{
	CheckTrue(Weapon->IsUnarmedMode());

	if (IsKnockdown)
	{

		//스킬취소
		Weapon->Skill_Canceled(0);
		return;
	}

	CheckFalse(State->IsSkillActionMode());
	CheckTrue(Weapon->GetSkillAction(0)->GetbInSkill());	//skill쿨타임중에는 스킬사용불가
	CheckFalse(Weapon->GetSkillActionIsChargetAttack(0));
	
	
	
	Weapon->Skill_Repeat(0);
	UserInterface->OnPressingCharge(Weapon->GetSkillAction(0)->GetChargePower());
	
	CheckNull(Weapon->GetAttachment());
	Weapon->GetAttachment()->OnCharging();

	if (Weapon->GetSkillAction(0)->GetChargePower() ==1 ||
		Weapon->GetSkillAction(0)->GetChargePower()==20||
		Weapon->GetSkillAction(0)->GetChargePower() == 30)
	{
		CheckNull(Weapon->GetAttachment());
		Weapon->GetAttachment()->OnCharge();
	}
	
}

void ACPlayer::OnXSkill()
{
	CheckTrue(Weapon->IsUnarmedMode());
	CheckFalse(State->IsIdleMode());
	CheckTrue(Weapon->GetSkillAction(1)->GetbInSkill());	//skill쿨타임중에는 스킬사용불가
	Weapon->Skill_Pressed(1);
	State->OnSkillActionMode();

	UserInterface->OnPressXSkill();

	
}

void ACPlayer::OffXSkill()
{
	CheckTrue(Weapon->IsUnarmedMode());
	Weapon->Skill_Released(1);
}

void ACPlayer::OnCSkill()
{
	CheckTrue(Weapon->IsUnarmedMode());
	CheckTrue(Weapon->GetSkillAction(2)->GetbInSkill());	//skill쿨타임중에는 스킬사용불가
	Weapon->Skill_Pressed(2);
	State->OnSkillActionMode();

	UserInterface->OnPressCSkill();


}

void ACPlayer::OffCSkil()
{
	CheckTrue(Weapon->IsUnarmedMode());
	Weapon->Skill_Released(2);
}

void ACPlayer::OnVSkill()
{
	CheckTrue(Weapon->IsUnarmedMode());
	CheckFalse(Weapon->IsBowMode());
	CheckFalse(AttackPoint->IsAwake());
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillActionMode());

	CheckTrue(Weapon->GetSkillAction(3)->GetbInSkill());	//skill쿨타임중에는 스킬사용불가
	
	Weapon->Skill_Pressed(3);

	UserInterface->ShowLockOnText();

	if (Weapon->GetSkillActionIsChargetAttack(3))
	{
		//UserInterface->OnShowChargeUI();
		OnPressedSubAction();
		return;

	}
	CheckTrue(Weapon->GetSkillActionIsChargetAttack(3));

	//ChargeAttack이 아닐경우

	
	State->SetActionMode();
	

}
void ACPlayer::OffVSkill()
{
	CheckTrue(Weapon->IsUnarmedMode());
	CheckFalse(Weapon->IsBowMode());
	CheckFalse(AttackPoint->IsAwake());
	CheckTrue(Weapon->GetSkillAction(3)->GetbInSkill());

	Weapon->Skill_Released(3);

	CheckFalse(Weapon->GetSkillActionIsChargetAttack(3));	//ChargeAttack일때만 아랫줄수행
	
	UserInterface->OnPressSpecialSkill();
	UserInterface->HideLockOnText();
	
	CheckNull(Weapon->GetAttachment());
	Weapon->GetAttachment()->OffCharge();


}
void ACPlayer::OnVSkillCharge()
{


	CheckTrue(Weapon->IsUnarmedMode());
	CheckFalse(Weapon->IsBowMode());
	CheckFalse(AttackPoint->IsAwake());
	CheckFalse(State->IsSkillActionMode());
	CheckTrue(Weapon->GetSkillAction(3)->GetbInSkill());	//skill쿨타임중에는 스킬사용불가
	CheckFalse(Weapon->GetSkillActionIsChargetAttack(3));


	Weapon->Skill_Repeat(3);
	//UserInterface->OnPressingCharge(Weapon->GetSkillAction(3)->GetChargePower());

	CheckNull(Weapon->GetAttachment());
	//Weapon->GetAttachment()->OnCharging();

}
void ACPlayer::Landed(const FHitResult& Hit)
{
	CheckFalse(State->IsIdleMode());
	Parkour->DoParkour(true);
}

void ACPlayer::OnPressedSubAction()
{
	

	bool bVisible = false;

	if (Weapon->GetType()==EWeaponType::Bow)
		bVisible = true;

	UserInterface->UpdateCrossHairVisibility(bVisible);
}

void ACPlayer::OnReleasedSubAction()
{
	bool bVisible = false;

	UserInterface->UpdateCrossHairVisibility(bVisible);
}

void ACPlayer::Damaged()
{
	//Apply Damage
	if (!!UserInterface)
		UserInterface->PlayDamageEffect();

	
	Movement->SetCanMove(false);
	HealthPoint->Damage(DamageData.Power);
	DamageData.Power = 0.0f;	//사용했으므로 초기화
	
	
	{
		Change_Color();

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "Restore_Color");
		//다시되돌리기
		GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle, timerDelegate, 0.2f, false);
	}

	
	CheckTrue(State->IsSkillActionMode());
	CheckTrue(IsKnockdown);

	bHit = true;
	//PlayAnimMontage(montage, playRate);
	if (bPlayDamageAnim==false)
	{
		Movement->SetbCanMove(false);
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda([=] {

			End_Damaged();
		});

		GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.5f, false);
	}
	//방향계산
	FVector ForwardVector = GetActorForwardVector();
	FVector ToHitVector = (CollisionPoint - GetActorLocation()).GetSafeNormal();

	// 내적을 사용하여 각도 계산
	float DotProduct = FVector::DotProduct(ForwardVector, ToHitVector);
	float Angle = FMath::Acos(DotProduct) * (180.0f / PI); // 라디안을 도로 변환

	// 외적을 사용하여 방향 판단
	FVector RightVector = FVector::CrossProduct(ForwardVector, FVector::UpVector); // 두 벡터의 외적
	float Direction = FVector::DotProduct(RightVector, ToHitVector);

	// 각도 방향 조정
	if (Direction < 0) {
		Angle = -Angle; // 왼쪽 방향
	}

	
	// 최종 각도 정규화
	HitValue = NormalizeAngle(Angle);


	


	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* hitData = DamageData.Event->HitData;

		TArray<FHitData> PlayerHitDatas;

		PlayerHitDatas = Weapon->GetHitDatas();

		FHitData PlayerHitData = {};
		UAnimMontage* montage = PlayerHitData.Montage;

		if (PlayerHitDatas.Num() > 0)
		{
			PlayerHitData = PlayerHitDatas[0];
			montage = PlayerHitData.Montage;

		}
		

		float playRate = hitData->PlayRate;

		if (montage == nullptr)
		{
			montage = DamagedMontage;
			playRate = 1.5f;
		}

		
		
		//hitData->PlayHitStop(GetWorld());		//히트스탑
		hitData->PlaySoundWave(this);	//플레이사운드
		hitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		//살아있을때만 처리
		if (HealthPoint->IsDead() == false)
		{

			FVector start = GetActorLocation();
			//eventInstagtor->플레이어의 controller반환
			FVector target = DamageData.Attacker->GetActorLocation();	//플레이어

			FVector direction = target - start;
			direction.Normalize();

			//그 반대로 밀어야하므로 -곱함
			LaunchCharacter(-direction * hitData->Launch, false, false);

			//바라보기
			//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));


		}
	}

	if (HealthPoint->IsDead())
	{
		State->SetDeadMode();
		return;
	}

	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;


}

void ACPlayer::End_Damaged()
{
	
	IsKnockdown = false;
	bHit = false;
	bPlayDamageAnim = false;
	

	Movement->SetCanMove(true);
	State->SetIdleMode();

	
	IsKnockdown = false;
	SetGuard(true);
}

void ACPlayer::TakeAttackPoint(float InAmount)
{
	CheckTrue(AttackPoint->IsAwake());
	AttackPoint->SetAttackPoint(InAmount);
}


float ACPlayer::NormalizeAngle(float Angle)
{
	while (Angle > 180.0f) Angle -= 360.0f;
	while (Angle < -180.0f) Angle += 360.0f;
	return Angle;
}

void ACPlayer::HideUI()
{
	UserInterface->HideUI();
	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "ShowUI");


	GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle, timerDelegate,22.f, false);

}

void  ACPlayer::ShowUI()
{
	UserInterface->ShowUI();
}

void ACPlayer::OnHealthPointUpdate(float InHealth, float InMaxHealth)
{

	UserInterface->OnHealthPointChanged(InHealth, InMaxHealth);

}

void ACPlayer::OnAttackPointUpdate(float InAttackPoint, float InMaxAttackPoint)
{

	UserInterface->OnAttackPointChanged(InAttackPoint, InMaxAttackPoint);

}

void ACPlayer::OnManaPointUpdate(float InMana, float InMaxMana)
{

	UserInterface->OnManaPointChanged(InMana, InMaxMana);

}

void ACPlayer::Change_Color()
{
	MaterialIndex = 0;

	while (true)
	{

		MaterialIndex = FMath::Clamp(MaterialIndex, 0, 15);
		GetMesh()->SetMaterial(MaterialIndex, HitMaterial);

		if (MaterialIndex == 15)
			break;


		++MaterialIndex;
		
		
		
		

	}
	
}

void ACPlayer::Restore_Color()
{

	MaterialIndex = 0;

	while (true)
	{

		MaterialIndex = FMath::Clamp(MaterialIndex, 0, 15);
		GetMesh()->SetMaterial(MaterialIndex, InitMaterials[MaterialIndex]);

		if (MaterialIndex == 15)
			break;


		++MaterialIndex;



	}

}

bool ACPlayer::IsAwakeMode()
{
	CheckNullResult(AttackPoint,false);
	bool bReturn = AttackPoint->IsAwake();

	return bReturn;
}
void ACPlayer::SetAwakeDatas()
{
	if (!!Weapon)
	{
		if (Weapon->IsSwordMode())
		{
			Weapon->GetDoAction()->SetDatas(DoActionDatas, HitDatas);
		}

		if (Weapon->IsBowMode())
		{
			Weapon->GetDoAction()->SetDatas(DoActionBowDatas, HitBowDatas);
			Weapon->GetDoAction()->SetArrowClass();
		}
	}

	AwakeDashLine();

	if (!!ManaPoint)
		ManaPoint->SetAddValue(0.1f);

}

void ACPlayer::OnKill()
{
	CheckNull(candidate);
	
	IIStatable* ai = Cast<IIStatable>(candidate);
	CheckNull(ai);

	//Kill을 위한 거리체크
	float CurrentDistance = FVector::Distance(GetActorLocation(),candidate->GetActorLocation());
	CheckFalse(CurrentDistance < MinDistance);

	//Kill을 위한 앞,뒤체크
	FVector direction = candidate->GetActorLocation() - GetActorLocation();

	float IsFront = FVector::DotProduct(GetActorForwardVector(), direction);
	CheckTrue(IsFront < MinDot);
	
	//Enemy가 player와 같은 정면을 응시하고있는지 체크
	float IsEnemyLookFront = FVector::DotProduct(GetActorForwardVector(), candidate->GetActorForwardVector());

	CheckFalse(IsEnemyLookFront > MinDot);
	
	
	FVector target = GetMesh()->GetSocketLocation("KillEnemy");

	candidate->SetActorLocation(FVector(target.X, target.Y, candidate->GetActorLocation().Z));
	StartKillMode();
	ai->StartKillMode();
	



}

void ACPlayer::StartKillMode()
{
	CLog::Log("Player_KillMode");
	PlayAnimMontage(HideAttackMontage);
}

void ACPlayer::SetAwakeMaterial()
{
	CheckNull(AwakeMaterialDyamic);
	GetMesh()->SetOverlayMaterial(AwakeMaterial);

}

void ACPlayer::SetAwakeMaterialParam(float Value)
{
	CheckNull(AwakeMaterialDyamic);
	AwakeMaterialDyamic->SetVectorParameterValue("Color", FVector4(Value, 0.0f, 0.8f, 1.0f));

	GetMesh()->SetOverlayMaterial(AwakeMaterial);
}

void ACPlayer::SetPostProcessVolumeMaterial()
{
	CheckNull(PostProcessVolume);
	CheckNull(PostProcessMaterial);

	

	PostProcessMaterialDyamic = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
	CheckNull(PostProcessMaterialDyamic);

	PostProcessVolume->Settings.WeightedBlendables.Array.Empty();

	FWeightedBlendable NewBlendable;
	NewBlendable.Object = PostProcessMaterialDyamic;
	NewBlendable.Weight = 1.0f;

	PostProcessVolume->Settings.WeightedBlendables.Array.Add(NewBlendable);

}

void ACPlayer::InitPostProcessVolumeMaterial()
{
	CheckNull(PostProcessVolume);
	PostProcessVolume->Settings.WeightedBlendables.Array.Empty();

}


void ACPlayer::InitAwakeDatas()
{
	UserInterface->OffShowSpecialSkill();

	if (!!Weapon)
	{
		if (Weapon->IsSwordMode())
		{
			Weapon->GetDoAction()->InitDatas();
		}

		if (Weapon->IsBowMode())
		{
			Weapon->GetDoAction()->InitDatas();
			Weapon->GetDoAction()->InitArrowClass();
		}
	}

	StopDashLine();

	if (!!ManaPoint)
		ManaPoint->SetAddValue(0.01f);
}

void ACPlayer::InitAwakeMaterial()
{
	GetMesh()->SetOverlayMaterial(nullptr);


}


void ACPlayer::SpawnGhostTrail()
{
	CheckFalse(AttackPoint->IsAwake());
	if (!!GhostTrailClass)
	{
		FVector location = GetActorLocation();
		location.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FActorSpawnParameters params;
		params.Owner = this;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	//충돌체가있어도 무조건 생성

		FTransform transform;
		transform.SetTranslation(location);

		GhostTrail = GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, params);
	}

}


void ACPlayer::OnRideHorse()
{
	CheckNull(HorseClass);
	CheckNull(HorseCallingMontage);

	

	PlayAnimMontage(HorseCallingMontage);
	bRide = true;
	
	
	
}

void ACPlayer::OffRideHorse()
{
	State->SetIdleMode();

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 500);

	TArray<AActor*> ignores;
	ignores.Add(this);

	bRide = false;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End,
		ETraceTypeQuery::TraceTypeQuery1,
		false, ignores, EDrawDebugTrace::None, DisMountresult, true);


}

void ACPlayer::SpawnHorse()
{
	State->SetRideMode();
	
	FActorSpawnParameters spawnParams;

	HorseToRide = GetWorld()->SpawnActor<ACHorse>(HorseClass, GetActorLocation(), GetActorRotation(), spawnParams);
	
	CheckNull(HorseToRide);
	HorseToRide->SetOwnerCharacter(this);

	AttachToComponent(HorseToRide->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "MountPoint");

	GetController()->Possess(HorseToRide);


}

void ACPlayer::DisMountHorse()
{


	if (DisMountresult.bBlockingHit)
	{
		DisMountLocation = FVector(GetActorLocation().X, GetActorLocation().Y, DisMountresult.ImpactPoint.Z);
		SetActorLocation(DisMountLocation);
	}


}

void ACPlayer::SetBlockSuccess()
{
	CheckNull(BlockMaterial);
	GetMesh()->SetOverlayMaterial(BlockMaterial);

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=] {

		GetMesh()->SetOverlayMaterial(nullptr);
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.2f, false);
}

void ACPlayer::OnJump()
{
	SetIsJump(true);
}

void ACPlayer::SetIsKnockdown(bool bInIsKnockdown)
{
	Movement->Stop();
	IsKnockdown = bInIsKnockdown;

	if (bInIsKnockdown == false)
	{
		UCSubAction* subAction = Weapon->GetSubAction();
		CheckNull(subAction);

		subAction->Cancel_Guard();

	}
}

void ACPlayer::SetGuard(bool bInGuard)
{
	CLog::Log("SetGuard!");
	ACAttachment* attachment = Weapon->GetAttachment();
	CheckNull(attachment);

	attachment->SetbGuard(bInGuard);
}

void ACPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ChangeColor_TimerHandle);	//꼭 타이머 핸들 없애주기
	
}

