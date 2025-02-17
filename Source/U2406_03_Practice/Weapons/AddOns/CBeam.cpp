

#include "Weapons/AddOns/CBeam.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Characters/CPlayer.h"

ACBeam::ACBeam()
{
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Niagara);

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACBeam::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACBeam::OnOverlap);
}


void ACBeam::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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