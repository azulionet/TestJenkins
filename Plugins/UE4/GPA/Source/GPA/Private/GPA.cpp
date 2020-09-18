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
#include "GPA.h"
#include "GPAAdapter.h"

#if PLATFORM_ANDROID
#include <dlfcn.h>
#elif PLATFORM_WINDOWS
#include "Windows/WindowsPlatformMisc.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"
#endif

#define LOCTEXT_NAMESPACE "FGPAModule"

void FGPAModule::StartupModule()
{
#if WITH_EDITOR

	UE_LOG(LogTemp, Log, TEXT("GPA is not running on editor."));
	ShutdownModule();
	return;

#endif

	UE_LOG(LogTemp, Log, TEXT("GPA - StartupModule"));

	bool bInit = false;
	LibraryHandle = nullptr;

#if PLATFORM_WINDOWS

	auto GPAPlg = IPluginManager::Get().FindPlugin("GPA");
	if (GPAPlg.Get() == nullptr) { return; }

	FString BaseDir = GPAPlg->GetBaseDir();
	FString LibraryPath;

#	if PLATFORM_64BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/GPA/lib/x64/NCGPA.dll"));
#	else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/GPA/lib/x86/NCGPA.dll"));
#	endif

	LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

#elif PLATFORM_ANDROID

	LibraryHandle = dlopen("libncgpa.so", RTLD_NOW | RTLD_LOCAL);

#endif
	if (LibraryHandle)
	{
		GPA = MakeUnique<FGPAAdapter>();

		if (GPA)
		{
			FFinishCallback Callback;
			Callback.BindRaw(this, &FGPAModule::ShutdownModule);

			bInit = GPA->Init(LibraryHandle, Callback);
		}
	}

	if (bInit == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("GPA - initialization failed."));
		ShutdownModule();
	}
}

void FGPAModule::ShutdownModule()
{
	GPA = nullptr;
	DLLCleanUp();
}

void FGPAModule::DLLCleanUp()
{
	if (LibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS

		FPlatformProcess::FreeDllHandle(LibraryHandle);

#elif PLATFORM_ANDROID

		dlclose(LibraryHandle);

#endif

		LibraryHandle = nullptr;

		UE_LOG(LogTemp, Log, TEXT("***** GPA : dll unload ***** "));
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGPAModule, GPA)