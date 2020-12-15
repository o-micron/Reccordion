/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Reccordion */

#ifndef _Included_Reccordion
#define _Included_Reccordion
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Reccordion
 * Method:    ReccordionInitialize
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_Reccordion_ReccordionInitialize
  (JNIEnv *, jclass);

/*
 * Class:     Reccordion
 * Method:    ReccordionDeinitialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Reccordion_ReccordionDeinitialize
  (JNIEnv *, jclass);

/*
 * Class:     Reccordion
 * Method:    ReccordionEnableLogRedirection
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Reccordion_ReccordionEnableLogRedirection
  (JNIEnv *, jclass);

/*
 * Class:     Reccordion
 * Method:    ReccordionDisableLogRedirection
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Reccordion_ReccordionDisableLogRedirection
  (JNIEnv *, jclass);

/*
 * Class:     Reccordion
 * Method:    ReccordionGetScreenWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Reccordion_ReccordionGetScreenWidth
  (JNIEnv *, jclass);

/*
 * Class:     Reccordion
 * Method:    ReccordionGetScreenHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Reccordion_ReccordionGetScreenHeight
  (JNIEnv *, jclass);

/*
 * Class:     Reccordion
 * Method:    ReccordionStartRecording
 * Signature: (Ljava/lang/String;Ljava/lang/String;IIII)V
 */
JNIEXPORT void JNICALL Java_Reccordion_ReccordionStartRecording
  (JNIEnv *, jclass, jstring, jstring, jint, jint, jint, jint);

/*
 * Class:     Reccordion
 * Method:    ReccordionStopRecording
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Reccordion_ReccordionStopRecording
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif