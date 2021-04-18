// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Animation/AnimMontage.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Project2DCharacter.generated.h"

UCLASS(config=Game)
class AProject2DCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	AProject2DCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void Attack();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	// Variables
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* FirstAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* SecondAttackMontage;

	UPROPERTY()
	UAnimInstance* AttackAnimInstance;
	
	int32 AttackCount = 0;
	bool Attacking = false;
	bool SaveAttack = false;

	// Functions
	UFUNCTION(BlueprintCallable)
	void ComboAttack();
	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	void PlayMontage(int32 MontageIndex);
};
