// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructionComponent.h"


// Sets default values for this component's properties
UDestructionComponent::UDestructionComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    Health = DefaultHealth;
}


// Called when the game starts
void UDestructionComponent::BeginPlay()
{
    Super::BeginPlay();
}


// Called every frame
void UDestructionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDestructionComponent::GetHit(int32 Damage) {
    Health -= Damage;
    if (Health <= 0) {
        GetOwner()->Destroy();
    }
}

