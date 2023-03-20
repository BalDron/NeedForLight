// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendCharacter.h"
#include "MortalityComponent.h"


// Sets default values
AFriendCharacter::AFriendCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MortalityComponent = CreateDefaultSubobject<UMortalityComponent>(TEXT("Mortality"));

}

// Called when the game starts or when spawned
void AFriendCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFriendCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFriendCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

