// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CLance.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Characters/CPlayer.h"

ACLance::ACLance()
{
 
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");

}

void ACLance::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACLance::OnComponentOverlap);
	
}

void ACLance::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	ACPlayer* player = Cast<ACPlayer>(character);
	CheckNull(player);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);	//Enemy_Boss와 이 함수에 들어온 OtherActor가 같으면 밑에줄실행x

	CheckNull(GetOwner());

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	hitdata.SendDamage(owner, nullptr, player);


}