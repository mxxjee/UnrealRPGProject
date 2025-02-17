
#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSubAction.h"
#include "Characters/CPlayer.h"
#include "Components/CapsuleComponent.h"

ACAttachment::ACAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
}


void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);

	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);
		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
			Collisions.Add(shape);
		}
	}

	OffCollisions();
	Super::BeginPlay();
	

}



void ACAttachment::OnCollisions()
{
	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAttachment::OffCollisions()
{
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	if (player)
	{
		CheckNull(CHelpers::GetComponent<UCStateComponent>(player));

		//일반공격일경우
		if (CHelpers::GetComponent<UCStateComponent>(player)->IsSkillActionMode() == false)
		{
			if (OnAttachmentEndCollision.IsBound())
				OnAttachmentEndCollision.Broadcast();
		}

		else
		{
			if (OnAttachmentEndCollisionSkill.IsBound())
			{
				OnAttachmentEndCollisionSkill.Broadcast();
			}
				
		}


	}
	
	else
	{
		if (OnAttachmentEndCollision.IsBound())
			OnAttachmentEndCollision.Broadcast();

	}
	
	for (UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::AttachToCollision(FName InCollisionName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InCollisionName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InCollisionName);
			return;
		}
	}
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	CheckTrue(OwnerCharacter == OtherActor);
	
	
	
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	//플레이어가 공격했을 경우에는 일반공격과 스킬공격을 구분해서 이벤트호출
	if (player)
	{
		CheckNull(CHelpers::GetComponent<UCStateComponent>(player));
		bool bGuard = CheckGuardState(OtherActor);

		if (bGuard == false)
		{
			//일반공격일경우
			if (CHelpers::GetComponent<UCStateComponent>(player)->IsSkillActionMode() == false)
			{
				if (OnAttachmentBeginOverlap.IsBound())
				{
					OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor), SweepResult, OverlappedComponent, OtherComp);

				}
			}

			else
			{
				/*스킬타입에 따라 다른 함수호출*/
				if (OnAttachmentBeginOverlapSkill.IsBound())
				{

					OnAttachmentBeginOverlapSkill.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor), SweepResult, OverlappedComponent, OtherComp);

				}
			}
		}
		
		else
		{
			bool bCheckAngle = GetAngle(OwnerCharacter, OtherActor);
			if (bCheckAngle)
			{
				//가드취소호출
				CancleGuard(OtherActor);

				if (OnAttachmentBeginOverlap.IsBound())
				{
					OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor), SweepResult, OverlappedComponent, OtherComp);

				}


			}

			else
			{
				Guard(OtherActor);
			}
		}
	}
	
	//Enemy공격 , Player피해
	else
	{
		bool bGuard = CheckGuardState(OtherActor);
		/*가드상태를 체크한 뒤 데미지 적용여부*/
		
		//가드상태가 아니라면
		if (bGuard == false)
		{
			CancleGuard(OtherActor);
			if (OnAttachmentBeginOverlap.IsBound())
			{
				OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor), SweepResult, OverlappedComponent, OtherComp);

			}
		}

		//가드상태라면, 앞/뒤 체크
		else
		{
			bool bCheckAngle = GetAngle(OwnerCharacter, OtherActor);

			if (bCheckAngle)
			{
				//가드취소호출
				CancleGuard(OtherActor);

				if (OnAttachmentBeginOverlap.IsBound())
				{
					OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor), SweepResult, OverlappedComponent, OtherComp);

				}

				
			}

			else
			{
				Guard(OtherActor);
			}
		}
		
	}

}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	if (OnAttachmentEndOverlap.IsBound())
	{
	
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));

	}
}

bool ACAttachment::CheckGuardState(AActor* InActor)
{
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InActor);
	CheckNullResult(state,false);
	
	UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(InActor);
	CheckNullResult(Weapon, false);

	CheckNullResult(Weapon->GetAttachment(), false);

	bool bState =state->IsSubActionMode();
	bool bGuard = Weapon->GetAttachment()->GetbGuard(InActor);

	return (bState && bGuard);
}

bool ACAttachment::GetAngle(AActor* InAttacker, AActor* InOther)
{
	FVector AttackerLocation = InAttacker->GetActorLocation();
	
	FVector OtherLocation = InOther->GetActorLocation();
	FRotator OtherRotation = InOther->GetActorRotation();

	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(OtherLocation, AttackerLocation);

	FRotator Delta=UKismetMathLibrary::NormalizedDeltaRotator(OtherRotation, LookAt);

	bool bCheck = UKismetMathLibrary::InRange_FloatFloat(Delta.Yaw ,-180, -135);
	bool bCheck2 = UKismetMathLibrary::InRange_FloatFloat(Delta.Yaw, 135, 180);



	return bCheck || bCheck2;

}

void ACAttachment::CancleGuard(AActor* InOther)
{
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(InOther);
	if (!!weapon)
	{
		UCSubAction* subAction = weapon->GetSubAction();
		if (!!subAction)
			subAction->Cancel_Guard();
	}


}

bool ACAttachment::GetbGuard(AActor* InOther)
{
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(InOther);
	if (!!weapon)
	{
		ACAttachment* otherAttachment = weapon->GetAttachment();
		if (!!otherAttachment)
			return (otherAttachment->ReturnbGuard());
	}

	return false;
}

void ACAttachment::Guard(AActor* InOther)
{
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(InOther);
	if (!!weapon)
	{
		UCSubAction* subAction = weapon->GetSubAction();
		if (!!subAction)
		{
			subAction->PlayParrySound();
			subAction->PlayEffect(InOther->GetWorld());
			
		}
	}

	

}

void ACAttachment::SetCollisionSize(float InHalfHeight, float InCapsuleRadius)
{
	UCapsuleComponent* capsule = CHelpers::GetComponent<UCapsuleComponent>(this);

	CheckNull(capsule);

	capsule->SetCapsuleSize(InCapsuleRadius,InHalfHeight);
}
