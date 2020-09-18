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
#include "Tickable.h"
#include "Modules/ModuleManager.h"

DECLARE_DELEGATE(FFinishCallback);

class FGPAAdapter : public FTickableGameObject
{
public: /** FTickableGameObject */

	virtual ~FGPAAdapter();

	virtual bool IsTickableWhenPaused() const override { return false; }
	virtual void Tick(float DeltaTime);
	virtual TStatId GetStatId() const override;

public:
	bool Init(void* DllHandle, FFinishCallback Callback);

	void OnPause();
	void OnResume();
	void OnDestroy();
	void CleanUp();

private:
	bool PushClientData() const;
	void PushAction();

	bool InitAPI(void* DllHandle);
	void ReleaseAPI();

	bool InitEngineDelegate();
	void ReleaseEngineDelegate();

public:

	bool bInit = false;

	FDelegateHandle OnResuemHandle;
	FDelegateHandle OnPauseHandle;
	FDelegateHandle OnDestroyHandle;

	// CustomParameter
	FDelegateHandle OnChangeResolutionHandle;
	FDelegateHandle OnChangeLowBatteryStateHandle;

	FFinishCallback Finish;

	// GPA Function
	using PFN_init = bool(*)(void* Platform, const char* HeaderVersion);
	using PFN_release = void(*)();
	using PFN_tick = void(*)(float DeltaTime, unsigned int& Result, float& Value);
	using PFN_validation = void(*)(void* GameActivity);

	using PFN_pushClientData = void(*)(const char* Data);
	using PFN_pushAction = void(*)(int Type, void* Action);
	using PFN_pushImageData = void(*)(unsigned char* Buffer, unsigned int Width, unsigned int Height, int ColorType, int FlipType);
	using PFN_onPause = void(*)();
	using PFN_onResume = void(*)();
	using PFN_pushDebugAction = void(*)(int Type, void* Action);

	PFN_init						FpInit = nullptr;
	PFN_release						FpRelease = nullptr;
	PFN_tick						FpTick = nullptr;
	PFN_validation					FpValidation = nullptr;
	PFN_pushClientData				FpPushClientData = nullptr;
	PFN_pushAction					FpPushAction = nullptr;
	PFN_pushImageData				FpPushImageData = nullptr;
	PFN_onPause						FpOnPause = nullptr;
	PFN_onResume					FpOnResume = nullptr;
	PFN_pushDebugAction				FpPushDebugAction = nullptr;
};
