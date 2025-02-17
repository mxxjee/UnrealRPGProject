// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CItem.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "Characters/CPlayer.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CManaPointComponent.h"

// Sets default values
ACItem::ACItem()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");

	Capsule->SetCollisionProfileName("OverlapAll");
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;


}

// Called when the game starts or when spawned
void ACItem::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACItem::OnComponentOverlap);
}



void ACItem::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	
	ACPlayer* player = Cast<ACPlayer>(character);
	CheckNull(player);

	switch (Type)
	{
		case EITemType::HEAL:
		{
			UCHealthPointComponent* health = CHelpers::GetComponent<UCHealthPointComponent>(player);
			CheckNull(health);
			health->Heal(10);

			PlayEffect(character);
			Destroy();
			break;
		}
		
		case EITemType::MANA:
		{
			UCManaPointComponent* mana = CHelpers::GetComponent<UCManaPointComponent>(player);
			CheckNull(mana);
			mana->SetAddManaPoint(20);

			PlayEffect(character);
			Destroy();
			break;
		}
	}


}