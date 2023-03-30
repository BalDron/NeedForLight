// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"

void AFriendAIController::BeginPlay() {
    Super::BeginPlay();

    if (AIBehavior != nullptr) {
        RunBehaviorTree(AIBehavior);

        AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent()->SetValueAsObject(
            TEXT("Player"),
            Player
        );

        GetBlackboardComponent()->SetValueAsBool(
            TEXT("CanWalk"),
            false
        );
    }
}

void AFriendAIController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AFriendAIController::ReleaseFriend() {
    GetBlackboardComponent()->SetValueAsBool(
        TEXT("CanWalk"),
        true
    );
}
