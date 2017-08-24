#include <string.h>
#include <jni.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <android/log.h> 
#include <Utils.h>
#include <sys/syscall.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <sched.h> 
#include <unistd.h> 

using namespace std;


string response;


static JavaVM *gJavaVM;
static jobject gObject;
static jclass gClass;
static JNIEnv* gEnv=0; 

std::string filePath = "/sdcard/";//downloadFile.txt";
//std::string filePath = "/sdcard/downloadFile.txt";
int dlBytes = 0,totalSize = 0;
bool isDone = false ;

#if 10
uint64_t get_tid() {
    pthread_t ptid = pthread_self();
    uint64_t threadId = 0;
    memcpy(&threadId, &ptid, std::min(sizeof(threadId), sizeof(ptid)));
    return threadId;
}
#endif
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
	for (int c = 0; c<size*nmemb; c++) {
		response.push_back(buf[c]);
       	}
	
        fstream ifs;
          ifs.open (filePath.c_str(), fstream::binary | fstream::in | fstream::out);
          if (ifs.is_open())
          {	
           ifs<<response;		   
           ifs.close();
          }else{
			  LOGE("FILED *** %s response = %s ",filePath.c_str() ,response.c_str());
		  }
		  
	return size*nmemb;
}

void callJavaApi1(std::string s){
		LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
		
	    JNIEnv * g_env;
    // double check it's all ok
	
    int getEnvStat = gJavaVM->GetEnv(reinterpret_cast<void**>(&g_env), JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        LOGE("GetEnv: not attached");
		
        if (gJavaVM->AttachCurrentThread(&g_env, NULL) != 0) {
            LOGE("Failed to attach");
        }
    } else if (getEnvStat == JNI_OK) {
        //
    } else if (getEnvStat == JNI_EVERSION) {
        LOGE("GetEnv: version not supported");
    }
	
        jstring jstr1 = gEnv->NewStringUTF(s.c_str());
        jclass gClass1 = (gEnv)->FindClass( "fr/bmartel/android/curlndk/CurlActivity");
        jmethodID messageMe = (gEnv)->GetMethodID( gClass1, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
        jobject result = (gEnv)->CallObjectMethod( gObject, messageMe, jstr1);
		(gEnv)->DeleteLocalRef(jstr1);
 

    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
    }

    //gJavaVM->DetachCurrentThread();
}

void callJavaApi(std::string s){
	LOGE("%s : %d ",__FUNCTION__,__LINE__);
//JNIEnv* env = g_env;

    JavaVM *javaVM = gJavaVM;
    JNIEnv *env;
    jint res = (javaVM)->GetEnv((void**)&env, JNI_VERSION_1_6);
    LOGE("%s : %d ",__FUNCTION__,__LINE__);
	if (res != JNI_OK) {
        res = (javaVM)->AttachCurrentThread(&env, NULL);
		LOGE("%s : %d ",__FUNCTION__,__LINE__);
        if (JNI_OK != res) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return ;
        }
		else
		    LOGE("AttachCurrentThread %s : %d ",__FUNCTION__,__LINE__);

    }
	else
		    LOGE("GetEnv %s : %d ",__FUNCTION__,__LINE__);

		
LOGE("%s : %d ",__FUNCTION__,__LINE__);
        jstring jstr1 = env->NewStringUTF(s.c_str());
		LOGE("%s : %d ",__FUNCTION__,__LINE__);

         jclass clazz = (env)->FindClass( "fr/bmartel/android/curlndk/CurlActivity");
		 LOGE("%s : %d ",__FUNCTION__,__LINE__);

        jmethodID messageMe = (env)->GetMethodID( clazz, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
		LOGE("%s : %d ",__FUNCTION__,__LINE__);

        jobject result = (env)->CallObjectMethod( gObject, messageMe, jstr1);
		LOGE("%s : %d ",__FUNCTION__,__LINE__);

		(env)->DeleteLocalRef(jstr1);
		LOGE("%s : %d ",__FUNCTION__,__LINE__);

		
		

}
int my_progress_func(void *bar, double dltotal,double dlnow,double ultotal,double ulnow)
{
	LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
    LOGE(" Progress %.2f / %.2f : %f / %f\n", dlnow, dltotal, ultotal, ulnow);
	isDone = false;
	dlBytes = dlnow;
	totalSize = dltotal;
	if (dlBytes >= totalSize)
		isDone = true ;
	
// The C++03 way:
std::ostringstream sstream;
sstream << dlnow <<" / " <<dltotal;
std::string Progress = sstream.str();
if(dlnow && dltotal  &&  (dlnow == dltotal))
{
	isDone = true;
	LOGE(" DOWNLOAD COMPLETED ");
	//callJavaApi("  DOWNLOAD COMPLETED");
}

return 0;
}

string curl_downloadFile( string Url){
        CURLcode res;
        long filetime = -1;
        double filesize = 0.0;

		const char* url = Url.c_str();
		
		std::string filename = Url.substr( Url.find_last_of("/") + 1 );	
		filePath.append(filename);
        FILE* file = fopen(filePath.c_str(),"w+");
		LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
		
		LOGE("%s : URL = %s FILE [  %s -- %s] ",__FUNCTION__,url,filePath.c_str(),filename.c_str());
		

        CURL* curl;
		dlBytes = 0,totalSize = 0, isDone = false;
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
void
Java_fr_bmartel_android_curlndk_CurlActivity_init( JNIEnv* env, jobject thiz )
{
	LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
	int status;
	int isAttached = 0;
	gEnv = env ;
	gObject = (jobject)(env)->NewGlobalRef( thiz);
	jclass clazz = (env)->GetObjectClass( thiz);
	gClass = (jclass)(env)->NewGlobalRef( clazz);
	
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
        const char* url = env->GetStringUTFChars(jUrl, 0);//jstring2string(env,jUrl);
		return env->NewStringUTF(curl_downloadFile(url).c_str());
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
        string  msg = "Calling JAVA From Native code";
		LOGE(" %s tid() - %lu",__FUNCTION__,get_tid());
        callJavaApi(msg);
        return (env)->NewStringUTF( "From -- NDK ");
}
	
jint Java_fr_bmartel_android_curlndk_CurlActivity_getdownloadProgress( JNIEnv* env, jobject obj){
	LOGE("Progress = %d ",dlBytes);			
        return (jint)dlBytes;
}

jint Java_fr_bmartel_android_curlndk_CurlActivity_getTotalFileSize( JNIEnv* env, jobject obj){
	LOGE("TotalFileSize = %d ",totalSize);			
        return (jint)totalSize;
}
		
jboolean Java_fr_bmartel_android_curlndk_CurlActivity_getisDownLoaded( JNIEnv* env, jobject obj){	
		LOGE("isDownLoaded = %d ",isDone);			
        return (jboolean)isDone;
    }
}