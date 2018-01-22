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

# Algorytm

# Sposób uruchomienia
