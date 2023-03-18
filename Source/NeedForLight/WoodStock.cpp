// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodStock.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AWoodStock::AWoodStock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWoodStock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWoodStock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWoodStock::RecieveWood() {
	if (WoodInStock < MaxWoodStock) {
		UE_LOG(LogTemp, Display, TEXT("Yay, more wood!"));
		WoodInStock += 1;
		return true;
	}
	return false;
}

bool AWoodStock::GiveWood() {
	if (WoodInStock > 0) {
		UE_LOG(LogTemp, Display, TEXT("Boo, less wood!"));
		WoodInStock -= 1;
		return true;
	}
	return false;
}