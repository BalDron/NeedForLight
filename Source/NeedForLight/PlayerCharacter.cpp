// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PlayerCharacterController.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputConfigData.h"
#include "EnhancedInput/Public/InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "WoodStock.h"
#include "MortalityComponent.h"

#include "LevelOneGameMode.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 0.5;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	TorchLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Torch Light"));
	TorchLight->SetupAttachment(SpringArm);
	TorchLight->SetIntensity(0.f);

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flash Light"));
	FlashLight->SetupAttachment(SpringArm);
	FlashLight->SetIntensity(0.f);

	FlashLightLitZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Flashlight Lit Zone"));
	FlashLightLitZone->SetupAttachment(SpringArm);

	TorchLightLitZone = CreateDefaultSubobject<USphereComponent>(TEXT("Torchlight Lit Zone"));
	TorchLightLitZone->SetupAttachment(SpringArm);

	MortalityComponent = CreateDefaultSubobject<UMortalityComponent>(TEXT("Mortality"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	FlashLightCapacity = FlashLightMaxCapacity;
	TorchLightCapacity = TorchLightMaxCapacity;
	
	HUD = CreateWidget(GetWorld(), HUDClass);
	HUD->AddToViewport();

	TurnFlash(false);
	TurnTorch(false);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLights(DeltaTime);

	ViewWidgets();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerCharacterController* PC = Cast<APlayerCharacterController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PC->GetLocalPlayer()
	);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(InputActions->Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	PEI->BindAction(InputActions->Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	PEI->BindAction(InputActions->TriggerLight, ETriggerEvent::Triggered, this, &APlayerCharacter::TriggerLight);
	PEI->BindAction(InputActions->SwitchLight, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchLight);
	PEI->BindAction(InputActions->PickUp, ETriggerEvent::Triggered, this, &APlayerCharacter::PickUpObject);
	PEI->BindAction(InputActions->Put, ETriggerEvent::Triggered, this, &APlayerCharacter::PutObject);
	PEI->BindAction(InputActions->ReloadLight, ETriggerEvent::Triggered, this, &APlayerCharacter::ReloadLight);
	PEI->BindAction(InputActions->Call, ETriggerEvent::Triggered, this, &APlayerCharacter::Call);
}

void APlayerCharacter::UpdateLights(float DeltaTime) {
	if (!IsLightActive) {
		return;
	}
	switch (ActiveLight) {
		case ChosenLight::FlashLight: {
			FlashLightLitZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			FlashLightCapacity -= LightEnergyConsumption * DeltaTime;
			if (FlashLightCapacity > FlashLightLowBoundary) {
				FlashLight->SetIntensity(FlashLightMaxIntensity);
			} else if (FlashLightCapacity > 0.f) {
				FlashLight->SetIntensity(
					FlashLightMaxIntensity * FlashLightCapacity / FlashLightLowBoundary
				);
			} else if (FlashLightCapacity <= 0.f) {
				FlashLightLitZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				FlashLight->SetIntensity(0.f);
				FlashLightCapacity = 0.f;
			}
			break;
		}
		case ChosenLight::TorchLight: {
			TorchLightCapacity -= LightEnergyConsumption * DeltaTime;
			TorchLightLitZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			if (TorchLightCapacity > TorchLightLowBoundary) {
				TorchLight->SetIntensity(TorchLightMaxIntensity);
			} else if (TorchLightCapacity > 0.f) {
				TorchLight->SetIntensity(
					TorchLightMaxIntensity * TorchLightCapacity / TorchLightLowBoundary
				);
			} else if (TorchLightCapacity <= 0.f) {	
				TorchLightLitZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				TorchLight->SetIntensity(0.f);
				TorchLightCapacity = 0.f;
			}
		}
	}
}

void APlayerCharacter::Call(const FInputActionValue& Value) {
	if (CallSound != nullptr) {
		UGameplayStatics::SpawnSoundAttached(
			CallSound,
			Camera,
			TEXT("Camera")
		);
		ALevelOneGameMode* GameMode = GetWorld()->GetAuthGameMode<ALevelOneGameMode>();
		if (GameMode != nullptr) {
			GameMode->PlayerCalls();
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value) {
	if (Controller != nullptr) {
		FVector2D MoveValue = Value.Get<FVector2D>();
		FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		if (MoveValue.Y != 0.f) {
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(
				Direction, 
				MoveValue.Y * UGameplayStatics::GetWorldDeltaSeconds(this) * MovementSpeed
			);
		}

		if (MoveValue.X != 0.f) {
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(
				Direction, 
				MoveValue.X * UGameplayStatics::GetWorldDeltaSeconds(this) * MovementSpeed
			);
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value) {
	if (Controller != nullptr) {
		const FVector2D LookValue = Value.Get<FVector2D>();
		if (LookValue.X != 0.f) {
			AddControllerYawInput(
				LookValue.X * UGameplayStatics::GetWorldDeltaSeconds(this) * LookSpeed
			);
		}
		if (LookValue.Y != 0.f) {
			AddControllerPitchInput(
				-LookValue.Y * UGameplayStatics::GetWorldDeltaSeconds(this) * LookSpeed
			);
		}
	}
}

void APlayerCharacter::TurnFlash(bool On) {
	if (On) {
		FlashLight->SetIntensity(FlashLightMaxIntensity);
	} else {
		FlashLight->SetIntensity(0.f);
	}
}

void APlayerCharacter::TurnTorch(bool On) {
	if (On) {
		TorchLight->SetIntensity(TorchLightMaxIntensity);
	} else {
		TorchLight->SetIntensity(0.f);
	}
}

void APlayerCharacter::TriggerLight(const FInputActionValue& Value) {
	IsLightActive = !IsLightActive;
	switch (ActiveLight) {
		case ChosenLight::FlashLight: {
			TurnTorch(false);
			TurnFlash(IsLightActive);
			break;
		}
		case ChosenLight::TorchLight: {
			TurnTorch(IsLightActive);
			TurnFlash(false);
			break;
		}
	}
}

void APlayerCharacter::SwitchLight(const FInputActionValue& Value) {
	switch (ActiveLight) {
		case ChosenLight::FlashLight: {
			ActiveLight = ChosenLight::TorchLight;
			TurnTorch(IsLightActive);
			TurnFlash(false);
			break;
		}
		case ChosenLight::TorchLight: {
			ActiveLight = ChosenLight::FlashLight;
			TurnTorch(false);
			TurnFlash(IsLightActive);
			break;
		}
	}
}

void APlayerCharacter::ViewWidgets() {
	FHitResult ObserverHit;
	bool bIsAnythingToPick = CheckForPick(ObserverHit);
	if (bIsAnythingToPick) {
		CanIPick = true;
	} else {
		CanIPick = false;
	}
	FHitResult GrabberHit;
	bool bIsAnywhereToPut = CheckForPut(GrabberHit);
	if (bIsAnywhereToPut) {
		CanIPut = true;
	} else {
		CanIPut = false;
	}
}

bool APlayerCharacter::CheckForPut(FHitResult& OutHitResult) const {
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * MaxPutPickDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool result = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere
	);
	if (result) {
		AActor* HitActor = OutHitResult.GetActor();
		if (HitActor->Tags.Contains("Puttable")) {
			return true;
		}
	}
	return false;
} 

bool APlayerCharacter::CheckForPick(FHitResult& OutHitResult) const {
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * MaxPutPickDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool result = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere
	);
	if (result) {
		AActor* HitActor = OutHitResult.GetActor();
		bool bCheckAnyTags = result && (
			HitActor->Tags.Contains("Pickable")
		);
		if (bCheckAnyTags) {
			return true;
		}
	}
	return false;
}

void APlayerCharacter::PickUpObject(const FInputActionValue& Value) {
	FHitResult GrabberHit;
	bool bIsAnythingToGrab = CheckForPick(GrabberHit);
	if (bIsAnythingToGrab) {
		AActor* HitActor = GrabberHit.GetActor();
		if (HitActor->Tags.Contains("Wood")) {
			WoodCapacity += 1;
			UE_LOG(LogTemp, Warning, TEXT("now i have %d wood!"), WoodCapacity);
			HitActor->Destroy();
		} else if (HitActor->Tags.Contains("Battery")) {
			BatteryCapacity += 1;
			UE_LOG(LogTemp, Warning, TEXT("now i have %d batteries!"), BatteryCapacity);
			HitActor->Destroy();
		} else if (HitActor->Tags.Contains("WoodStock")) {
			AWoodStock* WoodStock = Cast<AWoodStock>(HitActor);
			if (WoodCapacity < WoodMaxCapacity && WoodStock->GiveWood()) {
				WoodCapacity += 1;
			}
		} else if (HitActor->Tags.Contains("Crowbar") && !IsCrowBarPicked) {
			IsCrowBarPicked = true;
			HitActor->Destroy();
		}
	}
}

void APlayerCharacter::PutObject(const FInputActionValue& Value) {
	FHitResult GrabberHit;
	bool bIsAnythingToGrab = CheckForPut(GrabberHit);
	if (bIsAnythingToGrab) {
		AWoodStock* WoodStock = Cast<AWoodStock>(GrabberHit.GetActor());
		if (WoodCapacity > 0 && WoodStock->RecieveWood()) {
			WoodCapacity -= 1;
		}
	}
}

void APlayerCharacter::ReloadLight(const FInputActionValue& Value) {
	switch (ActiveLight) {
		case ChosenLight::None: {
			UE_LOG(LogTemp, Warning, TEXT("choose a light to reload"));
			break;
		}
		case ChosenLight::FlashLight: {
			if (BatteryCapacity > 0) {
				FlashLightCapacity += BatteryReloadAmount;
				BatteryCapacity -= 1;
				FlashLight->SetIntensity(
					FlashLightMaxIntensity
				);
			} else {
				UE_LOG(LogTemp, Warning, TEXT("no batteries"));
			}
			break;
		}
		case ChosenLight::TorchLight: {
			if (WoodCapacity > 0) {
				TorchLightCapacity += WoodReloadAmount;
				WoodCapacity -= 1;
				TorchLight->SetIntensity(
					TorchLightMaxIntensity
				);
			} else {
				UE_LOG(LogTemp, Warning, TEXT("no wood"));
			}
			break;
		}
	}
}

int32 APlayerCharacter::GetWoodCapacity() const {
	return WoodCapacity;
}

int32 APlayerCharacter::GetBatteryCapacity() const {
	return BatteryCapacity;
}

int32 APlayerCharacter::GetWoodMaxCapacity() const {
	return WoodMaxCapacity;
}

int32 APlayerCharacter::GetBatteryMaxCapacity() const {
	return BatteryMaxCapacity;
}

int32 APlayerCharacter::IsFlashlightActive() const {
	int32 State = 0;
	if (ActiveLight == ChosenLight::FlashLight) {
		State += 1;
		if (IsLightActive) { State += 1; }
	}
	return State;
}

int32 APlayerCharacter::IsTorchlightActive() const {
	int32 State = 0;
	if (ActiveLight == ChosenLight::TorchLight) {
		State += 1;
		if (IsLightActive) { State += 1; }
	}
	return State;
}

bool APlayerCharacter::CanPlayerPick() const {
	return CanIPick;
}

bool APlayerCharacter::CanPlayerPut() const {
	return CanIPut;
}
