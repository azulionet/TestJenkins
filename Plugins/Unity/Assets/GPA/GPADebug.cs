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
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace GPA
{
    public static class GPADebug
    {
        private struct GPADebugMessage
        {
            public GPADebugMessage(int k, string m)
            {
                key = k;
                message = m;
            }

            public int key;
            public string message;
        }

        public static bool mIsMessageReady { get; private set; } = false;
        private static List<GPADebugMessage> mDebugMessages = new List<GPADebugMessage>();

        public static bool BeginMessage()
        {
            return !mIsMessageReady;
        }

        public static void AddMessage(int key, [MarshalAs(UnmanagedType.LPStr)] string str)
        {
            if (mIsMessageReady == false)
            {
                mDebugMessages.Add(new GPADebugMessage(key, str));
            }
        }

        public static void EndMessage()
        {
            if (mDebugMessages.Count != 0)
            {
                mIsMessageReady = true;
            }
        }

        public static void GetAddedMessages(ref List<string> outList)
        {
            if (mIsMessageReady == true)
            {
                outList.Clear();

                List<GPADebugMessage> temp = new List<GPADebugMessage>(mDebugMessages);

                mDebugMessages.Clear();
                mIsMessageReady = false;

                foreach (var value in temp)
                {
                    outList.Add(value.message);
                }
            }
        }
    }
}