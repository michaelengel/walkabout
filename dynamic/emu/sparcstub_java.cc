/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * File:       sparcstub_java.cc
 * OVERVIEW:   Sparc Java language stub methods
 *============================================================================*/

/*
 * $Revision: 1.3 $
 * 18 Jun 01 - Nathan: File added to cvs
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "personality.h"

jobject core;
JNIEnv *env;
jmethodID runMethod;
jmethodID signalCallbackID;
jfieldID regsID, rdID, pcID, npcID, cfID, icountID, debugID, memID;

extern char **environ;
int debug = 0;

#ifdef INVOCATION
extern
#endif
Personality *persona;

bool initVM( );

int getArchitecture()
{
    return SPARC;
}

personality_t getDefaultPersona()
{
    return PER_SOLARIS;
}

JNIEXPORT jint JNICALL doSparcTrapJava( JNIEnv *iEnv, jobject obj, int trapno )
{
    env = iEnv;

//  New object reference needs to be assigned on Trap
    core = obj;

    if( trapno == 136 ) {
        jobject regs = env->GetObjectField( obj, regsID );
        jobject rd = env->GetObjectField( regs, rdID );
        jint *parms = env->GetIntArrayElements( (jintArray)rd, 0 );
        persona->handleSyscall( parms[1], (int *)parms+8 );
        env->ReleaseIntArrayElements( (jintArray)rd, parms, 0 );
    }
    return 0;
}

void setReg_rd( int n, int val )
{
    jobject regs = env->GetObjectField( core, regsID );
    jobject rd = env->GetObjectField( regs, rdID );
    if( rd == 0 ) {
        fprintf( stderr, "Unable to access regs.rd\n" );
        exit(1);
    }

    env->SetIntArrayRegion( (jintArray)rd, n, 1, (jint *) &val );
}

void setReg_pc( int val )
{
    jobject regs = env->GetObjectField( core, regsID );
    env->SetIntField( regs, pcID, val );
    env->SetIntField( regs, npcID, val );
}

void setReg_sp( int val )
{
    setReg_rd(14, val);
}

void setReg_CF( int val )
{
    jobject regs = env->GetObjectField( core, regsID );
    env->SetIntField( regs, cfID, val?1:0 );
}

void setMem( char *vm )
{
    jlong val = ((jlong)(unsigned int)vm)&0xFFFFFFFFL;
    env->SetLongField( core, memID, val );
}

void run( )
{
    env->SetBooleanField( core, debugID, debug );
    env->CallVoidMethod( core, runMethod );
}

void stop( int o0 )
{
    /* Brutal, but effective */
    jclass cls = env->FindClass("java/lang/System");
    jmethodID func = env->GetStaticMethodID( cls, "exit", "(I)V");
    env->CallStaticVoidMethod(cls, func, o0);
}

void printInstCount()
{
    long icount = env->GetLongField( core, icountID );
    fprintf( stderr, "Interpreted %ld instructions\n", icount );
}

void dumpMainRegisters(FILE *)
{
    /* FIXME: Stub this through later */
}

/* "Safe" versions of GetFieldID and GetMethodID */
jfieldID getField( jclass clz, char *name, char *type, char *msg )
{
    jfieldID var = env->GetFieldID( clz, name, type );
    if( var == 0 ) {
        fprintf( stderr, "Can't locate field %s, aborting\n", msg );
        exit(1);
    }
    return var;
}

jmethodID getMethod( jclass clz, char *name, char *type, char *msg )
{
    jmethodID var = env->GetMethodID( clz, name, type );
    if( var == 0 ) {
        fprintf( stderr, "Can't locate method %s, aborting\n", msg );
        exit(1);
    }
    return var;
}

/*
 * Find and save the identifiers of everything in the emu class
 * that we're actually interested in
 */
void loadEmuClass()
{
    jclass cls;

    cls = env->FindClass("sparcemu");
    if (cls == 0) {
        fprintf(stderr, "Can't find sparcemu class\n");
        exit(1);
    }
    jclass regCls = env->FindClass( "sparcemu$RegisterFile" );
    if (regCls == 0 ) {
        fprintf(stderr, "Can't find inner class sparcemu.RegisterFile\n" );
        exit(1);
    }

#ifdef INVOCATION
    jmethodID cons = getMethod( cls, "<init>", "(I)V", "sparcemu.sparcemu(int)");
    core = env->NewObject( cls, cons, 0 );
    if( core == 0 ) {
        fprintf( stderr, "Can't construct sparcemu object\n" );
        exit(1);
    }
#endif

    runMethod = getMethod( cls, "run", "()V", "sparcemu.run()" );
    regsID = getField( cls, "regs", "Lsparcemu$RegisterFile;", "sparcemu.regs" );
    rdID = getField( regCls, "rd", "[I", "sparcemu.regs.rd[]" );
    pcID = getField( regCls, "r_pc", "I", "sparcemu.regs.r_pc" );
    npcID = getField( regCls, "r_npc", "I", "sparcemu.regs.r_npc" );
    cfID = getField( regCls, "r_CF", "I", "sparcemu.regs.r_CF" );
    icountID = getField( cls, "icount", "J", "sparcemu.icount" );
    debugID = getField( cls, "debug", "Z", "sparcemu.debug" );
    memID = getField( cls, "mem", "J", "sparcemu.mem" );

    JNINativeMethod methods[] = { {"doTrap", "(I)I",
                                   doSparcTrapJava}};
    env->RegisterNatives( cls, methods, 1 );
}

#ifdef INVOCATION
/*
 * Initialize from the native side - invoke the JVM
 */
void initCore()
{
    JavaVM *jvm;
    JavaVMInitArgs vm_args;
    jint res;

    /* IMPORTANT: specify vm_args version # if you use JDK1.1.2 and beyond */
    memset( &vm_args, 0, sizeof(vm_args) );
    vm_args.version = JNI_VERSION_1_2;

    JNI_GetDefaultJavaVMInitArgs(&vm_args);

    /* Create the Java VM */
    res = JNI_CreateJavaVM(&jvm,(void **)&env,(void **)&vm_args);
    if (res < 0) {
        fprintf(stderr, "Can't create Java VM - error %d\n", res);
        exit(1);
    }
    loadEmuClass();
}

#else
/*
 * Initialize from the java side - JVM is already up so just load the class
 */
void initCore( JNIEnv *iEnv )
{
    env = iEnv;
    loadEmuClass();
}

#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL Java_sparcmain_initNative
    (JNIEnv *, jclass, jobjectArray, jint, jobject);
#ifdef __cplusplus
}
#endif


/*
 * Initialization function called from the java-side. Mostly stub.
 */
JNIEXPORT void JNICALL Java_sparcmain_initNative(
    JNIEnv * iEnv, jclass thisClass, jobjectArray jArgs, jint jArg, jobject inCore)
{
    initCore( iEnv ); 
    core = inCore;

    jboolean isCopy;
    jint sz = env->GetArrayLength(jArgs) - jArg;

    jobject returnObject;
    char **argv;

    argv = (char **)malloc (sizeof(char *) * sz);

    for (int i = 0; i < sz; i++) {
	argv[i] = (char *)env->GetStringUTFChars(
            (jstring)env->GetObjectArrayElement(jArgs, i+jArg), 0);
    }

    persona = Personality::getInstanceFor(PER_SOLARIS,SPARC);
    if( persona->execve( argv[0], (const char **)argv,
                         (const char **)environ ) == false ) {
        env->ThrowNew( env->FindClass("java/io/FileNotFoundException"),
                       "execve failed in native code" );
        /* Reachable? */
	return;
    }
    
    /* Release copied strings (slightly dubious as the array
    * could have changed in the meantime, but this shouldn't
    * be possible with the current java-side)
    */
    for( int i = jArg; i < sz; i++ ) {
        env->ReleaseStringUTFChars(
            (jstring)env->GetObjectArrayElement(jArgs, i), argv[i-jArg] );
    }
}

#endif /* INVOCATION */
