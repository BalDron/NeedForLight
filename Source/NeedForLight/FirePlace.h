// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirePlace.generated.h"

UCLASS()
class NEEDFORLIGHT_API AFirePlace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirePlace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPointLightComponent* Fire;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UStaticMeshComponent* Platform;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class USceneComponent* WoodStockPlace;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AWoodStock> WoodStockClass;

	AWoodStock* WoodStock;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float MaxEnergy = 100000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float EnergyRestoreAmount = 15000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float InitialEnergy = 50000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float EnergyBoundary = 10000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float EnergyConusmption = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float FireIntensity = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	class USphereComponent* FireLitZone;

	float Energy;
};
