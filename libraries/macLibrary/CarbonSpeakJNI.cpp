#include <iostream>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <jni.h>
#include <Carbon/Carbon.h>
#include "CarbonSpeakJNI.h"
#include "CarbonSpeak.h"

// Our speaker object.
CarbonSpeak *speaker;

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    speakNative
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_speakNative
(JNIEnv *env, jobject jobj, jstring text)
{
	const char *jBuff = env->GetStringUTFChars(text, NULL);
	bool result = speaker->speak(std::string((const char *)jBuff));
	env->ReleaseStringUTFChars(text, jBuff);

	return result;
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    speakBlockingNative
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_speakBlockingNative
(JNIEnv *env, jobject jobj, jstring text)
{
	const char *jBuff = env->GetStringUTFChars(text, NULL);
	bool result = speaker->speakBlocking(std::string((const char *)jBuff));
	env->ReleaseStringUTFChars(text, jBuff);
	
	return result;
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    stopNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_stopNative
(JNIEnv *env, jobject jobj)
{
	return speaker->stop();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    pauseNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_pauseNative
(JNIEnv *env, jobject jobj)
{
	return speaker->pause();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    resumeNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_resumeNative
(JNIEnv *env, jobject jobj)
{
	return speaker->resume();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    isSpeakingNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_isSpeakingNative
(JNIEnv *env, jobject jobj)
{
	return speaker->isSpeaking();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    setVoiceNative
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_setVoiceNative
(JNIEnv *env, jobject jobj, jint index)
{
	return speaker->setVoice((short)index);
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    setVolumeNative
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_setVolumeNative
(JNIEnv *env, jobject jobj, jdouble volume)
{
	return speaker->setVolume(volume);
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    setPitchNative
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_setPitchNative
(JNIEnv *env, jobject jobj, jdouble pitch)
{
	return speaker->setPitch(pitch);
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    setSpeedNative
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_setSpeedNative
(JNIEnv *env, jobject jobj, jdouble speed)
{
	return speaker->setSpeed(speed);
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    initializeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_initializeNative
(JNIEnv *env, jobject jobj)
{
	// Just create a new speaker.
	speaker = CarbonSpeak::inst();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    reinitializeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_reinitializeNative
(JNIEnv *env, jobject jobj)
{
	speaker->reinitialize();
}


/*
 * Class:     org_sodbeans_phonemic_tts_impl_CarbonSpeak
 * Method:    destroyNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sodbeans_phonemic_tts_impl_CarbonSpeak_destroyNative
(JNIEnv *env, jobject jobj)
{
	// Just delete our speaker.
	delete speaker;
}