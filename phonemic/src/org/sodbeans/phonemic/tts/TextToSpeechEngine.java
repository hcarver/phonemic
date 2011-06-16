/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.sodbeans.phonemic.tts;

import java.util.HashMap;

/**
 * Provides a standard enumerated type for representing the text to speech
 * engine on the system.
 * 
 * @author Andreas Stefik
 */
public enum TextToSpeechEngine {
    JAWS ("JAWS"),
    NVDA ("NVDA"),
    MICROSOFT_SAPI ("MICROSOFT_SAPI"),
    APPLE_CARBON ("APPLE_CARBON"),
    APPLE_SAY ("APPLE_SAY"),
    SPEECH_DISPATCHER ("SPEECH_DISPATCHER"),
    NULL ("NULL");

    private String engineName;
    TextToSpeechEngine(String name) {
        engineName = name;
    }

    /**
     * Returns a string representation of the name of the engine.
     * 
     * @return the engineName
     */
    public String getEngineName() {
        return engineName;
    }
    
    public String humanReadableName() {
        HashMap<String, String> names = new HashMap<String, String>();
        
        names.put(JAWS.toString(), "Jaws");
        names.put(NVDA.toString(), "NVDA");
        names.put(MICROSOFT_SAPI.toString(), "Microsoft SAPI");
        names.put(APPLE_CARBON.toString(), "Mac OS X Carbon");
        names.put(APPLE_SAY.toString(), "Mac OS X Say");
        
        return names.get(this.toString());
    }
}
