

#include "Components/CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Particles/ParticleSystemComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetAsset<UParticleSystem>(&ParticleAsset, "ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_aura_shine.P_ky_aura_shine'");
}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

}


void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(Target);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);
	CheckNull(state);
	CheckTrue(state->IsDeadMode());

	float distance = OwnerCharacter->GetDistanceTo(Target);
	if (distance > TraceDistance)
	{
		End();
		return;
	}

	FRotator controlRotation = OwnerCharacter->GetControlRotation();
	float Pitch = controlRotation.Pitch;

	FRotator OwnerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
	OwnerToTarget.Pitch = Pitch;

	APlayerController* controller = Cast<APlayerController>(OwnerCharacter->GetController());

	//보간제한(어느정도 값에 도달하면 고정시키고 보간멈추기)
	if (controlRotation.Equals(OwnerToTarget, FinishAngle))
	{
		bMovingFocus = false;
		controller->SetControlRotation(OwnerToTarget);
		return;
	}

	FRotator TargetRotation = FRotator(controlRotation.Pitch, OwnerToTarget.Yaw, OwnerToTarget.Roll);

	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation, TargetRotation, DeltaTime, RInterpTime);


	controller->SetControlRotation(result);

}


void UCTargetComponent::Toggle()
{
	if (Target == nullptr)
	{
		Start();
		return;
	}

	End();
}

void UCTargetComponent::Start()
{
	FVector location = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), location, location, TraceDistance, "Targeting", false, ignores, DrawDebug, hitResults, true);

	
	ACharacter* candidate = GetNearlyFrontAngle(hitResults);
	Change(candidate);


}

void UCTargetComponent::Change(ACharacter* InCandidate)
{
	if (InCandidate == nullptr)
	{
		End();
		return;
	}

	if (!!Particle)
		Particle->DestroyComponent();

	Particle = UGameplayStatics::SpawnEmitterAttached(ParticleAsset, InCandidate->GetMesh(), "Targeting", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	Target = InCandidate;

}

void UCTargetComponent::End()
{
	Target = nullptr;

	if (!!Particle)
		Particle->DestroyComponent();
}

ACharacter* UCTargetComponent::GetNearlyFrontAngle(const TArray<FHitResult>& InHitResults)
{
	float angle = -2.0f;
	ACharacter* candidate = nullptr;

	for (int i = 0;i < InHitResults.Num();++i)
	{
		FVector target = InHitResults[i].GetActor()->GetActorLocation();
		FVector direction = target - OwnerCharacter->GetActorLocation();
		direction.Normalize();

		FRotator rotator = OwnerCharacter->GetControlRotation();
		FVector forward = FQuat(rotator).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);

		
		FString str;
		str.Append(InHitResults[i].GetActor()->GetActorLabel());
		str.Append(" : ");
		str.Append(FString::SanitizeFloat(dot));

		CLog::Print(str);

		if (FMath::Sign(dot)==1)
		{
			if (dot > angle)
			{
				angle = dot;
				candidate = Cast<ACharacter>(InHitResults[i].GetActor());

			}
		}
		
	}

	return candidate;

}

void UCTargetComponent::MoveLeft()
{
	Move(false);
}
void UCTargetComponent::MoveRight()
{
	Move(true);
}

void UCTargetComponent::Move(bool bRight)
{
	CheckTrue(bMovingFocus);
	FVector location = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	ignores.Add(Target);

	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), location, location, TraceDistance, "Targeting", false, ignores, DrawDebug, hitResults, true);

	TMap<float, ACharacter*> map;

	for (const FHitResult& hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
		if (character == nullptr)
			continue;

		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

		FVector location2 = character->GetActorLocation();

		FVector direction = location2 - location;	//owenr에서 target을 바라보는 방향
		direction.Normalize();

		FVector cross = FVector::CrossProduct(forward, direction);	//컨트롤러의 전방벡터와 바라보는방향 외적

		float dot = FVector::DotProduct(FVector::UpVector, cross);
		map.Add(dot, character);
	}

	float minimum = FLT_MAX;
	ACharacter* candidate = nullptr;

	for (TPair<float, ACharacter*> pair : map)
	{
		float key = pair.Key;

		if (bRight)
		{
			if (key < 0.0f)
				continue;
		}

		else
		{
			if (key > 0.0f)
				continue;
		}

		if (FMath::Abs(key) < minimum)
		{
			minimum = FMath::Abs(key);
			candidate = pair.Value;
		}
	}

	bMovingFocus = true;
	Change(candidate);
}