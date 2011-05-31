/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package phonemictestapp;

import org.sodbeans.phonemic.*;
import org.sodbeans.phonemic.tts.*;

/**
 *
 * @author jeff
 */
public class PhonemicTestApp {
    private static String introText = "This is a simple application using the" +
            " Phonemic speech library. Since you didn't pass me any" +
            " command line arguments, I am just speaking this introductory text." +
            " This application demonstrates how to get started quickly with Phonemic." +
            " It is as simple as including the phonemic.jar in your project with the required" +
            " library files.";
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // Get a text-to-speech object from Phonemic.
        TextToSpeech speech = TextToSpeechFactory.getDefaultTextToSpeech();
        
        // We will either speak a little bit of introductory text, or we will
        // speak the first command line argument.
        if (args.length == 0)
        {
            speech.speakBlocking(introText);
        }
        else
        {
            speech.speakBlocking(args[0]);
        }
        
        // Terminate all Phonemic threads by exiting.
        System.exit(0);
    }
}
