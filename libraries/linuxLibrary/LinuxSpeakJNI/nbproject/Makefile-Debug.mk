#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/linuxSpeak.o \
	${OBJECTDIR}/LinuxSpeakJNI.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-shared -ldl -lspeechd -pthread
CXXFLAGS=-shared -ldl -lspeechd -pthread

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/libLinuxSpeakJNI.so

dist/libLinuxSpeakJNI.so: ${OBJECTFILES}
	${MKDIR} -p dist
	${LINK.cc} -shared -o dist/libLinuxSpeakJNI.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/linuxSpeak.o: linuxSpeak.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/lib/jvm/java-6-openjdk/include/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/linuxSpeak.o linuxSpeak.cpp

${OBJECTDIR}/LinuxSpeakJNI.o: LinuxSpeakJNI.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/lib/jvm/java-6-openjdk/include/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/LinuxSpeakJNI.o LinuxSpeakJNI.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/libLinuxSpeakJNI.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
