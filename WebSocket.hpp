#pragma once

#include "main.hpp"  
#include <curl/curl.h>

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

char* getNow() {
	time_t t = time(0);
	tm t2 = *localtime(&t);
	char now[80];
	strftime(now, sizeof(now), "%Y %m %d %r", &t2);
	return now;
}

// send caution data formed json
string sendToServerCaution(const char* json)
{
	// check it's called
	cout << "send caution : " << json << endl;

	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL; 

	string response_string;
	char* url = "http://210.125.96.109/jsontest2.php";

	curl_global_init(CURL_GLOBAL_ALL); 
	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, "userId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "jsondata",
		CURLFORM_COPYCONTENTS, json,
		CURLFORM_END);

	curl = curl_easy_init(); 
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string); 
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			cout << "sendToCauntion Error" << endl;

		curl_easy_cleanup(curl);
		curl_formfree(formpost);
		curl_slist_free_all(headerlist);
	}
	return response_string;
};

// send a video and face
string sendToServerVideo(const char* video_path)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL; 

	string response_string;
	char* url = "http://210.125.96.109/upload.php";

	curl_global_init(CURL_GLOBAL_ALL);  
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "userId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);  
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoDate",
		CURLFORM_COPYCONTENTS, getNow(),
		CURLFORM_END); 
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason",
		CURLFORM_COPYCONTENTS, "양손을 머리위로 든다",
		CURLFORM_END);   
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videofile",
		CURLFORM_FILE, video_path,
		CURLFORM_END); 

	curl = curl_easy_init(); 
	if (curl) { 
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost); 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string); 
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			cout << "videoUpload has occured an error" << endl;  

		curl_easy_cleanup(curl);
		curl_formfree(formpost); 
		curl_slist_free_all(headerlist);
	}
	return response_string;
};
