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





