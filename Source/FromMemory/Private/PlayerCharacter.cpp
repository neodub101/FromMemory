// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include <shlwapi.h>

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	FirstPersonCameraNew = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCameraNew"));
	FirstPersonCameraNew->SetupAttachment(GetMesh(), CameraSocketName);
	//FirstPersonCameraNew->SetRelativeLocation(FVector(0.f, 0.f, 64.f)); // eye height
	FirstPersonCameraNew->bUsePawnControlRotation = true;
//	check(FirstPersonCameraNew != nullptr);
	
	BobCurveTime = 0.0f;
	CurrentBobFrequency = 5.0f;
	CurrentBobAmplitude = 0.0f;
	
	GetCharacterMovement()->MaxWalkSpeed = 600.f;     
	//GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;
	
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FirstPersonContext, 0);
		}
	}
	
	 
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleHeadBob(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &APlayerCharacter::Walk);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::OnCrouchToggle);
		EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Started, this, &APlayerCharacter::Prone);
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AdventureCharacter."));
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	
	// Check if the controller possessing this Actor is valid
	if (Controller)
	{
		
		const FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementValue.X);
	 
		// Add forward and back movement
		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementValue.Y);
	}
}

void APlayerCharacter::Jump()
{
	Super::Jump();
}

void APlayerCharacter::HandleHeadBob(float DeltaTime)
{
	if (!FirstPersonCameraNew) return;
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	
	if (!::IsValid(MoveComp) || MoveComp->IsFalling()) 
	{
		ResetBob(DeltaTime);
		return;
	}
	 
	float CurrentSpeed = GetVelocity().Size2D();
	    
	// 1. Define Target Parameters for each range
	// Format: {Frequency, Amplitude}
	float TargetFreq = 0.0f;
	float TargetAmp = 0.0f;
	 
	if (CurrentSpeed > 10.0f) // If we are actually moving
	{
		if (CurrentSpeed > SprintSpeedThreshold)
		{
			// SPRINTING: Fast and heavy
			TargetFreq = 10.5f;
			TargetAmp = 11.0f;
		}
		else if (CurrentSpeed > WalkSpeedThreshold)
		{
			// RUNNING: Moderate
			TargetFreq = 40.5f;
			TargetAmp = 2.0f;
		}
		else
		{
			// WALKING: Slow and subtle
			TargetFreq = 4.0f;
			TargetAmp = 0.8f;
		}
	}
	 
	// 2. Smoothly Interpolate to the target range
	// This prevents "snapping" when changing speed states
	float InterpSpeed = 2.0f; 
	CurrentBobFrequency = FMath::FInterpTo(CurrentBobFrequency, TargetFreq, DeltaTime, InterpSpeed);
	CurrentBobAmplitude = FMath::FInterpTo(CurrentBobAmplitude, TargetAmp, DeltaTime, InterpSpeed);
	 
	// 3. Apply the Bobbing
	if (CurrentBobAmplitude > 0.1f)
	{
		BobCurveTime += DeltaTime * CurrentBobFrequency;
	 
		float VerticalBob = FMath::Sin(BobCurveTime) * CurrentBobAmplitude;
		float HorizontalBob = FMath::Cos(BobCurveTime * 0.5f) * (CurrentBobAmplitude * 0.5f);
	 
		FirstPersonCameraNew->SetRelativeLocation(FVector(20.0f, HorizontalBob, VerticalBob));
	}
	else
	{
		ResetBob(DeltaTime);
	}
	
}

 
	void APlayerCharacter::ResetBob(float DeltaTime)
	{
	if (!FirstPersonCameraNew) return;
		// Smoothly return camera to default local transform
		FVector CurrentLoc = FirstPersonCameraNew->GetRelativeLocation();
		FRotator CurrentRot = FirstPersonCameraNew->GetRelativeRotation();
	    
		FVector TargetLoc = FVector::ZeroVector; // Or your default offset
		FRotator TargetRot = FRotator::ZeroRotator;   //test
	 
		float ResetSpeed = 10.0f;
	    
		FirstPersonCameraNew->SetRelativeLocation(FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, ResetSpeed));
		FirstPersonCameraNew->SetRelativeRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, ResetSpeed));
	    
		// Reset the curve time when stopped to ensure the next walk starts from a neutral step
		if (GetVelocity().Size2D() < 1.0f)
		{
			BobCurveTime = 0.0f;
		}
	}

void APlayerCharacter::UpdateMovementSpeed()
{
	if (bIsProning)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedProned;
	}
	else if (bIsCrouched) // engine-replicated, same as your existing crouch logic
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedCrouched;
	}
	else if (bIsWalking) // your existing walk toggle bool
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedWalking;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedRunning;
	}
	
}

/*void APlayerCharacter::Crouch(bool bClientSimulation) override
{
	Super::Crouch(bClientSimulation);
}*/

void APlayerCharacter::Walk(const FInputActionValue& Value)
{
	/*bIsWalking = !bIsWalking;
	if (bIsWalking)
	{ 
	*/
		 
		bIsWalking = !bIsWalking;
		UpdateMovementSpeed();
		
	/*}*/
	/*else
	{
		
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}*/
}

void APlayerCharacter::Prone()
{
	bIsProning = !bIsProning;
	
	UpdateMovementSpeed();
	
	/*
	else
	{
		
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	*/
	 
	
	
 }

void APlayerCharacter::OnCrouchToggle()
{
	
	if (bIsCrouched)
	{
		UnCrouch();
 
	
    }
	else 
	{
		Crouch();
		UpdateMovementSpeed();
	}

}


