// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "PlayerCharacter.generated.h"

enum class ChosenLight
{
	None,
	FlashLight,
	TorchLight,
};

UCLASS()
class NEEDFORLIGHT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPointLightComponent* TorchLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpotLightComponent* FlashLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCapsuleComponent* FlashLightLitZone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* TorchLightLitZone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UMortalityComponent* MortalityComponent;

private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* CallSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ResponseSound;

	void Call(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed = 10.f;
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float LookSpeed = 10.f;
	void Look(const FInputActionValue& Value);


	ChosenLight ActiveLight = ChosenLight::FlashLight;
	bool IsLightActive = false;

	UPROPERTY(EditDefaultsOnly, Category = "Lights")
	float LightEnergyConsumption = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lights")
	float FlashLightMaxCapacity = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Lights")
	float FlashLightLowBoundary = 60.f;
	float FlashLightCapacity  = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Lights")
	float FlashLightMaxIntensity = 5000.f;


	UPROPERTY(EditDefaultsOnly, Category = "Lights")
	float TorchLightMaxCapacity = 250.f;
	UPROPERTY(EditDefaultsOnly, Category = "Lights")
	float TorchLightLowBoundary = 50.f;
	float TorchLightCapacity = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Lights")
	float TorchLightMaxIntensity = 5000.f;

	void TriggerLight(const FInputActionValue& Value);
	void SwitchLight(const FInputActionValue& Value);
	void ReloadLight(const FInputActionValue& Value);
	void TurnFlash(bool On);
	void TurnTorch(bool On);

	void UpdateLights(float DeltaTime);

	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 WoodMaxCapacity = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float WoodReloadAmount = 10000.f;
	int32 WoodCapacity = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 BatteryMaxCapacity = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float BatteryReloadAmount = 10000.f;
	int32 BatteryCapacity = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float MaxPutPickDistance = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float GrabRadius = 10.f;

	bool IsCrowBarPicked = false;

    bool CheckForInteract(struct FHitResult& OutHitResult) const;
	void InteractWithObject(const FInputActionValue& Value);

	bool CheckForPick(struct FHitResult& OutHitResult) const;
	void PickOrInteract(const FInputActionValue& Value);

	bool CheckForPut(struct FHitResult& OutHitResult) const;
	void PutObject(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* HUD;

	void ViewWidgets();

    UPROPERTY(EditDefaultsOnly, Category = "Action")
    float HitDistance = 50.f;

    UPROPERTY(EditDefaultsOnly, Category = "Action")
    float HitRadius = 2.f;
    
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<class UActorComponent> HitComponentClass;


	void Hit(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintPure)
	int32 GetWoodCapacity() const;

	UFUNCTION(BlueprintPure)
	int32 GetBatteryCapacity() const;

	
	UFUNCTION(BlueprintPure)
	int32 GetWoodMaxCapacity() const;

	UFUNCTION(BlueprintPure)
	int32 GetBatteryMaxCapacity() const;

	UFUNCTION(BlueprintPure)
	int32 IsFlashlightActive() const;

	UFUNCTION(BlueprintPure)
	int32 IsTorchlightActive() const;

	bool CanIInteract = false;
	UFUNCTION(BlueprintPure)
	bool CanPlayerInteract() const;

	bool CanIPick = false;
	UFUNCTION(BlueprintPure)
	bool CanPlayerPick() const;

	bool CanIPut = false;
	UFUNCTION(BlueprintPure)
	bool CanPlayerPut() const;

	UFUNCTION(BlueprintPure)
	bool DoesPlayerHaveCrowbar() const;
};
