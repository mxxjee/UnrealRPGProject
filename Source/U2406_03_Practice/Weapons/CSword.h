#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSword.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACSword : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;

public:	
	ACSword();
	
public:
	void OnCollision();
	void OffCollision();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	 
};
