// demo: CAN-BUS Shield, send data
// loovee@seeed.cc

#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

unsigned char temperature[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char humidity[8] = {0, 0, 0, 0, 0, 0, 1, 1};

void loop()
{
    // get data from sensors
    // temperature = 
    // humidity = 
    
    // parse data
    
    // send data:  id = 0x00, standard frame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(0x00, 0, 8, humidity);
    delay(5000);                       // send data per 100ms
}

// END FILE
