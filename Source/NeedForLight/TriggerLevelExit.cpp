// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerLevelExit.h"

#include "LevelOneGameMode.h"

ATriggerLevelExit::ATriggerLevelExit()
{
	PrimaryActorTick.bCanEverTick = true;
    Tags.Add(TEXT("Exit"));
    Tags.Add(TEXT("Interactable"));
}

void ATriggerLevelExit::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATriggerLevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerLevelExit::ExitLevel() {
    ALevelOneGameMode* GameMode = GetWorld()->GetAuthGameMode<ALevelOneGameMode>();
    if (GameMode != nullptr) {
        GameMode->ProcessLevelExit();
    }
}
