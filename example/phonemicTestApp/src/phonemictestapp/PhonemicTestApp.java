/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package phonemictestapp;

import java.util.Iterator;
import org.sodbeans.phonemic.*;
import org.sodbeans.phonemic.tts.*;

/**
 *
 * @author jeff
 */
public class PhonemicTestApp {

    private static final String longSampleText = "Is this the real life? "
            + "Is this just fantasy? "
            + "Caught in a landslide; "
            + "No escape from reality. "
            + "Open your eyes, "
            + "Look up to the skies and see. "
            + "I'm just a poor boy, I need no sympathy "
            + "Because I'm easy come, easy go: "
            + "A little high, little low. "
            + "Anyway the wind blows, doesn't really matter to me, to me! ";
    private static String introText = "This is a simple application using the "
            + " Phonemic speech library. I will read any text you pass as a command line argument. "
            + " Since you didn't pass me a "
            + " command line argument, I am just speaking this introductory text."
            + " This application demonstrates how to get started quickly with Phonemic."
            + " To get started using Phonemic with your own project, include the phonemic.jar"
            + " in your project with the required library files.";

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // Get a text-to-speech object from Phonemic.
        TextToSpeech speech = TextToSpeechFactory.getDefaultTextToSpeech();

        // We will either speak a little bit of introductory text, or we will
        // speak the first command line argument.
        if (args.length > 0) {
            speech.speakBlocking(args[0]);
        } else {
            // Default engine should be JAWS if it's installed.
            System.out.println("If JAWS is installed, it should be the default engine");
            System.out.println("Current engine: " + speech.getTextToSpeechEngine().getEngineName());

            if (speech.isSpeaking()) {
                System.out.println("PROBLEM: ENGINE THINKS IT'S SPEAKING WHEN IT ISN'T");
            }
            speech.speak("Basic speech should work");

            (new Thread(new SpeechKiller(5000))).start();
            speech.speak("Unblocking speech should be killable. " + longSampleText);

            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                // No big deal.
            }

            if (!speech.isSpeaking()) {
                System.out.println("PROBLEM: ENGINE THINKS IT'S NOT SPEAKING WHEN IT IS");
            }

            while (speech.isSpeaking()) {
                ;
            }

            (new Thread(new SpeechKiller(1000))).start();
            speech.speakBlocking("Blocking speech should not be killable. Surely "
                    + "these two sentences will take at least 2 seconds to say.");

            Iterator<SpeechVoice> voices = speech.getAvailableVoices();
            int voiceCount = 0;

            while (voices.hasNext()) {
                SpeechVoice voice = voices.next();
                voiceCount++;

                if (!speech.setVoice(voice)) {
                    System.out.println("PROBLEM: COULDN'T SET THE ENGINE'S VOICE");
                }

                if (!speech.getCurrentVoice().getName().contains(voice.getName())) {
                    System.out.println("PROBLEM: COULDN'T GET THE ENGINE'S VOICE");
                }
            }

            if (voiceCount == 0) {
                System.out.println("PROBLEM: NO VOICES FOUND");
            }

            speech.speak("This text will pause at some point for 1 second. And then it will continue. And then, later, it will stop. " + longSampleText);

            //Should pause
            speech.pause();

            // Should resume
            speech.resume();

            //Should stop
            speech.stop();

            double newSpeed = 3.0;

            // Should set the speed.
            speech.setSpeed(newSpeed);

            // Should set the volume
            double newVolume = 0.8;
            speech.setVolume(newVolume);

            speech.reinitialize();
            speech.speak("Should still work after reinitialising.");

            if (!speech.setTextToSpeechEngine(TextToSpeechEngine.MICROSOFT_SAPI)) {
                System.out.println("Could not set speech engine");
            }
            if (speech.getTextToSpeechEngine() != TextToSpeechEngine.MICROSOFT_SAPI) {
                System.out.println("Could not get speech engine");
            }

            if (!speech.setTextToSpeechEngine(TextToSpeechEngine.JAWS)) {
                System.out.println("Could not set speech engine");
            }
            if (speech.getTextToSpeechEngine() != TextToSpeechEngine.JAWS) {
                System.out.println("Could not get speech engine");
            }

            if (!speech.setTextToSpeechEngine(TextToSpeechEngine.NVDA)) {
                System.out.println("Could not set speech engine");
            }
            if (speech.getTextToSpeechEngine() != TextToSpeechEngine.NVDA) {
                System.out.println("Could not get speech engine");
            }
        }

        // Terminate all Phonemic threads by exiting.
        System.exit(0);
    }

    private static class SpeechKiller implements Runnable {

        private final long wait_ms;

        public SpeechKiller(long wait_ms) {
            this.wait_ms = wait_ms;
        }

        @Override
        public void run() {
            try {
                Thread.sleep(wait_ms);
            } catch (InterruptedException e) {
                // No big deal
            }
            TextToSpeechFactory.getDefaultTextToSpeech().stop();
        }
    }
}
