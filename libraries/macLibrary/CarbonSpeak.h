#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <Carbon/Carbon.h>

#ifndef _CARBONSPEAK_H
#define _CARBONSPEAK_H

class CarbonSpeak
{
private:
	static CarbonSpeak* instance; // current instance of CarbonSpeak

protected:
	SpeechChannel currentChannel; // speech channel handle
	short currentVoice; // current voice in use (-1 indicates system default)
	Fixed currentVolume; // current volume (default is set by system)
	Fixed currentRate; // current rate (default is set by system)
	Fixed currentPitch; // current pitch (default is set by system)
	static bool blocking; // used by StartSpeakingBlocking
	
	/*
	 * Protected constructor prevents instantiation outside of inst() call.
	 */
	CarbonSpeak();

	/*
	 * Initializes the text to speech engine.
	 */
	void initialize();
	
	/*
	 * Handles pitch changes during initialization, pitch changes, reinitialization and voice changes.
	 */
	void handlePitch();
	
	/*
	 * Called by the Speech Synthesis Manager when speech has finished.
	 */
	static void speechDoneProc(SpeechChannel inSpeechChannel, long inRefCon);
	
	/*
	 * Destroys the speech channel -- used during reinitialization.
	 */
	void destroyChannel();
public:
	/*
	 * Creates a new instance of CarbonSpeak if one doesn't already exist.
	 */
	static CarbonSpeak* inst();
	
	~CarbonSpeak();
	
    /*
     * Reinitializes the text to speech engine.
     */
    void reinitialize();
    
    /*
     * Speaks the given string.
     * 
     * @param text: The string to speak.
     * @return boolean indicates success or failure.
     */
    bool speak(std::string text);
	
    /*
     * Speaks the given string and blocks until speaking is complete.
     *
     * @param text: The string to speak.
     * @return boolean indicates success or failure.
     */
    bool speakBlocking(std::string text);
	
    /*
     * Pauses current speech (if any).
     *
     * @return boolean indicates success or failure.
     */
    bool pause();
	
    /*
     * Resumes current speech, if any is currently paused.
     *
     * @return boolean indicates success or failure.
     */
    bool resume();
	
    /*
     * Stops current speech (if any).
     *
     * @return boolean indicates success or failure.
     */
    bool stop();
	
    /*
     * Determines whether or not the engine is currently speaking.
     * Not supported by all engines. If not supported, false is always
     * returned.
     *
     * @return boolean indicates success or failure.
     */
    bool isSpeaking();
	
    /*
     * Gets the current volume. (0.0 - 1.0)
     *
     * @return double
     */
    double getVolume();
	
    /*
     * Gets the current speed.
     *
     * @return double
     */
    double getSpeed();

	/*
     * Gets the current pitch.
     *
     * @return double
     */
    double getPitch();
	
    /*
     * Sets the current volume. (0.0 - 1.0)
	 *
     * @param vol: the new volume
     * @return boolean indicates success or failure.
     */
    bool setVolume(double vol);
	
    /*
     * Sets the current speed.
	 *
     * @param speed: the new speed
     * @return boolean indicates success or failure.
     */
	bool setSpeed(double speed);

	/*
     * Sets the current pitch.
	 *
     * @param speed: the new pitch
     * @return boolean indicates success or failure.
     */
	bool setPitch(double pitch);
	
    /*
     * Set the current voice.
     *
     * @param voice: the new voice index (zero based)
     * @return boolean indicates success or failure.
     */
	bool setVoice(short voice);
};

#endif