
#include "Weapons/CSword.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"




ACSword::ACSword()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Mesh);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/Weapons/Sword/SK_Sword.SK_Sword'");
	Mesh->SetSkeletalMesh(mesh);

	//Ä¸½¶ ¼³Á¤
	Capsule->SetRelativeLocation(FVector(-13, -130, 0));
	Capsule->SetRelativeRotation(FRotator(0, 0, 90));
	Capsule->SetCapsuleHalfHeight(220.0f);
	Capsule->SetCapsuleRadius(11.0f);


}


void ACSword::BeginPlay()
{
	Super::BeginPlay();

	OffCollision();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACSword::OnBeginOverlap);
}

void ACSword::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent e;



	//ºí·çÇÁ¸°Æ®ÀÇ ApplyDamagage
	OtherActor->TakeDamage(20, e, GetOwner()->GetInstigatorController(), this);

	

	
	
}

void ACSword::OnCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACSword::OffCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}