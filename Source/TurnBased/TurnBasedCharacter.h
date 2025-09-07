// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TurnBasedCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AEnemieParent;
class UCharacterCombatComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATurnBasedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeTarget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilitySelect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;

public:
	ATurnBasedCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void ChangeTargets(const FInputActionValue& Value);

	void AbilityAction(const FInputActionValue& Value);

	void Dodge(const FInputActionValue& Value);

	void Parry(const FInputActionValue& Value);

	virtual void BeginPlay() override;
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Change Target
	FTimerHandle TargetChangeCooldownTimer;
	bool CanChangeTarget = true;
	float CurrentTargetVal = 0.f;
	

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool CanAttackBool = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsParrying;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsDodging;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCharacterCombatComponent* CombatComponent;

	UFUNCTION(BlueprintCallable)
	void CanAttack();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AEnemieParent* TargetEnemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AEnemieParent*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DodgeAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ParryAnim;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

