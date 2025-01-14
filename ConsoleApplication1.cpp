#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

struct Data { // struct danych 
    char airport_code[5] = { 0 };
    int day=0;
    int hour=0;
    int minutes = 0;
    int angle=-1;  // kat wiatru 
    int lowangle = -1;
    int highangle = -1;
    bool lowhighangles = false;
    int velocity; // predkosc wiatru
    int maxvelocity; // predkosc w porywach
    int visibility = 0; // widocznosc
    bool visibility_set = false;
    bool okclouds = false;
    int diffrentcloudcovers = 0;
    char cloudcover[5];
    int height[5];
    int temperature;
    int rose_temperature;
    int preasure;
    char weather_phenomena[5][5];
    char intensity[5][2];
    int phen_count = 0;
    bool phenomena_set;
    bool nosig = false;
    bool nsc = false;
};
struct WeatherEvent {
    const char* code;
    const char* description;
};

struct Airports {
    const char* code;
    const char* name;
};
Airports polishAirports[] = {
       {"EPWA", "Warszawa-Lotnisko Chopina"},
       {"EPMO", "Warszawa-Modlin"},
       {"EPGD", "Gdańsk-Rębiechowo"},
       {"EPKT", "Katowice-Pyrzowice"},
       {"EPKK", "Kraków-Balice"},
       {"EPWR", "Wrocław-Strachowice"},
       {"EPRZ", "Rzeszów-Jasionka"},
       {"EPPO", "Poznań-Ławica"},
       {"EPSC", "Szczecin-Goleniów"},
       {"EPBY", "Bydgoszcz-Szwederowo"},
       {"EPZG", "Zielona Góra-Babimost"},
       {"EPRA", "Radom-Sadków"},
};

WeatherEvent events[] = {
    {"DZ", "mżawka"},
    {"RA", "deszcz"},
    {"SN", "śnieg"},
    {"SG", "śnieg ziarnisty"},
    {"IC", "słupki lodowe"},
    {"GR", "grad"},
    {"PL", "deszcz lodowy"},
    {"GS", "krupa śnieżna/lodowa"},
    {"BR", "zamglenie"},
    {"FG", "mgła"},
    {"FU", "dym"},
    {"VA", "popiół wulkaniczny"},
    {"SA", "piasek"},
    {"HZ", "zmętnienie"},
    {"DU", "pył"},
    {"SQ", "nawałnica"},
    {"FC", "trąba powietrzna/wodna"},
    {"DS", "burza pyłowa"},
    {"PO", "silnie rozwinięte wiry pyłowe/piaskowe"},
    {"SS", "burza piaskowa"},
    {"UP", "nieznane opady"}
};
 WeatherEvent intensivity [] = {
    {"+", "silna/silny"},
    {"-", "lekka/lekki"},
    {"MI", "płycizna"},
    {"SH", "opad przelotny"},
    {"DR", "zamieć przyziemna"},
    {"Bl", "zawieja"},
    {"FZ", "zamrażanie"},
    {"BC","plastry"}
};
 
int word_lenght(char* wyraz) {
     int dlugosc = 0;
     while (wyraz[dlugosc] != '\0') {
         dlugosc++;
     }
     return dlugosc;
 }

const char* findDescriptionIntensivity(const char* code) {
     for (int i = 0; i < sizeof(intensivity) / sizeof(intensivity[0]); ++i) {
         if (strcmp(intensivity[i].code, code) == 0) {
             return intensivity[i].description;
         }
     }
     return "??";
}
const char* findDescriptionEvents(const char* code) {
    for (int i = 0; i < sizeof(events) / sizeof(events[0]); ++i) {
        if (strcmp(events[i].code, code) == 0) {
            return events[i].description;
        }
    }
    return "??";
}

const char* findAirports(const char* code) {
    for (int i = 0; i < sizeof(polishAirports) / sizeof(polishAirports[0]); ++i) {
        if (strcmp(polishAirports[i].code, code) == 0) {
            return polishAirports[i].name;
        }
    }
    return "??";
}
const char* findAirportsCode(const char* name) {
    for (int i = 0; i < sizeof(polishAirports) / sizeof(polishAirports[0]); ++i) {
        if (strcmp(polishAirports[i].name, name) == 0) {
            return polishAirports[i].code;
        }
    }
    return "??";
}


/*void print_metar2(struct Data* data) {
    for (int i = 0; i < 3; ++i) {
        if (data[i].airport_code[0] == 'E') {
            cout << findAirports(data[i].airport_code) << endl;
           
        }
        cout << "Dzien: " << data[i].day << " godzina: " << data[i].hour
            << ":" << (data[i].minutes < 10 ? "0" : "") << data[i].minutes << endl;
        cout << "Wiatr: ";
        if (data[i].angle == -2) {
            cout << "zmienia się o więcej niż 180° lub  nie można określić kierunku.";
        }
        else {
            cout << data[i].angle << "°";
        }
     
        cout <<"prędkość  " << data[i].velocity << " wezlow" << endl;
        if (data[i].lowhighangles == true) {
            cout << "Skrajne wartości kierunki wiatru od: " << data[i].lowangle << " do " << data[i].highangle << endl;
        };
        cout << "Temperatura: " << data[i].temperature << "°C" << endl;
        cout << "Temperatura punktu rosy: " << data[i].rose_temperature << "°C" << endl;
        cout << "Widzialność: ";
        if (data[i].visibility == -1) cout << "dobra" << endl;
        else cout << data[i].visibility << endl;
        
        for (int j = 0; j < data[i].diffrentcloudcovers; j++) {
           
            if (data[i].okclouds == true) cout << "ok";
            else if (data[i].cloudcover[j] == 'S') cout << "Brak zachmurzenia";
            else if (data[i].cloudcover[j] == 'E') cout << "Zachmurzenie o zapelnieniu od  1/8 do 2/8";
            else if (data[i].cloudcover[j] == 'C') cout << "Zachmurzenie o zapelnieniu od  3/8 do 4/8";
            else if (data[i].cloudcover[j] == 'B') cout << "Zachmurzenie o zapelnieniu od  5/8 do 7/8";
            else if (data[i].cloudcover[j] == 'V') cout << "Zachmurzenie całkowite ";
            if(data[i].okclouds == false) cout << "z podstawą chmur " << 100 * data[i].height[j] << " stóp nad terenem" << endl;
            
        }

        cout << "Cisnienie: " << data[i].preasure << "hPa" << endl;






        if(data[i].phen_count>0) cout << "Zjawiska pogodowe: " << endl;
        for (int j = 0; j < data[i].phen_count; j++) {
            if (word_lenght(data[i].weather_phenomena[j]) == 2) {
                cout << findDescriptionEvents(data[i].weather_phenomena[j]);
            }
            if (word_lenght(data[i].weather_phenomena[j]) == 3) {
                char code1[2] = { data[i].weather_phenomena[j][0], '\0' };
                cout << findDescriptionIntensivity(code1) << " ";
                char code2[3] = { data[i].weather_phenomena[j][1] ,data[i].weather_phenomena[j][2] ,'\0' };
                cout << findDescriptionEvents(code2);
            }
            if (word_lenght(data[i].weather_phenomena[j]) == 4) {
                char code1[2] = { data[i].weather_phenomena[j][0], '\0' };
                cout << findDescriptionIntensivity(code1) << " ";
                char code2[3] = { data[i].weather_phenomena[j][1] ,data[i].weather_phenomena[j][2] ,'\0' };
                cout << findDescriptionEvents(code2);
            }
           
            cout << endl;
           
           


            
        }
        
        cout << endl << "-------------------------------------" << endl;
    }
}
*/
void print_metar(struct Data data) {
    
        if (data.airport_code[0] == 'E') {
            cout << findAirports(data.airport_code) << endl;

        }
        cout << "Dzien: " << data.day << " godzina: " << data.hour
            << ":" << (data.minutes < 10 ? "0" : "") << data.minutes << endl;
        cout << "Wiatr: ";
        if (data.angle == -2) {
            cout << "zmienia się o więcej niż 180° lub  nie można określić kierunku.";
        }
        else {
            cout << data.angle << "°";
        }

        cout <<"prędkość  " << data.velocity << " wezlow" << endl;
        if (data.lowhighangles == true) {
            cout << "Skrajne wartości kierunki wiatru od: " << data.lowangle << " do " << data.highangle << endl;
        };
        cout << "Temperatura: " << data.temperature << "°C" << endl;
        cout << "Temperatura punktu rosy: " << data.rose_temperature << "°C" << endl;
        cout << "Widzialność: ";
        if (data.visibility == -1) cout << "dobra" << endl;
        else cout << data.visibility << endl;
        if (data.nsc == true) cout <<  "Brak znaczących chmur" << endl;
        for (int j = 0; j < data.diffrentcloudcovers; j++) {

            if (data.okclouds == true) cout << "ok";
            else if (data.cloudcover[j] == 'S') cout << "Brak zachmurzenia";
            else if (data.cloudcover[j] == 'E') cout << "Zachmurzenie o zapelnieniu od  1/8 do 2/8";
            else if (data.cloudcover[j] == 'C') cout << "Zachmurzenie o zapelnieniu od  3/8 do 4/8";
            else if (data.cloudcover[j] == 'B') cout << "Zachmurzenie o zapelnieniu od  5/8 do 7/8";
            else if (data.cloudcover[j] == 'V') cout << "Zachmurzenie całkowite ";
            if(data.okclouds == false) cout << "z podstawą chmur " << 100 * data.height[j] << " stóp nad terenem" << endl;

        }

        cout << "Cisnienie: " << data.preasure << "hPa" << endl;






        if(data.phen_count>0) cout << "Zjawiska pogodowe: " << endl;
        for (int j = 0; j < data.phen_count; j++) {
            if (word_lenght(data.weather_phenomena[j]) == 2) {
                cout << findDescriptionEvents(data.weather_phenomena[j]);
            }
            if (word_lenght(data.weather_phenomena[j]) == 3) {
                char code1[2] = { data.weather_phenomena[j][0], '\0' };
                cout << findDescriptionIntensivity(code1) << " ";
                char code2[3] = { data.weather_phenomena[j][1] ,data.weather_phenomena[j][2] ,'\0' };
                cout << findDescriptionEvents(code2);
            }
            if (word_lenght(data.weather_phenomena[j]) == 4) {
                char code1[2] = { data.weather_phenomena[j][0], '\0' };
                cout << findDescriptionIntensivity(code1) << " ";
                char code2[3] = { data.weather_phenomena[j][1] ,data.weather_phenomena[j][2] ,'\0' };
                cout << findDescriptionEvents(code2);
            }

            cout << endl;





        }
        if (data.nosig == true) cout << "Brak przewidywanych znaczących zmian pogody w ciągu najbliższych 2 godz.";
        cout << endl << "-------------------------------------" << endl;
    
}
void switchlastmetars(struct Data * lasts, struct Data current,int counter) { 
            lasts[2] = lasts[1];
            lasts[1] = lasts[0];
            lasts[0] = current;
   
}


void save_metar(struct Data data, ofstream& file) {
    
        if (data.airport_code[0] == 'E') {
            file << findAirports(data.airport_code) << endl;

        }
        file << "Dzien: " << data.day << " godzina: " << data.hour
            << ":" << (data.minutes < 10 ? "0" : "") << data.minutes << endl;
        file << "Wiatr: ";
        if (data.angle == -2) {
            file << "zmienia się o więcej niż 180° lub  nie można określić kierunku.";
        }
        else {
            file << data.angle << "°";
        }

        file << "prędkość  " << data.velocity << " wezlow" << endl;
        if (data.lowhighangles == true) {
            file << "Skrajne wartości kierunki wiatru od: " << data.lowangle << " do " << data.highangle << endl;
        };
        file << "Temperatura: " << data.temperature << "°C" << endl;
        file << "Temperatura punktu rosy: " << data.rose_temperature << "°C" << endl;
        file << "Widzialność: ";
        if (data.visibility == -1) file << "dobra" << endl;
        else file << data.visibility << endl;
        if (data.nsc == true) file << "Brak znaczących chmur" << endl;
        for (int j = 0; j < data.diffrentcloudcovers; j++) {

            if (data.okclouds == true) file << "ok";
            else if (data.cloudcover[j] == 'S') file << "Brak zachmurzenia";
            else if (data.cloudcover[j] == 'E') file << "Zachmurzenie o zapelnieniu od  1/8 do 2/8";
            else if (data.cloudcover[j] == 'C') file << "Zachmurzenie o zapelnieniu od  3/8 do 4/8";
            else if (data.cloudcover[j] == 'B') file << "Zachmurzenie o zapelnieniu od  5/8 do 7/8";
            else if (data.cloudcover[j] == 'V') file << "Zachmurzenie całkowite ";
            if (data.okclouds == false) file << "z podstawą chmur " << 100 * data.height[j] << " stóp nad terenem" << endl;

        }

        file << "Cisnienie: " << data.preasure << "hPa" << endl;






        if (data.phen_count > 0) file << "Zjawiska pogodowe: " << endl;
        for (int j = 0; j < data.phen_count; j++) {
            if (word_lenght(data.weather_phenomena[j]) == 2) {
                file << findDescriptionEvents(data.weather_phenomena[j]);
            }
            if (word_lenght(data.weather_phenomena[j]) == 3) {
                char code1[2] = { data.weather_phenomena[j][0], '\0' };
                file << findDescriptionIntensivity(code1) << " ";
                char code2[3] = { data.weather_phenomena[j][1] ,data.weather_phenomena[j][2] ,'\0' };
                file << findDescriptionEvents(code2);
            }
            if (word_lenght(data.weather_phenomena[j]) == 4) {
                char code1[2] = { data.weather_phenomena[j][0], '\0' };
                file << findDescriptionIntensivity(code1) << " ";
                char code2[3] = { data.weather_phenomena[j][1] ,data.weather_phenomena[j][2] ,'\0' };
                file << findDescriptionEvents(code2);
            }

            file << endl;





        }

        if (data.nosig == true) file << "Brak przewidywanych znaczących zmian pogody w ciągu najbliższych 2 godz.";







        file << endl << "-------------------------------------" << endl;
    
};

struct Data handle_word(const char* word, struct  Data data) {
    if (data.airport_code[0] == 0) {
        for (int i = 0; i < 4; i++) {
            data.airport_code[i] = word[i];
        }
        data.airport_code[4] = '\0';
    }

    else if (data.day == 0) {
        char day_str[3] = { word[0], word[1], '\0' }; 
        char hour_str[3] = { word[2], word[3], '\0' }; 
        char minutes_str[3] = { word[4], word[5], '\0' }; 

        data.day = atoi(day_str);      
        data.hour = atoi(hour_str);   
        data.minutes = atoi(minutes_str);
    }
    // Wiatr
    else if (word[strlen(word)-2] == 'K' && word[strlen(word)-1] == 'T') {
        
      
        char angle_str[4] = { word[0], word[1], word[2], '\0' }; 
        char vel_str[3] = { word[3], word[4], '\0' };       
        if (word[0] == 'V') {
            data.angle = -2;
        }
        else data.angle = atoi(angle_str);
        
        data.velocity= atoi(vel_str);    
    }
    // Porywy wiatru
    else if (word[3] == 'V') {
        data.lowhighangles = true;
        char lowangle_str[3] = { word[0],word[1],word[2] };
        char highangle_str[3] = { word[4],word[5],word[6] };

        data.lowangle = atoi(lowangle_str);
        data.highangle = atoi(highangle_str);
    }

    // Widocznosc

    else if (strlen(word) == 4 && data.visibility_set == false) {
        char vis_str[4] = { word[0], word[1],word[2], word[3] };
        data.visibility = atoi(vis_str);
        data.visibility_set = true;
    }


    else if (strlen(word) == 5 && word[0] == 'C' && word[1] == 'A' && word[2] == 'V' && word[3] == 'O' && word[4] == 'K') {

        data.okclouds = true;
        data.visibility = -1; // dobra widocznosc
        data.visibility_set = true;

    }
    else if (strlen(word) == 5 && word[0] == 'N' && word[1] == 'O' && word[2] == 'S' && word[3] == 'I' && word[4] == 'G') {

        data.nosig = true;

    }
    else if (word[0] == 'N' && word[1] == 'S' && word[2] == 'C') {
        data.nsc = true;

    }
    // temperatura
    else if (word[2] == '/' || word[3] == '/') {
        char temp_str[3];
        if (word[0] == 'M') {
            temp_str[0] = word[1];
            temp_str[1] = word[2];
            temp_str[2] = '\0';
        }
        else {
            temp_str[0] = word[0];
            temp_str[1] = word[1];
            temp_str[2] = '\0';
        }
            
          
        char rose_temp_str[3] = { word[strlen(word)-2], word[strlen(word)-1], '\0'};
        
        data.temperature = atoi(temp_str);  
        data.rose_temperature = atoi(rose_temp_str);  
        if (word[3] == 'M') data.rose_temperature *= -1;
        if (word[0] == 'M') data.temperature *= -1;
    }

    else if (strlen(word) == 6) {
        if (word[1] == 'K') {
            data.cloudcover[data.diffrentcloudcovers] = word[0];
        }
        else data.cloudcover[data.diffrentcloudcovers] = word[1];
        char height_str[3] = { word[3], word[4], word[5] };

        data.height[data.diffrentcloudcovers] = atoi(height_str);
        data.diffrentcloudcovers++;
    }


    

    // cisnienie
    else if (word[0] == 'Q') {
        char pres_str[5] = { word[1], word[2], word[3],word[4], '\0' };
        data.preasure = atoi(pres_str);
    }
    else {
        
        for (int i = 0; i < strlen(word); i++) {
            data.weather_phenomena[data.phen_count][i] = word[i];
        }
        data.weather_phenomena[data.phen_count][strlen(word)] = '\0';
        data.phen_count++;
        
    }
        
    return data;
}

void sortuj_po_nazwie(const char* filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << "\n";
        return;
    }

    ofstream outputFile("sorted.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Nie można otworzyć pliku wyjściowego: sorted.txt\n";
        inputFile.close();
        return;
    }

    char line[256];
    for (size_t i = 0; i < sizeof(polishAirports) / sizeof(Airports); ++i) {
        const char* currentCode = polishAirports[i].code;
        // Resetujemy pozycję w pliku do początku dla każdego kodu
        inputFile.clear();
        inputFile.seekg(0, std::ios::beg);

        while (inputFile.getline(line, sizeof(line))) {
            
            if (std::strncmp(line, currentCode, std::strlen(currentCode)) == 0) {
                outputFile << line << "\n";
            }
        }
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Zapisano posortowane dane w pliku sorted.txt\n";
}
void printnewestthree(const char* filename) {
    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku dane.txt" << endl;
        return;
    }
    char single_metar[256];
    for (int i = 0; i < 4; i++) {
        file.getline(single_metar, sizeof(single_metar));
        Data data;
        char* context = nullptr;
        char* token = strtok_s(single_metar, " ", &context);
        while (token != nullptr) {
            data = handle_word(token, data);
            token = strtok_s(nullptr, " ", &context);
        }
        print_metar(data);
    }
    file.close();
}

void printgenerated() {
    ifstream file;
    file.open("generated_metars.txt");
    char line[256]; // Bufor do przechowywania jednej linii tekstu
    if (file.getline(line, sizeof(line))) { // Pobierz jedną linię tekstu z pliku
        cout << "Wczytana linia: " << line << endl; // Wypisz linię na konsolę
    }
    

    file.close(); // Zamknij plik
}
void generate_metar() {
    ofstream file;
    file.open("generated_metars.txt");
    system("cls");
    char bufor[256];
    int liczba;
    cout << "Podaj: Miasto-NazwaLostnika: ";
    cin >> bufor;
    file << findAirportsCode(bufor) << "  ";
    cout << "Podaj: dzien miesiaca: ";
    cin >> liczba;
    if (liczba < 10) file << "0";
    file << liczba;
    cout << "Podaj godzine: (format godzinaminuta)";
    cin >> liczba;
    file << liczba << "Z";
    // Kierunek i prędkość wiatru
    cout << "Podaj kierunek wiatru (w stopniach): ";
    cin >> liczba;
    if (liczba < 10) file << "00";
    else if (liczba < 100) file << "0";
    file << liczba;

    cout << "Podaj prędkość wiatru (w węzłach): ";
    cin >> liczba;
    if (liczba < 10) file << "0";
    file << liczba << "KT ";

    // Widzialność i chmury
    file << "9999 ";

    // Temperatura i punkt rosy
    cout << "Podaj temperaturę (w °C): ";
    cin >> liczba;
    if (liczba < 0) file << "M" << abs(liczba) << "/";
    else file << liczba << "/";

    cout << "Podaj punkt rosy (w °C): ";
    cin >> liczba;
    if (liczba < 0) file << "M" << abs(liczba) << " ";
    else file << liczba << " ";

    // Ciśnienie atmosferyczne
    cout << "Podaj ciśnienie atmosferyczne (w hPa): ";
    cin >> liczba;
    file << "Q" << liczba << endl;
    file.close();

    printgenerated();

    
}
void program() {
    system("cls");
    char filename_in[256];
    char filename_out[256];

    
    cout << "Podaj nazwę pliku wejsciowego: ";
    cin >> filename_in;
    cout << "Podaj nazwę pliku wyjsciowego: ";
    cin >> filename_out;
    sortuj_po_nazwie(filename_in);
    ifstream file;
    file.open("sorted.txt");
    ofstream file2;
    file2.open(filename_out, ios::app); 
    
    printnewestthree(filename_in);

    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku dane.txt" << endl;
        return; 
    }
    if (!file2.is_open()) {
        cerr << "Nie mozna otworzyc pliku wyniki.txt" << endl;
        file.close();
        return; 
    }

    char single_metar[256]; 
    int counter = 0;
    while (file.getline(single_metar, sizeof(single_metar))) {
        Data data;
        char* context = nullptr; 
        char* token = strtok_s(single_metar, " ", &context);
        while (token != nullptr) {
            data = handle_word(token, data);
            token = strtok_s(nullptr, " ", &context);
        }
        
       // if(counter < 4) print_metar(data);
        counter++;
        

       
        

        
        save_metar(data, file2); 
      
    }
    file.close();
    file2.close();
}


void menu() {
    cout << "Imie Nazwisko Indeks" << endl;
    cout << "q - wyjdź z programu" << endl;
    cout << "o - wczytaj dane" << endl;
    cout << "x - przejdz do generowania metara" << endl;
    char A;

    while (true) {
        cin >> A;
        if (A == 'q') {
            exit(0);
        }
        if (A == 'o') {
            
            program();
            
        }
        if (A == 'x') {
            
            generate_metar();

        }
    }

}
int main()
{
    SetConsoleOutputCP(1250);
    menu();
   
}

