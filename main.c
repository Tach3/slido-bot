#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <locale.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "helper_functions.h"
#include "curl_functions.h"


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "en_US.UTF-8");

	char* url = NULL;
	char* question_number = NULL;
	char* vote_number = NULL;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printHelp();
			exit(1);
		}
		else if (strcmp(argv[i], "-u") == 0) {
			if (i + 1 < argc) {
				url = argv[++i];
			}
			else {
				fprintf(stderr, "Error: -u option requires a URL argument\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-q") == 0) {
			if (i + 1 < argc) {
				question_number = argv[++i]; 
			}
			else {
				fprintf(stderr, "Error: -q option requires a question number argument\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-v") == 0) {
			if (i + 1 < argc) {
				vote_number = argv[++i]; 
			}
			else {
				fprintf(stderr, "Error: -v option requires a number of votes argument\n");
				return 1;
			}
		}
		else {
			fprintf(stderr, "Error: Invalid option %s\n", argv[i]);
			return 1;
		}
	}


	if (url) {
		printf("URL: %s\n", url);
	}
	if (question_number) {
		printf("Question Number: %s\n", question_number);
	}
	if (vote_number) {
		printf("Number of Votes: %s\n", vote_number);
	}

	

	int i = NULL;
	char* string;
	char e_uuid_preallocated[76];
	int number_of_votes = atoi(vote_number);

	curl_global_init(CURL_GLOBAL_ALL);
	/* get event uuid link*/
	char* hash = strtok(url, "/");
	for (int i = 0; i < 3; i++) {
		hash = strtok(NULL, "/");
	}
	char event_uuid_link[71] = "https://app.sli.do/eu1/api/v0.5/app/events?hash=";
	strcat(event_uuid_link, hash);

	const cJSON* event_uuid = getEventUUID(event_uuid_link);
	printf("UUID: %s\n", event_uuid->valuestring);

	char auth_link[81] = "https://app.sli.do/eu1/api/v0.5/events/";
	strcat(auth_link, event_uuid->valuestring);
	strcpy(e_uuid_preallocated, auth_link);
	strcat(auth_link, "/auth");

	char** auth_token = malloc(number_of_votes * sizeof(char*));


#pragma omp parallel for
	for (i = 0; i < number_of_votes; i++) {
		auth_token[i] = getAuthToken(auth_link);  // Each thread calls getAuthToken
#if DEBUG
#pragma omp critical  // Ensure only one thread prints at a time
		{
			puts(auth_token[i]);
		}
#endif
	}

	char poll_summary_link[84];
	strcpy(poll_summary_link, e_uuid_preallocated);
	strcat(poll_summary_link, "/summary");
	const cJSON* summary_uuids = getUUIDsWithAuth(poll_summary_link, auth_token[0]);
	char section_uuid[37];
	char active_poll_uuid[37];
	parseSummary(summary_uuids, &section_uuid, &active_poll_uuid);

	char questions_link[150];
	strcpy(questions_link, e_uuid_preallocated);
	strcat(questions_link, "/polls-v2?sectionUuid=");
	strcat(questions_link, section_uuid);
	strcat(questions_link, "&onlyActive=true");

	const cJSON* question_uuid = getUUIDsWithAuth(questions_link, auth_token[0]);

	char options_uuid[37];
	char feedback_question_uuid[37];
	parseQuestionUUIDs(question_uuid, &options_uuid, &feedback_question_uuid, atoi(question_number));



	/* VOTES SENDER */
#pragma omp parallel for
	for (i = 0; i < number_of_votes; i++) {
		clapSlidoCheeks(e_uuid_preallocated, active_poll_uuid, auth_token[i], hash, options_uuid, feedback_question_uuid);
	}



	cJSON_Delete(event_uuid);
	cJSON_Delete(summary_uuids);
	free(auth_token);
	curl_global_cleanup();

	return 0;
}