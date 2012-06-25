/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.sodbeans.phonemic.tts.impl;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.sodbeans.phonemic.RequestType;
import org.sodbeans.phonemic.SpeechLanguage;
import org.sodbeans.phonemic.SpeechPriority;
import org.sodbeans.phonemic.SpeechProcessor;
import org.sodbeans.phonemic.SpeechVoice;
import org.sodbeans.phonemic.tts.TextToSpeech;
import org.sodbeans.phonemic.tts.TextToSpeechEngine;

/**
 * Represents a Phonemic client.
 * 
 * @author jeff
 */
public class ClientSpeak implements TextToSpeech {
    /**
     * The port of the server we wish to connect to.
     */
    protected int port = 0;
    
    /**
     * The hostname of the server we wish to connect to.
     */
    protected String host = null;
    
    /**
     * Our connection to the server.
     */
    protected Socket socket = null;
    
    /**
     * Our input/output streams to the client socket.
     */
    protected BufferedReader input = null;
    protected OutputStream output = null;
    
    /**
     * Create a new client connected to address.
     * 
     * @param host the hostname of the server (typically "localhost" or "127.0.0.1")
     * @param port the port of the server.
     */
    public ClientSpeak(String host, int port) {
        this.host = host;
        this.port = port;
    }
    
    /**
     * Initiate a connection with the server.
     * 
     * @throws IOException 
     */
    public void connect() throws IOException {
        this.socket = new Socket(this.host, this.port);
        
        // Get our input and output streams.
        this.input = new BufferedReader(new InputStreamReader(this.socket.getInputStream()));
        this.output = this.socket.getOutputStream();
        
        // Write two bytes requesting protocol version 1.
        this.output.write(0x0000);
        this.output.write(0x00001);
        
        // Wait for acceptance message.
        String okLine = this.input.readLine();
        
        if (okLine == null || !okLine.equals("OK")) {
            throw new IOException("Unknown response received from Phonemic server.");
        }
    }
    
    /**
     * Send a raw message.
     * @param msg 
     */
    private synchronized void sendRawMessage(String msg) {
        int length = msg.length();
        
        if (length > 0xFFFF) {
            length = 0xFFFF;
            msg = msg.substring(0, 0xFFFE);
        }
        try {
            // Write length header (2 bytes).
            this.output.write(ByteBuffer.allocate(4).putInt(length).array(), 2, 2);
            
            // Write message itself.
            this.output.write(msg.getBytes());
        } catch (IOException ex) {
            Logger.getLogger(ClientSpeak.class.getName()).log(Level.SEVERE, null, ex);
        }        
    }
    
    /**
     * Wait for a boolean response from the server.
     * 
     * @return 
     */
    public synchronized boolean getBooleanResponse() {
        try {
            String line = this.input.readLine();
            
            if (line == null)
                return false;
            
            return Boolean.parseBoolean(line);
        } catch (IOException ex) {
            Logger.getLogger(ClientSpeak.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
    }
    
    /**
     * Wait for a double response from the server.
     * 
     * @return 
     */
    public synchronized double getDoubleResponse() {
        try {
            String line = this.input.readLine();
            
            if (line == null)
                return 0.0;
            
            return Double.parseDouble(line);
        } catch (IOException ex) {
            Logger.getLogger(ClientSpeak.class.getName()).log(Level.SEVERE, null, ex);
            return 0.0;
        }
    }
    
    /**
     * Wait for a string response from the server.
     * 
     * @return 
     */
    public synchronized String getStringResponse() {
        try {
            String line = this.input.readLine();
            
            if (line == null)
                return "";
            
            return line;
        } catch (IOException ex) {
            Logger.getLogger(ClientSpeak.class.getName()).log(Level.SEVERE, null, ex);
            return "";
        }
    }
    
    public boolean canBlock() {
        sendRawMessage("canBlock");
        
        return getBooleanResponse();
    }

    public boolean canPause() {
        sendRawMessage("canPause");
        
        return getBooleanResponse();
    }

    public boolean canResume() {
        sendRawMessage("canResume");
        
        return getBooleanResponse();
    }

    public boolean canStop() {
        sendRawMessage("canStop");
        
        return getBooleanResponse();
    }

    public boolean canSetVoice() {
        sendRawMessage("canSetVoice");
        
        return getBooleanResponse();
    }

    public boolean canSetVolume() {
        sendRawMessage("canSetVolume");
        
        return getBooleanResponse();
    }

    public boolean canSetSpeed() {
        sendRawMessage("canSetSpeed");
        
        return getBooleanResponse();
    }

    public boolean canSetPitch() {
        sendRawMessage("canSetPitch");
        
        return getBooleanResponse();
    }

    public Iterator<SpeechVoice> getAvailableVoices() {
        ArrayList<SpeechVoice> voices = new ArrayList<SpeechVoice>();
        
        sendRawMessage("getAvailableVoices");
        
        // Parse lines of input until we get a blank line.
        String line = "";
        do {
            try {
                line = this.input.readLine();
                SpeechVoice v = new SpeechVoice(line, SpeechLanguage.ENGLISH_US);
                voices.add(v);
            } catch (IOException ex) {
                Logger.getLogger(ClientSpeak.class.getName()).log(Level.SEVERE, null, ex);
                line = "";
            }
        } while (!line.isEmpty());
        
        return voices.iterator();
    }

    public SpeechVoice getCurrentVoice() {
        sendRawMessage("getCurrentVoice");
        
        String name = getStringResponse();
        
        return new SpeechVoice(name, SpeechLanguage.ENGLISH_US);
    }

    public double getSpeed() {
        sendRawMessage("getSpeed");
        
        return getDoubleResponse();
    }

    public TextToSpeechEngine getTextToSpeechEngine() {
        sendRawMessage("getTextToSpeechEngine");
        
        String name = getStringResponse();
        
        return TextToSpeechEngine.valueOf(name);
    }

    public boolean setTextToSpeechEngine(TextToSpeechEngine engine) {
        sendRawMessage("setTextToSpeechEngine:" + engine.toString());
        
        return getBooleanResponse();
    }

    public Iterator<TextToSpeechEngine> getAvailableEngines() {
        ArrayList<TextToSpeechEngine> engines = new ArrayList<TextToSpeechEngine>();
        
        sendRawMessage("getAvailableEngines");
        
        // Parse lines of input until we get a blank line.
        String line = "";
        do {
            try {
                line = this.input.readLine();
                if (!line.isEmpty())
                    engines.add(TextToSpeechEngine.valueOf(line));
            } catch (IOException ex) {
                Logger.getLogger(ClientSpeak.class.getName()).log(Level.SEVERE, null, ex);
                line = "";
            }
        } while (!line.isEmpty());
        
        return engines.iterator();
    }

    public double getVolume() {
        sendRawMessage("getVolume");
        
        return getDoubleResponse();
    }

    public double getPitch() {
        sendRawMessage("getPitch");
        
        return getDoubleResponse();
    }

    public boolean isSpeaking() {
        sendRawMessage("isSpeaking");
        
        return getBooleanResponse();
    }

    public boolean pause() {
        sendRawMessage("pause");
        
        return getBooleanResponse();
    }

    public void reinitialize() {
        sendRawMessage("reinitialize");
    }

    public boolean respeak() {
        sendRawMessage("respeak");
        
        return getBooleanResponse();
    }

    public boolean copyToClipboard() {
        return false; // not implemented, TODO?
    }

    public boolean resume() {
        sendRawMessage("resume");
        
        return getBooleanResponse();
    }

    public boolean setVolume(double vol) {
        if (vol < 0.0)
            vol = 0.0;
        else if (vol > 1.0)
            vol = 1.0;
        
        sendRawMessage("setVolume:" + vol);
        
        return getBooleanResponse();
    }

    public boolean setSpeed(double speed) {
        if (speed < 0.0)
            speed = 0.0;
        else if (speed > 1.0)
            speed = 1.0;
        
        sendRawMessage("setSpeed:" + speed);
        
        return getBooleanResponse();    
    }

    public boolean setPitch(double pitch) {
        if (pitch < 0.0)
            pitch = 0.0;
        else if (pitch > 1.0)
            pitch = 1.0;   

        sendRawMessage("setPitch:" + pitch);
        
        return getBooleanResponse();    
    }

    public boolean setVoice(SpeechVoice voice) {
        sendRawMessage("setVoice:" + voice.toString());
        
        return getBooleanResponse();    
    }

    public boolean speak(String text) {
        return speak(text, SpeechPriority.MEDIUM);
    }

    public boolean speak(String text, SpeechPriority priority) {
        return speak(text, priority, RequestType.TEXT);
    }

    public boolean speak(String text, SpeechPriority priority, RequestType type) {
        sendRawMessage("speak:" + priority.toString() + ":" + type.toString() + ":" + text);
        
        return true; // assume success
    }

    public boolean speak(char c) {
        return speak(c, SpeechPriority.MEDIUM);
    }

    public boolean speak(char c, SpeechPriority priority) {
        sendRawMessage("speak:" + priority.toString() + ":CHARACTER:" + c);
        
        return true; // assume success
    }

    public boolean speak(SpeechProcessor proc) {
        String text = proc.process();
        RequestType type = proc.getRequestType();
        SpeechPriority priority = proc.getPriority();
        
        return speak(text, priority, type);
    }

    public boolean speakBlocking(String text) {
        return speakBlocking(text, SpeechPriority.MEDIUM);
    }

    public boolean speakBlocking(String text, SpeechPriority priority) {
        return speakBlocking(text, priority, RequestType.TEXT);
    }

    public boolean speakBlocking(String text, SpeechPriority priority, RequestType type) {
        sendRawMessage("speakBlocking:" + priority.toString() + ":" + type.toString() + ":" + text);
        
        return getBooleanResponse();    
    }

    public boolean speakBlocking(char c) {
        return speakBlocking(c, SpeechPriority.MEDIUM);
    }

    public boolean speakBlocking(char c, SpeechPriority priority) {
        sendRawMessage("speakBlocking:" + priority.toString() + ":CHARACTER:" + c);
        
        return getBooleanResponse();   
    }

    public boolean stop() {
        sendRawMessage("stop");
        
        return getBooleanResponse();
    }

    public void setSpeechEnabled(boolean enabled) {
        sendRawMessage("setSpeechEnabled:" + enabled);        
    }

    public boolean isSpeechEnabled() {
        sendRawMessage("isSpeechEnabled");
        
        return getBooleanResponse();
    }
    
    public double getVersion() {
        sendRawMessage("getVersion");
        
        return getDoubleResponse();
    }
}
