# Inteligentne systemy pomiarowe - projekt 1

## Cel projektu
Stworzyć aplikację pomiarową pozwalającą na komunikację z wykorzystaniem szyny danych CAN. Praca nad zadaniem projektowym wymaga wykorzystania zestawu Arduino + CAN Bus Shield + czujników CAN.

## CAN - co to jest?
CAN (Controller Area Network) to szeregowa magistrala komunikacyjna opracowana przez firmę Bosch GmbH w latach 80 XXw. Powstała ona w odpowiedzi na potrzebę znalezienia odpowiedniej metody komunikacji dla systemów kontrolnych, zapewniającej szybką i pozbawioną błędów transmisję danych między elementami wykonawczymi i moniturojącymi. W owych czasach była rewolucyjna pod względem niezawodności, łatwości implementacji, szybkości działania, zostając przy tym stosunkowo tanim rozwiązaniem. Jej głownym przeznaczeniem jest przemysł, w szczególności transport i przemysł maszynowy. 

## Podstawy teoretyczne działania szyny CAN
Specyfikacja CAN definiuje dwie pierwsze warstwy modelu ISO OSI, tj. warstwę fizyczną i warstwę łącza danych. Pozostałe warstwy, poza ostatnią (warstwą aplikacji), której implementacja zależy od projektanta sytemu, są nieużywane. 
### 1. Warstwa fizyczna określa:
  - topologię sieci - CAN jest magistralą, czyli wszystkie urządzenia połączonne są szeregowo wpólną szyną transmisyjną. Szyna składa się z dwóch lini: CAN_L i CAN_H. Minimalną liczbą przewodów do realizacji sieci CAN jest zatem kabel dwużyłowy. W celu eliminacji odbicia sygnałów na końcach magistrali stosuje się rezystory terminujące.
  ![Magistrala](http://mikrokontroler.pl/wp-content/uploads/artykuly/Interfejs_komunikacyjny_CAN_podstawy/rys3.jpg)
  - prędkość i zasięg transmisji - maksymalną dopuszczalną prędkością przesyłania danych jest 1Mb/s, jednak jest ona zależna od długości medium przesyłowego. Ważne jest, aby wszystkie urządzenia podłączone do magistrali działały z tą samą prędkością transmisji.  
  ![Predkosci](http://mikrokontroler.pl/wp-content/uploads/artykuly/Interfejs_komunikacyjny_CAN_podstawy/rys4.png)
  - kodowanie -  CAN wykorzystuje kodowanie NRZ (non-return to zero)
    - jedynce logicznej odpowiada stan wysoki
    - zeru logicznemu odpowiada stan niski
    
    Wysłanie tego samego bitu 5 razy pod rząd budzi jednak podejrzenia, więc nadawca po 5 takich samych bitach "wpycha" bit o wartości przeciwnej (tzw. Bit stuffing). W ten sposób mamy pewność co do poprawnego działania magistrali.
    
  - stany sygnału magistrali - CAN przesyła informacje metodą różnicową - stany odpowiadają różnicy napięć między liniami CAN_L i CAN_H. Istnieją dwa stany magistrali:
    - recesywny - napięcie na obu liniach równe, wynoszące 2-3V - reprezentuje jedynkę logiczną
    - dominujący - 2.75-4.5V na lini CAN_H i 0.5V-2.25V na lini CAN_L - reprezentuje zero logiczne
  

### 2. Warstwa łącza danych określa:
  - format ramki CAN - istnieją 4 rodzaje ramek:
    - data frame 
      - standardowa (CAN 2.0A) - 11-bitowa długość adresu
      ![Ramka standardowa](http://imgie.pl/images/2018/01/22/ramka9f39f.png)
      - rozszerzona (CAN 2.0B) - 29-bitowa długość adresu
    - remote frame
    - error frame
    - overload frame  
  - model komunikacji - infrastruktura multi-master - każde z urządzeń może pełnić funkcje mastera. Każdy węzeł może rozgłaszać wiadomości w sposób wolny, to od konfiguracji węzła docelowego zależy, czy zaakceptuje daną wiadomość.
  - dostęp do medium - metoda CSMA/CR (zmodyfikowana CSMA/CD) - w wyniku wystąpienia kolizji, jedna z wiadomości nadal jest przesyłana, natomiast druga (pozostałe) zostają wysłane po zwolnieniu medium. O tym, która zostanie wysłana decyduje priorytet określany na podstawie adresu (ID). 
  - wykrywanie błędów - CAN wykorzystuje wiele metod wykrywania błędów, np.:
    - suma CRC
    - potwierdzenie otrzymania wiadomości
    - Bit Stuffing
    - sprawdzanie poprawności ramki (zawieranie odpowiednich pól)
    - sprawdzanie poprawności wystawionego bitu


### Architektura
Z punktu widzenia projektu, kluczowe jest rozróżnienie 3 różnych metod projektowania węzła CAN.
1. **Węzeł zbudowany z trzech elementów - mikrokontrolera, kontrolera CAN i transceivera CAN - wykorzystana metoda.**
2. Węzeł zbudowany z mikrokontrolera z kontrolerem CAN i transceivera.
3. Mikrokontroler z wbudowanym kontrolerem CAN i transceiverem.
![Schemat](https://image.ibb.co/bskxTb/canbus.png)


## Urządzenia
Arduino Leonardo
![Alt text](images/leonardo-1-800x800.jpg "Arduino Leonardo")

Arduino Uno
![Alt text](images/uno.jpg "Arduino Uno")

Arduino CAN-Bus Shield
![Alt text](images/shield-can-2-800x800.jpg "Arduino CAN-Bus Shield")

Przewód db9-db9

![Alt text](images/db9.jpg "Przewód db9-db9")

## Schemat połączeniowy
![Alt text](images/schemat.png "Schemat połączeniowy")

## Algorytm
Przed rozpoczęciem **świadomej** pracy z projektem, należy zapoznać się z biblioteką CAN_BUS_SHIELD od Seeed-Studio (https://github.com/Seeed-Studio/CAN_BUS_Shield). W celu lepszego zrozumienia działania programu warto również odnieść się do dokumentacji modułów MCP2515 (CAN bus controller http://ww1.microchip.com/downloads/en/DeviceDoc/21801d.pdf) i MCP2551 (CAN transceiver http://www.microchip.com/wwwproducts/en/en010405). 
### 1. Arduino
Ponieważ magistrala CAN działa na zasadzie multi-slave, w obu programach inicjalizacja modułów odpowiedzialnych za komunikację odbywa się jednakowo. 
```
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
```
Parametr funkcji CAN.begin określa, z jaką prędkością przesyłane są dane w magistrali. Możliwe do wykorzystania prędkości:
```
#define CAN_5KBPS    1
#define CAN_10KBPS   2
#define CAN_20KBPS   3
#define CAN_25KBPS   4 
#define CAN_31K25BPS 5
#define CAN_33KBPS   6
#define CAN_40KBPS   7
#define CAN_50KBPS   8
#define CAN_80KBPS   9
#define CAN_83K3BPS  10
#define CAN_95KBPS   11
#define CAN_100KBPS  12
#define CAN_125KBPS  13
#define CAN_200KBPS  14
#define CAN_250KBPS  15
#define CAN_500KBPS  16
#define CAN_666kbps  17
#define CAN_1000KBPS 18
```
Jeśli wszystkie piny płytki Arduino CAN shield zostały poprawnie podłączone, otrzymamy na porcie szeregowym wiadomość "CAN BUS Shield init ok!".

W ramach projektu zostały stworzone dwa programy realizujące komunikację CAN za pomocą platformy Arduino:
  - Sender - program wysyłający dane z symulowanych czujników: temperatury, wilgotności i czujnika nieokreślonego. 
  ```
   prepare_data();
   CAN.sendMsgBuf(0x02, 0, 8, (unsigned char *)temperature);
   ```
   Funkcja "prepare_data" przygotowuje symulacyjne dane z wirtualnych czujników, natomiast funkcja "CAN.sendMsgBuf(ID_węzła,typ_ramki(0=standard,1=rozszerzona), liczba_bitów, bufor_nadawczy)" odpowiada za zarządzanie magistralą i wysłanie ramki. W ramach funkcji sprawdzane jest dostępność do medium, wybór pierwszego wolnego bufora, ustawienie odpowiednich flag oraz wysłanie danych. 
  - Receiver - jak sama nazwa wskazuje pełni funkcję odbiorczą. Wykorzystane w programie maski i filtry pozwalają na eliminację niepożądanych wiadomości.
  ```
    CAN.init_Mask(0, 0, 0x3ff);    //in order to use filters, we have to first define a Mask (according to DataSheet)                         
    CAN.init_Mask(1, 0, 0x3ff);    //this mask is "test all"-  that means every bit of filter must be satisfied (0b1111111111)
      
    CAN.init_Filt(0, 0, 0x01);    //1st filter, receives msg from ID 0x01 device                         
    CAN.init_Filt(1, 0, 0x02);    //2nd filter, receives msg from ID 0x02 device  
  ```
   Program działa w trybie przerwaniowym - kiedy kontroler magistrali informuje procesor o obecności wiadomości w buforze odbiorczym, Arduino pobiera dane i ustawia w buforze odpowiednie flagi, pozwalające buforowi na przyjęcie kolejnych porcji danych. Funckja za to odpowiedzialna prezentuje się następująco.
   ```
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
     }
   ```
   W programie ponadto występuje algorytm, pozwalający w prosty sposób dodawać kolejne filtry, umożliwiając odbieranie danych od niezdefiniowanych domyślnie ID. Służy do tego funkcja "Try_add_device()".
   W celu przyszłego usprawnienia aplikacji, oraz informowania użytkownika o stanie systemu, wprowadzony został prymitywny "software watchdog". Działa on w pętli głównej programu, wykorzystuje on systemowy UpCounter, który resetuje się po otrzymaniu wiadomości. W przypadku nie otrzymania wiadomości przez "TIMEOUT_INTERVAL" sekund, użytkownik informowany jest o stanie modułu oraz otrzymuje podpowiedzi od systemu.
   
### 2. PC
Napisany został skrypt w języku Python3.6, który znajduje się w folderze computer. Skrypt zczytuje dane z portu USB dzięki bibliotece serial, następnie odczytane dane zostają zweryfikowane czy są pomiarami czy komunikatem. Pomiary mają specyficzny format, który zaczyna się od liter "ID" następnie występuje numer chujnika, dwukropek, oraz 8 cyfr pomiaru w formacie dziesiętnym. Odebrane komunikaty zostają wyświetlane na standardowym wyjściu programu, a odebrane pomiary zostają przetworzone, wypisane na standardowym wyjściu oraz przesłane na serwer SmartStorm. Jeżeli wysyłanie danych się nie powiedzie z dowolnego powodu zostaje wyświetlony komunikat o niepowodzeniu a pomiar zostaje zapisany do pliku tekstowego failed_data. Poniżej zamieszczam kod wysyłania danych na serwer.
```
def send_data(sensor_id, value):
    dt = datetime.datetime.now()
    url = "http://alfa.smartstorm.io/api/v1/measure"
    request_data = {"user_id": "126127@interia.pl",
                    "sensor_id": sensor_id,
                    "desc": str(dt),
                    "measure_value": str(value)}
    try:
        requests.post(url, request_data, timeout=1)
    except requests.exceptions.RequestException:
        print("Could not send data to Smart Storm")
        with open('failed_data', 'a') as f:
            f.write(str(dt) + ',' + sensor_id + ',' + str(value) + '\n')
```
Użytkownik ma dodatkowo możliwość przesyłania danych poprzez port USB. Wpisanie ciągu znaków do standardowego wejścia i zatwierdzenie klawiszem Enter powoduje wysłanie wpisanego ciągu przez port USB. Program wgrany na Arduino odbiera dane, lecz odrzuca dane różne od liczb z przedziału 0-99. Podanie liczby z tego przedziału powoduje dodanie miernika o id równym tej liczbie, o ile nie osignęliśmy maksymalnej liczby urządzeń lub dany miernik jest już dodany.

## Sposób uruchomienia
W celu kompilacji programów na platformy Arduino, wymagane jest pobranie i dołączenie biblioteki CAN_BUS_SHIELD od Seeed-Studio. Wszystkie pozostałe pliki potrzebne do prawidłowego działania systemu znajdują się w tym repozytorium.
1. Wgrać odpowiedni program (sender/sender.ino lub receiver/receiver.ino) na płytki Arduino za pomocą programu Arduino IDE
- wybrać odpowiedni typ płytki
- wybrać odpowiedni port COM
- otworzyć plik źródłowy
- skompilować kod
- załadować skompilowany program do pamięci płytki
2. Podłączyć przewody komunikacyjne
- połączyć przewodem DB9-DB9 gniazda na obu shieldach
3. Zapewnić łączność komputera PC ze stroną internetową systemu smart-storm
4. Podłączyć przewody zasilające płytki Arduino
- obie płytki zasilane są przewodami USB, przy czym ważne jest, żeby najpierw uruchomić płytkę z programem receiver.ino
W tym momencie dane w odbiorniku są wystawiane na interfejs szeregowy USB, skąd mogą zostać pobrane przez skrypt napisany w języku Python.
5. Uruchomić program send.py

## Źródła
- https://smartrobots.pl/arduino-CAN-bus-shield
- https://elty.pl/pl/p/CAN-BUS-Shield-dla-Arduino/1094
- http://mikrokontroler.pl/2013/06/10/interfejs-komunikacyjny-can-podstawy/
