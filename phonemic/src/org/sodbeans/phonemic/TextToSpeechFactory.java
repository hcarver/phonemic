/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.sodbeans.phonemic;

import java.io.File;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.sodbeans.phonemic.daemon.PhonemicDaemon;
import org.sodbeans.phonemic.tts.*;
import org.sodbeans.phonemic.tts.impl.*;

/**
 * This factory class is the primary "way in" for users to get a text to
 * speech implementation.
 *
 * @author Andreas Stefik and Jeff Wilson
 */
public class TextToSpeechFactory {
    /**
     * The Phonemic version of this release.
     */
    public static final double PHONEMIC_VERSION = 1.1;
    
    /**
     * The TCP port where the Phonemic daemon will be listening.
     */
    public static final int PHONEMIC_SERVER_PORT = 56101;
    
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
                if (System.getProperty("os.arch").trim().equalsIgnoreCase("ppc"))
                    loadLibrary("CarbonSpeakJNI_PPC");
                else
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

    /**
     * Get the default text to speech implementation for this system. This method
     * will attempt to look for an existing Phonemic server; if one is not found,
     * a new one will be spawned, listening on TextToSpeechFactory.PHONEMIC_SERVER_PORT,
     * and bound to localhost. A client talking to this protocol will be returned.
     * If a Phonemic server cannot be spawned and one is not aleady running,
     * a local text to speech engine will be used.
     *
     * 
     * If an existing Phonemic server is running, it will be used.
     * @return a text to speech interface
     */
    public synchronized static TextToSpeech getDefaultTextToSpeech() {
        TextToSpeech tts = null;
        
        // Attempt to connect to a local daemon and return.
        try {
            tts = TextToSpeechFactory.getPhonemicClient("localhost");
            return tts;
        } catch (IOException ex) {
            // Connection failed -- fall through.
        }

        // The local server couldn't be reached--try spawning one.
        PhonemicDaemon daemon;
        try {
            daemon = TextToSpeechFactory.newPhonemicDaemon(speech);
            daemon.bind();
            daemon.start();
            
            // Attempt to connect.
            tts = TextToSpeechFactory.getPhonemicClient("localhost");
            return tts;
        } catch (UnknownHostException ex) {
            Logger.getLogger(TextToSpeechFactory.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(TextToSpeechFactory.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        // If we make it here, a server couldn't be connected to, nor could we
        // successfully spawn one. Return a simple speech object.
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
     * Start a new Phonemic server daemon, listening on localhost. The port
     * number that will be bound is TextToSpeechFactory.HONEMIC_SERVER_PORT.
     * 
     * @param textToSpeech the speech engine this daemon will talk to.
     */
    public synchronized static PhonemicDaemon newPhonemicDaemon(TextToSpeech textToSpeech) throws UnknownHostException, IOException {
        return newPhonemicDaemon(textToSpeech, InetAddress.getByName("localhost"));
    }

    /**
     * Start a new Phonemic daemon that binds on the specified address.
     * 
     * @param address 
     */
    public static synchronized PhonemicDaemon newPhonemicDaemon(TextToSpeech textToSpeech, InetAddress address) throws IOException {
        PhonemicDaemon daemon = new PhonemicDaemon(textToSpeech, PHONEMIC_SERVER_PORT, address);
        
        return daemon;
    }
    
    /**
     * Connect to the Phonemic server at 'hostname'. Multiple connections to the
     * same host are permitted.
     * 
     * @param hostname the host we wish to connect to (most commonly, localhost, or "127.0.0.1").
     * 
     * @return the client connection interface
     */
    public synchronized static TextToSpeech getPhonemicClient(String hostname) throws IOException {
        ClientSpeak c = new ClientSpeak(hostname, PHONEMIC_SERVER_PORT);
        c.connect();
        
        return c;
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
    
    /**
     * Returns whether or not the given file exists.
     * @param path
     * @return 
     */
    private static boolean fileExists(String path) {
        File f = new File(path);
        
        return f.exists();
    }
    /**
     * Determines if the given JAWS script is installed. This function checks
     * for the following path names, where %APPDATA% is the AppData environment
     * variable.
     * 
     * %APPDATA%\Freedom Scientific\JAWS\12.0\Settings\enu\$name.jcf
     * %APPDATA%\Freedom Scientific\JAWS\12.0\Settings\enu\$name.jss
     * %APPDATA%\Freedom Scientific\JAWS\12.0\Settings\enu\$name.jsb
     * %APPDATA%\Freedom Scientific\JAWS\12.0\Settings\enu\$name.jsd
     * %APPDATA%\Freedom Scientific\JAWS\12.0\Settings\enu\$name.jsh
     * 
     * @param name the script name to search for
     * @return 
     */
    public static boolean jawsScriptExists(String name) {
        if (!isWindows())
            return false;
        
        String appData = System.getenv("AppData");
        
        return fileExists(appData + "\\Freedom Scientific\\JAWS\\12.0\\Settings\\enu\\" + name + ".jcf")
                || fileExists(appData + "\\Freedom Scientific\\JAWS\\12.0\\Settings\\enu\\" + name + ".jsb")
                || fileExists(appData + "\\Freedom Scientific\\JAWS\\12.0\\Settings\\enu\\" + name + ".jsd")
                || fileExists(appData + "\\Freedom Scientific\\JAWS\\12.0\\Settings\\enu\\" + name + ".jsh")
                || fileExists(appData + "\\Freedom Scientific\\JAWS\\12.0\\Settings\\enu\\" + name + ".jss");
    }
    
    /**
     * Determine if the given NVDA script is installed. THis function checks
     * for the following path names, where %APPDATA% is the AppData environment
     * variable.
     * 
     * %APPDATA%\nvda\appModules\$name.py
     * %APPDATA%\nvda\appModules\$name.pyo
     * 
     * @param name the script name to search for.
     * @return 
     */
    public static boolean nvdaScriptExists(String name) {
        if (!isWindows())
            return false;
        
        String appData = System.getenv("AppData");

        return fileExists(appData + "\\nvda\\appModules\\" + name + ".py")
                || fileExists(appData + "\\nvda\\appModules\\" + name + ".pyo");
    }
}