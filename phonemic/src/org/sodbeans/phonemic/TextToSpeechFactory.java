/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.sodbeans.phonemic;

import java.io.File;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.sodbeans.phonemic.tts.*;
import org.sodbeans.phonemic.tts.impl.*;

/**
 * This factory class is the primary "way in" for users to get a text to
 * speech implementation.
 *
 * @author Andreas Stefik and Jeff Wilson
 */
public class TextToSpeechFactory {
    
    private static final OperatingSystem os = OperatingSystem.getOS();
    private static boolean carbonLibraryLoaded = false;
    private static boolean windowsSAPILoaded = false;
    private static boolean linuxLibraryLoaded = false;
    private static boolean isLoaded = false;
    private final static SpeechBridge speech = new SpeechBridge();
    private static final Logger logger =
            Logger.getLogger("org.sodbeans.phonemic.TextToSpeechFactory");

    //set the appropriate variables if loaded.
    private static CarbonSpeak carbonSpeak = null;
    private static AppleSaySpeak appleSaySpeak = null;
    private static WindowsSpeak windowsSpeak = null;
    private static LinuxSpeak linuxSpeak = null;

    static {
        // Load the proper library for this OS.

        if (os == OperatingSystem.MAC_OSX) {
            try {
                loadLibrary("CarbonSpeakJNI");
                carbonLibraryLoaded = true;
            }
            catch (Exception e) {
                StackTraceElement[] stackTrace = e.getStackTrace();
                logger.log(Level.SEVERE, "Could not load carbon library. " +
                    getStackTraceString(stackTrace));
            }
            catch (Error e) {
                StackTraceElement[] stackTrace = e.getStackTrace();
                logger.log(Level.SEVERE, "Could not load carbon library. " +
                    getStackTraceString(stackTrace));
            }

            if (carbonLibraryLoaded) {
                // yay! we can use carbon
                carbonSpeak = new CarbonSpeak();;
                speech.setSpeech(carbonSpeak);
                isLoaded = true;
            }
            else {
                // Revert to using the `say' command line utility
                appleSaySpeak = new AppleSaySpeak();
                speech.setSpeech(appleSaySpeak);
                isLoaded = true;
            }
        }
        else if (os == OperatingSystem.LINUX) {
            // Our only option is speech-dispatcher.
            try {
                if (OperatingSystem.getNumberBits() == 32) {
                    loadLibrary("LinuxSpeakJNI");
                    linuxLibraryLoaded = true;
                } else {
                    loadLibrary("LinuxSpeakJNI64");
                    linuxLibraryLoaded = true;
                }
            }
            catch (Exception e) {
                StackTraceElement[] stackTrace = e.getStackTrace();
                logger.log(Level.SEVERE, "Could not load speech-dispatcher library. " +
                    getStackTraceString(stackTrace));
            }
            catch (Error e) {
                StackTraceElement[] stackTrace = e.getStackTrace();
                logger.log(Level.SEVERE, "Could not load speech-dispatcher library. " +
                    getStackTraceString(stackTrace));
            }

            linuxSpeak = new LinuxSpeak();
            speech.setSpeech(linuxSpeak);
            isLoaded = true;
        }
        else if(isWindows()) {

            try {
                if(OperatingSystem.getNumberBits() == 64) {
                    loadLibrary("nvdaControllerClient64");
                    loadLibrary("SappyJNI64");
                }
                else {
                    loadLibrary("nvdaControllerClient32");
                    loadLibrary("SappyJNI");
                }
                windowsSpeak = new WindowsSpeak();
                speech.setSpeech(windowsSpeak);
                windowsSAPILoaded = true;
                isLoaded = true;
            }
            catch (Exception e) {
                StackTraceElement[] stackTrace = e.getStackTrace();
                logger.log(Level.SEVERE, "Could not load windows dll. " +
                    getStackTraceString(stackTrace));
            }
            catch (Error e) {
                StackTraceElement[] stackTrace = e.getStackTrace();
                logger.log(Level.SEVERE, "Could not load windows dll. " +
                    getStackTraceString(stackTrace));
            }
        }

        if(!isLoaded) { //log the problem
            logger.log(Level.SEVERE, "Text to speech libraries could not be loaded.");
        }
    }

    private static String getStackTraceString(StackTraceElement[] elements) {
        String str = "";
        for(int i = 0; i < elements.length; i++) {
            str += "\n" + elements[i];
        }
        return str;
    }
    /*
     * Get the specific TTS implementation for this OS.
     *
     * returns AbstractTextToSpeech object if OS is supported. null otherwise
     */
    public synchronized static TextToSpeech getDefaultTextToSpeech() {
        return speech;
    }

    /**
     * Allows the user to request a particular text to speech engine. Since
     * some engines are only available on some systems (e.g., JAWS on Windows,
     * Carbon on Mac OS X), this request may fail and return null.
     *
     * @param engine
     * @return
     */
    public synchronized static TextToSpeech getTextToSpeech(TextToSpeechEngine engine) {
        if (OperatingSystem.MAC_OSX == os && TextToSpeechEngine.APPLE_CARBON == engine) {
            if(carbonLibraryLoaded) {
                return carbonSpeak;
            }
        }
        else if (OperatingSystem.MAC_OSX == os && TextToSpeechEngine.APPLE_SAY == engine){
            return appleSaySpeak;
        }
        else if (isWindows() && TextToSpeechEngine.MICROSOFT_SAPI == engine){
            if(windowsSAPILoaded) {
                windowsSpeak.setTextToSpeechEngine(engine);
                return windowsSpeak;
            } //return null otherwise
        }
        else if (isWindows() && TextToSpeechEngine.JAWS == engine){
            if(windowsSAPILoaded) {
                windowsSpeak.setTextToSpeechEngine(engine);
                return windowsSpeak;
            } //return null otherwise
        }
        else if (isWindows() && TextToSpeechEngine.NVDA == engine){
            if(windowsSAPILoaded) {
                windowsSpeak.setTextToSpeechEngine(engine);
                return windowsSpeak;
            } //return null otherwise
        }
        else if (OperatingSystem.LINUX == os && TextToSpeechEngine.SPEECH_DISPATCHER == engine){
            //check that the Linux SO files loaded properly
            if (linuxLibraryLoaded) {
                return linuxSpeak;
            }
        }

        return null;
    }

    /**
     * Returns whether the system is on windows.
     *
     * @return
     */
    private static boolean isWindows() {
        return (OperatingSystem.WINDOWS7 == os ||
            OperatingSystem.WINDOWS_VISTA == os ||
            OperatingSystem.WINDOWS_XP == os);
    }

    /**
     * This flag returns whether or not the system as a whole loaded. If it is
     * false, you want to try and gather information on why it did not
     * load properly.
     *
     * @return
     */
    public static boolean isLoaded() {
        return isLoaded;
    }

    private static void loadLibrary(String library) {
        try {
            System.loadLibrary(library);
        }
        catch(Error error) {
            loadLibraryRelative(library);
        }
    }

    /**
     * Loads the specified library using System.loadLibrary() if the library is
     * found in the java.library.path. Otherwise, System.load() is used with
     * JNIPath prefixed.
     *
     * @param lib
     * @throws Exception
     */
    private static void loadLibraryRelative(String lib){
        boolean libFound = false;
        String JNIRoot = new File(
            TextToSpeechFactory.class.getProtectionDomain().getCodeSource().getLocation().getPath()).getParent();
        String JNIPath;
        String libFullName = lib;

        // Make the full library name
        if (isWindows()) {
            libFullName = lib + ".dll";
        }
        else if (os == OperatingSystem.MAC_OSX) {
            libFullName = "lib" + lib + ".jnilib";
        }
        else if (os == OperatingSystem.LINUX)
        {
            libFullName = "lib" + lib + ".so";
        }

        // Figure out the proper JNIPath and load the library.
        if (isWindows()) {
            JNIPath = JNIRoot.replaceAll("\\%20", " ") + "\\jni";
            System.load(JNIPath + "\\" + libFullName);
        }
        else {
            JNIPath = JNIRoot.replaceAll("\\%20", " ") + "/jni";
            System.load(JNIPath + "/" + libFullName);
        }
    }
}