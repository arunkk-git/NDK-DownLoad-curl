#include <string.h>
#include <jni.h>
#include <sstream>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <android/log.h> 
#include <Utils.h>

using namespace std;

// processing callback to handler class
typedef struct context {
    JavaVM  *javaVM;
    jclass   jniHelperClz;
    jobject  jniHelperObj;
    jclass   mainActivityClz;
    jobject  mainActivityObj;
    pthread_mutex_t  lock;
    int      done;
} Context;
Context g_ctx;

string response;
jobject g_obj;
JNIEnv* g_env;
std::string filePath = "/sdcard/";//downloadFile.txt";

//FILE* file;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
LOGE("%s : %d ",__FUNCTION__,__LINE__);
	for (int c = 0; c<size*nmemb; c++) {
		response.push_back(buf[c]);
       	}
        fstream ifs;
          ifs.open (filePath.c_str(), fstream::binary | fstream::in | fstream::out);
          if (ifs.is_open())
          {
           ifs<<response;
           ifs.close();
          }
	return size*nmemb;
}

void callJavaApi(std::string s){
	LOGE("%s : %d ",__FUNCTION__,__LINE__);
//JNIEnv* env = g_env;
 Context pctx =  g_ctx;
    JavaVM *javaVM = pctx.javaVM;
    JNIEnv *env;
    jint res = (javaVM)->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = (javaVM)->AttachCurrentThread(&env, NULL);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return ;
        }
    }

        jstring jstr1 = env->NewStringUTF(s.c_str());
         jclass clazz = (env)->FindClass( "fr/bmartel/android/curlndk/CurlActivity");
        jmethodID messageMe = (env)->GetMethodID( clazz, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
        jobject result = (env)->CallObjectMethod( g_obj, messageMe, jstr1);
		(env)->DeleteLocalRef(jstr1);
		
		

}
int my_progress_func(void *bar,
                     double t, /* dltotal */
                     double d, /* dlnow */
                     double ultotal,
                     double ulnow)
{
	LOGE("%s : %d ",__FUNCTION__,__LINE__);
    LOGE("my_progress_func %f / %f : %f / %f\n", d, t, ultotal, ulnow);
    // The C++11 way:
 //   std::string down = std::to_string(d);

// The C++03 way:
std::ostringstream sstream;
sstream << d;
std::string down = sstream.str();
//callJavaApi(down);

  return 0;
}

string curl_downloadFile( string Url){
        CURLcode res;
        long filetime = -1;
        double filesize = 0.0;

		const char* url = Url.c_str();
		
		std::string filename = Url.substr( Url.find_last_of("/") + 1 );	
		filePath.append(filename);

		
		LOGE("%s : URL = %s FILE [  %s -- %s] ",__FUNCTION__,url,filePath.c_str(),filename.c_str());
		

        CURL* curl;
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
return response;
}

extern "C" {

	JNIEXPORT jstring JNICALL 
	Java_fr_bmartel_android_curlndk_CurlActivity_getCurlResponse(JNIEnv *env, jobject thiz, jstring jUrl) {
		LOGE("%s : %d ",__FUNCTION__,__LINE__);

        CURLcode res;
        long filetime = -1;
        double filesize = 0.0;
        LOGE("getCurlResponse");
        const char* url = env->GetStringUTFChars(jUrl, 0);//jstring2string(env,jUrl);
		return env->NewStringUTF(curl_downloadFile(url).c_str());
	}

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
		LOGE("%s : %d ",__FUNCTION__,__LINE__);
		JNIEnv* env;
		memset(&g_ctx, 0, sizeof(g_ctx));
	
		g_ctx.javaVM = vm;
		if ((vm)->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
			return JNI_ERR; // JNI version not supported.
		}
		
		g_ctx.done = 0;
		g_ctx.mainActivityObj = NULL;
		return  JNI_VERSION_1_6;
	}	

	jstring Java_fr_bmartel_android_curlndk_CurlActivity_getJniString( JNIEnv* env, jobject obj){
		LOGE("%s : %d ",__FUNCTION__,__LINE__);
        g_env = env;
        g_obj = obj;
        string  msg = "ARUN KULKARNI from FUCNTION";
		int i = 2;
        std::ostringstream sstream;
		while(i--){
		sstream << i;
			std::string down = sstream.str();
            callJavaApi(down);
		}
        return (env)->NewStringUTF( "From -- NDK ");
    }
}