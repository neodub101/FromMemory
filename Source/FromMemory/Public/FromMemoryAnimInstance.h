// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FromMemoryAnimInstance.generated.h"

class APlayerCharacter;
class APawn;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class FROMMEMORY_API UFromMemoryAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
 

public:
	// Setup logic
	virtual void NativeInitializeAnimation() override;
	 
	// Frame-by-frame updates
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	 
protected:
	/** References cached for performance */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class APawn* OwnerPawn;
	
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<APlayerCharacter> CharacterRef;
	 
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class UCharacterMovementComponent* MovementComponent;
	 
	/** Movement Data exposed to AnimGraph */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Direction;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsFalling;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bShouldEliminate; // Example flag for state changes]
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsWalking;
	
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching;
	
	float CrouchSpeed;
};