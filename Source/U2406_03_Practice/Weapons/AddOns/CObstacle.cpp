// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CObstacle.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/DecalComponent.h"
#include "Characters/CPlayer.h"


ACObstacle::ACObstacle()
{
	
	//CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle");
	CHelpers::CreateComponent<UDecalComponent>(this, &Decal, "Decal",Particle);

	//Capsule->SetCollisionProfileName("OverlapAll");
	//Capsule->BodyInstance.bNotifyRigidBodyCollision = true;

	//Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Decal->SetVisibility(false);


}

// Called when the game starts or when spawned
void ACObstacle::BeginPlay()
{
	Super::BeginPlay();
	//Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACObstacle::OnComponentOverlap);
}



void ACObstacle::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	ACPlayer* player = Cast<ACPlayer>(character);
	CheckNull(player);

	ACharacter* owner = Cast<ACharacter>(GetOwner());
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

void ACObstacle::ShowDecals()
{
	CheckNull(Decal);
	Decal->SetVisibility(true);
}
