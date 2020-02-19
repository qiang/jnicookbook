#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include "recipeNo074_ErrnoGenerator.h"

JNIEXPORT jstring JNICALL Java_recipeNo074_ErrnoGenerator_generateErrno
  (JNIEnv * env, jobject obj, jboolean shouldIFail, jobject errnoValue, jobject errnoString) {

  // Heavily oversized buffer for errno description
  char errorBuffer[1024];

  FILE *fp = NULL;

  // Setting errno to 0 - before system call
  // e.g.: The CERT ® C Coding Standard: 98 Rules for Developing Safe, Reliable, and Secure Systems, Second Edition
  // advises to do so
  errno = 0;

  // This one will most probably fail
  if(shouldIFail) {
    fp = fopen("/this_will_fail", "r");
  } else {
    fp = fopen("/tmp/some-file", "w");
  }

  // We will return null, in case we have failed
  if( fp == NULL ) {
    // In addition to passing null back to Java, I am filling errnoString and errnoValue
    sprintf( errorBuffer, "[C] %s: errno: %d message: %s", shouldIFail?"true ":"false", errno, strerror(errno) );
    printf("%s\n", errorBuffer);

    jclass atomicIntegerCls  = (*env)->GetObjectClass (env, errnoValue);
    jmethodID setMethodID    = (*env)->GetMethodID(env, atomicIntegerCls, "set", "(I)V");

    jclass stringBuilderCls  = (*env)->GetObjectClass (env, errnoString);
    jmethodID appendMethodID = (*env)->GetMethodID(env, stringBuilderCls, "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");

    (*env)->CallVoidMethod(env,   errnoValue,  setMethodID,    errno);
    (*env)->CallObjectMethod(env, errnoString, appendMethodID, (*env)->NewStringUTF( env, strerror(errno) ) );

    return NULL;
   
  } else {
    sprintf( errorBuffer, "[C] %s: everything is fine - don't check errno", shouldIFail?"true ":"false" );
    printf("%s\n", errorBuffer);

    return (*env)->NewStringUTF( env, "Hello from JNI!");
  }

}

