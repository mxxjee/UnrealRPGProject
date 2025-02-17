// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CMagicBall.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACMagicBall::ACMagicBall()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->ProjectileGravityScale = 0.0f;

	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;//�ùķ��̼� �� ��Ʈ�̺�Ʈ ���� true
	Capsule->SetCollisionProfileName("Arrow");
}

// Called when the game starts or when spawned
void ACMagicBall::BeginPlay()
{
	Super::BeginPlay();
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Capsule->OnComponentHit.AddDynamic(this, &ACMagicBall::OnComponentHit);
	Projectile->SetActive(false);
}

void ACMagicBall::Shoot(const FVector& InForward)
{
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Shootcue, GetActorLocation());

	SetLifeSpan(5);

	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->SetActive(true);
	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0.8f;
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//�ٽ����ֱ�
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);

}

void ACMagicBall::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);


	if (!!character)
		SetLifeSpan(0.05f); //��û���

	else
	{
		SetLifeSpan(LifeSpanAfterCollision);
		return;
	}

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);	//Enemy_Boss�� �� �Լ��� ���� OtherActor�� ������ �ؿ��ٽ���x

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!!character && OnHitMagicBall.IsBound())
		OnHitMagicBall.Broadcast(this, character);		//����� �Լ��� InCauser,InotherCharacter




}
void ACMagicBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnMagicBallEndPlay.IsBound())
		OnMagicBallEndPlay.Broadcast(this);

}