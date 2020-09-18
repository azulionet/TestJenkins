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
#include "GPAAdapter.h"

#include "Engine/Engine.h"
#include "Misc/CoreDelegates.h"

#include "GPAPluginData.h"
#include "GPAScreenCapture.h"
#include "GPACollector.h"
#include "GPADebug.h"
#include "SharedDefine.h"

#if PLATFORM_ANDROID
#include <Android/AndroidApplication.h>
#include <dlfcn.h>
extern JavaVM* GJavaVM;
extern jclass GGameActivityClassID;
#endif

FGPAAdapter::~FGPAAdapter()
{
	UE_LOG(LogTemp, Warning, TEXT("GPA - ~FGPAAdapter()"));
	CleanUp();
}

bool FGPAAdapter::Init(void* DllHandle, FFinishCallback Callback)
{
	if (DllHandle == nullptr)
	{
		return false;
	}

	Finish = Callback;

	bInit = InitAPI(DllHandle);

	if (bInit)
	{
#if PLATFORM_WINDOWS
		bInit = FpInit(nullptr, GPA::_GPAHeaderVersion);
#elif PLATFORM_ANDROID
		bInit = FpInit(GJavaVM, GPA::_GPAHeaderVersion);
#endif
	}

	if (bInit)
	{
		bInit = InitEngineDelegate();
	}

	if (bInit)
	{
		UE_LOG(LogTemp, Warning, TEXT("GPA - Init Complete"));

		PushClientData();
		PushAction();
	}

	if (!bInit)
	{
		UE_LOG(LogTemp, Warning, TEXT("GPA - Init false"));
		CleanUp();
	}

	return bInit;
}

void FGPAAdapter::OnPause()
{
	UE_LOG(LogTemp, Warning, TEXT("GPA - FGPAAdapter::OnPause()"));

	if (FpOnPause)
	{
		FpOnPause();
	}
}

void FGPAAdapter::OnResume()
{
	UE_LOG(LogTemp, Warning, TEXT("GPA - FGPAAdapter::OnResume()"));

	if (FpOnResume)
	{
		FpOnResume();
	}
}

void FGPAAdapter::OnDestroy()
{
	UE_LOG(LogTemp, Warning, TEXT("GPA - FGPAAdapter::OnDestroy()"));
	CleanUp();
}

void FGPAAdapter::CleanUp()
{
	UE_LOG(LogTemp, Warning, TEXT("GPA - FGPAAdapter::CleanUp()"));

	ReleaseEngineDelegate();

	if (FpRelease)
	{
		FpRelease();
	}

	ReleaseAPI();
}

void FGPAAdapter::Tick(float DeltaTime)
{
	if (bInit == false)
	{
		return;
	}

	bool bDestroy = false;

	if (FpTick)
	{
		FGPACollector::SetFrameTime(DeltaTime);

#if PLATFORM_ANDROID
		if (FpValidation)
		{
			FpValidation(&GGameActivityClassID);
		}
#endif

		uint32 Result = 0;
		float Value = 0;

		FpTick(DeltaTime, Result, Value);

#if UE_BUILD_DEVELOPMENT
		FGPADebug::PrintMessage();
#endif

		if (Result > 0)
		{
			if (Result & (int)GPA::ETickResult::ScreenShot)
			{
				UE_LOG(LogTemp, Warning, TEXT("GPA - Abnormal - RequestScreenshot"));
				FGPAScreenCapture::RequestScreenshot();
			}

			if (Result & (int)GPA::ETickResult::AbnormalStart)
			{
				UE_LOG(LogTemp, Warning, TEXT("GPA - Abnormal detected"));
			}
			else if (Result & (int)GPA::ETickResult::AbnormalEnd)
			{
				UE_LOG(LogTemp, Warning, TEXT("GPA - Abnormal End"));
			}
			else if (Result & (int)GPA::ETickResult::DestroyGPA)
			{
				UE_LOG(LogTemp, Warning, TEXT("GPA - DestroyGPA"));
				bDestroy = true;
			}
		}
	}

	if (FGPAScreenCapture::IsScreenShotReady())
	{
		if (FpPushImageData)
		{
			FpPushImageData((unsigned char*)(FGPAScreenCapture::ImageRaw.GetData()),
				FGPAScreenCapture::ImageWidth, FGPAScreenCapture::ImageHeight,
				(int)GPA::EImageColorFormat::BGRA_8888, (int)GPA::EImageFlipOp::None);
			FGPAScreenCapture::Reset();
		}
	}

	if (bDestroy == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("GPA - Destroy Plugin"));
		Finish.ExecuteIfBound();
	}
}

TStatId FGPAAdapter::GetStatId() const
{
	return TStatId();
}

bool FGPAAdapter::PushClientData() const
{
	if (FpPushClientData)
	{
		FString Content = FGPAClientData::GetClientData();
		FpPushClientData(TCHAR_TO_ANSI(*Content));
	}

	return true;
}

void FGPAAdapter::PushAction()
{
	UE_LOG(LogTemp, Warning, TEXT("GPA - FGPAAdapter::PushAction()"));

#if PLATFORM_ANDROID
	if (FpPushAction)
	{
		FpPushAction((int)GPA::EMetrics::Current_scene, (void*)FGPACollector::GetSceneName);
		FpPushAction((int)GPA::EMetrics::Frame_time, (void*)FGPACollector::GetFrameTime);
		FpPushAction((int)GPA::EMetrics::Battery_charging, (void*)FGPACollector::GetBatteryCharging);
		FpPushAction((int)GPA::EMetrics::Drawcall_count, (void*)FGPACollector::GetDrawCallCount);
		FpPushAction((int)GPA::EMetrics::Primitives_count, (void*)FGPACollector::GetPrimitivesCount);
		FpPushAction((int)GPA::EMetrics::Battery_level, (void*)FGPACollector::GetBatteryLevel);
		FpPushAction((int)GPA::EMetrics::Battery_temperature, (void*)FGPACollector::GetBatteryTemperature);
		FpPushAction((int)GPA::EMetrics::Network_state, (void*)FGPACollector::GetNetworkState);
		FpPushAction((int)GPA::EMetrics::Texture_size, (void*)FGPACollector::GetTextureSize);
		FpPushAction((int)GPA::EMetrics::Rendertarget_size, (void*)FGPACollector::GetRenderTargetSize);
		FpPushAction((int)GPA::EMetrics::Current_resolution, (void*)FGPACollector::GetCurrentResolution);
	}
#elif PLATFORM_WINDOWS
	if (FpPushAction)
	{
		FpPushAction((int)GPA::EMetrics::Current_scene, (void*)FGPACollector::GetSceneName);
		FpPushAction((int)GPA::EMetrics::Frame_time, (void*)FGPACollector::GetFrameTime);
		FpPushAction((int)GPA::EMetrics::Drawcall_count, (void*)FGPACollector::GetDrawCallCount);
		FpPushAction((int)GPA::EMetrics::Primitives_count, (void*)FGPACollector::GetPrimitivesCount);
		FpPushAction((int)GPA::EMetrics::Texture_size, (void*)FGPACollector::GetTextureSize);
		FpPushAction((int)GPA::EMetrics::Rendertarget_size, (void*)FGPACollector::GetRenderTargetSize);
		FpPushAction((int)GPA::EMetrics::Current_resolution, (void*)FGPACollector::GetCurrentResolution);
	}
#endif // PLATFORM

#if UE_BUILD_DEVELOPMENT
	if (FpPushDebugAction)
	{
		FpPushDebugAction((int)GPA::EDebugMessageOp::BeginMessage, (void*)FGPADebug::BeginMessage);
		FpPushDebugAction((int)GPA::EDebugMessageOp::AddMessage, (void*)FGPADebug::AddMessage);
		FpPushDebugAction((int)GPA::EDebugMessageOp::EndMessage, (void*)FGPADebug::EndMessage);
	}
#endif // UE_BUILD_DEVELOPMENT
}

bool FGPAAdapter::InitAPI(void* DllHandle)
{
#if PLATFORM_WINDOWS
	FpInit = (PFN_init)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_Init"));
	FpRelease = (PFN_release)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_Release"));
	FpTick = (PFN_tick)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_Tick"));
	FpValidation = (PFN_validation)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_Validation"));
	FpPushClientData = (PFN_pushClientData)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_PushClientData"));
	FpPushAction = (PFN_pushAction)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_PushAction"));
	FpPushImageData = (PFN_pushImageData)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_PushImageData"));
	FpOnPause = (PFN_onPause)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_OnPause"));
	FpOnResume = (PFN_onResume)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_OnResume"));
	FpPushDebugAction = (PFN_pushDebugAction)FPlatformProcess::GetDllExport(DllHandle, TEXT("GPA_PushDebugAction"));
#elif PLATFORM_ANDROID
	FpInit = (PFN_init)dlsym(DllHandle, "GPA_Init");
	FpRelease = (PFN_release)dlsym(DllHandle, "GPA_Release");
	FpTick = (PFN_tick)dlsym(DllHandle, "GPA_Tick");
	FpValidation = (PFN_validation)dlsym(DllHandle, "GPA_Validation");
	FpPushClientData = (PFN_pushClientData)dlsym(DllHandle, "GPA_PushClientData");
	FpPushAction = (PFN_pushAction)dlsym(DllHandle, "GPA_PushAction");
	FpPushImageData = (PFN_pushImageData)dlsym(DllHandle, "GPA_PushImageData");
	FpOnPause = (PFN_onPause)dlsym(DllHandle, "GPA_OnPause");
	FpOnResume = (PFN_onResume)dlsym(DllHandle, "GPA_OnResume");
	FpPushDebugAction = (PFN_pushDebugAction)dlsym(DllHandle, "GPA_PushDebugAction");
#endif

	if (FpInit && FpRelease && FpTick && FpValidation&& FpPushClientData &&
		FpPushAction && FpPushAction && FpPushImageData && FpOnPause &&
		FpOnResume)
	{
		return true;
	}

	ReleaseAPI();
	return false;
}

void FGPAAdapter::ReleaseAPI()
{
	FpInit = nullptr;
	FpRelease = nullptr;
	FpTick = nullptr;
	FpValidation = nullptr;
	FpPushClientData = nullptr;
	FpPushAction = nullptr;
	FpPushImageData = nullptr;
	FpOnPause = nullptr;
	FpOnResume = nullptr;
	FpPushDebugAction = nullptr;
}

bool FGPAAdapter::InitEngineDelegate()
{
	ReleaseEngineDelegate();

	OnResuemHandle = FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FGPAAdapter::OnPause);
	OnPauseHandle = FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FGPAAdapter::OnResume);
	OnDestroyHandle = FCoreDelegates::ApplicationWillTerminateDelegate.AddRaw(this, &FGPAAdapter::OnDestroy);

	if (OnResuemHandle.IsValid() == false ||
		OnPauseHandle.IsValid() == false ||
		OnDestroyHandle.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("GPA - Delegate registration error."));

		ReleaseEngineDelegate();
		return false;
	}

	return true;
}

void FGPAAdapter::ReleaseEngineDelegate()
{
	if (OnResuemHandle.IsValid())
	{
		FCoreDelegates::ApplicationWillEnterBackgroundDelegate.Remove(OnResuemHandle);
		OnResuemHandle.Reset();
	}
	if (OnPauseHandle.IsValid())
	{
		FCoreDelegates::ApplicationHasEnteredForegroundDelegate.Remove(OnPauseHandle);
		OnPauseHandle.Reset();
	}
	if (OnDestroyHandle.IsValid())
	{
		FCoreDelegates::ApplicationWillTerminateDelegate.Remove(OnDestroyHandle);
		OnDestroyHandle.Reset();
	}
}