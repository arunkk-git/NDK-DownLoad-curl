//
// Created by ananth on 8/19/2017.
//

#ifndef NDK_CURL_SAMPLE_MASTER_UTILS_H
#define NDK_CURL_SAMPLE_MASTER_UTILS_H

#include <string.h>
#include <jni.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <android/log.h>

#define  LOG_TAG    "ARUNKKK"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


std::string jstring2string(JNIEnv *env, jstring jStr) ;

//File * createFile(char *);

#endif //NDK_CURL_SAMPLE_MASTER_UTILS_H
