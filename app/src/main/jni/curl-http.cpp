#include <string.h>
#include <jni.h>
#include <sstream>
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


void callJavaApi(std::string s){

        JNIEnv *env =  g_env;
        jstring jstr1 = env->NewStringUTF(s.c_str());
         jclass clazz = (env)->FindClass( "fr/bmartel/android/curlndk/CurlActivity");
        jmethodID messageMe = (env)->GetMethodID( clazz, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
        jobject result = (env)->CallObjectMethod( g_obj, messageMe, jstr1);
}

int my_progress_func(void *bar,
                     double t, /* dltotal */
                     double d, /* dlnow */
                     double ultotal,
                     double ulnow)
{
    LOGE("%f / %f : %f / %f\n", d, t, ultotal, ulnow);
    // The C++11 way:
 //   std::string down = std::to_string(d);

// The C++03 way:
std::ostringstream sstream;
sstream << d;
std::string down = sstream.str();

//callJavaApi(down);

  return 0;
}

extern "C" {

	JNIEXPORT jstring JNICALL 
	Java_fr_bmartel_android_curlndk_CurlActivity_getCurlResponse(JNIEnv *env, jobject thiz, jstring jUrl) {

        CURLcode res;
        long filetime = -1;
        double filesize = 0.0;


        LOGE("getCurlResponse");
        const char* url = env->GetStringUTFChars(jUrl, 0);//jstring2string(env,jUrl);
		CURL* curl;
        const char *filename = strrchr(url, '/') + 1;

		curl_global_init(CURL_GLOBAL_ALL);

		curl=curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL,url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);

		/* Switch on full protocol/debug output */

            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);


		curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		curl_global_cleanup();

LOGE("fileSize = %f  fileName = %s ",filesize, filename);
		return env->NewStringUTF(response.c_str());
	}



jstring Java_fr_bmartel_android_curlndk_CurlActivity_getJniString( JNIEnv* env, jobject obj){
        g_env = env;
        g_obj = obj;
        string  msg = "ARUN KULKARNI from FUCNTION";
        callJavaApi("ARUN KULKARNI SEPERATE API");
        return (env)->NewStringUTF( "From -- NDK ");
    }
}