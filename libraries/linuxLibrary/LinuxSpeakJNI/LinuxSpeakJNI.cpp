#include <iostream>
#include <jni.h>
#include "linuxSpeak.h"
#include "LinuxSpeakJNI.h"

LinuxSpeak *speaker;

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    speakNative
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_speakNative
  (JNIEnv *env, jobject jobj, jstring text)
{
    const char *jBuff = env->GetStringUTFChars(text, NULL);
    bool result = speaker->speak(std::string((const char *)jBuff));
    env->ReleaseStringUTFChars(text, jBuff);

    return result;
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    speakCharNative
 * Signature: (C)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_speakCharNative
  (JNIEnv *env, jobject jobj, jstring c)
{
    const char *jBuff = env->GetStringUTFChars(c, NULL);
    bool result = speaker->speakChar(std::string((const char *)jBuff));
    env->ReleaseStringUTFChars(c, jBuff);

    return result;
}
/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    speakBlockingNative
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_speakBlockingNative
  (JNIEnv *env, jobject jobj, jstring text)
{
    const char *jBuff = env->GetStringUTFChars(text, NULL);
    bool result = speaker->speakBlocking(std::string((const char *)jBuff));
    env->ReleaseStringUTFChars(text, jBuff);

    return result;
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    speakCharBlockingNative
 * Signature: (C)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_speakCharBlockingNative
  (JNIEnv *env, jobject jobj, jstring c)
{
    const char *jBuff = env->GetStringUTFChars(c, NULL);
    bool result = speaker->speakBlocking(std::string((const char *)jBuff));
    env->ReleaseStringUTFChars(c, jBuff);

    return result;
}
/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    stopNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_stopNative
  (JNIEnv *env, jobject jobj)
{
    return speaker->stop();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    pauseNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_pauseNative
  (JNIEnv *env, jobject jobj)
{
    return speaker->pause();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    resumeNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_resumeNative
  (JNIEnv *env, jobject jobj)
{
    return speaker->resume();
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    getVoicesNative
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_getVoicesNative
  (JNIEnv *env, jobject jobj)
{
    std::string result = "<?xml version=\"1.0\"?>";
    std::vector<std::string> voices = speaker->getVoices();

    for (unsigned int x = 0; x < voices.size(); x++)
    {
        result += "<voice>" + voices[x] + "</voice>\n";
    }
    
    return env->NewStringUTF(result.c_str());
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    getVoiceNative
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_getVoiceNative
  (JNIEnv *env, jobject jobj)
{
    return env->NewStringUTF((speaker->getVoice()).c_str());
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    setVoiceNative
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_setVoiceNative
  (JNIEnv *env, jobject jobj, jstring voice)
{
	const char *jBuff = env->GetStringUTFChars(voice, NULL);
	bool result = speaker->setVoice(std::string((const char *)jBuff));
	env->ReleaseStringUTFChars(voice, jBuff);

	return result;
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    setVolumeNative
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_setVolumeNative
  (JNIEnv *env, jobject jobj, jint vol)
{
    return speaker->setVolume(vol);
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    setSpeedNative
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_setSpeedNative
  (JNIEnv *env, jobject jobj, jint speed)
{
    return speaker->setRate(speed);
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    setSpeedNative
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_setPitchNative
  (JNIEnv *env, jobject jobj, jint pitch)
{
    return speaker->setPitch(pitch);
}
/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    isSpeakingNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_isSpeakingNative
  (JNIEnv *, jobject)
{
    return speaker->isSpeaking();
}
/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    initializeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_initializeNative
  (JNIEnv *env, jobject jobj)
{
    speaker = LinuxSpeak::inst("sodbeans");
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    reinitializeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_reinitializeNative
  (JNIEnv *env, jobject jobj)
{
    speaker->reinitialize("sodbeans");
}

/*
 * Class:     org_sodbeans_phonemic_tts_impl_LinuxSpeak
 * Method:    destroyNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_sodbeans_phonemic_tts_impl_LinuxSpeak_destroyNative
  (JNIEnv *env, jobject jobj)
{
    delete speaker;
}