#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollisionSkill);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther, FHitResult, InResult, UPrimitiveComponent*, OverlappedComponent, UPrimitiveComponent*, OtherComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FAttachmentBeginOverlapSkill, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther, FHitResult, InResult, UPrimitiveComponent*, OverlappedComponent, UPrimitiveComponent*, OtherComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlapSkill, class ACharacter*, InAttacker, class ACharacter*, InOther);



UCLASS()
class U2406_03_PRACTICE_API ACAttachment : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* Root;
public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

public:
	void OnCollisions();
	void OffCollisions();

protected:
	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachToCollision(FName InCollisionName);
private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
		class ACharacter* OwnerCharacter;

public:
	UFUNCTION(BlueprintNativeEvent)
		void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnUnEquip();
	virtual void OnUnEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnCharging();
	virtual void OnCharging_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnCharge();
	virtual void OnCharge_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OffCharge();
	virtual void OffCharge_Implementation() {}

private:
	bool CheckGuardState(AActor* InActor);
	bool GetAngle(AActor* InAttacker, AActor* InOther);
	void CancleGuard(AActor* InOther);
	void Guard(AActor* InOther);


public:
	FORCEINLINE void SetbGuard(bool InbGuard) { bCanGuard = InbGuard; }
	FORCEINLINE bool ReturnbGuard() {return bCanGuard;}
	
	bool GetbGuard(AActor* InActor);

public:
	void SetCollisionSize(float InHalfHeight, float InCapsuleRadius);
public:
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision OnAttachmentEndCollision;

	FAttachmentEndCollisionSkill OnAttachmentEndCollisionSkill;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;

	FAttachmentBeginOverlapSkill OnAttachmentBeginOverlapSkill;
	FAttachmentEndOverlapSkill OnAttachmentEndOverlapSkill;


	
private:
	TArray<class UShapeComponent*> Collisions;

private:
	bool bCanGuard=true;
};
