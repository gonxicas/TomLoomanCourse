#include "SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget"))
		return;
	}
	
	FVector2d ScreenPosition;
	if(!UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		return;
	}
	
	float Scale = UWidgetLayoutLibrary::GetViewportScale(this);	
	ScreenPosition /= Scale;

	if(!ParentSizeBox)
	{
		return;
	}
	
	ParentSizeBox->SetRenderTranslation(ScreenPosition);
}
