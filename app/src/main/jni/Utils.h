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

/*
uint64_t get_tid() {
    pthread_t ptid = pthread_self();
    uint64_t threadId = 0;
    memcpy(&threadId, &ptid, sizeof(ptid));
    return threadId;
}
*/
//File * createFile(char *);

#endif //NDK_CURL_SAMPLE_MASTER_UTILS_H
