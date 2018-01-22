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

const int Buff1=1;
const int Buff2=2;
const int Buff3=3;
void loop()
{
    // get data from sensors
    // temperature = 
    // humidity = 
    
    // parse data
    
    // send data:  id = 0x00, standard frame, data len = 8, stmp: data buf
    
    //We can either send a messange through a specified buffer or through a first found free buffer
    //It may be neccessary to use specific buffer when there are different prorities to the informations of if we need to store old informations
    //In this assignment there is no need to buffor data, as we do not need old data at any time
    //Its better to look for the first free buffor and just go for it
    
    CAN.sendMsgBuf(0x02, 0, 8, temperature);
    delay(100);                      
    CAN.sendMsgBuf(0x01, 0, 8, humidity);
    delay(100); 
    CAN.sendMsgBuf(0x05, 0, 8, humidity);
    delay(100); 

}

// END FILE
