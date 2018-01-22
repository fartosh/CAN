# CAN
CAN (Controller Area Network) to szeregowa magistrala komunikacyjna opracowana przez firmę Bosch GmbH w latach 80 XXw. Powstała ona w odpowiedzi na potrzebę znalezienia odpowiedniej metody komunikacji dla systemów kontrolnych, zapewniającej szybką i pozbawioną błędów transmisję danych między elementami wykonawczymi i moniturojącymi. W owych czasach była rewolucyjna pod względem niezawodności, łatwości implementacji, szybkości działania, zostając przy tym stosunkowo tanim rozwiązaniem. Jej głownym przeznaczeniem jest przemysł, w szczególności transport i przemysł maszynowy. 
# Cel projektu
Stwórz aplikację pomiarową pozwalającą na komunikację z wykorzystaniem szyny danych CAN. Praca nad zadaniem projektowym będzie wymagała wykorzystania zestawu Arduino + CAN Bus Shield + czujników CAN.
# Urządzenia
Arduino Leonardo
![Alt text](images/leonardo-1-800x800.jpg "Arduino Leonardo")

Arduino Uno
![Alt text](images/uno.jpg "Arduino Uno")

Arduino CAN-Bus Shield
![Alt text](images/shield-can-2-800x800.jpg "Arduino CAN-Bus Shield")

Przewód db9-db9

![Alt text](images/db9.jpg "Przewód db9-db9")

# Podstawy teoretyczne magistrali CAN
Specyfikacja CAN definiuje dwie pierwsze warstwy modelu ISO OSI, tj. warstwę fizyczną i warstwę łącza danych. Pozostałe warstwy, poza ostatnią (warstwą aplikacji), której implementacja zależy od projektanta systemu, są nieużywane. 

1. Warstwa fizyczna określa:
- topologię sieci - CAN jest magistralą, czyli wszystkie urządzenia połączonne są szeregowo wpólną szyną transmisyjną. Szyna składa się z dwóch lini: CAN_L i CAN_H. Minimalną liczbą przewodów do realizacji sieci CAN jest zatem kabel dwużyłowy. W celu eliminacji odbicia sygnałów na końcach magistrali stosuje się rezystory terminujące.
- prędkość transmisji
- zasięg transmisji
- kodowanie
- stany sygnału magistrali

2. Warstwa łącza danych określa:
- format ramki CAN
- model komunikacji
- dostęp do medium
- wykrywanie błędów

# Schemat połączeniowy
![Alt text](images/schemat.png "Schemat połączeniowy")

# Algorytm

# Sposób uruchomienia
