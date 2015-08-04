#pragma once

#include "main.hpp"  
#include <curl/curl.h>

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

// send caution data formed json
string sendToServerCaution(char* json)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char buf[] = "Expect:";

	string response_string;
	char* url = "http://210.125.96.109/upload.php";

	curl_global_init(CURL_GLOBAL_ALL);
	/* videoId */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* userId */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "userId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* videoDangerPoint */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoDangerPoint",
		CURLFORM_COPYCONTENTS, "70",
		CURLFORM_END);
	/* videoDate */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoDate",
		CURLFORM_COPYCONTENTS, "2015.08.02/03:01",
		CURLFORM_END);
	/* Reason1 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason1",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* Reason2 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason2",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* Reason3 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason3",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* Reason4 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason4",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* Reason5 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason5",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* visitorFace */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "visitorFace",
		CURLFORM_FILE, "C:\\Users\\Public\\Pictures\\Sample Pictures\\Chrysanthemum.jpg",
		CURLFORM_END);
	/* Fill in the file upload field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videofile",
		CURLFORM_FILE, "C:\\Users\\lec\\Desktop\\my\\p\\project_5_sgen_2015_summer_\\CCTV\\CCTV\\temp_origin.avi",
		CURLFORM_END); 

	/* Fill in the submit field too, even if this is rarely needed */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "submit",
		CURLFORM_COPYCONTENTS, "send",
		CURLFORM_END);

	curl = curl_easy_init();
	/* initialize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, buf);
	if (curl) {

		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		
		// to get response text.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string); 

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		cout << res << endl;

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}
	return response_string;
};

// send a video and face
string sendToServerVideo(char* video_path, char* face_path)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char buf[] = "Expect:";

	string response_string;
	char* url = "http://210.125.96.109/upload.php";

	curl_global_init(CURL_GLOBAL_ALL);
	/* videoId */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* userId */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "userId",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* videoDangerPoint */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoDangerPoint",
		CURLFORM_COPYCONTENTS, "70",
		CURLFORM_END);
	/* videoDate */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videoDate",
		CURLFORM_COPYCONTENTS, "2015.08.02/03:01",
		CURLFORM_END);
	/* Reason1 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason1",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* Reason2 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason2",
		CURLFORM_COPYCONTENTS, "1",
		CURLFORM_END);
	/* Reason3 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason3",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* Reason4 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason4",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* Reason5 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "Reason5",
		CURLFORM_COPYCONTENTS, "0",
		CURLFORM_END);
	/* visitorFace */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "visitorFace",
		CURLFORM_FILE, "C:\\Users\\Public\\Pictures\\Sample Pictures\\Chrysanthemum.jpg",
		CURLFORM_END);
	/* Fill in the file upload field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "videofile",
		CURLFORM_FILE, "C:\\Users\\lec\\Desktop\\my\\p\\project_5_sgen_2015_summer_\\CCTV\\CCTV\\temp_origin.avi",
		CURLFORM_END);

	/* Fill in the filename field */
	/*
	curl_formadd(&formpost,
	&lastptr,
	CURLFORM_COPYNAME, "filename",
	CURLFORM_COPYCONTENTS, "C:\\Users\\youjin\\hyj\\video\\test1.avi",
	CURLFORM_END);
	*/

	/* Fill in the submit field too, even if this is rarely needed */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "submit",
		CURLFORM_COPYCONTENTS, "send",
		CURLFORM_END);

	curl = curl_easy_init();
	/* initialize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, buf);
	if (curl) {

		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		// to get response text.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		cout << res << endl;

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}
	return response_string;
};
