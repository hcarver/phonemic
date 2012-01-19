= Phonemic version 1.2, January 19, 2012 =

== Introduction ==
This is the second official release of the Phonemic speech library. This update primarily consists of minor bug fixes and added support for the PowerPC
architecture on Mac OS X systems.

== Bug Fixes ==
This release fixes various minor bugs found in the last Phonemic release, 1.0. The most critical bug fixed would cause Phonemic to hang when an application closed,
causing the application to remain open until closed manually by the user.

== Added Support ==
This release now includes support for the PowerPC architecture on Mac OS X systems. A new library file is now included in the JNI folder, called "libCarbonSpeakJNI_PPC.jnilib".

== Build Notes ==
For notes on modifying included libraries, please see the 1.0 release notes. However, should you desire to modify the PowerPC libraries included with Phonemic,
XCode 3 (3.2.1 preferred) must be used, with the "Active Architecture" setting on "ppc." Note that the same codebase is used for both the Intel and PowerPC
libraries.

== Further Information ==
For more information on setting up Phonemic,  please see the release notes for the 1.0 release.