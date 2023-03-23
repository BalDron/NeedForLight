// Fill out your copyright notice in the Description page of Project Settings.


#include "MortalityComponent.h"
#include "GameFramework/Actor.h"
#include "LevelOneGameMode.h"
// #include "Components/PrimitiveComponent.h"


// Sets default values for this component's properties
UMortalityComponent::UMortalityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMortalityComponent::BeginPlay()
{
	Super::BeginPlay();

	TimeLeft = TimeToDie;
	
}


// Called every frame
void UMortalityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (State == MortalState::Dead) {
		// UE_LOG(LogTemp, Warning, TEXT("%s is dead."), *GetOwner()->GetName());
	} else if (IsOwnerLit()) {
		// UE_LOG(LogTemp, Warning, TEXT("%s is lit"), *GetOwner()->GetName());
		State = MortalState::Lit;
	} else {
		// UE_LOG(LogTemp, Warning, TEXT("%s is NOT lit"), *GetOwner()->GetName());
		State = MortalState::UnLit;
	}

	switch (State) {
		case MortalState::Lit: {
			TimeLeft = TimeToDie;
			break;
		} case MortalState::UnLit: {
			TimeLeft -= DeltaTime;
			if (TimeLeft <= 0) {
				State = MortalState::Dead;
				ALevelOneGameMode* GameMode = GetWorld()->GetAuthGameMode<ALevelOneGameMode>();
				if (GameMode != nullptr) {
					GameMode->ProcessCharacterDeath(GetOwner());
				}
			}
			break;
		} case MortalState::Dead: {
			break;
		}
	}
}

bool UMortalityComponent::IsOwnerLit() {
	AActor* Owner = GetOwner();
	TArray<UPrimitiveComponent*> Components;
	Owner->GetOverlappingComponents(Components);
	for (UPrimitiveComponent* Component : Components) {
		if (Component->ComponentHasTag(TEXT("LitZone"))) {
			return true;
		}
	}
	// TArray<AActor*> Actors;
	// Owner->GetOverlappingActors(Actors);
	// for (AActor* Actor : Actors) {
	// 	if (Actor->ActorHasTag(TEXT("LitZone"))) {
	// 		UE_LOG(LogTemp, Display, TEXT("lit by actor"));
	// 		return true;
	// 	}
	// }
	return false;
}
