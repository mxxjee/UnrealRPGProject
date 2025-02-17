#pragma once

#include "CoreMinimal.h"

#define CheckTrue(x) { if(x == true) return; }

#define CheckFalse(x) { if(x == false) return; }

#define CheckNull(x) { if(x == nullptr) return; }

#define CheckTrueResult(x, y) { if(x == true) return y; }

#define CheckFalseResult(x, y) { if(x == false) return y;}

#define CheckNullResult(x, y) { if(x == nullptr) return y;}

#define CreateTextRenderComponent()\
{\
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);\
	Text->SetRelativeLocation(FVector(0, 0, 100));\
	Text->SetRelativeRotation(FRotator(0, 180, 0));\
	Text->SetRelativeScale3D(FVector(2));\
	Text->TextRenderColor = FColor::Red;\
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;\
	Text->Text = FText::FromString(GetName().Replace(L"Default__", L""));\
}

class U2406_03_PRACTICE_API CHelpers
{
public:
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{

			(*OutComponent)->SetupAttachment(InParent);
			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}
	
	template <typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template <typename T>
	static void CreateActorComponent(AActor* InActor, TObjectPtr<T>* OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}


	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		*OutObject = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));


	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> FindClass(*InPath);
		*OutClass = FindClass.Class;
	}
	
	template<typename T>
	static T* FindActor(UWorld* InWorld)
	{
		for (AActor* Actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!Actor && Actor->IsA<T>())
			{
				return Cast<T>(Actor);
			}
		}

		return nullptr;
	}

	template<typename T>
	static void FindActors(UWorld* InWorld,TArray<T*>& OutActors)
	{
		for (AActor* Actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!Actor && Actor->IsA<T>())
			{
				OutActors.Add(Cast<T>(Actor));
			}
		}

		
	}

	template <typename T>
	static T* GetComponent(AActor* InActor)
	{
		//GetComponentByClass(UClass*) 이므로 staticClass로생성
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	//이름으로 찾는 컴포넌트.
	template <typename T>
	static T* GetComponent(AActor* InActor, const FString& InName)
	{
		TArray<T*> components;
		InActor->GetComponents<T>(components);

		//이름매칭 후 첫번째걸리는애 리턴
		for (T* component : components)
		{
			if (component->GetName() == InName)
				return component;
		}

		//못찾았으면 null
		return NULL;
	}
};






