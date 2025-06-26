#include "logger.h"
#include <stdio.h>

void connect_database() {
	log_message(LOG_INFO, "Try connecting to Database...");
	log_message(LOG_ERROR, "Failed connection: Did't find server.");
}

void process_data() {
	log_message(LOG_DEBUG, "Starting process user data.");

	for (int i = 0; i < 3; i++) {
		log_message(LOG_DEBUG, "Processing number: %d.", i + 1);
	}
	
	log_message(LOG_INFO, "Processing data successfully.");
}


int main() {
	if (logger_init("app.log", LOG_DEBUG) != 0) {
		return 1; 
	}

	log_message(LOG_NOTICE, "Program's logger starts.");

	connect_database();
	process_data();

	log_message(LOG_WARNING, "Space is not enough (remaining 10%%).");

	printf("\n--- CHANGE LOG LEVEL TO WARNING ---\n\n");

	log_set_level(LOG_WARNING);

	log_message(LOG_INFO, "This message will not be displayed.");
	log_message(LOG_DEBUG, "TThis message will not be displayed.");
	log_message(LOG_ERROR, "This message will not be displayed.");
	log_message(LOG_CRITICAL, "This is critical error!");

	logger_clean();

    return 0;
}
