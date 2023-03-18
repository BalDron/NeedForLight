// Fill out your copyright notice in the Description page of Project Settings.


#include "FirePlace.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "WoodStock.h"
#include "Components/PointLightComponent.h"


// Sets default values
AFirePlace::AFirePlace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(RootComponent);

	Fire = CreateDefaultSubobject<UPointLightComponent>(TEXT("Fire"));
	Fire->SetupAttachment(RootComponent);

	WoodStockPlace = CreateDefaultSubobject<USceneComponent>(TEXT("WoodStock"));
	WoodStockPlace->SetupAttachment(RootComponent);

	Energy = InitialEnergy;
}

// Called when the game starts or when spawned
void AFirePlace::BeginPlay()
{
	Super::BeginPlay();

	FVector WoodStockLocation = WoodStockPlace->GetComponentLocation();
	UE_LOG(LogTemp, Warning, TEXT("Spawn point: %f %f %f"), 
		WoodStockLocation.X,
		WoodStockLocation.Y,
		WoodStockLocation.Z
	);
	WoodStock = Cast<AWoodStock>(
		GetWorld()->SpawnActor(
			WoodStockClass,
			&WoodStockLocation
		)
	);
}

// Called every frame
void AFirePlace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Energy > EnergyBoundary) {
		Fire->SetIntensity(FireIntensity);
		Energy -= EnergyConusmption * DeltaTime;
	} else if (Energy > 0.f) {
		Fire->SetIntensity(FireIntensity * Energy / EnergyBoundary);
		Energy -= EnergyConusmption * DeltaTime;
	} else {
		Energy = 0.f;
		Fire->SetIntensity(0.f);
	}
	if (Energy < EnergyBoundary && WoodStock->GiveWood()) {
		Energy += EnergyRestoreAmount;
	}
}
