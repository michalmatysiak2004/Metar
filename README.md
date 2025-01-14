# Weather METAR Program

## Opis projektu
Program umożliwia analizę danych pogodowych w formacie METAR oraz ich generowanie i przetwarzanie. Obsługuje dane dla różnych lotnisk w Polsce, umożliwiając ich odczyt, wyświetlanie oraz zapis w zorganizowany sposób.

## Funkcjonalności
1. **Obsługa menu**:
   - Opcja `q`: Zamyka program.
   - Opcja `o`: Odczytuje dane z pliku , wyświetla przystępne dla użytkownika dane na ekranie oraz zapisuje je do pliku .
   - Opcja 'x' : Wprowadzamy dane, aby wygenerowac Metar.
2. **Odczyt i zapis danych pogodowych**:
   - Wyświetlanie i zapis takich informacji jak:
     - Dzień i czas UTC wykonania pomiaru.
     - Kierunek i prędkość wiatru.
     - Temperatura.
     - Ciśnienie atmosferyczne.
     - Widzialność w metrach.
     - Zjawiska atmosferyczne.
     - Wielkość zachmurzenia.

4. **Generowanie METAR**:
   - Tworzenie depeszy METAR na podstawie danych wprowadzonych przez użytkownika:
     - Nazwa lotniska (np. Gdańsk → EPGD).
     - Dzień miesiąca i czas UTC.
     - Kierunek i prędkość wiatru.
     - Temperatura.
     - Ciśnienie atmosferyczne.

5. **Zapis danych w plikach**:
   - Dane zapisane są w sposób uporządkowany, np. w plikach `pogoda_gd.txt` czy `pogoda_lotnisko.txt`.

6. **Przetwarzanie danych dla wielu lotnisk**:
   - Analiza danych METAR dla różnych lotnisk w Polsce z pliku źródłowego.
   - Grupowanie wyników według nazw lotnisk i zapisywanie ich do plików.

## Wymagania
- Kompilator zgodny z C++11 lub
