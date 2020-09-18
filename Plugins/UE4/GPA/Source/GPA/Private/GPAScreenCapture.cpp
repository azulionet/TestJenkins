// GPA - NCSOFT Publishing Tech Division

// MIT License

// Copyright (c) 2020 NCSOFT Corporation. All rights reserved.

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following
// conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "GPAScreenCapture.h"
#include "Engine/Engine.h"

TArray<FColor> FGPAScreenCapture::ImageRaw;
bool FGPAScreenCapture::bScreenshotReady = false;
uint32 FGPAScreenCapture::ImageWidth = 0;
uint32 FGPAScreenCapture::ImageHeight = 0;
FDelegateHandle FGPAScreenCapture::OnDrawnHandle;

bool FGPAScreenCapture::RequestScreenshot()
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr) { return false; }
	if (bScreenshotReady == true) { return false; }

	FGPAScreenCapture::Reset();
	OnDrawnHandle = GEngine->GameViewport->OnDrawn().AddStatic(&FGPAScreenCapture::TakeScreenshot);
	return true;
}

void FGPAScreenCapture::TakeScreenshot()
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr) { return; }

	if (bScreenshotReady == true)
	{
		if (OnDrawnHandle.IsValid())
		{
			GEngine->GameViewport->OnDrawn().Remove(OnDrawnHandle);
			OnDrawnHandle.Reset();
		}
		return;
	}

	FIntRect CaptureRect(0, 0, 0, 0);
	auto* Viewport = GEngine->GameViewport->Viewport;

	if (Viewport == nullptr) { return; }
	CaptureRect.Max = Viewport->GetSizeXY();
	ImageRaw.Empty();

	if (Viewport->ReadPixels(ImageRaw, FReadSurfaceDataFlags(), CaptureRect))
	{
		ImageWidth = Viewport->GetSizeXY().X;
		ImageHeight = Viewport->GetSizeXY().Y;

		bScreenshotReady = true;

		if (OnDrawnHandle.IsValid())
		{
			GEngine->GameViewport->OnDrawn().Remove(OnDrawnHandle);
			OnDrawnHandle.Reset();
		}
	}
}