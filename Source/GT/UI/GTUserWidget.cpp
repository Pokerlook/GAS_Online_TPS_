// Fill out your copyright notice in the Description page of Project Settings.


#include "GTUserWidget.h"
#include "GameFramework/PlayerController.h"

void UGTUserWidget::RefreshWdiget()
{
	InvalidateLayoutAndVolatility();
}

FReply UGTUserWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    // 엔터 키 확인
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        HandleEnterKey();
        return FReply::Handled();
    }

    // ESC 키 확인
    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        HandleEscKey();
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(MyGeometry, InKeyEvent);
}
