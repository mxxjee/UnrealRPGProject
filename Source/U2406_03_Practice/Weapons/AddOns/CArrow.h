#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitArrow, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitFloorArrow, class AActor*, InCauser);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestroyActor);

UCLASS(Blueprintable)
class U2406_03_PRACTICE_API ACArrow : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "Soundcue")
		class USoundCue* Shootcue;

	UPROPERTY(EditDefaultsOnly, Category = "Soundcue")
		class USoundCue* SpecialCue;

	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float LifeSpanAfterCollision = 1;//충돌 후 사라질시간
	
private:
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;

	UPROPERTY( VisibleAnywhere)
		class UProjectileMovementComponent* Projectile;

public:	
	ACArrow();
	void Shoot(const FVector& InForward);
	void Shoot(const FVector& InForward, float Speed,int InOverlapCount);
	void SkillShoot(const FVector& InForward, float Speed = 1000.f);
	void SkillIceShoot(const FVector& InForward, float Speed = 1000.f);
	void ShootAwake(const FVector& InForward);
	void ShootSpecialSkill(const FVector& InForward);

public:
	FORCEINLINE void AddIngoreActor(AActor* InActor) { Ignores.Add(InActor); }
	
public:
	void SetArrowMaterial(FLinearColor InColor);

protected:
	UFUNCTION(BlueprintNativeEvent)
		void SetNiagara(bool bInbool);
	void SetNiagara_Implementation(bool bInbool) {};

	UFUNCTION(BlueprintNativeEvent)
		void SetChargeSkillNiagara(bool bInbool);
	void SetChargeSkillNiagara_Implementation(bool bInbool) {};

	UFUNCTION(BlueprintNativeEvent)
		void SetSkillNiagara(bool bInbool);
	void SetSkillNiagara_Implementation(bool bInbool) {};

	UFUNCTION(BlueprintNativeEvent)
		void SetIceSkillNiagara(bool bInbool);
	void SetIceSkillNiagara_Implementation(bool bInbool) {};

	UFUNCTION(BlueprintNativeEvent)
		void SetAwakeNiagara(bool bInbool);
	void SetAwakeNiagara_Implementation(bool bInbool) {};

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	FHitArrow OnHitArrow;
	FHitFloorArrow OnFloorHitArrow;
	FProjectileEndPlay OnEndPlay;
private:
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	TArray<AActor*> Ignores;



private:
	bool bOverlap=false;
	int OverlapCount=0;

	int CurrentOverlapCount = 0;

private:
	class UMaterialInstanceDynamic* DynamicMaterial;
};
