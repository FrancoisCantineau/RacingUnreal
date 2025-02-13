#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyPauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** Show Menu*/
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMenu();

	/** Hide Menu */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideMenu();
};
