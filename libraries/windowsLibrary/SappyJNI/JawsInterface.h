#include "stdafx.h"
#include "SpeakInterface.h"
#include <windows.h>
#include <string>
#include <atlstr.h>
#include <atlcom.h>
#include <comdef.h>
#include <atlbase.h>

using std::string;

#ifndef JAWS_INTERFACE_H
#define JAWS_INTERFACE_H

class JawsInterface : public SpeakInterface
{
 private:

  CComDispatchDriver jawsServer;
  CComPtr<IDispatch> lpTDispatch;

  _variant_t vResult;
  char lpResult[512];
  DWORD dwResult;
  HRESULT create_r;
  HRESULT function_r;

  bool LoadCOM(string API);

public:
	//Default constructor
	JawsInterface();

	//Destructor
	~JawsInterface();

	virtual bool Speak(const wchar_t *StringToSpeak, bool Interrupt);
	virtual bool BlockSpeak(const wchar_t* StringToSpeak, bool Interrupt);
	virtual bool Load();
	virtual void Unload();
	virtual bool StopSpeech();
	virtual bool PauseSpeech();
	virtual bool ResumeSpeech();
	virtual bool SetVoiceNative(CString voice);
	virtual const char* GetSpeechEngineNative();
	virtual std::string GetVoiceNative();
	virtual bool SetVolume(long vol);
	virtual long GetVolume();
	virtual bool SetRate(long rate);
	virtual long GetRate();
	virtual bool isSpeaking();

	bool RunScript(const wchar_t* ScriptName);

};


#endif // JAWS_SPEAKER_H 