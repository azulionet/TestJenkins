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

///////////////////////////////////////////////////////////////////////////
// IMPORTANT : DO NOT MODIFY THIS WHEN YOU DONT KNOW WHAT YOU ARE DOING. //
///////////////////////////////////////////////////////////////////////////

namespace GPA
{
	constexpr const char* const _GPAHeaderVersion = "0.5.0";
	constexpr const char* const _GPAGame = "GPAGame";

	enum class EDeviceInfo : unsigned int
	{
		//env_device
		Device_uuid = 0,
		Device_manufacturer,
		Device_model,
		Device_board,
		Device_resolution_x,
		Device_resolution_y,
		Device_ip,
		Device_telecommunications,
		//env_cpu
		Cpu_architecture,
		Cpu_core,
		Cpu_clockrange_min,
		Cpu_clockrange_max,
		//env_gpu
		Gpu_model,
		Gpu_gles_version,
		Gpu_vulkan_version,
		Gpu_driver_version,
		Gpu_api,
		Gpu_extension,
		//env_memory
		Memory_ram_total,
		Memory_rom_total,
		Memory_rom_available,
		//env_os
		Os_type,
		Os_version,
		Os_sdk_version,
		Os_language,
		Os_locale,
		//env_game
		Game_name,
		Game_version,
		Game_package,
		//env_plugin
		Plugin_version,
		//env_engine
		Engine_name,
		Engine_version,
		MAX_ENV
	};

	// All String format
	enum class EMetrics : unsigned int
	{
		Current_scene = 0,
		Frame_time,
		Cpu_usage,
		Cpu_frequency,
		Thread_usage,
		Gpu_usage,
		Gpu_frequency,
		Network_usage,
		Battery_charging,
		Power_saving,
		Drawcall_count,
		Primitives_count,
		Gpu_cycles,
		Gpu_vertex_cycles,
		Gpu_fragment_cycles,

		Memory_usage,
		Battery_level,
		Battery_temperature,
		Network_state,

		Frame_image_url,
		Texture_size,
		Rendertarget_size,
		Gpu_tiler_cycles,
		Gpu_ext_memory_read,
		Gpu_ext_memory_write,

		Current_resolution,
		Gpu_temperature,

		MAX_METRICS
	};

	enum class EImageColorFormat : unsigned int
	{
		BGRA_8888, // UE4
		RGBA_8888, // Unity
		MAX_COLOR_FORMAT
	};

	enum class ETickResult : unsigned int
	{
		Success = 0,
		ScreenShot = 1 << 0,
		AbnormalStart = 1 << 1,
		AbnormalEnd = 1 << 2,
		DeviceLost = 1 << 3,
		BufferFull = 1 << 4,
		HasPolicy = 1 << 5,
		DestroyGPA = 1 << 6
	};

	enum class EImageFlipOp : unsigned int
	{
		None = 0,
		Horizontal = 1 << 0,
		Vertical = 1 << 1
	};

	enum class EDebugMessageOp : unsigned int
	{
		BeginMessage = 0,
		AddMessage,
		EndMessage,
		MAX_DEBUG_MESSAGES
	};
}