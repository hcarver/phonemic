//An abstract class for screen readers
//--1/26/10--

#include <atlstr.h>
#include <string>

#include "stdafx.h"

#ifndef SPEAK_INTERFACE_H
#define SPEAK_INTERFACE_H

const char* const JAWS_ENGINE("JAWS");
const char* const NVDA_ENGINE("NVDA");
const char* const MICROSOFT_SAPI_ENGINE("MICROSOFT_SAPI");
const char* const NULL_ENGINE("NULL");

class SpeakInterface
{
	public: 
		virtual bool Speak(const wchar_t* StringToSpeak, bool Interrupt) = 0;
		virtual bool BlockSpeak(const wchar_t* StringToSpeak, bool Interrupt) = 0;
		virtual bool Load() = 0;
		virtual void Unload() = 0;
		virtual bool StopSpeech() = 0;
		virtual bool PauseSpeech() = 0;
		virtual bool ResumeSpeech() = 0;
		virtual bool SetVoiceNative(CString voice) = 0;
		virtual std::string GetVoiceNative() = 0;
		virtual const char* GetSpeechEngineNative() = 0;
		virtual bool SetVolume(long vol) = 0;
		virtual long GetVolume() = 0;
		virtual bool SetRate(long rate) = 0;
		virtual long GetRate() = 0;
		virtual bool isSpeaking() = 0;
};

#endif //SPEAK_INTERFACE_H 