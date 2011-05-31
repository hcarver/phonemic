#include <iostream>
#include <string>
#include <pthread.h>
#include <Carbon/Carbon.h>
#include "CarbonSpeak.h"

pthread_mutex_t mutex;
pthread_cond_t conditional;
CarbonSpeak* CarbonSpeak::instance = NULL;
bool CarbonSpeak::blocking = false;

/*
 * Creates a new instance of CarbonSpeak if one doesn't already exist.
 */
CarbonSpeak* CarbonSpeak::inst()
{
	if (instance == NULL)
	{
		instance = new CarbonSpeak();
	}
	
	return instance;
}
CarbonSpeak::CarbonSpeak()
{
	// Initialize mutex and conditional for use...
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&conditional, NULL);
	// -1 indicates system default voice
	currentVoice = -1;
	
	// Default volume (1.0)
	currentVolume = 65536.0;
	
	// Default rate (250.0)
	currentRate = 250.0 * 65536.0;
	
	// Default pitch (43 seems to be the system default).
	currentPitch = 43.0 * 65536.0;
	
	// Initialize our channel.
	initialize();
}

CarbonSpeak::~CarbonSpeak()
{	
	destroyChannel();
}

/*
 * Initializes the text to speech engine.
 */
void CarbonSpeak::initialize()
{
	// Create a new speech channel.
	OSErr err = NewSpeechChannel(NULL, &currentChannel);
	
	if (err != noErr)
	{
		// TODO Error handling
		return;
	}
	
	// Set volume, rate and pitch to values we know.
	SetSpeechInfo(currentChannel, soVolume, &currentVolume);
	SetSpeechInfo(currentChannel, soRate, &currentRate);
	handlePitch();
	// Set the callback reference parameter to this instance. Some SSM callbacks may need it.
	SetSpeechInfo(currentChannel, soRefCon, this);
	
	// Set speech done callback. Called when the SSM has finished speaking a block of text.
	SetSpeechInfo(currentChannel, soSpeechDoneCallBack, (void *)CarbonSpeak::speechDoneProc);
}

/*
 * Handles pitch changes during initialization, pitch changes, reinitialization and voice changes.
 */
void CarbonSpeak::handlePitch()
{
	SetSpeechPitch(currentChannel, currentPitch);
}

/*
 * Destroys the speech channel -- used during reinitialization.
 */
void CarbonSpeak::destroyChannel()
{
	// Dispose of the speech channel... and that's it!
	DisposeSpeechChannel(currentChannel);
}

/*
 * Called by the Speech Synthesis Manager when speech has finished.
 */
void CarbonSpeak::speechDoneProc(SpeechChannel inSpeechChannel, long inRefCon)
{
	if (blocking)
	{
		blocking = false;
		pthread_cond_broadcast(&conditional);
	}
}

/*
 * Reinitializes the text to speech engine.
 */
void CarbonSpeak::reinitialize()
{
	destroyChannel();
	initialize();
	
	// Re-set all parameters to their previous values.
	if (currentVoice != -1)
	{
		VoiceSpec theVoice;
		GetIndVoice(currentVoice, &theVoice);
		SetSpeechInfo(currentChannel, soCurrentVoice, &theVoice);
	}
	
	SetSpeechInfo(currentChannel, soVolume, &currentVolume);
	SetSpeechInfo(currentChannel, soRate, &currentRate);
	handlePitch();
}

/*
 * Speaks the given string.
 * 
 * @param text: The string to speak.
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::speak(std::string text)
{
	OSErr err;
	
	err = SpeakBuffer(currentChannel, text.c_str(), text.length(), NULL);
	
	return (err == noErr);
}

/*
 * Speaks the given string and blocks until speaking is complete.
 *
 * @param text: The string to speak.
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::speakBlocking(std::string text)
{
	OSErr err;
	// Don't send empty strings, because the SSM SpeechDone callback will never fire
	// and we'll be blocking indefinitely.
	if (text.length() < 1)
	{
		return false;
	}
	
	// Stop any current speech first.
	stop();
	
	err = SpeakBuffer(currentChannel, text.c_str(), text.length(), NULL);
	
	if (err != noErr)
	{
		return false;
	}
	else
	{
		blocking = true;
	}

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&conditional, &mutex);
	pthread_mutex_unlock(&mutex);
	return true;
}

/*
 * Pauses current speech (if any).
 *
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::pause()
{
	OSErr err = PauseSpeechAt(currentChannel, kEndOfWord);
	
	return (err == noErr);
}

/*
 * Resumes current speech, if any is currently paused.
 *
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::resume()
{
	OSErr err = ContinueSpeech(currentChannel);
	
	return (err == noErr);
}

/*
 * Stops current speech (if any).
 *
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::stop()
{
	OSErr err = StopSpeech(currentChannel);
	
	return (err == noErr);
}

/*
 * Determines whether or not the engine is currently speaking.
 * Not supported by all engines. If not supported, false is always
 * returned.
 *
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::isSpeaking()
{
	return (SpeechBusy() > 0);
}

/*
 * Gets the current volume. (0.0 - 1.0)
 *
 * @return double
 */
double CarbonSpeak::getVolume()
{
	return currentVolume / 65536.0;
}

/*
 * Gets the current speed. (0.0 - 1.0)
 *
 * @return double
 */
double CarbonSpeak::getSpeed()
{
	return currentRate / 65536.0;
}

/*
 * Gets the current pitch.
 *
 * @return double
 */
double CarbonSpeak::getPitch() {
	return currentPitch / 65536.0;
}

/*
 * Sets the current volume. (0.0 - 1.0)
 *
 * @param vol: the new volume
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::setVolume(double vol)
{
	if (vol >= 0 && vol <= 1.0)
	{
		currentVolume = vol * 65536.0;
		OSErr err = SetSpeechInfo(currentChannel, soVolume, &currentVolume);
		return (err == noErr);
	}
	
	return false;
}

/*
 * Sets the current speed. (0.0 - 1.0)
 *
 * @param speed: the new speed
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::setSpeed(double speed)
{
	if (speed >= 0.0)
	{
		currentRate = speed * 65536.0;
		OSErr err = SetSpeechInfo(currentChannel, soRate, &currentRate);
		return (err == noErr);
	}
	
	return false;
}

/*
 * Sets the current pitch.
 *
 * @param speed: the new pitch
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::setPitch(double pitch)
{
	if (pitch >= 0.0)
	{
		currentPitch = pitch * 65536.0;
		OSErr err = SetSpeechPitch(currentChannel, currentPitch);
		return (err == noErr);
	}
	
	return false;
}

/*
 * Set the current voice.
 *
 * @param voice: the new voice index (zero based)
 * @return boolean indicates success or failure.
 */
bool CarbonSpeak::setVoice(short voice)
{
	OSErr err;
	VoiceSpec newVoice;
	
	err = GetIndVoice(voice, &newVoice);
	if (err != noErr)
	{
		return false; // invalid voice?
	}
	
	// We have to destroy and re-initialize our channel, unfortunately.
	destroyChannel();
	initialize();
	
	// Attempt to set the voice...
	err = SetSpeechInfo(currentChannel, soCurrentVoice, &newVoice);
	
	if (err != noErr)
	{
		return false; // invalid voice?
	}
	else
	{
		// It's safe to assume this is the new voice.
		currentVoice = voice;
	}
	
	// Update volume, rate and pitch data so they remain constant.
	SetSpeechInfo(currentChannel, soVolume, &currentVolume);
	SetSpeechInfo(currentChannel, soRate, &currentRate);
	handlePitch();
	return true; // hope we succeeded!
}