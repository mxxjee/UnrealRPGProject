
#include "Weapons/AddOns/CArrow.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


ACArrow::ACArrow()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	

	Projectile->ProjectileGravityScale = 0.0f;

	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;//시뮬레이션 중 히트이벤트 생성 true
	Capsule->SetCollisionProfileName("Arrow");

	
}


void ACArrow::BeginPlay()
{
	Super::BeginPlay();
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACArrow::OnComponentOverlap);
	Projectile->SetActive(false);

	UStaticMeshComponent* Meshcomp = CHelpers::GetComponent<UStaticMeshComponent>(this);

	UMaterialInterface* material = Meshcomp->GetMaterial(0);

	DynamicMaterial = UMaterialInstanceDynamic::Create(material, this);

	Meshcomp->SetMaterial(0, DynamicMaterial);
	
	
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);

}

void ACArrow::Shoot(const FVector& InForward)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Shootcue, GetActorLocation());

	SetLifeSpan(5);
	SetNiagara(true);
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->SetActive(true);

	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0.8f;
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate=FTimerDelegate::CreateLambda([&]
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//다시켜주기
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);
	
}

void ACArrow::Shoot(const FVector& InForward, float Speed,int InOverlapCount)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Shootcue, GetActorLocation());

	if (Speed >= 500)
	{
		bOverlap = true;
		Capsule->SetCollisionProfileName("OverlapAll");
		OverlapCount = InOverlapCount;
	}
	SetLifeSpan(5);
	SetChargeSkillNiagara(true);
	Projectile->InitialSpeed += Speed;
	Projectile->Velocity = InForward * Projectile->InitialSpeed;

	CLog::Log(Projectile->InitialSpeed);
	Projectile->SetActive(true);

	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0.8f;
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//다시켜주기
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);

}

void ACArrow::SkillShoot(const FVector& InForward, float Speed)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Shootcue, GetActorLocation());

	SetNiagara(true);
	SetLifeSpan(5);
	SetSkillNiagara(true);
	Projectile->InitialSpeed += Speed;
	Projectile->Velocity = InForward * Projectile->InitialSpeed;

	Projectile->SetActive(true);

	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0;

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//다시켜주기
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);

}

void ACArrow::SkillIceShoot(const FVector& InForward, float Speed)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Shootcue, GetActorLocation());

	SetNiagara(true);
	SetLifeSpan(5);
	SetIceSkillNiagara(true);
	Projectile->InitialSpeed += Speed;
	Projectile->Velocity = InForward * Projectile->InitialSpeed;

	Projectile->SetActive(true);

	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0;

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//다시켜주기
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);

}

void ACArrow::ShootAwake(const FVector& InForward)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Shootcue, GetActorLocation());

	SetLifeSpan(5);
	SetAwakeNiagara(true);
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->SetActive(true);

	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0;
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//다시켜주기
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);

}

void ACArrow::ShootSpecialSkill(const FVector& InForward)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpecialCue, GetActorLocation());

	
	SetLifeSpan(3);
	SetAwakeNiagara(true);
	Projectile->Velocity = InForward * Projectile->InitialSpeed;

	CLog::Log(Projectile->InitialSpeed);
	Projectile->SetActive(true);

	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0.8f;
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//다시켜주기
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);

}


void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);


	SetNiagara(false);
	if (!!character)
		SetLifeSpan(0.05f); //즉시삭제

	else
	{
		SetLifeSpan(LifeSpanAfterCollision);
		if (OnFloorHitArrow.IsBound())
			OnFloorHitArrow.Broadcast(this);
		return;
	}

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);	//Player와 이 함수에 들어온 OtherActor가 같으면 밑에줄실행x

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!!character && OnHitArrow.IsBound())
		OnHitArrow.Broadcast(this, character);		//연결될 함수에 InCauser,InotherCharacter
	
	
	

}


void ACArrow::SetArrowMaterial(FLinearColor InColor)
{
	
	CheckNull(DynamicMaterial);
	//InColor
	DynamicMaterial->SetVectorParameterValue("Color", InColor);

}
void ACArrow::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);	//Player와 이 함수에 들어온 OtherActor가 같으면 밑에줄실행x

	

	++CurrentOverlapCount;

	CLog::Log(CurrentOverlapCount);
	if (!!character && OnHitArrow.IsBound())
		OnHitArrow.Broadcast(this, character);


	if (CurrentOverlapCount == OverlapCount)
	{
		SetNiagara(false);

		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(0.05f);

		OverlapCount = 0;
	}
}

