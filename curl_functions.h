#pragma once

const cJSON* getEventUUID( char *event_uuid_link ) {

	CURL* curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;
	const cJSON* uuid;

	chunk.memory = malloc(1);  /* grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, event_uuid_link);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

	/* some servers do not like requests that are made without a user-agent
	   field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	res = curl_easy_perform(curl_handle);
		/* check for errors */
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		return NULL;
	}
	else {
		cJSON* json = cJSON_Parse(chunk.memory);

		if (json == NULL) {
			printf("error parsing JSON\n");
			curl_easy_cleanup(curl_handle);
			free(chunk.memory);
			return NULL;
		}
		else {
			uuid = cJSON_GetObjectItemCaseSensitive(json, "uuid");
			curl_easy_cleanup(curl_handle);
			free(chunk.memory);
			return uuid;
		}
	}
}

char* getAuthToken( char* auth_link ) {

	CURL* curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;
	const cJSON* auth_token;

	chunk.memory = malloc(1);  /* grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, auth_link);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

	///* some servers do not like requests that are made without a user-agent
	//   field, so we provide one */
	int user_agent_switch = rand() % (10 + 1 - 0) + 0;
	switch (user_agent_switch) {
	case(1):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/37.0.2062.94 Chrome/37.0.2062.94 Safari/537.36");
		break;
	case(2):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36");
		break;
	case(3):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko");
		break;
	case(4):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0");
		break;
	case(5):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_5) AppleWebKit/600.8.9 (KHTML, like Gecko) Version/8.0.8 Safari/600.8.9");
		break;
	case(6):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (iPad; CPU OS 8_4_1 like Mac OS X) AppleWebKit/600.1.4 (KHTML, like Gecko) Version/8.0 Mobile/12H321 Safari/600.1.4");
		break;
	case(7):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36");
		break;
	case(8):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36");
		break;
	case(9):
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36 Edge/12.10240");
		break;
	default:
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0");
	}
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, "{\"initialAppViewer\":\"browser--other\",\"granted_consents\":[\"StoreEssentialCookies\"]}");

	/* get it! */
	res = curl_easy_perform(curl_handle);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		return "curl_easy_perform() failed: %s";
	}
	else {
		cJSON* json = cJSON_Parse(chunk.memory);

		if (json == NULL) {
			curl_easy_cleanup(curl_handle);
			free(chunk.memory);
			return "error parsing JSON\n";
		}
		else {
			auth_token = cJSON_GetObjectItemCaseSensitive(json, "access_token");
			curl_easy_cleanup(curl_handle);
			free(chunk.memory);
			if (auth_token == NULL)
				return "captcha error";
			return auth_token->valuestring;
		}
	}
}

const cJSON* getUUIDsWithAuth(char* poll_summary_link, char* auth_token) {

	char auth_header[67] = "Authorization: Bearer ";
	strcat(auth_header, auth_token);
	CURL* curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;
	const cJSON* uuid;

	chunk.memory = malloc(1);
	chunk.size = 0;


	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, poll_summary_link);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, auth_header);
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		curl_easy_cleanup(curl_handle);
		return NULL;
	}
	else {
		curl_easy_cleanup(curl_handle);
		return cJSON_Parse(chunk.memory);
	}

}

void clapSlidoCheeks(char* e_uuid_preallocated,
					 char* active_poll_uuid, 
					 char* auth_token, 
					 char* hash, 
					 char* feedback_question_uuid, 
					 char* feedback_question_option_uuid) {

	struct MemoryStruct chunk;
	chunk.memory = malloc(1);  /* grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */


	/* prepare neccesary header strings */
	char auth_header[67] = "authorization: Bearer ";
	strcat(auth_header, auth_token);

	char vote_link[124];
	strcpy(vote_link, e_uuid_preallocated);
	strcat(vote_link, "/polls/");
	strcat(vote_link, active_poll_uuid);
	strcat(vote_link, "/vote");

	char referer[68] = "referer: https://app.sli.do/event/";
	strcat(referer, hash);
	strcat(referer, "/live/polls");

	char clientIDHeader[29] = "x-client-id: fH4mJ9RkX7sQ3pW";


	/* postfield data */
	char post_data[249] = "{\"feedback_uuid\":\"";
	strcat(post_data, active_poll_uuid);
	strcat(post_data, "\",\"votings\": [{\"feedback_question_uuid\":\"");
	strcat(post_data, feedback_question_option_uuid);
	strcat(post_data, "\",\"feedback_question_option_uuid\":\"");
	strcat(post_data, feedback_question_uuid);
	strcat(post_data, "\",\"is_anonymous\":true}],\"is_anonymous\":true}");

	/* curl request */
	CURL* curl_handle;
	CURLcode res;

	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, vote_link);

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "accept: application/json, text/plain, */*");
	headers = curl_slist_append(headers, "accept-language: en-GB,en;q=0.6");
	headers = curl_slist_append(headers, auth_header);
	headers = curl_slist_append(headers, "content-type: application/json");
	headers = curl_slist_append(headers, "origin: https://app.sli.do");
	headers = curl_slist_append(headers, referer);
	headers = curl_slist_append(headers, "sec-ch-ua: \"Brave\";v=\"123\", \"Not:A - Brand\";v=\"8\", \"Chromium\";v=\"123\"");
	headers = curl_slist_append(headers, "sec-ch-ua-mobile: ?0");
	headers = curl_slist_append(headers, "sec-ch-ua-platform: \"Windows\"");
	headers = curl_slist_append(headers, "sec-fetch-dest: empty");
	headers = curl_slist_append(headers, "sec-fetch-mode: cors");
	headers = curl_slist_append(headers, "sec-fetch-site: same-origin");
	headers = curl_slist_append(headers, "sec-gpc: 1");
	//headers = curl_slist_append(headers, "user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36");
	headers = curl_slist_append(headers, clientIDHeader);
	headers = curl_slist_append(headers, "x-slidoapp-version: SlidoParticipantApp/53.200.4 (web)");
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post_data);

	res = curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);
}
