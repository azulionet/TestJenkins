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
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace GPA
{
    [RequireComponent(typeof(GPAScreenCapture))]
    public partial class GPAPlugin : MonoBehaviour
    {
        #region INSPECTOR

        public GPAScreenCapture mImageCapture;

        #endregion INSPECTOR

        #region SINGLETON

        private static GPAPlugin Instance = null;
        public static bool bDestroyPermanently = false;

        public static GPAPlugin Ins()
        {
            if (bDestroyPermanently == true)
            {
                return null;
            }

            if (Instance == null)
            {
                Instance = FindObjectOfType(typeof(GPAPlugin)) as GPAPlugin;
                if (Instance == null)
                {
                    Instance = new GameObject("GPA", typeof(GPAPlugin)).GetComponent<GPAPlugin>();
                }
            }

            return Instance;
        }

        #endregion SINGLETON

        public bool mInit { get; private set; }
        public bool mAbnormal { get; private set; }

        public AndroidJavaClass androidPluginClass = null;
        public AndroidJavaObject androidContext = null;

        private void Awake()
        {
            if (Instance != null)
            {
                CleanUp();
                return;
            }

            Debug.Log("------------------------------- GPA2 android plugin init ");

            Init();
            return;

            void Init()
            {
                Instance = this;

                DontDestroyOnLoad(this);

                try
                {
                    mInit = GPA_UnityInit(GetGPAVersion());

                    Debug.Log("GPA Init Result - " + mInit);

                    if (mInit == true)
                    {
                        PushClientData();
                        PushAction();

                        mImageCapture = gameObject.GetComponent<GPAScreenCapture>();

                        if (mImageCapture == null)
                        {
                            mImageCapture = gameObject.AddComponent<GPAScreenCapture>();
                        }

                        mImageCapture.enabled = true;

                        AndroidInit();
                    }
                }
                catch (Exception e)
                {
                    Debug.Log("GPA2 Exception : " + e.Message);
                }
            }

            void AndroidInit()
            {
#if UNITY_ANDROID
                try
                {
                    androidPluginClass = new AndroidJavaClass("com.ncsoft.gpa.GPAPlugin");

                    if (androidPluginClass == null)
                    {
                        Debug.Log("GPA - no plugin class");
                    }

                    using (AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer"))
                    {
                        if (unityPlayer == null)
                        {
                            Debug.Log("GPA - no unityPlayer class");
                        }

                        using (AndroidJavaObject activity = unityPlayer?.GetStatic<AndroidJavaObject>("currentActivity") ?? null)
                        {
                            if (unityPlayer == null)
                            {
                                Debug.Log("GPA - no current activity class");
                            }

                            androidContext = activity?.Call<AndroidJavaObject>("getApplicationContext") ?? null;

                            if (androidContext == null)
                            {
                                Debug.Log("GPA - no current androidContext class");
                            }
                        }
                    }
                }
                catch (Exception E)
                {
                    Debug.Log("GPA - Exception : " + E.Message);
                }

#endif
            }
        }

        private void CleanUp()
        {
            Debug.Log("GPA - CleanUp");

            mInit = false;
            Destroy(this);
        }
        
        private void Update()
        {
            if (mInit == false) { return; }

            bool bDestroy = false;

            GPA_Tick(Time.deltaTime, out uint outResult1, out float Result2);

            if (outResult1 > 0)
            {
                if ((outResult1 & 1) == 1)
                {
                    RequestScreenShot();
                }

                if ((outResult1 & (1 << 1)) == (1 << 1))
                {
                    mAbnormal = true;
                }
                else if ((outResult1 & (1 << 2)) == (1 << 2))
                {
                    mAbnormal = false;
                }
                else if ((outResult1 & (1 << 6)) == (1 << 6))
                {
                    bDestroy = true;
                }
            }

            if (bDestroy == true)
            {
                bDestroyPermanently = true;
                Debug.Log("GPA - start Destory");
                CleanUp();
            }
        }

        private List<string> mDebugString = new List<string>();
        void OnGUI()
        {
            if (Debug.isDebugBuild)
            {
                GPADebug.GetAddedMessages(ref mDebugString);

                GUIStyle style = GUI.skin.textArea;
                style.alignment = TextAnchor.MiddleLeft;
                style.fontSize = 20;

                GUILayout.Space(100);
                GUILayout.BeginVertical(style);
                for (int index = 0; index < mDebugString.Count; ++index)
                {
                    GUILayout.Label(mDebugString[index], style);
                }
                GUILayout.EndVertical();
            }
        }

        private void PushClientData()
        {
            Debug.Log("GPA - PushClientData");

            var ClientData = GPAClientData.GetClientData();
            GPA_PushClientData(ClientData);
        }

        private void PushAction()
        {
            Debug.Log("GPA - PushAction");

            // Action
            {
                GPA_PushAction(0, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateCurrentSceneName));
                GPA_PushAction(1, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateFrameTime));
                GPA_PushAction(8, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateBatteryCharging));
                GPA_PushAction(10, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateDrawCallCount));
                GPA_PushAction(11, Marshal.GetFunctionPointerForDelegate(GPACollector.delegatePrimitivesCount));
                GPA_PushAction(16, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateBatteryLevel));
                GPA_PushAction(17, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateBatteryTemperature));
                GPA_PushAction(18, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateNetworkState));
                GPA_PushAction(20, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateTextureSize));
                GPA_PushAction(21, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateRenderTargetSize));
            }

            // Debug Option
            if (Debug.isDebugBuild)
            {
                GPA_PushDebugAction(0, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateBegineMessage));
                GPA_PushDebugAction(1, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateAddMessage));
                GPA_PushDebugAction(2, Marshal.GetFunctionPointerForDelegate(GPACollector.delegateEndMessage));
            }
        }

        public void RequestScreenShot()
        {
            if (mInit == false) { return; }

            Debug.Log("GPA - RequestScreenShot");
            mImageCapture.RequestScreenshot((byte[] imageData) =>
            {
                GPA_PushImageData(imageData, (uint)Screen.width, (uint)Screen.height, 1, 2);
            });
        }

        private void OnApplicationPause(bool pauseStatus)
        {
            if (mInit == false) { return; }

            if (pauseStatus == true)
            {
                Debug.Log("GPA - OnPause");
                GPA_OnPause();
            }
            else
            {
                Debug.Log("GPA - OnResume");
                GPA_OnResume();
            }
        }

        private void OnDestroy()
        {
            if (mInit == false) { return; }

            bDestroyPermanently = true;
            Debug.Log("GPA - OnDestroy");
            GPA_Release();
        }

        public static bool AddCustomMetric(string name, GPALevel level)
        {
            if (Ins() == null || Ins().mInit == false) { return false; }

            return GPA_AddCustomMetric(name, (int)level, new IntPtr());
        }

        public static bool SetCustomMetric(string name, int val)
        {
            if (Ins() == null || Ins().mInit == false) { return false; }

            return GPA_SetCustomMetricInteger(name, val);
        }

        public static bool SetCustomMetric(string name, float val)
        {
            if (Ins() == null || Ins().mInit == false) { return false; }

            return GPA_SetCustomMetricFloat(name, val);
        }

        public static bool SetCustomMetric(string name, bool val)
        {
            if (Ins() == null || Ins().mInit == false) { return false; }

            return GPA_SetCustomMetricBool(name, val);
        }

        public static bool SetCustomMetric(string name, string val)
        {
            if (Ins() == null || Ins().mInit == false) { return false; }

            return GPA_SetCustomMetricString(name, val);
        }
    }
}