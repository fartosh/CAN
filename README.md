# CAN
CAN (Controller Area Network) to szeregowa magistrala komunikacyjna opracowana przez firmę Bosch GmbH w latach 80 XXw. Powstała ona w odpowiedzi na potrzebę znalezienia odpowiedniej metody komunikacji dla systemów kontrolnych, zapewniającej szybką i pozbawioną błędów transmisję danych między elementami wykonawczymi i moniturojącymi. W owych czasach była rewolucyjna pod względem niezawodności, łatwości implementacji, szybkości działania, zostając przy tym stosunkowo tanim rozwiązaniem. Jej głownym przeznaczeniem jest przemysł, w szczególności transport i przemysł maszynowy. 
# Cel projektu
Stwórz aplikację pomiarową pozwalającą na komunikację z wykorzystaniem szyny danych CAN. Praca nad zadaniem projektowym będzie wymagała wykorzystania zestawu Arduino + CAN Bus Shield + czujników CAN.
# Urządzenia
![Alt text](http://smartrobots.pl/image/cache/catalog/arduino/leonardo-1-800x800.jpg "Arduino Leonardo")
# Podstawy teoretyczne działania szyny CAN
Specyfikacja CAN definiuje dwie pierwsze warstwy modelu ISO OSI, tj. warstwę fizyczną i warstwę łącza danych. Pozostałe warstwy, poza ostatnią (warstwą aplikacji), której implementacja zależy od projektanta sytemu, są nieużywane. 
1. Warstwa fizyczna określa:
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
  

2. Warstwa łącza danych określa:
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
    
Z punktu widzenia projektu, kluczowe jest rozróżnienie 3 różnych metod projektowania węzła CAN.
** 1. Węzeł zbudowany z trzech elementów - mikrokontrolera, kontrolera CAN i transceivera CAN - wykorzystana metoda.**
2. Węzeł zbudowany z mikrokontrolera z kontrolerem CAN i transceivera.
3. Mikrokontroler z wbudowanym kontrolerem CAN i transceiverem.

    
# Schemat połączeniowy

# Algorytm

# Sposób uruchomienia
