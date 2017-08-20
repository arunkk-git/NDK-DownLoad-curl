#include <string.h>
#include <jni.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <android/log.h> 
#include <Utils.h>

using namespace std;

string response;
jobject g_obj;
JNIEnv* g_env;
const char * fileName = "/sdcard/downloadFile.txt";
FILE* file;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {

	for (int c = 0; c<size*nmemb; c++) {
		response.push_back(buf[c]);
       	}

        fstream ifs;
          ifs.open (fileName, fstream::binary | fstream::in | fstream::out);

          if (ifs.is_open())
          {
           ifs<<response;
           ifs.close();
          }

	return size*nmemb;
}


extern "C" {

	JNIEXPORT jstring JNICALL 
	Java_fr_bmartel_android_curlndk_CurlActivity_getCurlResponse(JNIEnv *env, jobject thiz, jstring jUrl) {

        LOGE("getCurlResponse");
        const char* url = env->GetStringUTFChars(jUrl, 0);//jstring2string(env,jUrl);
		CURL* curl;


		curl_global_init(CURL_GLOBAL_ALL);

		curl=curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL,url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);

		curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return env->NewStringUTF(response.c_str());
	}

void callJavaApi(std::string s){

        JNIEnv *env =  g_env;
        jstring jstr1 = env->NewStringUTF(s.c_str());
         jclass clazz = (env)->FindClass( "fr/bmartel/android/curlndk/CurlActivity");
        jmethodID messageMe = (env)->GetMethodID( clazz, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
        jobject result = (env)->CallObjectMethod( g_obj, messageMe, jstr1);
}

jstring Java_fr_bmartel_android_curlndk_CurlActivity_getJniString( JNIEnv* env, jobject obj){
        g_env = env;
        g_obj = obj;
        string  msg = "ARUN KULKARNI from FUCNTION";
        callJavaApi("ARUN KULKARNI SEPERATE API");
        return (env)->NewStringUTF( "From -- NDK ");
    }
}