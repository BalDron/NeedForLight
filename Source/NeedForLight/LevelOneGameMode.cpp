// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelOneGameMode.h"
#include "FriendCharacter.h"
#include "PlayerCharacter.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"


void ALevelOneGameMode::BeginPlay() {
	Super::BeginPlay();

    SetupFriendCharacter();
    SetupPlayerCharacter();
}

void ALevelOneGameMode::PlayerCalls() {
    GetWorldTimerManager().SetTimer(
        ResponseTimer, this, 
        &ALevelOneGameMode::CallFriend, 
        ResponseDelay
    );
}

void ALevelOneGameMode::CallFriend() {
    FriendCharacter->Respond();
}

void ALevelOneGameMode::SetupFriendCharacter() {
    for (AFriendCharacter* Friend : TActorRange<AFriendCharacter>(GetWorld())) {
        if (Friend != nullptr) {
            FriendCharacter = Friend;
            break;
        }
    }
    if (FriendCharacter == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("LevelOneGameMode: Couldn't find FriendCharacter"));
    }
}

void ALevelOneGameMode::SetupPlayerCharacter() {
    PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (PlayerCharacter == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("LevelOneGameMode: Couldn't find PlayerCharacter"));
    }
}

void ALevelOneGameMode::ProcessCharacterDeath(AActor* DeadActor) {
    if (DeadActor == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT(
                "ALevelOneGameMode::ProcessCharacterDeath >> DeadActor pointer is not set"
            )
        );
        return;
    }
    if (DeadActor->Tags.Contains("Player")) {
        UE_LOG(LogTemp, Warning, TEXT("You are dead. Restart in %f seconds"), RestartDelay);
        APlayerCharacter* Character = Cast<APlayerCharacter>(DeadActor);
        APlayerController* Controller = Cast<APlayerController>(Character->GetController());
        GetWorldTimerManager().SetTimer(
            RestartTimer, Controller,
            &APlayerController::RestartLevel, 
            RestartDelay
        );
    } else if (DeadActor->Tags.Contains("Friend")) {
        UE_LOG(LogTemp, Warning, TEXT("Your friend is dead."));
        DeadActor->Destroy();
    }
}
