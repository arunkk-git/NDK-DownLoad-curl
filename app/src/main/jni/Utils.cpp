//
// Created by ananth on 8/19/2017.
//
#include <Utils.h>
#include <string.h>
#include <jni.h>
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <android/log.h>

using namespace std;




//////////////////////////////
std::string jstring2string(JNIEnv *env, jstring jStr) {
    std::vector<char> charsCode;

    if (!jStr)
        return "";


    const jchar *chars = env->GetStringChars(jStr, NULL);
    jsize len = env->GetStringLength(jStr);
    jsize i;

    for( i=0;i<len; i++){
        int code = (int)chars[i];
        charsCode.push_back( code );
    }
    env->ReleaseStringChars(jStr, chars);

    return  std::string(charsCode.begin(), charsCode.end());
}
