#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_MainWeapon UMETA(DisplayName = "MainWeapon"),
	EWT_SubWeapon UMETA(DisplayName = "SubWeapon"),
	EWT_BoomWeapon UMETA(DisplayName = "BombWeapon"),
	EWT_SpecialWeapon UMETA(DisplayName = "SpecialWeapon")
};