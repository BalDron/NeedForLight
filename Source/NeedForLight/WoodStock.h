// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WoodStock.generated.h"

UCLASS()
class NEEDFORLIGHT_API AWoodStock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWoodStock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool RecieveWood();

	UFUNCTION()
	bool GiveWood();

private:
	int32 WoodInStock = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxWoodStock = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UStaticMeshComponent* Platform;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UCapsuleComponent* Capsule;
};
