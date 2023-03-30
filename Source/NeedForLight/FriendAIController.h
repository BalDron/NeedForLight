// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FriendAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class NEEDFORLIGHT_API AFriendAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    void ReleaseFriend();

private:
    UPROPERTY(EditDefaultsOnly)
    UBehaviorTree* AIBehavior;
	
};
