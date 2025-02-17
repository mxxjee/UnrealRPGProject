// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CShockWave.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/CPlayer.h"

ACShockWave::ACShockWave()
{
 	
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box");
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->ProjectileGravityScale = 0.0f;
	
}


void ACShockWave::BeginPlay()
{
	Super::BeginPlay();

	/*Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/

	//Box->OnComponentBeginOverlap.AddDynamic(this, &ACShockWave::OnComponentOverlap);
	Projectile->SetActive(false);
}

void ACShockWave::Shoot(const FVector& InForward)
{
	SetLifeSpan(LifeSpan);

	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->SetActive(true);

	Projectile->bRotationFollowsVelocity = true;
	
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]
	{
		Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//다시켜주기
	});

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.05f, false);

}

void ACShockWave::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	ACPlayer* player = Cast<ACPlayer>(character);
	CheckNull(player);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);	//Enemy_Boss와 이 함수에 들어온 OtherActor가 같으면 밑에줄실행x

	CheckNull(GetOwner());

	ACharacter* owner =Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	player->SetIsKnockdown(true);
	hitdata.SendDamage(owner, nullptr, player);

	FVector start = player->GetActorLocation();
	FVector target = GetActorLocation();

	FVector direction = target - start;
	direction.Normalize();

	//그 반대로 밀어야하므로 -곱함
	player->LaunchCharacter(-direction * Launch, false, false);

}