

#include "Components/CAttackPointComponent.h"
#include "Global.h"


UCAttackPointComponent::UCAttackPointComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	AttackPoint = 0;
	bAwake = false;
}


// Called when the game starts
void UCAttackPointComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UCAttackPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//각성중

	if (bAwakening)
	{
		Value += 100;
		if (OnAwekening.IsBound())
			OnAwekening.Broadcast(Value);

	}

	else
	{
		if (bAwake)
		{
			Value -= 10;
			if (OnAwekening.IsBound())
				OnAwekening.Broadcast(Value);

			AttackPoint = FMath::Clamp(AttackPoint, 0, MaxAttackPoint);
			AttackPoint -= 0.05f;

			if (OnAttackPointchanged.IsBound())
				OnAttackPointchanged.Broadcast(AttackPoint, MaxAttackPoint);

			if (AttackPoint < 0)
			{
				EndAwake();
				return;
			}

		}

		else
		{
			if (AttackPoint == MaxAttackPoint)
				bCanAwake = true;
		}
	}
	
}


void UCAttackPointComponent::SetAttackPoint(float InAmount)
{ 
	//InAmount는 양수값임.
	AttackPoint += (InAmount);


	//최대~최소값
	AttackPoint = FMath::Clamp(AttackPoint, 0.0f, MaxAttackPoint);

	if (OnAttackPointchanged.IsBound())
		OnAttackPointchanged.Broadcast(AttackPoint, MaxAttackPoint);
	
	
	
}

void UCAttackPointComponent::Begin_AwakeMonatge()
{
	bAwakening = true;
	if (OnBeginAwakening.IsBound())
		OnBeginAwakening.Broadcast();
}


void UCAttackPointComponent::End_AwakeMontage()
{
	bAwakening = false;
	Awake();
}

void UCAttackPointComponent::Awake()
{
	bAwake = true;
	bCanAwake = false;
	CLog::Log("Awake!!");

	if (OnBeginAwaked.IsBound())
		OnBeginAwaked.Broadcast();
}

void UCAttackPointComponent::EndAwake()
{
	bAwake = false;
	AttackPoint = 0;
	Value = 0;
	CLog::Log("Awake_End");

	if (OnEndAwaked.IsBound())
		OnEndAwaked.Broadcast();
}