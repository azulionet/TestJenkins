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
using System.Collections;
using System.Text;
using UnityEngine;

namespace GPA
{
    public class GPAScreenCapture : MonoBehaviour
    {
        private byte[] mBuffer;
        private WaitForEndOfFrame mEndFrame = new WaitForEndOfFrame();
        private Coroutine mNowRunningCoroutine = null;

        public bool IsProgress() => mNowRunningCoroutine != null;

        private Texture2D texture = null;

        public void Start()
        {
            mNowRunningCoroutine = null;
            texture = new Texture2D(Screen.currentResolution.width, Screen.currentResolution.height, TextureFormat.RGBA32, false);

            Debug.Log("GPA2 log Capture Start - x : " + Screen.currentResolution.width + " /// y : " + Screen.currentResolution.height);
        }

        public void RequestScreenshot(System.Action<byte[]> callback)
        {
            if (mNowRunningCoroutine != null)
            {
                return;
            }

            mNowRunningCoroutine = StartCoroutine(Progress(callback));
        }

        private IEnumerator Progress(System.Action<byte[]> callback)
        {
            yield return mEndFrame;

            if (callback == null)
            {
                yield break;
            }

            int w = Screen.currentResolution.width;
            int h = Screen.currentResolution.height;

            Debug.Log("GPA2 log Capture -- abnormal!!! -- x : " + w + " /// y : " + h);

            texture.ReadPixels(new Rect(0, 0, w, h), 0, 0);
            texture.Apply();

            mBuffer = texture.GetRawTextureData();

            StringBuilder sLog = new StringBuilder();

            mNowRunningCoroutine = null;
            callback(mBuffer);
        }
    }
}