#include "../include/Reccordion.h"

#include "../../../../../include/Reccordion/Reccordion.h"

/*
 * Class:     Reccordion
 * Method:    ReccordionInitialize
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_Reccordion_ReccordionInitialize(JNIEnv*, jclass)
{
    return ReccordionInitialize();
}

/*
 * Class:     Reccordion
 * Method:    ReccordionDeinitialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_Reccordion_ReccordionDeinitialize(JNIEnv*, jclass)
{
    ReccordionDeinitialize();
}

/*
 * Class:     Reccordion
 * Method:    ReccordionEnableLogRedirection
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_Reccordion_ReccordionEnableLogRedirection(JNIEnv*, jclass)
{
    ReccordionEnableLogRedirection();
}

/*
 * Class:     Reccordion
 * Method:    ReccordionDisableLogRedirection
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_Reccordion_ReccordionDisableLogRedirection(JNIEnv*, jclass)
{
    ReccordionDisableLogRedirection();
}

/*
 * Class:     Reccordion
 * Method:    ReccordionGetScreenWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_Reccordion_ReccordionGetScreenWidth(JNIEnv*, jclass)
{
    return ReccordionGetScreenWidth();
}

/*
 * Class:     Reccordion
 * Method:    ReccordionGetScreenHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_Reccordion_ReccordionGetScreenHeight(JNIEnv*, jclass)
{
    return ReccordionGetScreenHeight();
}

/*
 * Class:     Reccordion
 * Method:    ReccordionStartRecording
 * Signature: (Ljava/lang/String;Ljava/lang/String;IIII)V
 */
JNIEXPORT void JNICALL
Java_Reccordion_ReccordionStartRecording(JNIEnv* env,
                                         jclass  cl,
                                         jstring directory,
                                         jstring name,
                                         jint    x,
                                         jint    y,
                                         jint    width,
                                         jint    height)
{
    const char* nativeDirectoryString = env->GetStringUTFChars(directory, 0);
    const char* nativeNameString      = env->GetStringUTFChars(name, 0);
    ReccordionStartRecording(nativeDirectoryString, nativeNameString, x, y, width, height);
    env->ReleaseStringUTFChars(name, nativeNameString);
    env->ReleaseStringUTFChars(directory, nativeDirectoryString);
}

/*
 * Class:     Reccordion
 * Method:    ReccordionStopRecording
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_Reccordion_ReccordionStopRecording(JNIEnv*, jclass)
{
    ReccordionStopRecording();
}