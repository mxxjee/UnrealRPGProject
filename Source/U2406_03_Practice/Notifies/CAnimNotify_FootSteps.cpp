// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_FootSteps.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Sound\SoundCue.h"
#include "Components/CParkourComponent.h"

FString UCAnimNotify_FootSteps::GetNotifyName_Implementation() const
{
	return "FootSteps";
}

void UCAnimNotify_FootSteps::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* owner = MeshComp->GetOwner();
	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(owner);

	CheckNull(owner);
	CheckNull(parkour);

	CheckFalse(parkour->GetParkourType() == EParkourType::Max);

	TArray<AActor*> Toignore;
	Toignore.Add(owner);

	FHitResult hitResult;

	FVector start = owner->GetActorLocation();
	FVector end = start + FVector(0, 0, -100);

	UKismetSystemLibrary::LineTraceSingle(owner->GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		Toignore,
		EDrawDebugTrace::None,
		hitResult,
		true
	);

	if (hitResult.bBlockingHit)
	{
		EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(hitResult);

		switch (SurfaceType)
		{
			case SurfaceType_Default:
				break;
			case SurfaceType1:
				UGameplayStatics::PlaySoundAtLocation(owner->GetWorld(), Dirt.sound, hitResult.ImpactPoint);
				break;
			case SurfaceType2:
				UGameplayStatics::PlaySoundAtLocation(owner->GetWorld(), Stone.sound, hitResult.ImpactPoint);
				break;
			case SurfaceType3:
				UGameplayStatics::PlaySoundAtLocation(owner->GetWorld(), Water.sound, hitResult.ImpactPoint);
				break;
			case SurfaceType4:
				UGameplayStatics::PlaySoundAtLocation(owner->GetWorld(), Gravel.sound, hitResult.ImpactPoint);
				break;
			case SurfaceType5:
				UGameplayStatics::PlaySoundAtLocation(owner->GetWorld(), Carpet.sound, hitResult.ImpactPoint);
				break;
			case SurfaceType6:
				break;
			case SurfaceType7:
				break;
			case SurfaceType8:
				break;
			case SurfaceType9:
				break;
			case SurfaceType10:
				break;
			case SurfaceType11:
				break;
			case SurfaceType12:
				break;
			case SurfaceType13:
				break;
			case SurfaceType14:
				break;
			case SurfaceType15:
				break;
			case SurfaceType16:
				break;
			case SurfaceType17:
				break;
			case SurfaceType18:
				break;
			case SurfaceType19:
				break;
			case SurfaceType20:
				break;
			case SurfaceType21:
				break;
			case SurfaceType22:
				break;
			case SurfaceType23:
				break;
			case SurfaceType24:
				break;
			case SurfaceType25:
				break;
			case SurfaceType26:
				break;
			case SurfaceType27:
				break;
			case SurfaceType28:
				break;
			case SurfaceType29:
				break;
			case SurfaceType30:
				break;
			case SurfaceType31:
				break;
			case SurfaceType32:
				break;
			case SurfaceType33:
				break;
			case SurfaceType34:
				break;
			case SurfaceType35:
				break;
			case SurfaceType36:
				break;
			case SurfaceType37:
				break;
			case SurfaceType38:
				break;
			case SurfaceType39:
				break;
			case SurfaceType40:
				break;
			case SurfaceType41:
				break;
			case SurfaceType42:
				break;
			case SurfaceType43:
				break;
			case SurfaceType44:
				break;
			case SurfaceType45:
				break;
			case SurfaceType46:
				break;
			case SurfaceType47:
				break;
			case SurfaceType48:
				break;
			case SurfaceType49:
				break;
			case SurfaceType50:
				break;
			case SurfaceType51:
				break;
			case SurfaceType52:
				break;
			case SurfaceType53:
				break;
			case SurfaceType54:
				break;
			case SurfaceType55:
				break;
			case SurfaceType56:
				break;
			case SurfaceType57:
				break;
			case SurfaceType58:
				break;
			case SurfaceType59:
				break;
			case SurfaceType60:
				break;
			case SurfaceType61:
				break;
			case SurfaceType62:
				break;
			case SurfaceType_Max:
				break;
			default:
				break;
		}
	}
}