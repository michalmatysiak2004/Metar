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
    for (int i = 0; i < 3; ++i) {
        if (data[i].airport_code[0] == 'E') {
            cout << "GDANSK" << endl;
        }
        cout << "Dzien: " << data[i].day << " godzina: " << data[i].hour
            << ":" << (data[i].minutes < 10 ? "0" : "") << data[i].minutes << endl;
        cout << "Wiatr: " << data[i].angle << " stopni, " << data[i].velocity
            << " wezlow" << endl;
        cout << "Temperatura: " << data[i].temperature << "°C" << endl;
        cout << "Temperatura punktu rosy: " << data[i].rose_temperature << "°C" << endl;
        cout << "-------------------------------------" << endl;
    }
}

void switchlastmetars(struct Data * lasts, struct Data current,int counter) { 
            lasts[2] = lasts[1];
            lasts[1] = lasts[0];
            lasts[0] = current;
   
}


void save_metar(struct Data data, ofstream& file) {
    if (data.airport_code[0] == 'E') {
        file << "GDANSK" << endl;
    }
    file << "Dzien: " << data.day << "godzina: " << data.hour << ":"
        << data.minutes << endl;
    file << "Wiatr: " << data.angle << " stoopni, " << data.velocity << " wezlow" << endl;
    file << "Temperature: " << data.temperature << "°C" << endl;
    file << "Rose Temperature: " << data.rose_temperature << "°C" << endl;
};

struct Data handle_word(const char* word, struct  Data data) {
    if (data.airport_code[0] == 0) {
        for (int i = 0; i < 4; i++) {
            data.airport_code[i] = word[i];
        }
    }
    else if (data.day == 0) {
        char day_str[3] = { word[0], word[1], '\0' }; 
        char hour_str[3] = { word[2], word[3], '\0' }; 
        char minutes_str[3] = { word[4], word[5], '\0' }; 

        data.day = atoi(day_str);      
        data.hour = atoi(hour_str);   
        data.minutes = atoi(minutes_str);
    }
    else if (word[strlen(word)-2] == 'K' && word[strlen(word)-1] == 'T') {
        
      
        char angle_str[4] = { word[0], word[1], word[2], '\0' }; 
        char vel_str[3] = { word[3], word[4], '\0' };       

        data.angle = atoi(angle_str); 
        data.velocity= atoi(vel_str);    
    }
    else if (word[2] == '/') { 
        
        char temp_str[3] = { word[0], word[1], '\0' };  
        char rose_temp_str[3] = { word[3], word[4], '\0' };  

        data.temperature = atoi(temp_str);  
        data.rose_temperature = atoi(rose_temp_str);  
    }
        
    return data;
}

void program() {
    ifstream file;
    file.open("dane.txt");
    ofstream file2;
    file2.open("wyniki.txt", ios::app); 


    Data lastmetars[3]; // tablica 3 ostatnich metarów
    Data data;



    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku dane.txt" << endl;
        return; // Zakończ funkcję, jeśli nie można otworzyć pliku
    }
    if (!file2.is_open()) {
        cerr << "Nie mozna otworzyc pliku wyniki.txt" << endl;
        file.close();
        return; // Zakończ funkcję, jeśli nie można otworzyć pliku wynikowego
    }

    char single_metar[256]; // Bufor do przechowywania linii z pliku
    int counter = 0;
    while (file.getline(single_metar, sizeof(single_metar))) {
        
        char* context = nullptr; // Kontekst wymagany przez strtok_s
        char* token = strtok_s(single_metar, " ", &context);
        while (token != nullptr) {
            data = handle_word(token, data); // Przekazujemy wyraz do funkcji handle_word
            token = strtok_s(nullptr, " ", &context);
        }
        
        switchlastmetars(lastmetars,data,counter);
        counter++;
        print_metar(lastmetars);

        
        cout << "Nacisnij Enter, aby zapisac ten METAR do pliku..." << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Czekaj na Enter

        save_metar(data, file2); // Zapisz pojedynczy METAR do pliku
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

