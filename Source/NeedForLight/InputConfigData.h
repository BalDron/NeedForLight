// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class NEEDFORLIGHT_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Move;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Look;

	// lights
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* FlashLight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* TorchLight;
	
	// .. and inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ReloadLight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* PickUp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Put;
};
