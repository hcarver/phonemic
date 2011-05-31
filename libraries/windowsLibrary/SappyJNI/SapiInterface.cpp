#include "stdafx.h"
#include "SapiInterface.h"
#include <sstream>
#include <sapi.h>

#ifndef SAPII_INTERFACE_CPP
#define SAPII_INTERFACE_CPP

SapiInterface::SapiInterface()
{
	//Initialize the COM library for this thread
	CoInitialize(NULL);

	ISpVoice * pVoice = NULL;
	HRESULT hr = S_OK;
}

SapiInterface::~SapiInterface()
{
  Unload();
}


bool SapiInterface::Speak(const wchar_t* StringToSpeak, bool Interrupt)
{
  if(SUCCEEDED(engineLoaded))
  {
	pVoice->Speak(StringToSpeak, SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
	//engineLoaded = pVoice->Speak(StringToSpeak, SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
	return true;
  }
  return false;
}

bool SapiInterface::BlockSpeak(const wchar_t* StringToSpeak, bool Interrupt)
{
  if(SUCCEEDED(engineLoaded))
  {
	pVoice->Speak(StringToSpeak, /*SPF_ASYNC |*/ SPF_PURGEBEFORESPEAK, NULL);
	//engineLoaded = pVoice->Speak(StringToSpeak, SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
	pVoice->WaitUntilDone(-1);
	return true;
  }
  return false;
}

bool SapiInterface::Load()
{
	engineLoaded = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	return (SUCCEEDED(engineLoaded));
}

void SapiInterface::Unload()
{
  if( SUCCEEDED( engineLoaded ) ) {
		pVoice->Release();
		pVoice = NULL;
		CoUninitialize();
	}
}

bool SapiInterface::StopSpeech()
{
	if( SUCCEEDED( engineLoaded ))
    {
		pVoice->Speak(L" ", SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
    }
	return true;
}

bool SapiInterface::SetVoiceNative(CString voice)
{
	CComPtr<IEnumSpObjectTokens>    cpEnum;
	CSpDynamicString*				szDescription;
	CComPtr<ISpObjectToken>         cpVoiceToken; //the token is the voice
	CComPtr<ISpVoice>               cpVoice;
	ULONG                           ulCount = 0;

	//This gets an enumeration of all voices on the system
	hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
	if (!SUCCEEDED(hr) ){
		return false; //enumeration of voices failed
	}

	hr = cpEnum->GetCount(&ulCount);
	szDescription = new CSpDynamicString [ulCount];
	UINT i = 0;
	while (SUCCEEDED(hr) && ulCount--)
	{
	   cpVoiceToken.Release();
	   if (SUCCEEDED (hr))
	   {
		  hr = cpEnum->Next(1, &cpVoiceToken, NULL);
		  hr = SpGetDescription(cpVoiceToken, &szDescription[i]);
	   }
	   if (SUCCEEDED (hr))
	   {
		  if(CString(szDescription[i]) == voice) {
				hr = pVoice->SetVoice(cpVoiceToken);
				delete [] szDescription;
				return true;
		   }
	   }
		i++;
	}
	//delete the voice list if one is not found and return false
	delete [] szDescription;
	return false;
}

bool SapiInterface::isSpeaking()
{
	SPVOICESTATUS *status = new SPVOICESTATUS();
	LPWSTR *bm = new LPWSTR();
	DWORD running_state = 0;

	if(pVoice->GetStatus(status,bm) == S_OK)
	  running_state = status->dwRunningState;

	/*If running state is 0 Sapi isn't speaking
	so return false*/
	delete status, bm;
	return (running_state == SPRS_IS_SPEAKING) ? true : false;
}

const char* SapiInterface::GetSpeechEngineNative()
{
	return "MICROSOFT_SAPI";
}

/*
 * Some notes about this function:
 * This function returns a list of available SAPI voices. It has been fixed and should be working correclty
 * on Windows 7 and all other versions of Windows.
 *
 * The solution to getting this function working was found here:
 * http://stackoverflow.com/questions/4336245/how-to-return-a-java-string-in-c-using-jni
 *
 * In addition to the above StackOverflow thread, it was necessary to change this function to return an std::string instance instead
 * of a character pointer, and to use stringstreams instead of CStrings.
 */
std::string SapiInterface::GetVoiceNative()
{
	
	std::stringstream sstream;
	sstream << "<?xml version=\"1.0\"?>";

	ISpVoice * pVoice = NULL;	
	CComPtr<ISpObjectToken>        cpVoiceToken;
	CComPtr<IEnumSpObjectTokens>   cpEnum;
	CComPtr<ISpVoice>              cpVoice;
	ULONG                          ulCount = 0;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if(SUCCEEDED(hr))
	{
		WCHAR   **m_ppszTokenIds;
		USES_CONVERSION;
		CComPtr<IEnumSpObjectTokens>    cpEnum;
		WCHAR**							szDescription;
		ISpObjectToken                  *pToken = NULL;
		CComPtr<ISpObjectToken>         cpVoiceToken; //the token is the voice
		CComPtr<ISpVoice>               cpVoice;
		ULONG                           ulCount = 0;

		if(SUCCEEDED(hr))
		{
			hr = cpVoice.CoCreateInstance( CLSID_SpVoice );
			if(SUCCEEDED(hr))
			{
				WCHAR *pszCurTokenId = NULL;
				ULONG ulIndex = 0, ulNumTokens = 0, ulCurToken = -1;

				hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

				if (hr == S_OK)
				{
					hr = cpEnum->GetCount( &ulNumTokens );

					if (SUCCEEDED(hr) && ulNumTokens != 0)
					{
						szDescription = new WCHAR* [ulNumTokens];
						m_ppszTokenIds = new WCHAR* [ulNumTokens];

						ZeroMemory(m_ppszTokenIds, ulNumTokens * sizeof(WCHAR *));

						UINT i =0;
						while (cpEnum->Next(1, &pToken, NULL) == S_OK)
						{
							//Don't care about return value in next line:
							hr = SpGetDescription(pToken, &szDescription[ulIndex]);
							ulIndex++;

							int utf8_length = WideCharToMultiByte(
								  CP_UTF8,           // Convert to UTF-8
								  0,                 // No special character conversions required 
													 // (UTF-16 and UTF-8 support the same characters)
								  szDescription[i],             // UTF-16 string to convert
								  -1,                // utf16 is NULL terminated (if not, use length)
								  NULL,              // Determining correct output buffer size
								  0,                 // Determining correct output buffer size
								  NULL,              // Must be NULL for CP_UTF8
								  NULL);             // Must be NULL for CP_UTF8

							if (utf8_length == 0) {
								return std::string("WideCharToMultibyte error.");
							}
							
							char *utf8_voice = new char[utf8_length];
							utf8_length = WideCharToMultiByte(
							  CP_UTF8,           // Convert to UTF-8
							  0,                 // No special character conversions required 
												 // (UTF-16 and UTF-8 support the same characters)
							  szDescription[i],             // UTF-16 string to convert
							  -1,                // utf16 is NULL terminated (if not, use length)
							  utf8_voice,              // UTF-8 output buffer
							  utf8_length,       // UTF-8 output buffer size
							  NULL,              // Must be NULL for CP_UTF8
							  NULL);             // Must be NULL for CP_UTF8
							//strConcatenateXML += "<voice>" + szDescription[i] + "</voice>";

							sstream << "<voice>" << utf8_voice << "</voice>";
							pToken->Release();
							pToken = NULL;
							i++;
						}

						delete [] szDescription;
					}
					else
						{
						//strConcatenateXML = "No voice found. (5)";
						sstream << "No voice found. (5)";
					}
				}
				else
				{
					//strConcatenateXML = "No voice found. (4)";
					sstream << "No voice found. (4)";
				}
			}
			else
			{
				//strConcatenateXML = "No voice found. (3)";
				sstream << "No voice found. (3)";
			}
		}
		else
		{
			//strConcatenateXML = "No voice found. (2)";
			sstream << "No voice found. (2)";
		}
	}
	else
	{
		//strConcatenateXML = "No voice found. (1)";
		sstream << "No voice found. (1)";
	}

	return sstream.str();
}

bool SapiInterface::PauseSpeech() {
	return (pVoice->Pause() == S_OK);
}

bool SapiInterface::ResumeSpeech() {
	return (pVoice->Resume() == S_OK);
}

bool SapiInterface::SetVolume(long vol) {
	return (pVoice->SetVolume((USHORT)vol) == S_OK);
}

long SapiInterface::GetVolume() {
	USHORT vol;

	if (pVoice->GetVolume(&vol) == S_OK)
		return (long)vol;
	else
		return 0;
}

bool SapiInterface::SetRate(long rate) {
	return (pVoice->SetRate(rate) == S_OK);
}

long SapiInterface::GetRate() {
	long rate;
	
	if (pVoice->GetRate(&rate) == S_OK)
		return rate;
	else
		return 0;
}

#endif SAPII_INTERFACE_CPP