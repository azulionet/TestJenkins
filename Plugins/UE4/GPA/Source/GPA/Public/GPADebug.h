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

struct GPADebugMessage
{
	int Key;
	FString Message;
};

class FGPADebug
{
public:
	static bool BeginMessage()
	{
		return !IsMessageReady;
	}

	static void AddMessage(int Key, const char* str)
	{
		if (IsMessageReady == false)
		{
			DebugMessages.Add({ Key, FString(ANSI_TO_TCHAR(str)) });
		}
	}

	static void EndMessage()
	{
		if (DebugMessages.Num() != 0)
		{
			IsMessageReady = true;
		}
	}

	// Game Thread
	static void PrintMessage()
	{
		if (IsMessageReady == true && GEngine)
		{
			FVector2D TextScale{ 0.8f, 0.8f };

			TArray<GPADebugMessage> OutMessages = DebugMessages;
			DebugMessages.Empty();
			IsMessageReady = false;

			GEngine->ClearOnScreenDebugMessages();
			for (int messageIndex = 0; messageIndex < OutMessages.Num(); ++messageIndex)
			{
				GEngine->AddOnScreenDebugMessage(
					OutMessages[messageIndex].Key, 10,
					(OutMessages[messageIndex].Key == 512) ? FColor::Red : FColor::Emerald, *OutMessages[messageIndex].Message, true, TextScale);
			}
		}
	}

private:
	static TArray<GPADebugMessage> DebugMessages;
	static bool IsMessageReady;
};