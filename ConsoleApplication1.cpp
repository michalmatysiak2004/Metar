// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <fstream>
#include <cstring>


using namespace std;

struct Data { // struct danych 
    char airport_code[4] = { 0 };
    int day=0;
    int hour=0;
    int minutes=0;
    int angle=0;  // kat wiatru 
    int velocity; // predkosc wiatru
    int maxvelocity; // predkosc w porywach
    int visibility; // widocznosc
    int phenomena; // zjawiska atmosferyczne
    int cloudcover;
    int height;
    int temperature;
    int rose_temperature;
};

void print_metar(struct Data* data) {
    
}
void switchlastmetars(struct Data * data, struct Data current) {
    
}
void save_metar(struct Data* data, ofstream& file) {
    
    
    if (data->airport_code[0] == 'E') {
        file << "GDANSK" << endl;
    }
    
    file << "Dzien: " << data->day << "godzina: " << data->hour << ":"
        << data->minutes << endl;
    file << "Wiatr: " << data->angle << " stoopni, " << data->velocity << " wezlow" << endl;
    file << "Temperature: " << data->temperature << "°C" << endl;




    file << "Rose Temperature: " << data->rose_temperature << "°C" << endl;
};

void handle_word(const char* word, struct  Data* data) {
    if (data->airport_code[0] == 0) {
        for (int i = 0; i < 4; i++) {
            data->airport_code[i] = word[i];
        }
    }
    else if (data->day == 0) {
        char day_str[3] = { word[0], word[1], '\0' }; // Dzień: pierwsze dwa znaki
        char hour_str[3] = { word[2], word[3], '\0' }; // Godzina: kolejne dwa znaki
        char minutes_str[3] = { word[4], word[5], '\0' }; // Minuty: następne dwa znaki

        data->day = std::atoi(day_str);      // Konwersja na liczbę całkowitą
        data->hour = std::atoi(hour_str);    // Konwersja na liczbę całkowitą
        data->minutes = std::atoi(minutes_str); // Konwersja na liczbę całkowitą
    }
    else if (word[strlen(word)-2] == 'K' && word[strlen(word)-1] == 'T') {
        
        // Sprawdź, czy na końcu jest "KT"
        // Zakładając, że word zawiera kierunek i prędkość (np. "25009KT")
        char angle_str[4] = { word[0], word[1], word[2], '\0' }; // Kierunek: pierwsze trzy znaki
        char vel_str[3] = { word[3], word[4], '\0' };        // Prędkość: kolejne dwa znaki

        data->angle = std::atoi(angle_str);  // Konwersja na liczbę całkowitą (kierunek wiatru)
        data->velocity= std::atoi(vel_str);     // Konwersja na liczbę całkowitą (prędkość wiatru)
    }
    else if (word[2] == '/') {  // Przetwarzanie formatu temperatury punktu rosy
        // Zakładając, że word jest w formacie "03/02" gdzie 03 to temperatura, a 02 to temperatura punktu rosy
        char temp_str[3] = { word[0], word[1], '\0' };  // Temperatura: pierwsze dwa znaki
        char rose_temp_str[3] = { word[3], word[4], '\0' };  // Temperatura punktu rosy: kolejne dwa znaki

        data->temperature = std::atoi(temp_str);  // Konwersja na temperaturę
        data->rose_temperature = std::atoi(rose_temp_str);  // Konwersja na temperaturę punktu rosy
    }
        
    
}

void program() {
    std::ifstream file;
    file.open("dane.txt");
    std::ofstream file2;
    file2.open("wyniki.txt", std::ios::app); // Otwórz plik w trybie dopisywania
    Data lastmetars[3];
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku dane.txt" << std::endl;
        return; // Zakończ funkcję, jeśli nie można otworzyć pliku
    }
    if (!file2.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku wyniki.txt" << std::endl;
        file.close();
        return; // Zakończ funkcję, jeśli nie można otworzyć pliku wynikowego
    }

    char single_metar[256]; // Bufor do przechowywania linii z pliku
    while (file.getline(single_metar, sizeof(single_metar))) {
        Data* data = new Data;
        char* context = nullptr; // Kontekst wymagany przez strtok_s
        char* token = strtok_s(single_metar, " ", &context);
        while (token != nullptr) {
            handle_word(token, data); // Przekazujemy wyraz do funkcji handle_word
            token = strtok_s(nullptr, " ", &context);
        }

        switchlastmetars(lastmetars,);
        print_metar(data);

        
        std::cout << "Nacisnij Enter, aby zapisac ten METAR do pliku..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Czekaj na Enter

        save_metar(data, file2); // Zapisz pojedynczy METAR do pliku
        delete data;
        cout << endl;
    }
    file.close();
    file2.close();
}



void menu() {
    cout << "Imie Nazwisko Indeks" << endl;
    cout << "q - wyjdź z programu" << endl;
    cout << "o - wczytaj dane" << endl;
    char A;

    while (true) {
        cin >> A;
        if (A == 'q') {
            exit(0);
        }
        if (A == 'o') {
            system("cls");
            program();
            
        }
    }

}
int main()
{
    menu();
}

