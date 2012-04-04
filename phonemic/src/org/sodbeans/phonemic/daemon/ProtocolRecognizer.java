/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.sodbeans.phonemic.daemon;

import java.net.Socket;

/**
 * A ProtocolRecognizer handles individual client requests.
 * 
 * @author jeff
 */
public abstract class ProtocolRecognizer {
    public abstract void parse(char[] data);
}
