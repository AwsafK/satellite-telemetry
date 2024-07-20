/*

• Name of the Module: Assignment 3 Sattelite 
• Date Due: 02/21/2024
• Author: Awsaf Ahmad Kabir
• Program Purpose: 
A low memory print program for a satellite: Comet-666. It communicates it sends telemetry information across a communication link to
ground station, Shangri-La.

*/




#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>

using namespace std;

#define      GroundStationOutputChannel        cout
#define      masterInfoSatellite               masterInfoDiscriminatedUnionStruct.masterUnion.satelliteInformation
#define      masterInfoExperiment              masterInfoDiscriminatedUnionStruct.masterUnion.experimentInformation
#define      infoStoredArrayatindexSatellite   infoStoredArray[index].masterUnion.satelliteInformation
#define      infoStoredArrayatindexExperiment  infoStoredArray[index].masterUnion.experimentInformation

const string TELEMETRY_INPUT_CHANNEL = "inputTelemetryData.txt";

enum class MESSAGE_ID_ENUM_CLASS { SATELLITE_INFORMATION_MESSAGE = 1, EXPERIMENT_INFORMATION_MESSAGE = 2};
MESSAGE_ID_ENUM_CLASS messageIdEnum;

enum class INFO_TYPE_ENUM_CLASS  { SATELLITE_INFORMATION, EXPERIMENT_INFORMATION, EMPTY };
const uint8_t                    ARRAY_DATA_STORAGE_SIZE = 5;

float_t minTemperature  = 212,       // -50 - 212
        maxTemperature  = -50;
float_t minVoltage = 100.0,          // 0.0 - 100.0
        maxVoltage = 0.0;

struct SatelliteInfoStruct {
    float_t temperature;
    float_t voltage;
};
struct ExperimentInfoStruct {
    uint16_t radiationCount;
    uint16_t latchupEventsCount;
};
union MasterUnion {
    SatelliteInfoStruct  satelliteInformation;
    ExperimentInfoStruct experimentInformation;
};

// Descriminated Union
struct MasterInfoDiscriminatedUnionStruct {
    INFO_TYPE_ENUM_CLASS infoTypeEnum;
    MasterUnion          masterUnion;
};

int main() {
    MasterInfoDiscriminatedUnionStruct infoStoredArray[ARRAY_DATA_STORAGE_SIZE];
    uint8_t                            entryPositionIndex = 0;
    MasterInfoDiscriminatedUnionStruct masterInfoDiscriminatedUnionStruct;
    
    ifstream                           telemetryInputChannel(TELEMETRY_INPUT_CHANNEL);

    // Check if open failure, pause screen, leave
    if (telemetryInputChannel.fail())
    {
        cout << "File Failed to Open" << endl;
        exit(EXIT_FAILURE);
    }


    GroundStationOutputChannel << setprecision(1) << fixed << showpoint;

    // in infoStoredArray set each element infoType to EMPTY
    for (uint8_t i = 0; i < ARRAY_DATA_STORAGE_SIZE; i++) {
        infoStoredArray[i].infoTypeEnum = INFO_TYPE_ENUM_CLASS::EMPTY;
    }


    uint16_t messageIdUInt;
    while (telemetryInputChannel >> messageIdUInt) {
        GroundStationOutputChannel <<
            "Current Telemetry Information: " << endl <<
            "------------------------------ " << endl;

        // convert file number messageIdUInt to enumerated messageIdEnum variable 
 
        messageIdEnum = static_cast<MESSAGE_ID_ENUM_CLASS>(messageIdUInt);


        
        switch (messageIdEnum) {

        case MESSAGE_ID_ENUM_CLASS::SATELLITE_INFORMATION_MESSAGE:

            // set discriminated union masterInfoDiscriminatedUnionStruct infoType to SATELLITE INFORMATION

            masterInfoDiscriminatedUnionStruct.infoTypeEnum = INFO_TYPE_ENUM_CLASS::SATELLITE_INFORMATION;

            // read in from telemetryInputChannel into union masterInfoSatellite temperature and 

            telemetryInputChannel >> masterInfoSatellite.temperature >> masterInfoSatellite.voltage;

            // send this information to the Ground Station Output Channel, see output in the assignment

            GroundStationOutputChannel << setw(17) << left << 
                "Temperature " << right << " : " << right << masterInfoSatellite.temperature << endl <<
                setw(17) << left <<
                "Voltage: " << right << " : " << right << masterInfoSatellite.voltage << endl << endl;

            // put masterInfoDiscriminatedUnionStruct into stored array at the entry Position Index

            infoStoredArray[entryPositionIndex] = masterInfoDiscriminatedUnionStruct;

            // adjust entryPositionIndex to next entry or back to start index if past end of array

            entryPositionIndex = (entryPositionIndex + 1) % ARRAY_DATA_STORAGE_SIZE;

          break; //MESSAGE_ID_ENUM_CLASS::SATELLITE_INFORMATION_MESSAGE:

          case MESSAGE_ID_ENUM_CLASS::EXPERIMENT_INFORMATION_MESSAGE:

            // set discriminated union masterInfoDiscriminatedUnionStruct infoTypeEnum to EXPIRIMENT INFORMATION

              masterInfoDiscriminatedUnionStruct.infoTypeEnum = INFO_TYPE_ENUM_CLASS::EXPERIMENT_INFORMATION;

            // read in from telemetryInputChannel into union masterInfoExperiment radiation Count and latchupEventsCount

              telemetryInputChannel >> masterInfoExperiment.radiationCount >> masterInfoExperiment.latchupEventsCount;

            // send this information to the Ground Station Output Channel, see output in the assignment

              GroundStationOutputChannel << setw(26) << left << "Radiation Count" << right << " : " << right <<
                  masterInfoExperiment.radiationCount << endl << setw(26) << left << "Latch Up Event Count " << right 
                  << " : " << right << masterInfoExperiment.latchupEventsCount << endl << endl;

            // put masterInfoDiscriminatedUnionStruct into stored array at the entry Position Index

              infoStoredArray[entryPositionIndex] = masterInfoDiscriminatedUnionStruct;

            // adjust entryPositionIndex to next entry or back to start index if past end of array

              entryPositionIndex = (entryPositionIndex + 1) % ARRAY_DATA_STORAGE_SIZE;

          break; //MESSAGE_ID::EXPERIMENT_INFORMATION_MESSAGE:

        }//switch (infoStoredArray[index].infoType)

        // set summary information initialization

        uint16_t totalRadiationCount    = 0,
                 totalLatchupEventCount = 0,
                 infoSICount            = 0,
                 infoEICount            = 0;

        //set SI Info calculate min max for SI



        GroundStationOutputChannel <<
            "History:" << endl <<
            "--------" << endl;

        for (uint8_t index = 0; index < ARRAY_DATA_STORAGE_SIZE; index++) {
            switch (infoStoredArray[index].infoTypeEnum) {

              case INFO_TYPE_ENUM_CLASS::SATELLITE_INFORMATION:

                // send to ground station output channel temperature and voltage

                GroundStationOutputChannel << setw(17) << left <<
                    "Temperature " << right << " : " << right 
                    << infoStoredArray[index].masterUnion.satelliteInformation.temperature << endl << setw(17) << left
                    << "Voltage: " << right << " : " << right << infoStoredArray[index].masterUnion.satelliteInformation.voltage <<
                    endl << endl;

                // increment info SI Count

                  infoSICount++;

                // set running min max summary information (temperature, voltage)
                  
                  for (uint8_t i = 0; i < ARRAY_DATA_STORAGE_SIZE; i++)
                  {
                      if (infoStoredArray[index].masterUnion.satelliteInformation.temperature < minTemperature)
                      {
                          minTemperature = infoStoredArray[index].masterUnion.satelliteInformation.temperature;
                      }
                      if (infoStoredArray[index].masterUnion.satelliteInformation.temperature > maxTemperature)
                      {
                          maxTemperature = infoStoredArray[index].masterUnion.satelliteInformation.temperature;
                      }
                      if (infoStoredArray[index].masterUnion.satelliteInformation.voltage < minVoltage)
                      {
                          minVoltage = infoStoredArray[index].masterUnion.satelliteInformation.voltage;
                      }
                      if (infoStoredArray[index].masterUnion.satelliteInformation.voltage > maxVoltage)
                      {
                          maxVoltage = infoStoredArray[index].masterUnion.satelliteInformation.voltage;
                      }
                  }
                  
              break; //case SATELLITE_INFORMATION:

              case INFO_TYPE_ENUM_CLASS::EXPERIMENT_INFORMATION:

                // send to ground station output channel radiation and latchup Events Count
                
                  GroundStationOutputChannel << setw(26) << left << "Radiation Count" << right << " : " << right <<
                      infoStoredArray[index].masterUnion.experimentInformation.radiationCount << endl <<
                      setw(26) << left << "Latch Up Event Count " << right << " : " << right <<
                      infoStoredArray[index].masterUnion.experimentInformation.latchupEventsCount << endl << endl;

                // increment info EI Count
                  infoEICount++;
                // increase sum up totals for radiationCount and LatchupEventCount  
                // from infoStoredArrayatindexExperiment for radiationCount and latchupEventCount                  
                  
                  totalRadiationCount += infoStoredArrayatindexExperiment.radiationCount;
                  totalLatchupEventCount += infoStoredArrayatindexExperiment.latchupEventsCount;
                    
              break;//case EXPERIMENT_INFORMATION:

              case INFO_TYPE_ENUM_CLASS::EMPTY:
                // do nothing for empty array items (skip)
              break; // EMPTY:
               
            }//switch (messageID)

        }//for summary

        GroundStationOutputChannel <<
            "Summary Information" << endl <<
            "-------------------" << endl <<
            "Number of Satellite  Information Records: " << infoSICount << endl <<
            "Number of Experiment Information Records: " << infoEICount << endl <<
            "Total Radiation Count      : " << totalRadiationCount      << endl <<
            "Total Latch Up Event Count : " << totalLatchupEventCount   << endl <<
            "Maximum Temperature        : " << maxTemperature           << endl <<
            "Minimum Temperature        : " << minTemperature           << endl <<
            "Maximum Voltage            : " << maxVoltage               << endl <<
            "Minimum Voltage            : " << minVoltage               << endl << endl;

        // hold screen GroundStationOutputChannel

        cout << "Press the enter key once or twice to continue..." << endl;
        cin.ignore();
        cin.get();


    }//while

    GroundStationOutputChannel << "Program Done" << endl;

    exit(EXIT_SUCCESS);

}//main()
