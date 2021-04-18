// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project2DCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AProject2DCharacter::AProject2DCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

void AProject2DCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup animations
	AttackAnimInstance = GetMesh()->GetAnimInstance();

}

// Input

void AProject2DCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProject2DCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProject2DCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProject2DCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Attacking", IE_Pressed, this, &AProject2DCharacter::Attack);
}

void AProject2DCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void AProject2DCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AProject2DCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

// Animations

void AProject2DCharacter::Attack()
{
	if (Attacking) 
	{
		SaveAttack = true;
	}
	if (!Attacking)
	{
		Attacking = true;

		PlayMontage(AttackCount);
	}
}

void AProject2DCharacter::ComboAttack()
{
	if (SaveAttack) 
	{
		SaveAttack = false;

		PlayMontage(AttackCount);
	}
}

void AProject2DCharacter::ResetCombo()
{
	AttackCount = 0;
	Attacking = false;
	SaveAttack = false;
}

void AProject2DCharacter::PlayMontage(int32 MontageIndex)
{
	if (!FirstAttackMontage) { return; }
	if (!SecondAttackMontage) { return; }
	if (!AttackAnimInstance) { return; }

	if (MontageIndex == 0)
	{
		AttackCount = 1;
		AttackAnimInstance->Montage_Play(FirstAttackMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
	}
	if (MontageIndex == 1)
	{
		AttackCount = 0;
		AttackAnimInstance->Montage_Play(SecondAttackMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
	}
}
