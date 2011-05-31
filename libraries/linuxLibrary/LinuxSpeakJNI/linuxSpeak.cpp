
/*
 * File:   DispatchSpeak.cpp
 * Author: jeff
 *
 * Created on January 13, 2011, 1:31 PM
 */

#include <iostream>
#include <string>
#include <vector>
#include <dlfcn.h>
#include <libspeechd.h>
#include "linuxSpeak.h"

sem_t blocking_semaphore;
LinuxSpeak* LinuxSpeak::instance = NULL;
const std::string SPEECHD_LIBRARY_NAME = "libspeechd.so.2";
bool LinuxSpeak::blocking = false;
bool LinuxSpeak::speechActive = false;

// Function called to instantiate
LinuxSpeak* LinuxSpeak::inst(std::string app_name)
{
    // If we are already initialized, return the existing instance.
    if (instance != NULL)
    {
        return instance;
    }

    // Load the library.
    void *dh = dlopen(SPEECHD_LIBRARY_NAME.c_str(), RTLD_NOW);

    if (dh == NULL)
    {
        std::cout << "LINUX Error: Could not load library: " << dlerror()
                << std::endl;
        return NULL;
    }

    instance = new LinuxSpeak(app_name);

    return LinuxSpeak::instance;
}

void LinuxSpeak::reinitialize(std::string app_name) {
    // Kill it!
    spd_close(speech_conn);

    // ... and try again.
    init(app_name);
}
/*
 * When speech is completed, wake up the main thread.
 */
void LinuxSpeak::endOfSpeech(size_t msg_id, size_t client_id,
    SPDNotificationType type)
{
    speechActive = false;
    if (blocking)
    {
        // Tell the main thread to wake up.
        sem_post(&blocking_semaphore);
    }
}

/*
 * If speech starts, or resumes, change speechActive appropriately.
 */
void LinuxSpeak::startOfSpeech(size_t msg_id, size_t client_id,
    SPDNotificationType type)
{
    speechActive = true;
}

/*
 * If speech pauses, or cancels, change speechActive appropriately.
 */
void LinuxSpeak::cancelSpeech(size_t msg_id, size_t client_id,
    SPDNotificationType type) {
    speechActive = false;
}

// Constructor
LinuxSpeak::LinuxSpeak(std::string app_name)
{
    // Initialize semaphore for blocking purposes.
    sem_init(&blocking_semaphore, 0, 0);

    // Set up some good defaults.
    current_volume = 0; // normal volume
    current_rate = 0; // normal rate
    current_pitch = 0; // normal pitch

    // Since there is no way to ask speech-dispatcher what voice
    // is currently being used, these are empty until set by the user.
    current_voice = "";

    speechActive = false; // no speech yet.
    init(app_name);
}

// Actual initialization.
void LinuxSpeak::init(std::string app_name)
{
    // Attempt to connect to speech-dispatcher.
    speech_conn = spd_open(app_name.c_str(), NULL, NULL, SPD_MODE_THREADED);

    if (speech_conn == NULL)
    {
        // TODO PROPER error handling
        std::cerr << "LinuxSpeak ERROR: Could not open a connection to speech-dispatcher!"
                << std::endl;
        return;
    }

    // Set the volume and rate to values we know.
    spd_set_volume(speech_conn, current_volume);
    spd_set_voice_rate(speech_conn, current_rate);

    // Set the 'end' and 'cancel' callbacks for blocking purposes.
    speech_conn->callback_end = endOfSpeech;
    speech_conn->callback_cancel = speech_conn->callback_pause = cancelSpeech;
    speech_conn->callback_begin = speech_conn->callback_resume = startOfSpeech;

    spd_set_notification_on(speech_conn, SPD_BEGIN);
    spd_set_notification_on(speech_conn, SPD_PAUSE);
    spd_set_notification_on(speech_conn, SPD_RESUME);
    spd_set_notification_on(speech_conn, SPD_CANCEL);
    spd_set_notification_on(speech_conn, SPD_END);

    blocking = false; // blocking is not currently enabled
}

LinuxSpeak::~LinuxSpeak()
{
    // Clean up the connection.
    spd_close(speech_conn);
}

/*
 * Get the current volume being used by speech-dispatcher.
 */
int LinuxSpeak::getVolume()
{
    return current_volume;
}

/*
 * Get the current rate being used by speech-dispatcher.
 */
int LinuxSpeak::getRate()
{
    return current_rate;
}

/*
 * Get the current pitch being used by speech-dispatcher.
 */
int LinuxSpeak::getPitch()
{
    return current_pitch;
}

/*
 * Get the current voice.
 */
std::string LinuxSpeak::getVoice()
{
    return current_voice;
}

/*
 * Get all voices available to speech-dispatcher.
 */
std::vector<std::string> LinuxSpeak::getVoices()
{
    std::vector<std::string> voices;
    SPDVoice **vlist = spd_list_synthesis_voices(speech_conn);

    int x = 0;
    while (vlist[x] != NULL)
    {
        voices.push_back(std::string(vlist[x]->name));
        ++x;
    }

    return voices;
}

/*
 * Set the volume for speech-dispatcher.
 */
bool LinuxSpeak::setVolume(int vol)
{
    int result = spd_set_volume(speech_conn, vol);

    if (result == 0)
    {
        current_volume = vol;
        return true;
    }

    return false;
}

/*
 * Set the rate for speech-dispatcher.
 */
bool LinuxSpeak::setRate(int rate)
{
    int result = spd_set_voice_rate(speech_conn, rate);

    if (result == 0)
    {
        current_rate = rate;
        return true;
    }

    return false;
}

bool LinuxSpeak::setPitch(int pitch)
{
    int result = spd_set_voice_pitch(speech_conn, pitch);

    if (result == 0)
    {
        current_pitch = pitch;
        return true;
    }

    return false;
}

/*
 * Set the output voice to `voice'.
 * Upon failure, returns false.
 */
bool LinuxSpeak::setVoice(std::string voice)
{
    // Load the list of voices and attempt to find this voice.
    SPDVoice **vlist = spd_list_synthesis_voices(speech_conn);

    int x = 0;
    bool found = false;
    while (vlist[x] != NULL)
    {
        if (std::string(vlist[x]->name) == voice) {
            found = true;
            break;
        }
        ++x;
    }

    if (found) {
        int result = spd_set_synthesis_voice(speech_conn, voice.c_str());
        if (result == 0)
        {
            current_voice = vlist[x]->name;
            return true;
        }
    }

    return false;
}


/*
 * Speaks the given `text'.
 */
bool LinuxSpeak::speak(std::string text)
{
    if (blocking)
    {
        return false;
    }

    return (spd_say(speech_conn, SPD_TEXT, text.c_str()) > 0);
}

/*
 * Speaks the given chararacter.
 */
bool LinuxSpeak::speakChar(std::string c)
{
    if (blocking)
    {
        return false;
    }
    
    return (spd_char(speech_conn, SPD_TEXT, c.c_str()));
}

/*
 * Speaks the given `text', blocking until speech has finished.
 */
bool LinuxSpeak::speakBlocking(std::string text)
{
    // Don't interrupt any current speech.
    if (blocking)
    {
        return false;
    }

    blocking = true; // let callback know to post semaphore.

    // Stop any current speech -- otherwise, blocking won't go to
    // completion.
    spd_stop(speech_conn);

    int st = spd_say(speech_conn, SPD_TEXT, text.c_str());

    if (st > 0)
    {
        // Wait for speech-dispatcher to finish.
        sem_wait(&blocking_semaphore);
        blocking = false;
        return true;
    }

    // Something failed.
    return false;
}

/*
 * Speaks the given character, blocking until speech has finished.
 */
bool LinuxSpeak::speakCharBlocking(std::string c)
{
    // Don't interrupt any current speech.
    if (blocking)
    {
        return false;
    }

    blocking = true; // let callback know to post semaphore.

    // Stop any current speech -- otherwise, blocking won't go to
    // completion.
    spd_stop(speech_conn);

    int st = spd_char(speech_conn, SPD_TEXT, c.c_str());

    if (st > 0)
    {
        // Wait for speech-dispatcher to finish.
        sem_wait(&blocking_semaphore);
        blocking = false;
        return true;
    }

    // Something failed.
    return false;
}

/*
 * Pause any active speech
 */
bool LinuxSpeak::pause()
{
    if (blocking)
    {
        return false;
    }

    return (spd_pause(speech_conn) == 0);
}

/*
 * Resume any active speech
 */
bool LinuxSpeak::resume()
{
    if (blocking)
    {
        return false;
    }

    return (spd_resume(speech_conn) == 0);
}

/*
 * Stop any active speech
 */
bool LinuxSpeak::stop()
{
    return (spd_stop(speech_conn) == 0);
}

/*
 * Query to find out if the engine is speaking.
 */
bool LinuxSpeak::isSpeaking() {
    return speechActive;
}