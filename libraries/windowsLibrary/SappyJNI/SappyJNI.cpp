// This is the main DLL file.

#include "stdafx.h"
#include "SpeakInterface.h"
#include "SapiInterface.h"
#include "JawsInterface.h"
#include "NVDAInterface.h"
#include "SappyJNI.h"
#include <sapi.h>
#include <atlstr.h>
// Next three lines for purpose of debugging
#include <iostream>
#include <fstream>
using namespace std;

using namespace SappyJNI;

//Create an interface for potential screen readers
SpeakInterface *Interface;
const jchar *Engine = NULL; // the last requested engine.

typedef std::basic_string<TCHAR> tstring;

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    speakNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_speakNative
(JNIEnv *env, jobject obj, jstring str) {
	jboolean isCopy;
	const jchar* stringChars = (env)->GetStringChars(str, &isCopy);
	LPCWSTR sayThis = (LPCWSTR)stringChars;

	Interface->Speak(sayThis,true);
	env->ReleaseStringCritical(str, stringChars);
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    isSpeaking
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_isSpeaking
  (JNIEnv *, jobject)
{
	return Interface->isSpeaking();
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    stopNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_stopNative
(JNIEnv * env, jobject obj) {

	Interface->StopSpeech();
}

/*
*	Gets a string representation of the text to speech engine.
*/
JNIEXPORT jstring JNICALL Java_org_sapi_TextToSpeech_getTextToSpeechEngine
(JNIEnv *env,jobject obj) {
	if(Interface != NULL) {
		const char *ch = Interface->GetSpeechEngineNative();
		return env->NewStringUTF(ch);
	}
	else {
		const char *ch = "NULL";
		return env->NewStringUTF(ch);
	}
}

bool setEngine(const jchar *stringChars) {
	LPCWSTR sayThis = (LPCWSTR)stringChars;
	CString input = sayThis;
	CString jaws_string = L"JAWS";
	CString nvda_string = L"NVDA";
	CString sapi_string = L"MICROSOFT_SAPI";
	SpeakInterface *NewInterface = NULL;

	bool jaws_request = input.Compare(jaws_string) == 0;
	bool nvda_request = input.Compare(nvda_string) == 0;
	bool sapi_request = input.Compare(sapi_string) == 0;

	if(!jaws_request && !nvda_request && !sapi_request) {
		return false;	
	}

	SpeakInterface *JawsTest,*NVDATest;
	JawsTest = new JawsInterface();
	NVDATest = new NVDAInterface();

	if(jaws_request) {
		if(JawsTest->Load() && JawsTest->Speak(L"",true)){
			delete JawsTest;
			delete NVDATest;
			NewInterface = new JawsInterface();
			NewInterface->Load();
			NewInterface->Speak(L"", true);
		}
	}
	else if(nvda_request) {
		if(NVDATest->Load()){
			delete JawsTest;
			delete NVDATest;
			NewInterface = new NVDAInterface();
			NewInterface->Speak(L"", true);
		}
	}
	else if(sapi_request) {
		delete JawsTest;
		delete NVDATest;
		NewInterface = new SapiInterface();
		NewInterface->Load();
		NewInterface->Speak(L"", true);
	}

	if(NewInterface != NULL) {
		Interface->Unload();
		delete Interface;
		Interface = NewInterface;
		return true;
	}

	return false;
}
/*
 * Attempts to set the text to speech engine.
 */
JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setTextToSpeechEngine
(JNIEnv *env,jobject obj,jstring str) {
	jboolean isCopy;
	
	// Set the last requested engine.

	const jchar* stringChars = (env)->GetStringChars(str, &isCopy);
	Engine = stringChars;
	return setEngine(stringChars);
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    createTTSNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_createTTSNative
(JNIEnv *, jobject) {
	//These Jaws and NVDA objects are created to test whether
	//Jaws or NVDA is currently running on the system
	SpeakInterface *JawsTest,*NVDATest;
	JawsTest = new JawsInterface();
	NVDATest = new NVDAInterface();

	//If jaws can speak load the jaws interface
	if(JawsTest->Load() && JawsTest->Speak(L"",true)){
		delete JawsTest;
		delete NVDATest;
		Interface = new JawsInterface();
	}

	//If JAWS cannot speak and NVDA can, load the NVDA interface.
	else if(NVDATest->Load()){
		delete JawsTest;
		delete NVDATest;
		Interface = new NVDAInterface();
	}

	//If jaws and NVDA cannot, load the sapi interface
	else{
		Interface = new SapiInterface();
	}
	
	if(!Interface->Load()){
		delete Interface;
	}
	// Have the interface speak an empty line--if you don't,
	// SAPI doesn't speak when you call the speakNative function
	else{
		Interface->Speak(L"", true);
	}
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    destroyTTSNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_destroyTTSNative
(JNIEnv *, jobject) {

	Interface->Unload();
	delete Interface;
	Interface = NULL;
}

//patched together with pieces from the Quadmore project
//http://www.quadmore.com/JAVA_to_SAPI/ 
//but also from some of the SAPI documentation
JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setVoiceNative(JNIEnv *env,jobject obj,jstring strInput)
{
	const char *str = env->GetStringUTFChars(strInput, 0);
	CString strVoiceSelected(str);
	env->ReleaseStringUTFChars(strInput, str);
	if (str == NULL)
	{
		return false; /* OutOfMemoryError already thrown */
	}

	return(Interface->SetVoiceNative(strVoiceSelected));
}

//Copied almost verbatim, with modifications for compiling on Visual Studio 2008 (outside of MFC)
//from the quadmore project
//http://www.quadmore.com/JAVA_to_SAPI/
JNIEXPORT jstring JNICALL Java_org_sapi_TextToSpeech_getVoicesNative(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(Interface->GetVoiceNative().c_str());
}

// This uses a blocking call for speaking; interfaces that support blocking calls will not be
// interrupted while speaking.
JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_speakBlockNative
	(JNIEnv *env,jobject obj,jstring str)
{
   jboolean isCopy;
   const jchar* stringChars = (env)->GetStringChars(str, &isCopy);
   LPCWSTR sayThis = (LPCWSTR)stringChars;

   Interface->BlockSpeak(sayThis,true);
   env->ReleaseStringCritical(str, stringChars);
}

JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_reinitializeNative
	(JNIEnv *env,jobject obj)
{
	jboolean isCopy;
	if (Engine != NULL) {
		// Re-load by changing the engine again.
		setEngine(Engine);
		return;
	}

	// No engine is selected--use the default method.
	Interface->Unload();
	delete Interface;

	SpeakInterface *JawsTest,*NVDATest;
	JawsTest = new JawsInterface();
	NVDATest = new NVDAInterface();

	//If jaws can speak load the jaws interface
	if(JawsTest->Load() && JawsTest->Speak(L"",true))
		Interface = new JawsInterface();

	// If JAWS cannot and NVDA loads, load the NVDA interface.
	else if(NVDATest->Load())
		Interface = new NVDAInterface();

	//If jaws cannot, load the sapi interface
	else
		Interface = new SapiInterface();

	if(!Interface->Load())
		delete Interface;

	delete JawsTest;
	delete NVDATest;
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    pauseNative
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_pauseNative
	(JNIEnv *env,jobject obj)
{
	return Interface->PauseSpeech();
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    resumeNative
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_resumeNative
	(JNIEnv *env,jobject obj)
{
	return Interface->ResumeSpeech();
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    setRateNative
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setRateNative
	(JNIEnv *env,jobject obj, jlong rate)
{
	return Interface->SetRate((long)rate);
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    getRateNative
 * Signature: ()V
 */
JNIEXPORT jlong JNICALL Java_org_sapi_TextToSpeech_getRateNative
(JNIEnv *env,jobject obj)
{
	return Interface->GetRate();
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    setVolumeNative
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setVolumeNative
	(JNIEnv *env,jobject obj, jlong vol)
{
	return Interface->SetVolume((long)vol);
}

/*
 * Class:     org_sapi_TextToSpeech
 * Method:    getVolumeNative
 * Signature: ()V
 */
JNIEXPORT jlong JNICALL Java_org_sapi_TextToSpeech_getVolumeNative
	(JNIEnv *env,jobject obj)
{
	return Interface->GetVolume();
}