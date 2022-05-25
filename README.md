# UE4_Portfolio_LO3D

포트폴리오 및 모바일게임 라스트오리진 유저에게 배포를 목적으로 만든 라스트오리진 팬게임 입니다.

&nbsp;


## 플레이 영상
![1](https://user-images.githubusercontent.com/13048481/170157547-4dc99c06-6fbc-4bc7-b815-513607739bd8.png)
&nbsp;
https://www.youtube.com/watch?v=oPXyp3uiNyk

&nbsp;
## 구성
* 엔진 : Unreal Engine 4 (4.27.2)
* IDE : Microsoft Visual Studio Community 2019
* 개발기간 : 2022/03/28 ~ 2022/05/23 (약 2달)
* 개발인원 : 1인

&nbsp;
## 기술설명서

### 캐릭터 이동
```c
void AMainCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0&&!bPickUp)
    {
       const FRotator Rotation = Controller->GetControlRotation();
       const FRotator YawRotation = FRotator{ 0.f,Rotation.Yaw,0.f };
       FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
       AddMovementInput(Direction, Value);
    }
} 

void AMainCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0&&!bPickUp)
    {
      const FRotator Rotation = Controller->GetControlRotation();
      const FRotator YawRotation = FRotator{ 0.f,Rotation.Yaw,0.f };
      FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
      AddMovementInput(Direction, Value);
    }
}
    
```
Character Controller의 Yaw값을 구해 회전변환행렬인 FRotationMatrix를 이용   
X,Y축에 따라 캐릭터의 전후좌우 이동 구현
&nbsp;

* AnimBP를 이용하여 캐릭터의 이동 동작 구현   
![2](https://user-images.githubusercontent.com/13048481/170163839-4db9603d-f28a-420a-b78e-5c1c572310e5.png)   


* Surface를 이용한 타일별 발자국 소리 및 이펙트 구현   
![3](https://user-images.githubusercontent.com/13048481/170164815-c4f368c5-67c7-4e03-a44f-4c6aaedf4fe2.png)&nbsp;
캐릭터의 위치에서 라인트레이스를 Z축 -방향으로 트레이싱하여 Hit지점의 Actor가 가진 PhysicsMaterial 정보를 통해
구현   

### 라인트레이스
```c
bool AMainCharacter::TraceUnderCrossHair(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	//뷰포트의 사이즈를 먼저 구한다
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	//라인트레이스를 어느지점부터 어느지점까지 할껀지 구하기 (시작지점 끝지점 구하기)
	FVector2D CrossHairLocation{ ViewportSize.X / 2.f, ViewportSize.Y / 2.f };	//스크린을 가로 1 세로 1이라 가정할때 길이를 반으로 나누면 화면 정가운데 지점이 된다
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;
	//DeprojectScreenToWorld는 2D상의 좌표를 3D로 변환 해주는 역할을 한다
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrossHairLocation, CrossHairWorldPosition, CrossHairWorldDirection);

	if (bScreenToWorld)
	{
		const FVector Start{ CrossHairWorldPosition };
		const FVector End{ Start + CrossHairWorldDirection * 50000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		//히트하는 경우
		if (OutHitResult.bBlockingHit)
		{
			//라인트레이싱 끝부분을 히트한 부분 위치로 변환
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}
```
* 뷰포트의 Width Height의 사이즈를 구하고 반으로 나누면 화면 중간 위치를 구할 수 있다
* 구한 위치를 3D좌표로 투영하여 값을 구하고 라인트레이스를 한다
* 라인트레이스의 시작위치는 카메라의 위치이며 끝위치는 카메라 시작지점부터 화면정중앙의 지점의 위치를 더한 값이 된다 


### 아이템 슬롯
```c
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
TArray<AWeaponGun*> MainWeaponSlot;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
TArray<AWeaponGun*> SubWeaponSlot;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
TArray<AWeaponGun*> BoomWeaponSlot;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
TArray<AWeaponGun*> SpecialWeaponSlot;

for (int i = 0; i < INVENTORY_CAPACITY; i++)
{
    MainWeaponSlot.Add(nullptr);
    SubWeaponSlot.Add(nullptr);
    BoomWeaponSlot.Add(nullptr);
    SpecialWeaponSlot.Add(nullptr);	
}
```
* 무기타입별 슬롯을 만들며 게임시작시 Add를 통해 nullptr을 담은 배열을 생성      


### 아이템 습득
```c
if (MainAnimInstance)
{
	bPickUp = true;
	MainAnimInstance->Montage_Play(PickUpMontage);
	MainAnimInstance->Montage_JumpToSection("PickUpStart", PickUpMontage);
	FTimerHandle PickUpHandle;
	GetWorldTimerManager().SetTimer(PickUpHandle, this, &AMainCharacter::PickUpEnd, 0.71f);
}

case EWeaponType::EWT_MainWeapon:
{
	MainWeaponSlot[0] = gun;
	MainWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
	EquipSpare->AttachActor(MainWeaponSlot[0], GetMesh());
}break;

```
* 라인트레이스된 아이템을 습득하며 습득시 습득 애니메이션이 실행되고 GetWorldTimerManager을 통해 애니메이션 이후   
아이템 습득을 수행함   
* 아이템 습득시 캐릭터 스켈레탈 메쉬의 소켓에 부착되며 아이템 슬롯에 등록되게 된다

### 아이템 교체
```c
switch (GetEquippedWeapon()->GetWeaponType())
{
	case EWeaponType::EWT_MainWeapon:
	{

	}break;
	case EWeaponType::EWT_SubWeapon:
	{
		AnimInstance->Montage_Play(SwapMontage);
		AnimInstance->Montage_JumpToSection("NoEquipSwap", SwapMontage);
		GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapMainWeapon, 0.8f);
	}break;
	case EWeaponType::EWT_BoomWeapon:
	{
		AnimInstance->Montage_Play(LauncherSwapMontage);
		AnimInstance->Montage_JumpToSection("EquipLauncher", LauncherSwapMontage);
		GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapMainWeapon, 0.69f);
	}break;
	case EWeaponType::EWT_SpecialWeapon:
	{
		AnimInstance->Montage_Play(SpecialSwapMontage);
		AnimInstance->Montage_JumpToSection("EquipSpecial", SpecialSwapMontage);
		GetWorldTimerManager().SetTimer(SwapHandle, this, &AMainCharacter::SwapMainWeapon, 0.8f);
	}break;
}

if (MainWeaponSlot[0] != nullptr)
{
	EquipWeapon(MainWeaponSlot[0]);
	if (SubWeaponSlot[0] != nullptr)
	{
		SubWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
		EquipSpare2->AttachActor(SubWeaponSlot[0], GetMesh());
	}
	if (BoomWeaponSlot[0] != nullptr)
	{
		BoomWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
		EquipSpare3->AttachActor(BoomWeaponSlot[0], GetMesh());
	}
	if (SpecialWeaponSlot[0] != nullptr)
	{
		SpecialWeaponSlot[0]->SetItemState(EItemState::EIS_EquipSpare);
		EquipSpare4->AttachActor(SpecialWeaponSlot[0], GetMesh());
	}
}

```
* 아이템 교체시 교체 모션이 작동되며, GetWorldTimerManager를 이용하여 모션 후에 교체기능이 동작하도록 구현
* 교체시 이전 장비는 장착에서 해제되며 캐릭터 등쪽에 있는 무기 소켓으로 부착되게 됨
* 신규 장착하는 장비는 오른손의 소켓에 부착되게 됨   


### 발사체




