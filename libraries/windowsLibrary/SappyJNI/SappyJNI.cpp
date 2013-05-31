/**
  * This file implements the native functions that are expected to exist in the Java class
  *  TextToSpeech
  * from the package 
  *  org.sapi
  *
  * These are:
  *  public native void speakNative(String string);
  *  public native void speakBlockNative(String string);
  *  public native boolean isSpeaking();
  *  public native boolean setVoiceNative(String string);
  *  public native boolean setTextToSpeechEngine(String string);
  *  public native String getTextToSpeechEngine();
  *  public native boolean pauseNative();
  *  public native boolean resumeNative();
  *  public native void stopNative();
  *  public native boolean setRateNative(long rate);
  *  public native long getRateNative();
  *  public native boolean setVolumeNative(long rate);
  *  public native long getVolumeNative();
  *  public native void createTTSNative();
  *  public native void destroyTTSNative();
  *  public native String getVoicesNative();
  *  public native void reinitializeNative();
  */

#include "stdafx.h"
#include "SpeakInterface.h"
#include "SappyJNI.h"

#include "JawsInterface.h"
#include "NVDAInterface.h"
#include "SapiInterface.h"

typedef wchar_t* WideString;

SpeakInterface* SpeechInterface = NULL;

JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_speakNative
(JNIEnv *javaEnvironment, jobject object, jstring string){
	jboolean dummy;
	const jchar* chars = javaEnvironment->GetStringChars(string, &dummy);

	SpeechInterface->Speak((const WideString) chars, true);
	// Free the created chars
	javaEnvironment->ReleaseStringChars(string, chars);
}

JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_speakBlockNative
(JNIEnv *javaEnvironment, jobject object, jstring string){
	jboolean dummy;
	const jchar* chars = javaEnvironment->GetStringChars(string, &dummy);

	SpeechInterface->BlockSpeak((const WideString) chars, true);
	// Free the created chars
	javaEnvironment->ReleaseStringChars(string, chars);
}

JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_isSpeaking
(JNIEnv *javaEnvironment, jobject object){
	return SpeechInterface->isSpeaking();
}

JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setVoiceNative
(JNIEnv *javaEnvironment, jobject object, jstring voice){
    // Convert from jstring to char*
	jboolean dummy;
	const jchar* voiceInNativeChars = javaEnvironment->GetStringChars(voice, &dummy);

	// Take the char*, create a managed CString then release the char*
	CString nativeVoiceString((const WideString) voiceInNativeChars);
	javaEnvironment->ReleaseStringChars(voice, voiceInNativeChars);

	return SpeechInterface->SetVoiceNative(nativeVoiceString);
}

bool setTTSEngineInternal(CString nativeEngineString){
	// Create a new speech interface based on the input engine name
	SpeakInterface* newSpeechInterface = NULL;

	if(nativeEngineString == CString(JAWS_ENGINE)){
		newSpeechInterface = new JawsInterface();
	}
	else if(nativeEngineString == CString(NVDA_ENGINE)){
		newSpeechInterface = new NVDAInterface();
	}
	else if (nativeEngineString == CString(MICROSOFT_SAPI_ENGINE)){
		newSpeechInterface = new SapiInterface();
	}

	// If we haven't yet created an interface, we didn't recognise the name 
	// so we can't create the engine.
	if(newSpeechInterface == NULL){
		return false;
	}

	// Try to load the interface and test speech will work.
	// If so, we replace the current interface and return success.
	if(newSpeechInterface->Load() && newSpeechInterface->Speak((WideString) "", false)){
		// If we already have an interface, dispose of it.
		if(SpeechInterface != NULL){
			SpeechInterface->Unload();
			delete SpeechInterface;
		}

		SpeechInterface = newSpeechInterface;
		return true;
	}

	// The new interface didn't work so we delete it and return false;
	delete newSpeechInterface;
	return false;
}

JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setTextToSpeechEngine
(JNIEnv *javaEnvironment, jobject object, jstring engine){
	// Get the requested engine as a useable CString
	jboolean dummy;
	const jchar* nativeEngineChars = javaEnvironment->GetStringChars(engine, &dummy);
	CString nativeEngineString((const WideString) nativeEngineChars);
	javaEnvironment->ReleaseStringChars(engine, nativeEngineChars);

	return setTTSEngineInternal(nativeEngineString);
}

/**
 * NOTE: This function is one of few that might be called when no engine
 * has been initialised. So we need to account for that here.
 */
JNIEXPORT jstring JNICALL Java_org_sapi_TextToSpeech_getTextToSpeechEngine
(JNIEnv *javaEnvironment, jobject object){
	const char* name;
	if(SpeechInterface != NULL){
		name = SpeechInterface->GetSpeechEngineNative();
	}
	else{
		name = NULL_ENGINE;
	}
	return javaEnvironment->NewStringUTF(name);
}

JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_pauseNative
(JNIEnv *javaEnvironment, jobject object){
	return SpeechInterface->PauseSpeech();
}

JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_resumeNative
(JNIEnv *javaEnvironment, jobject object){
	return SpeechInterface->ResumeSpeech();
}

JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_stopNative
(JNIEnv *javaEnvironment, jobject object){
	SpeechInterface->StopSpeech();
}

JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setRateNative
(JNIEnv *javaEnvironment, jobject object, jlong rate){
	return SpeechInterface->SetRate((long) rate);
}

JNIEXPORT jlong JNICALL Java_org_sapi_TextToSpeech_getRateNative
(JNIEnv *javaEnvironment, jobject object){
	return SpeechInterface->GetRate();
}

JNIEXPORT jboolean JNICALL Java_org_sapi_TextToSpeech_setVolumeNative
(JNIEnv *javaEnvironment, jobject object, jlong volume){
	return SpeechInterface->SetVolume((long) volume);
}

JNIEXPORT jlong JNICALL Java_org_sapi_TextToSpeech_getVolumeNative
(JNIEnv *javaEnvironment, jobject object){
	return SpeechInterface->GetVolume();
}

JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_createTTSNative
(JNIEnv *javaEnvironment, jobject object){
	bool gotInterface = false;

	// First of all, try to use the JAWS Engine
	gotInterface = setTTSEngineInternal(JAWS_ENGINE);

	// If we failed to use the JAWS Engine, use the NVDA engine
	if(!gotInterface){
		gotInterface = setTTSEngineInternal(NVDA_ENGINE);
	}

	// If we failed to use JAWS and NVDA, try to use the MS SAPI engine.
	if(!gotInterface) {
		gotInterface = setTTSEngineInternal(MICROSOFT_SAPI_ENGINE);
	}
}

/**
 * This is another function that could easily be called when we don't 
 * yet have an initialised Speech Interface.
 */
JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_destroyTTSNative
(JNIEnv *javaEnvironment, jobject object){
	// Check if the interface exists.
	// If it does, unload it, free its memory and remove its pointer.
	if (SpeechInterface != NULL){
		SpeechInterface->Unload();
		delete SpeechInterface;
		SpeechInterface = NULL;
	}
}

JNIEXPORT jstring JNICALL Java_org_sapi_TextToSpeech_getVoicesNative
(JNIEnv *javaEnvironment, jobject object){
	return javaEnvironment->NewStringUTF(SpeechInterface->GetVoiceNative().c_str());
}

JNIEXPORT void JNICALL Java_org_sapi_TextToSpeech_reinitializeNative
(JNIEnv *javaEnvironment, jobject object){
	// If we don't already have a speech interface, initialise one.
	if(SpeechInterface == NULL){
		Java_org_sapi_TextToSpeech_createTTSNative(javaEnvironment, object);
		return;
	}

	// If we already have a TTS Engine, we set the engine to be
	// the engine with the same name.
	setTTSEngineInternal(SpeechInterface->GetSpeechEngineNative());
}