#include <string.h>
#include <jni.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <android/log.h> 
#include <Utils.h>

using namespace std;

string response;
const char * fileName = "/sdcard/downloadFile.txt";
FILE* file;
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {


	for (int c = 0; c<size*nmemb; c++) {
		response.push_back(buf[c]);
       	}

/*    std::ofstream file1(fileName);
        std::string my_string = "Hello text in file\n";
        file1 << response;
*/

        fstream ifs;
          ifs.open (fileName, fstream::binary | fstream::in | fstream::out);

          if (ifs.is_open())
          {
        //   ifs.write("string to binary", strlen("string to binary"));
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

		//FILE* file = fopen("/sdcard/helloARUN.txt","w+");
            file = fopen(fileName,"w+");
            if (file != NULL)
            {
            LOGE("file is created successfully ....");
                fputs("HELLO WORLD!\n", file);
                fflush(file);
            //    fclose(file);
            }

		
		curl_global_init(CURL_GLOBAL_ALL);

		curl=curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL,url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);

		curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return env->NewStringUTF(response.c_str());
	}
}