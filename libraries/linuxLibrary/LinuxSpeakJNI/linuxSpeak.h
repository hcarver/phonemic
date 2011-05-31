/*
 * File:   DispatchSpeak.h
 * Author: jeff
 *
 * Created on January 13, 2011, 1:31 PM
 */

#ifndef LINUXSPEAK_H
#define	LINUXSPEAK_H

#include <string>
#include <vector>
#include <semaphore.h>
#include <libspeechd.h>

class LinuxSpeak {
private:
    static LinuxSpeak* instance;
protected:
    // Protected constructor prevents instantiation this way
    LinuxSpeak(std::string app_name);
    void init(std::string app_name);

    // Member variables
    SPDConnection *speech_conn; // our link to speech-dispatcher
    SPDPriority speech_priority;
    int current_volume;
    int current_rate;
    int current_pitch;
    std::string current_voice;
    static bool speechActive; // are we speaking?
    static bool blocking; // Are we currently blocking?

    // Callback functions for speech-dispatcher
    /*
     * If speech starts, or resumes, change speechActive appropriately.
     */
    static void startOfSpeech(size_t msg_id, size_t client_id,
        SPDNotificationType type);

    /*
     * If speech pauses, or cancels, change speechActive appropriately.
     */
    static void cancelSpeech(size_t msg_id, size_t client_id,
        SPDNotificationType type);
    /*
     * When speech is completed, wake up the main thread and
     * change speechActive appropriately.
     */
    static void endOfSpeech(size_t msg_id, size_t client_id,
        SPDNotificationType type);
public:
    static LinuxSpeak* inst(std::string app_name);
    virtual ~LinuxSpeak();
    void reinitialize(std::string app_name);
    
    /*f
     * Get the current volume being used by speech-dispatcher.
     */
    int getVolume();

    /*
     * Get the current rate being used by speech-dispatcher.
     */
    int getRate();

    /*
     * Get the current pitch being used by speech-dispatcher.
     */
    int getPitch();

    /*
     * Get the current voice.
     */
    std::string getVoice();

    /*
     * Get all voices available to speech-dispatcher.
     */
    std::vector<std::string> getVoices();

    /*
     * Set the volume for speech-dispatcher.
     */
    bool setVolume(int vol);

    /*
     * Set the rate for speech-dispatcher.
     */
    bool setRate(int rate);

    /*
     * Set the pitch for speech-dispatcher
     */
    bool setPitch(int pitch);

    /*
     * Set the output voice to `voice'.
     * Upon failure, returns false.
     */
    bool setVoice(std::string voice);

    /*
     * Speaks the given `text'.
     */
    bool speak(std::string text);

    /*
     * Speaks the given chararacter.
     */
    bool speakChar(std::string c);

    /*
     * Speaks the given `text', blocking until speech has finished.
     */
    bool speakBlocking(std::string text);

    /*
     * Speaks the given character, blocking until speech has finished.
     */
    bool speakCharBlocking(std::string c);
    
    /*
     * Pause any active speech
     */
    bool pause();
    /*
     * Resume any active speech
     */
    bool resume();

    /*
     * Stop any active speech
     */
    bool stop();

    /*
     * Query to find out if the engine is speaking.
     */
    bool isSpeaking();
};

#endif	/* LINUXSPEAK_H */

