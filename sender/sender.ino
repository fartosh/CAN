//authors: JS ML BP
//Based on exaples from Seeed-Studio/CAN_BUS_Shield library
//https://github.com/Seeed-Studio/CAN_BUS_Shield

#include <mcp_can.h>
#include <SPI.h>

#define Send_interval 10000
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
int temp = 2200;
int hum = 2000;
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
char humidity[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

int get_temp(void)
{
  //TO DO: communication with temperature sensor
  temp = temp + random (-100,100);
  return temp;
}

int get_hum(void)
{
  //TO DO: communication with temperature sensor
  hum = hum + random (-10,10);
  return hum;
}

void prepare_data(void)
{
    int temp_buf = get_temp();  //simulation of temperature sensor
    int hum_buf = get_hum();  //simulation of humidity sensor
    
    //parsing data onto unsigned char array
    temperature[7] = (temp_buf % 10);
    temperature[6] = (temp_buf % 100) / 10;
    temperature[5] = (temp_buf % 1000) / 100;
    temperature[4] = (temp_buf / 1000);
    
    humidity[7] = (hum_buf % 10);
    humidity[6] = (hum_buf % 100) / 10;
    humidity[5] = (hum_buf % 1000) / 100;
    humidity[4] = (hum_buf /1000);

//    Serial.println(temperature);
//    Serial.println(sizeof(temperature));
//    Serial.println(humidity);
//    Serial.println(sizeof(humidity));     
}

void loop()
{
    // get data from sensors
    prepare_data();
  
    //We can either send a message through a specified buffer or through a first free buffer found.
    //It may be neccessary to use specific buffer when there are different prorities to the informations of if we need to store old informations.
    //In this assignment there is no need to buffor data, as we do not need old data at any time.
    //Its better to look for the first free buffor and just go for it.
    
    // send data:  id = 0xXX, frame type (0 - standard frame), data len , data buf
    CAN.sendMsgBuf(0x02, 0, 8, (unsigned char *)temperature);
    delay(100);  //messages shouldn't be sent faster than 1/20ms because of using interrupt driven receiving                      
    CAN.sendMsgBuf(0x01, 0, 8, (unsigned char *)humidity);
    delay(100); 
    CAN.sendMsgBuf(0x05, 0, 8, (unsigned char *)data);
    delay(100); 
    delay(Send_interval);

}

//EOF
