/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.sodbeans.phonemic.daemon;

import java.net.Socket;

/**
 * The implementation of the Phonemic 1.0 protocol.
 * 
 * @author jeff
 */
public class V1Recognizer extends ProtocolRecognizer {
    @Override
    public void parse(char[] data) {
        System.out.println("Recvd message: " + String.valueOf(data));
    }

}
