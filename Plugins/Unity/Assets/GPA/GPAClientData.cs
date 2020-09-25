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
using System.Text;
using UnityEngine;

namespace GPA
{
    public class GPAClientData : MonoBehaviour
    {
        public static string GetClientData()
        {
            StringBuilder builder = new StringBuilder();

            builder.Append("device_uuid").Append("$").Append(SystemInfo.deviceUniqueIdentifier).Append(",");
            builder.Append("device_resolution_x").Append("$").Append(Screen.currentResolution.width).Append(",");
            builder.Append("device_resolution_y").Append("$").Append(Screen.currentResolution.height).Append(",");
            builder.Append("gpu_api").Append("$").Append(SystemInfo.graphicsDeviceType.ToString()).Append(",");
            builder.Append("engine_name").Append("$").Append("Unity").Append(",");
            builder.Append("engine_version").Append("$").Append(Application.unityVersion).Append(",");

            return builder.ToString();
        }
    }
}