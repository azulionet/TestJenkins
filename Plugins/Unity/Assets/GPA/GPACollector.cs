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
using AOT;
using System;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace GPA
{
    // todo : 일부 남음
    public static class GPACollector
    {
        #region UTIL

        public static string GetStaticStringField(string className, string fieldName)
        {
            IntPtr rawClass;
            IntPtr method;

            try
            {
                rawClass = AndroidJNI.FindClass(className);
                method = AndroidJNI.GetStaticFieldID(rawClass, fieldName, "Ljava/lang/String;");
            }
            catch
            {
                throw;
            }

            return AndroidJNI.GetStaticStringField(rawClass, method);
        }

        public static string GetStaticField_JavaObj(string className, string fieldName)
        {
            AndroidJavaClass jc = new AndroidJavaClass(className);
            return jc.GetStatic<string>(fieldName);
        }

        #endregion UTIL

        // Scene Name ----------------------------------------------------

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate string pfnGetCurrentSceneName();

        [MonoPInvokeCallback(typeof(pfnGetCurrentSceneName))]
        [return: MarshalAs(UnmanagedType.LPStr)]
        public static string GetCurrentSceneName()
        {
            return SceneManager.GetActiveScene().name;
        }

        public static pfnGetCurrentSceneName delegateCurrentSceneName;

        // FrameTime ----------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate float pfnGetFrameTime();

        [MonoPInvokeCallback(typeof(pfnGetFrameTime))]
        public static float GetFrameTime()
        {
            return Time.unscaledDeltaTime;
        }

        public static pfnGetFrameTime delegateFrameTime;

        // Battery Charging ----------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int pfnGetBatteryCharging();

        [MonoPInvokeCallback(typeof(pfnGetBatteryCharging))]
        [return: MarshalAs(UnmanagedType.I4)]
        public static int GetBatteryCharging()
        {
            return (SystemInfo.batteryStatus == BatteryStatus.Discharging) == true ? 0 : 1;
        }

        public static pfnGetBatteryCharging delegateBatteryCharging;

        // DrawCallCount ----------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int pfnGetDrawCallCount();

        [MonoPInvokeCallback(typeof(pfnGetDrawCallCount))]
        [return: MarshalAs(UnmanagedType.I4)]
        public static int GetDrawCallCount()
        {
            int totalDrawCallCount = 0;

            foreach (var obj in Resources.FindObjectsOfTypeAll(typeof(Renderer)))
            {
                if ((obj as Renderer)?.isVisible ?? false)
                {
                    ++totalDrawCallCount;
                }
            }

            return totalDrawCallCount;
        }

        public static pfnGetDrawCallCount delegateDrawCallCount;

        // PrimitiveCount ----------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int pfnGetPrimitivesCount();

        [MonoPInvokeCallback(typeof(pfnGetPrimitivesCount))]
        [return: MarshalAs(UnmanagedType.I4)]
        public static int GetPrimitivesCount()
        {
            int totalVertexes = 0;
            int totalTriangles = 0;

            foreach (MeshFilter mf in Resources.FindObjectsOfTypeAll(typeof(MeshFilter)))
            {
                if (mf.gameObject.GetComponent<Renderer>()?.isVisible ?? false)
                {
                    if (mf.mesh.isReadable)
                    {
                        totalVertexes += mf.mesh.vertexCount;
                        totalTriangles += mf.mesh.triangles.Length;
                    }
                }
            }

            return totalTriangles * 3;
        }

        public static pfnGetPrimitivesCount delegatePrimitivesCount;

        // Battery Level ----------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int pfnGetBatteryLevel();

        [MonoPInvokeCallback(typeof(pfnGetBatteryLevel))]
        [return: MarshalAs(UnmanagedType.I4)]
        public static int GetBatteryLevel()
        {
            return (int)(SystemInfo.batteryLevel * 100.0f);
        }

        public static pfnGetBatteryLevel delegateBatteryLevel;

        // Battery Temperature ----------------------------------------------------

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate float pfnGetBatteryTemperature();

        [MonoPInvokeCallback(typeof(pfnGetBatteryTemperature))]
        [return: MarshalAs(UnmanagedType.R4)]
        public static float GetBatteryTemperature()
        {
            int temperature = GPAPlugin.Ins()?.androidPluginClass?.CallStatic<int>("NativeGetBatteryTemperature", GPAPlugin.Ins().androidContext) ?? 0;
            return temperature / 10.0f;
        }

        public static pfnGetBatteryTemperature delegateBatteryTemperature;

        // NetworkState ----------------------------------------------------
        // #define ANDROIDTHUNK_CONNECTION_TYPE_NONE 0
        // #define ANDROIDTHUNK_CONNECTION_TYPE_AIRPLANEMODE 1
        // #define ANDROIDTHUNK_CONNECTION_TYPE_ETHERNET 2
        // #define ANDROIDTHUNK_CONNECTION_TYPE_CELL 3
        // #define ANDROIDTHUNK_CONNECTION_TYPE_WIFI 4
        // #define ANDROIDTHUNK_CONNECTION_TYPE_WIMAX 5
        // #define ANDROIDTHUNK_CONNECTION_TYPE_BLUETOOTH 6
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int pfnGetNetworkState();

        [MonoPInvokeCallback(typeof(pfnGetNetworkState))]
        [return: MarshalAs(UnmanagedType.I4)]
        public static int GetNetworkState()
        {
            int neworkState = 0;
            switch (Application.internetReachability)
            {
                case NetworkReachability.NotReachable:
                    {
                        neworkState = 0;
                    }
                    break;

                case NetworkReachability.ReachableViaCarrierDataNetwork:
                    {
                        neworkState = 3;
                    }
                    break;

                case NetworkReachability.ReachableViaLocalAreaNetwork:
                    {
                        neworkState = 4;
                    }
                    break;
            }

            return neworkState;
        }

        public static pfnGetNetworkState delegateNetworkState;

        // GetTextureSize ----------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int pfnGetTextureSize();

        [MonoPInvokeCallback(typeof(pfnGetTextureSize))]
        [return: MarshalAs(UnmanagedType.I4)]
        public static int GetTextureSize()
        {
            return 0;
        }

        public static pfnGetTextureSize delegateTextureSize;

        // RenderTargetSize ----------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int pfnGetRenderTargetSize();

        [MonoPInvokeCallback(typeof(pfnGetRenderTargetSize))]
        [return: MarshalAs(UnmanagedType.I4)]
        public static int GetRenderTargetSize()
        {
            return 0;
        }

        public static pfnGetRenderTargetSize delegateRenderTargetSize;

        // for debugging ------------------------------------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate bool pfnBeginMessage();

        [MonoPInvokeCallback(typeof(pfnBeginMessage))]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static bool BeginMessage()
        {
            return GPADebug.BeginMessage();
        }

        public static pfnBeginMessage delegateBegineMessage;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void pfnAddMessage(int hash, IntPtr pStr);

        [MonoPInvokeCallback(typeof(pfnAddMessage))]
        public static void AddMessage(int hash, IntPtr pStr)
        {
            GPADebug.AddMessage(hash, Marshal.PtrToStringAnsi(pStr));
        }

        public static pfnAddMessage delegateAddMessage;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void pfnEndMessage();

        [MonoPInvokeCallback(typeof(pfnBeginMessage))]
        public static void EndMessage()
        {
            GPADebug.EndMessage();
        }

        public static pfnEndMessage delegateEndMessage;

        static GPACollector()
        {
            delegateCurrentSceneName = GetCurrentSceneName;
            delegateFrameTime = GetFrameTime;
            delegateBatteryCharging = GetBatteryCharging;
            delegateDrawCallCount = GetDrawCallCount;
            delegatePrimitivesCount = GetPrimitivesCount;
            delegateBatteryLevel = GetBatteryLevel;
            delegateBatteryTemperature = GetBatteryTemperature;
            delegateNetworkState = GetNetworkState;
            delegateTextureSize = GetTextureSize;
            delegateRenderTargetSize = GetRenderTargetSize;

            // debugging
            delegateBegineMessage = BeginMessage;
            delegateAddMessage = AddMessage;
            delegateEndMessage = EndMessage;
        }
    }
}