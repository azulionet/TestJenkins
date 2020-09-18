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
#pragma once
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Runtime/RHI/Public/RHI.h"
#include <string>

#if PLATFORM_ANDROID
#include "android/native_window.h"
#include "Android/AndroidWindow.h"
struct ANativeWindow;

#if ENGINE_MINOR_VERSION < 23
#include "Android/AndroidMisc.h"
#else
#include "Android/AndroidPlatformMisc.h"
#endif

#elif PLATFORM_WINDOWS
#include "Windows/MinWindows.h"
#include "Engine/Engine.h"
#endif

class FGPACollector
{
public:
	static const char* GetSceneName()
	{
		if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		{
			return "Unknown";
		}
		FWorldContext const* ContextToUse = NULL;

		for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
		{
			if (WorldContext.WorldType == EWorldType::Game)
			{
				ContextToUse = &WorldContext;
				break;
			}
		}

		if (ContextToUse != NULL)
		{
			GSceneName = TCHAR_TO_ANSI(*ContextToUse->World()->GetMapName());
		}
		else
		{
			return "Unknown";
		}

		//g_SceneName = TCHAR_TO_ANSI(*GEngine->GameViewport->GetWorld()->GetMapName());
		return GSceneName.c_str();
	}

	static float GetFrameTime()
	{
		return GFrameTime;
	}

	static void SetFrameTime(float FrameTime)
	{
		GFrameTime = FrameTime;
	}

	static int GetNetworkState()
	{
#if PLATFORM_ANDROID
		return static_cast<int>(FAndroidMisc::GetNetworkConnectionType());
#else
		return 0;
#endif
	}

	static int GetDrawCallCount()
	{
		return GNumDrawCallsRHI;
	}

	static int GetPrimitivesCount()
	{
		return GNumPrimitivesDrawnRHI;
	}

	static int GetTextureSize()
	{
		return GCurrentTextureMemorySize;
	}
	static int GetRenderTargetSize()
	{
		return GCurrentRendertargetMemorySize;
	}

	static int GetBatteryLevel()
	{
#if PLATFORM_ANDROID
		return FAndroidMisc::GetBatteryState().Level;
#else
		return 0;
#endif
	}

	static float GetBatteryTemperature()
	{
#if PLATFORM_ANDROID
		return static_cast<float>(FAndroidMisc::GetDeviceTemperatureLevel());
#else
		return 0.0f;
#endif
	}

	static int GetBatteryCharging()
	{
#if PLATFORM_ANDROID
		return static_cast<int>(!FAndroidMisc::IsRunningOnBattery());
#else
		return 0;
#endif
	}

	static uint64 GetCurrentResolution()
	{
		int32 X = 0;
		int32 Y = 0;

		GetResolution(X, Y);

		static const auto MAKE_UINT64 = [](int a, int b) -> uint64
		{ return ((uint64)(((uint32)((a) & 0xffffffff)) | ((uint64)((uint32)((b) & 0xffffffff))) << 32));};
		return MAKE_UINT64(X, Y);
	}

	static void GetResolution(int32& X, int32& Y)
	{
#if PLATFORM_ANDROID

#if ENGINE_MINOR_VERSION >= 24
		auto* Window = (ANativeWindow*)FAndroidWindow::GetHardwareWindow_EventThread();
#else
		auto* Window = (ANativeWindow*)FAndroidWindow::GetHardwareWindow();
#endif

		X = ANativeWindow_getWidth(Window);
		Y = ANativeWindow_getHeight(Window);
#else

		RECT rt;
		if (GEngine == nullptr || GEngine->GameViewport == nullptr)
		{
			HWND hwnd = GetActiveWindow();

			GetClientRect((HWND)hwnd, &rt);

			X = rt.right;
			Y = rt.bottom;
			return;
		}

		auto hwnd = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		GetClientRect((HWND)hwnd, &rt);

		X = rt.right;
		Y = rt.bottom;

#endif
	}

	static const char* GetGameVersion()
	{
		FString TargetName;

		return TCHAR_TO_ANSI(*TargetName);
	}

	static float GFrameTime;
	static std::string GSceneName;
};


