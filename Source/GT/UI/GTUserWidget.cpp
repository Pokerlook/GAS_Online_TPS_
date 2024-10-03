// Fill out your copyright notice in the Description page of Project Settings.


#include "GTUserWidget.h"
#include "GameFramework/PlayerController.h"

void UGTUserWidget::RefreshWdiget()
{
	InvalidateLayoutAndVolatility();
}

FReply UGTUserWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    // ���� Ű Ȯ��
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        HandleEnterKey();
        return FReply::Handled();
    }

    // ESC Ű Ȯ��
    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        HandleEscKey();
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(MyGeometry, InKeyEvent);
}
