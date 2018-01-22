

#include <SPI.h>
#include "mcp_can.h"



const int SPI_CS_PIN = 10;
int WDG_timmer=0;
MCP_CAN CAN(SPI_CS_PIN);                                    
int filter_index = 2;


void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(1000);
    }
    Serial.println("CAN BUS Shield init ok!");
    
    attachInterrupt(0, MCP2515_ISR, FALLING);
    
    CAN.init_Mask(0, 0, 0x3ff);                         
    CAN.init_Mask(1, 0, 0x3ff);
    
    CAN.init_Filt(0, 0, 0x01);                         
    CAN.init_Filt(1, 0, 0x02);                          
                     
    
}
void MCP2515_ISR()
{
    unsigned char len = 0;
    unsigned char buf[8];

    if(CAN_MSGAVAIL == CAN.checkReceive())            
    {
        CAN.readMsgBuf(&len, buf);    

        unsigned int canId = CAN.getCanId();
        
        switch (canId){
          case 1:
            Serial.print("Wilgotnosc:");
            for(int i = 0; i<len; i++)    
            {
              Serial.print(buf[i], HEX);
            }
            break;
          case 2:
            Serial.print("Temperatura:");
            for(int i = 0; i<len; i++)    
            {
              Serial.print(buf[i], HEX);
            }
            break;
          default:
            Serial.print("ID: ");
            Serial.print(canId);
            Serial.print(". Wiadomosc od urzadzenia:");
            for(int i = 0; i<len; i++)    
            {
              Serial.print(buf[i], HEX);
            }
            break;
        }
        Serial.println();
        WDG_timmer=0;
    }
}

void Try_add_device(void){  
    String new_number;
    while(Serial.available()){
      char new_buffor = (Serial.read());
      new_number = new_number+new_buffor;
    }
    if(filter_index<6){
      switch(new_number.length()){
        case 1:
          if(isDigit(new_number[0])){
            CAN.init_Filt(filter_index, 0, new_number.toInt());
            Serial.print("Dodano nowe urzadzenie o numerze ");
            Serial.println(new_number.toInt());
            filter_index++;
          }
          break;
        case 2:
          if(isDigit(new_number[0]) && isDigit(new_number[1])){
            CAN.init_Filt(filter_index, 0, new_number.toInt());
            Serial.print("Dodano nowe urzadzenie o numerze ");
            Serial.println(new_number.toInt());
            filter_index++;
            
          }
          break;
        default:
          Serial.println("Podaj numer urzadzenia które ma byc dodane (od 3 do 99, np. 5)");
          break;
        }
     }
     else 
       Serial.println("Nie mozna dodac wiecej filtrow");
}


void loop()
{
    WDG_timmer++;
    if(WDG_timmer>500){
      Serial.println("Od 5s nie otrzymano danych");
      if(!CAN.checkError())
      {
        Serial.println("Nie wykryto awarii modulu CAN -  sprawdz poprawnosc polaczenia");
      }
      else 
        Serial.println("Awaria modułu CAN - potrzebny reset");
      WDG_timmer=0;
    }
    if(Serial.available()){
      Try_add_device();
    }
    delay(10);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
