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
import java.util.Iterator;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.sodbeans.phonemic.RequestType;
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
     * Send a raw message. TODO: Make private
     * @param msg 
     */
    public void sendRawMessage(String msg) {
        int length = msg.length();
        
        if (length > 65535) {
            length = 65535;
            msg = msg.substring(0, 0xFFFFE);
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
    
    public boolean canBlock() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean canPause() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean canResume() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean canStop() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean canSetVoice() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean canSetVolume() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean canSetSpeed() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean canSetPitch() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public Iterator<SpeechVoice> getAvailableVoices() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public SpeechVoice getCurrentVoice() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public double getSpeed() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public TextToSpeechEngine getTextToSpeechEngine() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean setTextToSpeechEngine(TextToSpeechEngine engine) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public Iterator<TextToSpeechEngine> getAvailableEngines() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public double getVolume() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public double getPitch() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean isSpeaking() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean pause() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void reinitialize() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean respeak() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean copyToClipboard() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean resume() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean setVolume(double vol) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean setSpeed(double speed) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean setPitch(double pitch) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean setVoice(SpeechVoice voice) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speak(String text) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speak(String text, SpeechPriority priority) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speak(String text, SpeechPriority priority, RequestType type) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speak(char c) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speak(char c, SpeechPriority priority) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speak(SpeechProcessor proc) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speakBlocking(String text) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speakBlocking(String text, SpeechPriority priority) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speakBlocking(String text, SpeechPriority priority, RequestType type) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speakBlocking(char c) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean speakBlocking(char c, SpeechPriority priority) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean stop() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void setSpeechEnabled(boolean enabled) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public boolean isSpeechEnabled() {
        throw new UnsupportedOperationException("Not supported yet.");
    }
}
