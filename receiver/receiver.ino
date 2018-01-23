//authors: JS ML BP
//Based on exaples from Seeed-Studio/CAN_BUS_Shield library
//https://github.com/Seeed-Studio/CAN_BUS_Shield

#include <SPI.h>
#include "mcp_can.h"

#define MAX_FILTER_INDEX  5
#define TIMEOUT_INTERVAL 3000   //*10ms

const int SPI_CS_PIN = 10;  
MCP_CAN CAN(SPI_CS_PIN);                                    

int filter_index = 2; //index of next available filter, as we have 6 filters, this parameter should never be higher than 5
int WDG_timmer=0; //counter detecting if can shield works as intended

int filter_IDs[5] = {1,2,0,0,0};

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
    
    CAN.init_Mask(0, 0, 0x3ff);    //in order to use filters, we have to first define a Mask (according to DataSheet)                         
    CAN.init_Mask(1, 0, 0x3ff);    //this mask is "test all"-  that means every bit of filter must be satisfied (0b1111111111)
      
    CAN.init_Filt(0, 0, 0x01);    //1st filter, receives msg from ID 0x01 device                         
    CAN.init_Filt(1, 0, 0x02);    //2nd filter, receives msg from ID 0x02 device                
                     
    
}
void MCP2515_ISR()    //interrupt routine for receiving
{
    unsigned char len = 0;    //length of received msg
    unsigned char buf[8];     //read buffor

    if(CAN_MSGAVAIL == CAN.checkReceive())      //set RX and TX flag, allowing us to receive msg      
    {
        CAN.readMsgBuf(&len, buf);      //reads the messange, clears used buffor

        unsigned int canId = CAN.getCanId();  //gets the ID of the sending node
        
        Serial.print("ID");
        Serial.print(canId);
        Serial.print(":");
        for(int i = 0; i<len; i++)    
        {
          Serial.print(buf[i], HEX);
        }
        
//        switch (canId){    //depending on received ID, we already know what is the messange label
//          case 1:
//            Serial.print("ID");
//            Serial.print(canId);
//            Serial.print(" Hum:");
//            for(int i = 0; i<len; i++)    
//            {
//              Serial.print(buf[i], HEX);
//            }
//            break;
//          case 2:
//            Serial.print("ID");
//            Serial.print(canId);
//            Serial.print(" Temp:");
//            for(int i = 0; i<len; i++)    
//            {
//              Serial.print(buf[i], HEX);
//            }
//            break;
//          default:        //unknow node, still received the messange
//            Serial.print("ID");
//            Serial.print(canId);
//            Serial.print(" NC:");
//            for(int i = 0; i<len; i++)    
//            {
//              Serial.print(buf[i], HEX);
//            }
//
//            break;
//        }
        Serial.println();
        WDG_timmer=0;  //got msg- we can reset wdg timer
    }
}

//function to check if we can add another filter
bool index_available(void){
  if(filter_index<MAX_FILTER_INDEX)
  {  
   // Serial.println(filter_index);
    return 1;
  }
  else return 0;
}

bool ID_on_list(int id){
  for(int i=0; i<MAX_FILTER_INDEX; i++)
    if(id == filter_IDs[i])
      return 1;
  return 0;
}

//function adding additional filter in case device with unknown ID apears on the bus. To add a filter just type its ID number (1-99) through serial terminal.
void Try_add_device(void){  
    String new_number;
    while(Serial.available()){
      char new_buffor = (Serial.read());
      new_number = new_number+new_buffor;
    }
    if(index_available()){  //check if there is index available
      switch(new_number.length()){  //parsing case - TO DO: optimisation (when there are 5 filters it should check if the input ID is already on list)
        case 1:
          if(isDigit(new_number[0])){
            if(!ID_on_list(new_number.toInt())){
              CAN.init_Filt(filter_index, 0, new_number.toInt());   //add new filter based on filter index and ID of the node
              Serial.print("Dodano nowe urzadzenie o numerze ");
              Serial.print(new_number.toInt());
              Serial.print(" . Mozna dodac jeszcze ");
              Serial.print(MAX_FILTER_INDEX-filter_index-1);
              Serial.println(" ID urzadzen.");
              filter_IDs[filter_index] = new_number.toInt();
              filter_index++;
            }
            else
              Serial.println("ID urzadzenia jest juz dodane");
          }
          break;
        case 2:
          if(isDigit(new_number[0]) && isDigit(new_number[1])){
            if(!ID_on_list(new_number.toInt())){
              CAN.init_Filt(filter_index, 0, new_number.toInt()); //add new filter based on filter index and ID of the node
              Serial.print("Dodano nowe urzadzenie o numerze ");
              Serial.print(new_number.toInt());
              Serial.print(" . Mozna dodac jeszcze ");
              Serial.print(MAX_FILTER_INDEX-filter_index-1);
              Serial.println(" ID urzadzen.");
              filter_IDs[filter_index] = new_number.toInt();
              filter_index++;
            }
            else
              Serial.println("ID urzadzenia jest juz dodane");
          }
          break;
        default:  //got anything else than 1-99
          Serial.println("Podaj numer urzadzenia które ma byc dodane (od 3 do 99, np. 5)");
          break;
        }
     }
     else 
       Serial.println("Nie mozna dodac wiecej filtrow");
}

//main loop
void loop()
{
    WDG_timmer++; //we increase the timer every 10ms
    if(WDG_timmer>TIMEOUT_INTERVAL){   //if we havent got messange in 500 main loop cycles, we assume something is wrong
      Serial.println("Od 30s nie otrzymano danych");
      if(!CAN.checkError())  //its either the connection between modules
      {
        Serial.println("Nie wykryto awarii modulu CAN -  sprawdz poprawnosc polaczenia");
      }
      else //or our shield may be broken
        Serial.println("Awaria modułu CAN - potrzebny reset");
      WDG_timmer=0; //clear the timer to avoid useless spam
    }
    if(Serial.available()){ //got anything on Serial? that may be a new device - go check it
      Try_add_device();  
    }
    delay(10);
}

//EOF
