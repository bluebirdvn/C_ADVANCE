#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_NODESENSOR 10
#define MAX_DATA_STRING 256

char collecttedDataBuffer[MAX_DATA_STRING*10];
typedef struct{
    int id;
}SensorNode;

int getData(SensorNode* node){
    if(node == NULL){
        return -1;
    }
    return (rand()%1000) + node->id*1000;
}
typedef struct SensorManagerINS SensorManager;

struct SensorManagerINS{
    SensorNode* sensors[MAX_NODESENSOR];
    int sensors_count;
    char internalDataBuffer[MAX_DATA_STRING*10];
    int (*collectData)(SensorManager* self);
    void (*pushData)(SensorManager* self, const char* filename);
    void (*addSensor)(SensorManager* self, SensorNode* sensor);
};

static SensorManager* SensorManagerInstance = NULL;



void addSensorImplement(SensorManager* self, SensorNode* sensor){
    if (self == NULL || sensor == NULL) {
        fprintf(stderr, "Error: SensorManager of SensorNode is NULL");
        return;
    }
    if(self->sensors_count < MAX_NODESENSOR) {
        self->sensors[self->sensors_count++] = sensor;
        printf("SensorNode %d added to SensorManager.\n", sensor->id);

    } else {
        printf("SensorManager: Can't add more Node. Limit reached.\n");
    }
}

int collectDataToBufferImplement(SensorManager *self){
    if (self == NULL) {
        fprintf(stderr, "Error: SensorManager is NULL for collectDataToBuffer.\n");
        return -1;
    }

    printf("\n---Collecting data into internal buffer ---\n");
    if (self->sensors_count == 0) {
        printf("SensorManger: no sensor to collect data from.\n");
        self->internalDataBuffer[0]='\0';
        return -1;
    }
    self->internalDataBuffer[0]='\0';
    char temp[MAX_DATA_STRING];

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(temp, sizeof(temp), "Data collect at: %Y-%m-%d %H:%M:%S\n", tm_info);
    strcat(self->internalDataBuffer, temp);

    int dataColectCount = 0;
    for (int i = 0; i< self->sensors_count; ++i) {
        if (self->sensors[i] != NULL) {
            int data = getData(self->sensors[i]);
            printf("Sensor ID %d: DATA = %d ", self->sensors[i]->id, data);
            snprintf(temp, MAX_DATA_STRING, "Sensor ID %d: DATA = %d\n", self->sensors[i]->id, data);
            strcat(self->internalDataBuffer, temp);
            dataColectCount++;
        }
    }
    printf("Data collection to internal buffer complete.\n");
    return dataColectCount;
}

void pushBufferToStorageImplement(SensorManager* self, const char* filename){
    if (self == NULL) {
        fprintf(stderr, "Error: SensorManger is NULL for pushBufferToStorge.\n");
        return;
    }
    if (strlen(self->internalDataBuffer) == 0) {
        printf("SensorManager: Internal buffer is empty");
        return;
    }

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening data file to push data");
        return;
    }
    fprintf(file, "%s", self->internalDataBuffer);
    fprintf(file, "-------------------------------\n");
    fclose(file);
    printf("Data has been pushed to file %s\n", filename);
}

static void initSensorManager(SensorManager *sensorManager){
    sensorManager->sensors_count = 0;
    for (int i = 0; i < MAX_NODESENSOR; ++i) {
        sensorManager->sensors[i] = NULL;
    }
    sensorManager->internalDataBuffer[0] = '\0';

    sensorManager->collectData = collectDataToBufferImplement;
    sensorManager->pushData = pushBufferToStorageImplement;
    sensorManager->addSensor = addSensorImplement;
    printf("SensorManager instace created and init.\n");
}

SensorManager* SensorManagerGetInstance(){
    if (SensorManagerInstance == NULL) {
        SensorManagerInstance = (SensorManager*)malloc(sizeof(SensorManager));
        
        if (SensorManagerInstance == NULL) {
            perror("Failed to allocate memory for SensorManager");
            exit(EXIT_FAILURE);
        }
        initSensorManager(SensorManagerInstance);
    } else {
        printf("SensorManager instance already exists");

    }
    return SensorManagerInstance;
}


void destroySensorManagerInstance() {
    if (SensorManagerInstance != NULL) {
        printf("Destroying SensorManager instance...\n");
        free(SensorManagerInstance);
        SensorManagerInstance = NULL;
        printf("SensorManager instance destroyed.\n");
    }
}

int main() {
    srand(time(NULL));

    SensorManager* sensorM1 = SensorManagerGetInstance();

    SensorNode n1= {1}, n2 = {2}, n3 = {3};

    if (sensorM1->addSensor) {
        sensorM1->addSensor(sensorM1, &n1);
        sensorM1->addSensor(sensorM1, &n2);
        sensorM1->addSensor(sensorM1, &n3);
    }

    const char* data_file = "sensorData.txt";

    if (sensorM1->collectData) {
        int items_collected = sensorM1->collectData(sensorM1);
        if (items_collected > 0) {
            printf("%d data items collected to buffer.\n", items_collected);

        }
    }

if (sensorM1->pushData) {
    sensorM1->pushData(sensorM1, data_file);
}

printf("\n--- Collecting and pushing again ---\n");
    if (sensorM1->collectData) {
        sensorM1->collectData(sensorM1);
    }
    if (sensorM1->pushData) {
        sensorM1->pushData(sensorM1, data_file);
    }

destroySensorManagerInstance();

printf("\nProgram finshed.\n");
return 0;
}
