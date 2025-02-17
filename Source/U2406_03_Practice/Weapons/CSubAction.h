
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleased);

UCLASS()
class U2406_03_PRACTICE_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }

public:
	UCSubAction();

public:
	FOnPressed OnPressed;
	FOnReleased OnReleased;

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);
	virtual void Tick(float DeltaTime) {};
public:
	virtual void Pressed();
	virtual void Released();

	virtual void PlayParrySound() {};
	virtual void PlayEffect(UWorld* InWorld) {};

	UFUNCTION()
		virtual void Cancel_Guard() {};


protected:
	bool bInAction;	//Pressed ½Ã true

	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;	
	class UCMovementComponent* Movement;
};

