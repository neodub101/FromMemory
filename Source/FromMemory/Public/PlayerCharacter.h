// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpringArmComponent.generated.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UCameraComponent;


UCLASS()
class FROMMEMORY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();
	
	
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	 UCameraComponent* FirstPersonCameraNew;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FName CameraSocketName = FName("camera_jnt");
	
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMeshComponent;
	
	
	// First-person primitives field of view
	UPROPERTY(EditAnywhere, Category = Camera)
	float FirstPersonFieldOfView = 70.0f;
	 
	// First-person primitives view scale
	UPROPERTY(EditAnywhere, Category = Camera)
	float FirstPersonScale = 0.6f;
	
	UPROPERTY(EditAnywhere, Category = Camera)
	FVector FirstPersonCameraOffset = FVector(2.8f, 5.9f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;


	
	UPROPERTY(EditAnywhere, Category = "HeadBob")
	float WalkSpeedThreshold = 1.0f;
	 
	UPROPERTY(EditAnywhere, Category = "HeadBob")
	float SprintSpeedThreshold = 10.0f;
	 
	// Used to store the current 'active' values during interpolation
	float BobCurveTime;
	float CurrentBobFrequency = 0.0f;
	float CurrentBobAmplitude = 0.0f;
	
	
	float Walkspeed;
	
public:	
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	virtual void Jump() override;	
	
	UFUNCTION()
	void HandleHeadBob(float DeltaTime);
	
	UFUNCTION()
	void ResetBob(float DeltaTime);
	
	//bool walk() const;
	
};
