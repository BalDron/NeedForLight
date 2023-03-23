// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NeedForLightGameModeBase.h"
#include "LevelOneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NEEDFORLIGHT_API ALevelOneGameMode : public ANeedForLightGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;	

public:
	void PlayerCalls();

private:
	class AFriendCharacter* FriendCharacter;
	class APlayerCharacter* PlayerCharacter;
	
	UPROPERTY(EditDefaultsOnly)
	float ResponseDelay = 2.25f;

	FTimerHandle ResponseTimer;

	void CallFriend();

	void SetupFriendCharacter();
	void SetupPlayerCharacter();
};
