#pragma once

void printHelp() {
    printf("How to use slido-bot: \n");
    printf("Syntax: [-u|h|v|V]\n");
    printf("options: \n");
    printf("u     slido poll url\n");
    printf("h     print this help.\n");
    printf("q     vote question number\n");
    printf("v     how many votes\n");
}

struct MemoryStruct {
	char* memory;
	size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void parseSummary(const cJSON* summary_uuids, char* section_uuid, char* active_poll_uuid) {
    // Extract the "bySection" object/array
    const cJSON* bySection = cJSON_GetObjectItemCaseSensitive(summary_uuids, "bySection");

    if (bySection == NULL) {
        printf("Error: 'bySection' not found\n");
        return;
    }
    if (cJSON_IsArray(bySection)) {
        cJSON* section;
        cJSON_ArrayForEach(section, bySection) {
            const cJSON* activePollUuid = cJSON_GetObjectItemCaseSensitive(section, "activePollUuid");
            if (cJSON_IsString(activePollUuid) && (activePollUuid->valuestring != NULL)) {
                strcpy(active_poll_uuid, activePollUuid->valuestring);
            }
        }
    }
    else if (cJSON_IsObject(bySection)) {
        cJSON* sectionUuid;
        cJSON_ArrayForEach(sectionUuid, bySection) {
            strcpy(section_uuid, sectionUuid->string);
            const cJSON* activePollUuid = cJSON_GetObjectItemCaseSensitive(sectionUuid, "activePollUuid");
            if (cJSON_IsString(activePollUuid) && (activePollUuid->valuestring != NULL)) {
                strcpy(active_poll_uuid, activePollUuid->valuestring);
            }
        }
    }
    else {
        printf("Error: 'bySection' is neither an array nor an object\n");
    }
}

void parseQuestionUUIDs(const cJSON* question_uuid, char* options_uuid, char* feedback_question_uuid, int question_number) {
    const cJSON* poll = NULL;
    const cJSON* question = NULL;
    const cJSON* option = NULL;

    int question_index = 0;
    int option_index = 0;

    cJSON_ArrayForEach(poll, question_uuid) {
        const cJSON* questions = cJSON_GetObjectItemCaseSensitive(poll, "questions");
        if (!cJSON_IsArray(questions)) {
            printf("Error: 'questions' is not an array\n");
            return;
        }

        cJSON_ArrayForEach(question, questions) {
            const cJSON* options = cJSON_GetObjectItemCaseSensitive(question, "options");
            if (!cJSON_IsArray(options)) {
                printf("Error: 'options' is not an array\n");
                return;
            }
            cJSON_ArrayForEach(option, options) {
                const cJSON* uuid = cJSON_GetObjectItemCaseSensitive(option, "uuid");
                const cJSON* pollQuestionUuid = cJSON_GetObjectItemCaseSensitive(option, "pollQuestionUuid");

                if (cJSON_IsString(uuid) && uuid->valuestring != NULL && option_index == question_number - 1) {
                    strcpy(options_uuid, uuid->valuestring);
                }

                if (cJSON_IsString(pollQuestionUuid) && pollQuestionUuid->valuestring != NULL) {
                    strcpy(feedback_question_uuid, pollQuestionUuid->valuestring);
                }

                option_index++;
            }

            question_index++;
        }
    }
}