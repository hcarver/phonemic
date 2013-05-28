/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package org.sodbeans.phonemic;

/**
 * Represents the operating systems supported.
 *
 * @author Jeff Wilson and Andreas Stefik
 */
public enum OperatingSystem {
    WINDOWS_XP,
    WINDOWS_VISTA,
    WINDOWS7,
    OTHER_WINDOWS,
    MAC_OSX,
    LINUX;

    /**
     * Returns the number of bits this operating system is, typically 32
     * or 64.
     *
     * @return the number of bits.
     */
    public static int getNumberBits() {
        String numBits = System.getProperty("sun.arch.data.model");
        return Integer.parseInt(numBits);
    }
    /*
     * Get the current operating system in use.
     *
     * @returns The current operating system
     */
    public static OperatingSystem getOS() {
        String os = System.getProperty("os.name");
        if(os.compareTo("Windows Vista") == 0) {
            return WINDOWS_VISTA;
        }
        else if(os.compareTo("Windows XP") == 0)
        {
            return WINDOWS_XP;
        }
        else if (os.compareTo("Windows 7") == 0) {
            return WINDOWS7;
        }
        else if (os.compareTo("Mac OS X") == 0) {
            return MAC_OSX;
        }
        else if (os.compareTo("Linux") == 0) {
            // Turns out on Linux, it's just "Linux." -- cool
            return LINUX;
        }
        else if (os.toLowerCase().contains("win")){
            // This library doesn't actually distinguish between the different
            // versions of Windows in the way it works, so we may as well attempt
            // to handle any other Windows version in the same way.
            return OTHER_WINDOWS;
        }
        else {
            return null;
        }
    }
}
