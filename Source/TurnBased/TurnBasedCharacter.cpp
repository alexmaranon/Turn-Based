// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedCharacter.h"

#include "CharacterCombatComponent.h"
#include "EnemieParent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "kismet/GameplayStatics.h"
#include "EnemieParent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATurnBasedCharacter

ATurnBasedCharacter::ATurnBasedCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATurnBasedCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATurnBasedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATurnBasedCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATurnBasedCharacter::Look);


		EnhancedInputComponent->BindAction(ChangeTarget, ETriggerEvent::Triggered, this, &ATurnBasedCharacter::ChangeTargets);

		EnhancedInputComponent->BindAction(AbilitySelect, ETriggerEvent::Triggered, this, &ATurnBasedCharacter::AbilityAction);

		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ATurnBasedCharacter::Dodge);

		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &ATurnBasedCharacter::Parry);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATurnBasedCharacter::CanAttack()
{
	//Anim of widget ability


	CanAttackBool = true;

}

void ATurnBasedCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATurnBasedCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATurnBasedCharacter::ChangeTargets(const FInputActionValue& Value)
{

	float TargetValue = Value.Get<float>();

	if (!CanAttackBool){return;}

	if (!CanChangeTarget || Enemies.Num() == 0)
		return;

	if(Controller!=nullptr)
	{
		CanChangeTarget = false;

		CurrentTargetVal += TargetValue;
		if(CurrentTargetVal<0)
		{
			CurrentTargetVal = Enemies.Num()-1;
		}
		else if(CurrentTargetVal> Enemies.Num() - 1)
		{
			CurrentTargetVal = 0;
		}

		TargetEnemy = Enemies[CurrentTargetVal];

		//due to a bug on the 5.4 of unreal in which the input action has to be on Triggered to change values

		GetWorld()->GetTimerManager().SetTimer(
			TargetChangeCooldownTimer,
			FTimerDelegate::CreateLambda([this]()
				{
					CanChangeTarget = true;
				}),
			0.3f,
			false);

	}
}

void ATurnBasedCharacter::AbilityAction(const FInputActionValue& Value)
{

	if(CanAttackBool)
	{
		

		float Ability = Value.Get<float>() -1 ;

		if(Ability >= CombatComponent->AvailableAbilities.Num())
		{
			return;
		}

		if(!TargetEnemy){return;}

		CombatComponent->Attack(CombatComponent->AvailableAbilities[Ability],TargetEnemy->CombatComponent);


		//PlayAnimMontage(CombatComponent->AvailableAbilities[Ability]->AttackAttributes.Anim);

		CanAttackBool = false;

	}

}

void ATurnBasedCharacter::Dodge(const FInputActionValue& Value)
{

	if(!IsDodging && !IsParrying && !CanAttackBool)
	{
		if(DodgeAnim)
		{
			float Duration = GetMesh()->GetAnimInstance()->Montage_Play(DodgeAnim);
			if (Duration > 0.f)
			{
				IsDodging = true;
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]() {
						//Manage effectiveness rates
						IsDodging = false;
						
					}, Duration+0.3f, false);



			}
		}
		
	}

}

void ATurnBasedCharacter::Parry(const FInputActionValue& Value)
{

	if(!IsDodging && !IsParrying && !CanAttackBool)
	{
		if (ParryAnim)
		{
			float Duration = GetMesh()->GetAnimInstance()->Montage_Play(ParryAnim);
			if (Duration > 0.f)
			{
				IsParrying = true;
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
						//Manage effectiveness rates
						IsParrying = false;

					}, Duration + 0.3f, false);
			}
		}
		
	}

}

void ATurnBasedCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemieParent::StaticClass(), FoundActors);
	CombatComponent = this->FindComponentByClass<UCharacterCombatComponent>();
	for (AActor* Enemy : FoundActors)
	{
		AEnemieParent* EnemyFound = Cast<AEnemieParent>(Enemy);
		if(EnemyFound)
		{
			Enemies.Add(EnemyFound);
		}

	}

	if(Enemies.Num()>0)
	{
		TargetEnemy = Enemies[0];
	}
}
