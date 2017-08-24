/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <Utils.h>

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   hello-jni/app/src/main/java/com/example/hellojni/HelloJni.java
 */
static JavaVM *gJavaVM;
static jobject gObject;
static jclass gClass;
static JNIEnv* gEnv=0; 

void callJavaApi(char * s){
#if 1	
		LOGE(" %s tid() - %lu gEnv   %x - gJavaVM = %x",__FUNCTION__,get_tid(),gEnv ,gJavaVM);
		
	    JNIEnv * g_env =gEnv;
    // double check it's all ok
	
    int getEnvStat = (*gJavaVM)->GetEnv(gJavaVM,(void**)(&g_env), JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        LOGE("GetEnv: not attached %d",__LINE__);
		
        if ((*gJavaVM)->AttachCurrentThread(gJavaVM,&g_env, NULL) != 0) {
            LOGE("Failed to attach %d",__LINE__);
        }
    } else if (getEnvStat == JNI_OK) {
        LOGE("JNI_OK  %d g_env =%x",__LINE__,g_env);//
    } else if (getEnvStat == JNI_EVERSION) {
        LOGE("GetEnv: version not supported  %d",__LINE__);
    }
	
        jstring jstr1 = (*g_env)->NewStringUTF(g_env,s);
        jclass gClass1 = (*g_env)->FindClass(g_env, "fr/bmartel/android/curlndk/CurlActivity");
        jmethodID messageMe = (*g_env)->GetMethodID( g_env,gClass1, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
	    jobject result = (*g_env)->CallObjectMethod ( g_env, gObject, messageMe,jstr1); //
		(*g_env)->DeleteLocalRef(g_env,jstr1);
 
/*
    if ((*g_env)->ExceptionCheck(g_env)) {
        (*g_env)->ExceptionDescribe(g_env);
    }
  */
  //(*gJavaVM)->DetachCurrentThread(gJavaVM);
	
#endif
}
char *  curl_downloadFile( char * Url){
     LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
	 //for (int i = 0 ; i <10;i++)
callJavaApi("From seperate Thread ...")	 ;
return __FUNCTION__;
}

JNIEXPORT jstring JNICALL
Java_fr_bmartel_android_curlndk_CurlActivity_stringFromJNI( JNIEnv* env,jobject thiz ){
#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

    return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
}

void Java_fr_bmartel_android_curlndk_CurlActivity_init( JNIEnv* env, jobject thiz ){
	LOGE(" %s tid() - %lu JNIEnv = %x ",__FUNCTION__,get_tid() ,env );
	gEnv = env ;
	gObject = (*env)->NewGlobalRef(env, thiz);
	jclass clazz = (*env)->GetObjectClass(env, thiz);
	gClass = (jclass)(*env)->NewGlobalRef(env, clazz);
	
	if (!clazz) {
		LOGE("callback_handler: failed to get object Class");
		goto failure;
	}
failure:
	return;
}

JNIEXPORT jstring JNICALL 
Java_fr_bmartel_android_curlndk_CurlActivity_getCurlResponse(JNIEnv *env, jobject obj, jstring jUrl) {
		LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
        const char* url = (*env)->GetStringUTFChars(env,jUrl, 0);//jstring2string(env,jUrl);
		return (*env)->NewStringUTF(env,curl_downloadFile(url));
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void * reserved)  { 
LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
gJavaVM = vm; 
return JNI_VERSION_1_6; 
} 

void JNI_OnUnload(JavaVM *vm, void *reserved)  { 
gJavaVM = NULL; 
} 

jstring Java_fr_bmartel_android_curlndk_CurlActivity_getJniString( JNIEnv* env, jobject obj){	
        char *  msg = "Calling JAVA From Native code";
		LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
        callJavaApi(msg);
        return (*env)->NewStringUTF(env, "From -- NDK ");
}
