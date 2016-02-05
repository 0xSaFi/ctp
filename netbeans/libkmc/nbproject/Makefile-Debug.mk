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
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1ce05ac7/kmcapplication.o \
	${OBJECTDIR}/_ext/1ce05ac7/kmccalculatorfactory.o \
	${OBJECTDIR}/_ext/1ce05ac7/version.o \
	${OBJECTDIR}/_ext/1ce05ac7/version_nb.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibkmc.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibkmc.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibkmc.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibkmc.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibkmc.a

${OBJECTDIR}/_ext/1ce05ac7/kmcapplication.o: nbproject/Makefile-${CND_CONF}.mk ../../src/libkmc/kmcapplication.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1ce05ac7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../include/votca/kmc -I../../../include/votca/tools -I../../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1ce05ac7/kmcapplication.o ../../src/libkmc/kmcapplication.cc

${OBJECTDIR}/_ext/1ce05ac7/kmccalculatorfactory.o: nbproject/Makefile-${CND_CONF}.mk ../../src/libkmc/kmccalculatorfactory.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1ce05ac7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../include/votca/kmc -I../../../include/votca/tools -I../../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1ce05ac7/kmccalculatorfactory.o ../../src/libkmc/kmccalculatorfactory.cc

${OBJECTDIR}/_ext/1ce05ac7/version.o: nbproject/Makefile-${CND_CONF}.mk ../../src/libkmc/version.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1ce05ac7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../include/votca/kmc -I../../../include/votca/tools -I../../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1ce05ac7/version.o ../../src/libkmc/version.cc

${OBJECTDIR}/_ext/1ce05ac7/version_nb.o: nbproject/Makefile-${CND_CONF}.mk ../../src/libkmc/version_nb.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1ce05ac7
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../include/votca/kmc -I../../../include/votca/tools -I../../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1ce05ac7/version_nb.o ../../src/libkmc/version_nb.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibkmc.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
