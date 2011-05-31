#include "stdafx.h"
#include <windows.h>
#include "JawsInterface.h"

#ifndef JAWS_INTERFACE_CPP
#define JAWS_INTERFACE_CPP

JawsInterface::JawsInterface()
{
  //Initialize the COM library for this thread
	CoInitialize(NULL);

   //Initialize the result and hresult to NULL
   dwResult = NULL;
   function_r = NULL;
   create_r = NULL;
}

JawsInterface::~JawsInterface()
{
	Unload();
}

bool JawsInterface::Load()
{
	//Attempt to load the Jaws API from registry
	if(LoadCOM("FreedomSci.JawsApi"))
	{
		jawsServer = lpTDispatch;
		return true;
	}

  return false;
}

void JawsInterface::Unload()
{
	//If the COM object was successfully created enter this if block
	if(SUCCEEDED(create_r))
	{
		//Make sure this is released otherwise CoUnitialize
		//attempts to release a NULL pointer
		jawsServer.Release();
		lpTDispatch.Release();
		CoUninitialize();
	}
}

bool JawsInterface::SetVoiceNative(CString voice)
{
	return true;
}

std::string JawsInterface::GetVoiceNative()
{
	return std::string("");
}

const char* JawsInterface::GetSpeechEngineNative()
{
	return "JAWS";
}

bool JawsInterface::LoadCOM(string API)
{
   //Create the JawsApi object on the local system 
   create_r = lpTDispatch.CoCreateInstance(_bstr_t(API.c_str()));

   /*if (!SUCCEEDED(create_r))
   {
	   lpTDispatch.Release();
   }*/
   
   //return true if the object is created successfully
   return (SUCCEEDED(create_r) ? true : false);
}

bool JawsInterface::Speak(const wchar_t *StringToSpeak, bool Interrupt)
{
   function_r = jawsServer.Invoke2(_bstr_t("SayString"),&_variant_t(StringToSpeak),&_variant_t(Interrupt), &vResult);

   return (function_r == S_OK ? (bool)vResult : false);
}

bool JawsInterface::BlockSpeak(const wchar_t *StringToSpeak, bool Interrupt)
{
   function_r = jawsServer.Invoke2(_bstr_t("SayString"),&_variant_t(StringToSpeak),&_variant_t(Interrupt), &vResult);

   return (function_r == S_OK ? (bool)vResult : false);
}

bool JawsInterface::StopSpeech()
{
   function_r = jawsServer.Invoke0(_bstr_t("StopString"), &vResult);

   return (function_r == S_OK ? (bool)vResult : false);
}

bool JawsInterface::isSpeaking(){return false;}

bool JawsInterface::RunScript(const wchar_t* ScriptName)
{
   function_r =jawsServer.Invoke1(_bstr_t("RunScript"),&_variant_t(ScriptName), &vResult);
	
   return (function_r == S_OK ? (bool)vResult : false);
}

bool JawsInterface::PauseSpeech() {
	// not supported
	return false;
}
bool JawsInterface::ResumeSpeech() {
	// not supported
	return false;
}

bool JawsInterface::SetVolume(long vol) {
	// not supported
	return false;
}
long JawsInterface::GetVolume() {
	// not supported.
	return 0;
}

bool JawsInterface::SetRate(long rate) {
	// not supported
	return false;
}

long JawsInterface::GetRate() {
	// not supported.
	return 0;
}

#endif //JAWS_INTERFACE_CPP
