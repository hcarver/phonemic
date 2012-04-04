/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.sodbeans.phonemic.daemon;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * A handler for Phonemic clients.
 * @author jeff
 */
public class ClientHandler extends Thread {
    /**
     * Our connection to the client.
     */
    private Socket client = null;
    
    public ClientHandler(Socket client) {
        this.client = client;
    }
    
    /**
     * The main loop.
     */
    @Override
    public void run() {
        // The client's input stream.
        InputStream input = null;
        OutputStream output = null;
        
        // The recognizer we will use.
        ProtocolRecognizer recognizer = null;
        
        try {
            // Get an input and output stream from the client.
            input = this.client.getInputStream();
            output = this.client.getOutputStream();
        } catch (IOException ex) {
            Logger.getLogger(ClientHandler.class.getName()).log(Level.SEVERE, null, ex);
            return; // failed to get an input or output stream.
        }
        
        // Version will be between 0x00000000 and 0xFFFFFFFF.
        int version = 0;
        
        try {
            version = input.read();
            version = version << 8;
            version |= input.read();
        } catch (IOException ex) {
            Logger.getLogger(ClientHandler.class.getName()).log(Level.SEVERE, null, ex);
            return; // failed to read first two bytes.
        }
        
        // Select an appropriate recognizer.
        if (version == 0x0001) {
            recognizer = new V1Recognizer();
            try {
                output.write("OK\n".getBytes());
            } catch (IOException ex) {
                Logger.getLogger(ClientHandler.class.getName()).log(Level.SEVERE, null, ex);
                return; // failed to write to client.
            }
        } else {
            // Inappropriate version flag. Terminate connection.
            try {
                this.client.close();
            } catch (IOException ex) {
                Logger.getLogger(ClientHandler.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            return;
        }
        
        // Read input from the client, pass to recognizer.
        while (true) {
            // Get the message size. will be between 0x00000000 to 0xFFFFFFFF
            int messageSize = 0;
            try {
                messageSize = input.read();
                messageSize = messageSize << 8;
                messageSize |= input.read();
            } catch (IOException ex) {
                Logger.getLogger(ClientHandler.class.getName()).log(Level.SEVERE, null, ex);
                
                // Couldn't read from client.
                return;
            }
            
            if (messageSize > 0) {
                byte[] data = new byte[messageSize];
                try {
                    input.read(data);
                    recognizer.parse(new String(data).toCharArray());
                } catch (IOException ex) {
                    Logger.getLogger(ClientHandler.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }
}