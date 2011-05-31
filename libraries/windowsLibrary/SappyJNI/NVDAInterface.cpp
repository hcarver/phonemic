#include"stdafx.h"
#include <direct.h>
#include <stdlib.h>
#include"NVDAInterface.h"
#include "nvdaController.h"

#ifndef NVDA_INTERFACE_CPP
#define NVDA_INTERFACE_CPP

bool NVDAInterface::Speak(const wchar_t* StringToSpeak, bool Interrupt)
{
	return (nvdaController_speakText(StringToSpeak) != 0) ? false : true;
}

bool NVDAInterface::BlockSpeak(const wchar_t* StringToSpeak, bool Interrupt)
{
	return (nvdaController_speakText(StringToSpeak) != 0) ? false : true;
}

bool NVDAInterface::Load()
{
	return (nvdaController_testIfRunning() != 0) ? false : true;
}

void NVDAInterface::Unload() {}

bool NVDAInterface::StopSpeech()
{
	return (nvdaController_cancelSpeech() != 0) ? false : true;
}

bool NVDAInterface::SetVoiceNative(CString voice) {return false;}

std::string NVDAInterface::GetVoiceNative() 
{ 
	return std::string("");
}

const char* NVDAInterface::GetSpeechEngineNative()
{
	return "NVDA";
}

bool NVDAInterface::isSpeaking(){return false; }

bool NVDAInterface::PauseSpeech() {
	// not supported
	return false;
}
bool NVDAInterface::ResumeSpeech() {
	// not supported
	return false;
}

bool NVDAInterface::SetVolume(long vol) {
	// not supported
	return false;
}
long NVDAInterface::GetVolume() {
	// not supported.
	return 0;
}

bool NVDAInterface::SetRate(long rate) {
	// not supported
	return false;
}

long NVDAInterface::GetRate() {
	// not supported.
	return 0;
}

#endif NVDA_INTERFACE_CPP