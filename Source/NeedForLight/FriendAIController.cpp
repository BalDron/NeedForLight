// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"
#include "FriendCharacter.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"

void AFriendAIController::BeginPlay() {
    Super::BeginPlay();

    if (AIBehavior != nullptr) {
        RunBehaviorTree(AIBehavior);

        AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent()->SetValueAsObject(
            TEXT("Player"),
            Player
        );

        GetBlackboardComponent()->ClearValue(
            TEXT("CanWalk")
        );
    }
    FindBlockingPlate();
}

void AFriendAIController::FindBlockingPlate() {
    for (AActor* Actor : TActorRange<AActor>(GetWorld())) {
        if (Actor != nullptr && Actor->Tags.Contains("BlockingPlate")) {
            BlockingPlate = Actor;
            bReleased = false;
            break;
        }
    }
}

void AFriendAIController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    bool CanWalkState = GetBlackboardComponent()->GetValueAsBool(
        TEXT("CanWalk")
    );
    if (CanWalkState) {
        UE_LOG(LogTemp, Warning, TEXT("%d"), BlockingPlate);
    }
    if (!bReleased && BlockingPlate == nullptr) {
        ReleaseFriend();
        bReleased = true;
    }
}

void AFriendAIController::ReleaseFriend() {
    GetBlackboardComponent()->SetValueAsBool(
        TEXT("CanWalk"),
        true
    );
}
