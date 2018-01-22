//authors: JS ML BP
//Based on exaples from Seeed-Studio/CAN_BUS_Shield library
//https://github.com/Seeed-Studio/CAN_BUS_Shield

#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
int temp = 2200;
int hum = 200;
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



char temperature[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char humidity[8] = {0, 0, 0, 0, 0, 0, 1, 1};
String temp_buf;
String hum_buf;


String get_temp(void)
{
  //TO DO: communication with temperature sensor
  temp = temp + random (-100,100);
  return String(temp);
}

String get_hum(void)
{
  //TO DO: communication with temperature sensor
  hum = hum + random (-10,10);
  return String(hum);
}

void loop()
{
    // get data from sensors
    temp_buf = get_temp();
    hum_buf = get_hum();
    temp_buf.toCharArray(temperature, 8);
    hum_buf.toCharArray(humidity, 8);
    // humidity = 
    // parse data
    Serial.println(temperature);
    Serial.println(sizeof(temperature));
    
    Serial.println(humidity);
    Serial.println(sizeof(humidity));
    
    //We can either send a messange through a specified buffer or through a first found free buffer
    //It may be neccessary to use specific buffer when there are different prorities to the informations of if we need to store old informations
    //In this assignment there is no need to buffor data, as we do not need old data at any time
    //Its better to look for the first free buffor and just go for it
    
    // send data:  id = 0xXX, frame type (0 - standard frame), data len , data buf
    CAN.sendMsgBuf(0x02, 0, 8, (unsigned char *)temperature);
    delay(100);  //messanges shouldn't be sent faster than 1/20ms because of using interrupt driven receiving                      
    CAN.sendMsgBuf(0x01, 0, 8, (unsigned char *)humidity);
    delay(100); 
    CAN.sendMsgBuf(0x05, 0, 8, (unsigned char *)humidity);
    delay(100); 

}

//EOF
