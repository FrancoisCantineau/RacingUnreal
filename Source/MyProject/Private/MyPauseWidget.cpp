#include "MyPauseWidget.h"

void UMyPauseWidget::ShowMenu()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UMyPauseWidget::HideMenu()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}