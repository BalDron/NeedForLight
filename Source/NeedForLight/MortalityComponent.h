// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MortalityComponent.generated.h"


enum class MortalState {
	Lit,
	UnLit,
	Dead,
	Inactive,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEEDFORLIGHT_API UMortalityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMortalityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mortality")
	float TimeToDie = 10.f;

	MortalState State = MortalState::Lit;

	float TimeLeft;

	UFUNCTION()
	bool IsOwnerLit();
};
