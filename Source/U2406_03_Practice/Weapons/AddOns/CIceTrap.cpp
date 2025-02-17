// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CIceTrap.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/CStateComponent.h"
// Sets default values
ACIceTrap::ACIceTrap()
{
 
	
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");

	Capsule->SetCollisionProfileName("OverlapAll");
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACIceTrap::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACIceTrap::OnComponentOverlap);
}

void ACIceTrap::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);


	
	ACEnemy* enemy = Cast<ACEnemy>(character);
	CheckNull(enemy);
	
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(enemy);
	if (!!boss)
	{
		UCStateComponent* bossState = CHelpers::GetComponent<UCStateComponent>(boss);
		CheckNull(bossState);

		if (bossState->IsSkillActionMode())
		{
			boss->SpawnInvincibleUI();
			return;
		}
	}

	hitdata.SendDamage(owner, nullptr, enemy);
	
}



