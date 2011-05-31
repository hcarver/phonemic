//An abstract class for screen readers
//--4/27/10--

#include "stdafx.h"
#include "nvdaController.h"
#include "SpeakInterface.h"
#include <windows.h>
#include <atlstr.h>
#include <string>

using std::string;

#ifndef NVDA_INTERFACE_H
#define NVDA_INTERFACE_H

class NVDAInterface : public SpeakInterface
{
	public: 
		virtual bool Speak(const wchar_t* StringToSpeak, bool Interrupt);
		virtual bool BlockSpeak(const wchar_t* StringToSpeak, bool Interrupt);
		virtual bool Load();
		virtual void Unload();
		virtual bool StopSpeech();
		virtual bool PauseSpeech();
		virtual bool ResumeSpeech();
		virtual bool SetVoiceNative(CString voice);
		virtual std::string GetVoiceNative();
		virtual const char* GetSpeechEngineNative();
		virtual bool SetVolume(long vol);
		virtual long GetVolume();
		virtual bool SetRate(long rate);
		virtual long GetRate();
		virtual bool isSpeaking();
};

#endif //NVDA_INTERFACE_H 