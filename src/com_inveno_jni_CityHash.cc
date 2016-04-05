#include <iostream>
#include <string.h>
#include <cstdio>
#include "com_inveno_jni_CityHash.h"
#include "city.h"


/* c/c++ string turn to java jstring */
jstring charToJstring(JNIEnv** env, const char* pat)
{
	jclass     strClass = (*env)->FindClass("java/lang/String");
	jmethodID  ctorID   = (*env)->GetMethodID(strClass, "", "([BLjava/lang/String;)V");
	jbyteArray bytes    = (*env)->NewByteArray(strlen(pat));
	(*env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
	jstring    encoding = (*env)->NewStringUTF("UTF-8");
	return (jstring)(*env)->NewObject(strClass, ctorID, bytes, encoding);
}

/* java jstring turn to c/c++ char* */
char* jstringToChar(JNIEnv** env, jstring jstr)
{
    char* pStr = NULL;
    jclass     jstrObj   = (*env)->FindClass("java/lang/String");
    jstring    encode    = (*env)->NewStringUTF("utf-8");
    jmethodID  methodId  = (*env)->GetMethodID(jstrObj, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray byteArray = (jbyteArray)(*env)->CallObjectMethod(jstr, methodId, encode);
    jsize      strLen    = (*env)->GetArrayLength(byteArray);
    jbyte      *jBuf     = (*env)->GetByteArrayElements(byteArray, JNI_FALSE);
    if (jBuf > 0)
    {
        pStr = (char*)malloc(strLen + 1);
        if (!pStr)
        {
            return NULL;
        }
        memcpy(pStr, jBuf, strLen);
        pStr[strLen] = 0;
    }
    (*env)->ReleaseByteArrayElements(byteArray, jBuf, 0);
    return pStr;
}

JNIEXPORT jstring JNICALL Java_com_inveno_jni_CityHash_cityHash64(JNIEnv * env, jclass jobject, jstring srcString)
{
	char * src = jstringToChar(&env, srcString);
	std::string str(src);

	uint64_t encoded = CityHash64(str.c_str(), str.size());
	char str_encoded[128];
	sprintf(str_encoded, "%llu", encoded);
	if(src != NULL)
	{
		free(src);
		src = 0;
	}
	return (env)->NewStringUTF(str_encoded);
}
