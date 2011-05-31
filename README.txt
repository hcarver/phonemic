Phonemic version 1.0, June 1, 2011
==================================

Introduction
=============
This is the first official release of the Phonemic speech library. Phonemic is a Java library that allows its users to write cross-platform speaking applications.

Big Features
=============
* Support for many major speech systems, including Microsoft SAPI, JAWS, NVDA, VoiceOver and ORCA.
* Provides a single API to communicate with all engines.
* Support for various types of speech modifications (when available):
	* Volume control
	* Pitch control (either manually or automatically through the speakChar functions).
	* Speed control
* Speech modifications are consistent across engines.
* Support for blocking speech calls (when available).
* Ability to query an engine for its features.
* Ability to switch engines on the fly (Windows and Mac OS X only).
* A queuing system that enables the use of speech priorities, even when not supported natively by the engine.
* A speech processing mechanism to enable advanced pre-processing of speech before it is sent to an engine.
* Ability to query for / change voice when supported by the engine.

Supported Engines
==================
Phonemic currently supports the following platforms and speech engines:

* Windows XP, Windows Vista and Windows 7:
	* Microsoft Speech API (SAPI)
	* JAWS
	* NVDA
* Mac OS X
	* Carbon (on Intel Macs only), the same speech interface used by VoiceOver.
	* The `say' command
* Linux
	* speech-dispatcher (available by default on Vinux and Ubuntu), the same speech interface used by ORCA.

Usage
======
The Phonemic library is very simple to use. Simply include the `phonemic.jar' file in your application, along with the required libraries (located under phonemic/jni). The JNI libraries may be placed anywhere you wish, but you must set the java.library.path variable correctly, either in your IDE or manually as a command to java.

The libraries are as follows. Note that if you do not intend to support a particular platform, you do not have to include its libraries in your distribution.

* Windows XP (32-bit only), Windows Vista and Windows 7
	* Interop.SpeechLib.5.3.dll (SAPI supporting DLLs)
	* SappyJNI.dll (SAPI, JAWS and NVDA support, 32-bit)
	* SappyJNI64.dll (SAPI, JAWS and NVDA support, 64-bit)
	* nvdaControllerClient[32|64].dll (NVDA support)
* Mac OS X
	* libCarbonSpeakJNI.jnilib (Carbon support)
* Linux
	* libLinuxSpeakJNI.so (speech-dispatcher support)

A simple demo application is included as a Netbeans project under testApp/.

Build Notes
============
Although libraries come with Phonemic pre-built, should you want to modify any of them, it is necessary to use the following systems.

* Windows
	* To modify the Windows DLL (SappyJNI/SappyJNI64.dll), Visual Studio 2008 must be used.
* Mac OS X
	* To modify the Carbon speech library, libCarbonSpeakJNI.jnilib, you must use XCode 3. (3.2.1 preferred)
* Linux
	* The Linux library (libLinuxSpeakJNI.so) can be loaded as a standard Netbeans project.

The source code for all libraries is included under libraries/. Pre-built libraries can be found under phonemic/jni.