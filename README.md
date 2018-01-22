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
  - format ramki CAN
  - model komunikacji
  - dostęp do medium
  - wykrywanie błędów
# Schemat połączeniowy

# Algorytm

# Sposób uruchomienia
