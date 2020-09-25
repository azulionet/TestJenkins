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
using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace GPA
{
    public enum GPALevel
    {
        SendLevel1,
        SendLevel2,
        SendLevel3,
        SendLevel4,
        SendLevel5,
    }

    public partial class GPAPlugin : MonoBehaviour
    {
        #region __GPA_DEFINE__

        private static string GetGPAVersion()
        {
            return "0.5.0";
        }

        #endregion __GPA_DEFINE__

        [DllImport("ncgpa")]
        private static extern bool GPA_UnityInit([MarshalAs(UnmanagedType.LPStr)] string headerVersion);

        [DllImport("ncgpa")]
        private static extern void GPA_Release();

        [DllImport("ncgpa")]
        private static extern void GPA_Tick(float deltaTime, out uint outResult, out float outVal);

        [DllImport("ncgpa")]
        private static extern void GPA_PushClientData([MarshalAs(UnmanagedType.LPStr)] string data);

        [DllImport("ncgpa")]
        private static extern void GPA_PushAction(int type, IntPtr action);

        [DllImport("ncgpa")]
        private static extern void GPA_PushImageData([MarshalAs(UnmanagedType.LPStr)] byte[] data, uint width, uint height, int colorType, int FlipType);

        [DllImport("ncgpa")]
        private static extern void GPA_OnPause();

        [DllImport("ncgpa")]
        private static extern void GPA_OnResume();

        [DllImport("ncgpa")]
        public static extern bool GPA_AddCustomMetric([MarshalAs(UnmanagedType.LPStr)] string name, int level, IntPtr fpValueChangedCallback);

        [DllImport("ncgpa")]
        public static extern bool GPA_SetCustomMetricBool([MarshalAs(UnmanagedType.LPStr)] string name, bool value);

        [DllImport("ncgpa")]
        public static extern bool GPA_SetCustomMetricString([MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.LPStr)] string value);

        [DllImport("ncgpa")]
        public static extern bool GPA_SetCustomMetricInteger([MarshalAs(UnmanagedType.LPStr)] string name, int value);

        [DllImport("ncgpa")]
        public static extern bool GPA_SetCustomMetricFloat([MarshalAs(UnmanagedType.LPStr)] string name, float value);

        [DllImport("ncgpa")]
        public static extern bool GPA_IsReadyToSetCustomMetric([MarshalAs(UnmanagedType.LPStr)] string name);

        [DllImport("ncgpa")]
        private static extern void GPA_PushDebugAction(int type, IntPtr action);
    }
}