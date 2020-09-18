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

#include "GPAPluginData.h"
#include "GPACollector.h"
#include "HardwareInfo.h"

#if PLATFORM_ANDROID
#if ENGINE_MINOR_VERSION < 23
#include "Android/AndroidMisc.h"
#else
#include "Android/AndroidPlatformMisc.h"
#endif
#elif PLATFORM_WINDOWS
#include "Windows/WindowsPlatformMisc.h"
#endif

FString FGPAClientData::GetClientData()
{
	UE_LOG(LogTemp, Warning, TEXT("GetClientData"));

	FString Content = "";
	Content.Reserve(256);

#if PLATFORM_WINDOWS

	Add(Content, "device_uuid", FGenericPlatformMisc::GetDeviceId());
	Add(Content, "game_version", FHardwareInfo::GetHardwareInfo(NAME_RHI));

#elif PLATFORM_ANDROID

	Add(Content, "device_uuid", FAndroidMisc::GetDeviceId());

#endif // Platform

	int32 X = 0, Y = 0;
	FGPACollector::GetResolution(X, Y);
	Add(Content, "device_resolution_x", X);
	Add(Content, "device_resolution_y", Y);

	Add(Content, "gpu_api", FHardwareInfo::GetHardwareInfo(NAME_RHI));

	Add(Content, "engine_name", EPIC_PRODUCT_IDENTIFIER);
	Add(Content, "engine_version", ENGINE_VERSION_STRING);

	return Content;
}

void FGPAClientData::Add(FString& Out, const char* Key, FString Source)
{
	if (Key == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	if (Source.Len() > 0)
	{
		Out += Source;
	}

	Out += GetValueSeperator();
}

void FGPAClientData::Add(FString& Out, const char* Key, const TCHAR* Source)
{
	if (Key == nullptr || Source == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	Out += Source;
	Out += GetValueSeperator();
}

void FGPAClientData::Add(FString& Out, const char* Key, const ANSICHAR* Source)
{
	if (Key == nullptr || Source == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	Out += Source;
	Out += GetValueSeperator();
}

void FGPAClientData::Add(FString& Out, const char* Key, uint64 Source)
{
	if (Key == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	Out += FString::Printf(TEXT("%llu"), Source);
	Out += GetValueSeperator();
}

void FGPAClientData::Add(FString& Out, const char* Key, int64 Source)
{
	if (Key == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	Out += FString::Printf(TEXT("%lli"), Source);
	Out += GetValueSeperator();
}

void FGPAClientData::Add(FString& Out, const char* Key, int32 Source)
{
	if (Key == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	Out += FString::FromInt(Source);
	Out += GetValueSeperator();
}

void FGPAClientData::Add(FString& Out, const char* Key, bool Source)
{
	if (Key == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	Out += (Source == true ? "true" : "false");
	Out += GetValueSeperator();
}

void FGPAClientData::Add(FString& Out, const char* Key, float Source)
{
	if (Key == nullptr) { return; }

	Out += Key;
	Out += GetTypeSeperator();

	Out += FString::SanitizeFloat(Source);
	Out += GetValueSeperator();
}
