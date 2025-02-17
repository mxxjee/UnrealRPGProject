// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CSpawnLoc.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
ACSpawnLoc::ACSpawnLoc()
{
    /*CHelpers::CreateComponent<UDecalComponent>(this, &Decal, "Decal");
    Decal->SetVisibility(false);*/

    CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle");
   
}

void ACSpawnLoc::ShowDecals()
{
    //CheckNull(Decal);
    //Decal->SetVisibility(true);

    CheckNull(Particle);
    Particle->ActivateSystem();

    FTimerHandle timerhandle;
    FTimerDelegate timerDelegate;

    timerDelegate.BindLambda([=] {

        //Decal->SetVisibility(false);
        Particle->DeactivateSystem();
    });

    GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, AfterSecond, false);
}


