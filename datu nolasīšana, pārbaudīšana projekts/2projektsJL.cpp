// Jānis Lejniekks 4.grupa 211REc027
#include <iostream>
#include <fstream> // atļauj piekļuve failu izvadei, ievadei
#include <sstream> //csv  sadalīšanai
#include <vector> // sarakstu funkcionalitātes 
#include <string> 
#include <algorithm> // std::sort, std::find
#include <iomanip> // setprecision(2) noapaļo pareizi cenu
#include <limits>  //numeric_limits

using namespace std;

//precīzs maršruta ieraksts
struct Route {
    string start;
    string end;
    string day;
    string time;
    double price;
};
//funkcija datu ielādei no faila
vector<Route> loadRoutes(const std::string &filename, const std::string &errorFile) {
    vector<Route> routes; //] vektors lai saglabātu "routes"
    ofstream errFile(errorFile); // atver error Failu 
    ifstream inFile; //ifstream, lai lasītu failu
    inFile.open("db.csv"); 

    if (!inFile) { // Pārbauda vai failu var atvērt
        return routes; 
    }
    //Faila lasīšana, errFile izveide
    string line; // lai saglabātu faila līnijas
    while (getline(inFile, line)) { //lasa failu līniju pa līnijai
        istringstream ss(line); // string --> streamline
        string start, end, day, time, priceStr; 
        double price;
        

        // noņem atstarpes un dzēš tukšās līnijas
       auto trim = [](std::string &str) {
            if (!str.empty()) {
                str.erase(0, str.find_first_not_of(" \t"));  // Noņem sākuma atstarpes
                str.erase(str.find_last_not_of(" \t") + 1); // Noņem beigu atstarpes
            }    
        };
        trim(line);
        if (line.empty()) continue; // Ignorē tukšās rindas

         //pārbauda vai ir nepareizas rindas(komatu skaitu), tukšas rindas
         int commaCount = count(line.begin(), line.end(), ',');
            if (commaCount != 4) { // Ja nepareizs komatu skaits, izvada pilnu rindu
            errFile << line << "\n";
            continue;
        }

        if (getline(ss, start, ',') && getline(ss, end, ',') && getline(ss, day, ',') &&
            getline(ss, time, ',') && getline(ss, priceStr)) {
            trim(start);
            trim(end);
            trim(day);
            trim(time);
            trim(priceStr);

            //Day pārbaude 
        if (day != "Pr" && day != "Ot" && day != "Tr" && day != "Ce" && 
            day != "Pt" && day != "St" && day != "Sv") {
            errFile << line << "\n";
            continue;
        }

            // Price pārbaude
        try {
            price = stod(priceStr); //mēģina pārvērst price uz double tipu
        } catch (...) {
            errFile << line << "\n"; //nepareizi cenu ieraksti uz errorFile
            continue;
        }

            // Time pārbaude
        if (time.size() != 5 || time[2] != ':' || !isdigit(time[0]) || 
            !isdigit(time[1]) || !isdigit(time[3]) || !isdigit(time[4])) {   // pārbauda pareizo formātu kā 00:00
            errFile << line << "\n"; //nepareizi ieraksti --> errFile
            continue;
        }
            //datu pieveinošana routes vektoram (pareizais)
            routes.push_back({start, end, day, time, price});
        } 
    }
    return routes; // atgriež vektoru(sarakstu) ar pareizajiem ierakstiem
}

//Funkcija vaicājumam "a" ROUTE
void routesStops(const vector<Route> &routes, const string &start, const string &end) {
    for (const auto &route : routes) {
        if (route.start == start && route.end == end) {
            cout << route.start << " " << route.end << " " << route.day << " " << route.time << " " << fixed << setprecision(2) << route.price << endl; //izvades struktūra!! price preceision 2 ciapri aiz komata
        }
    }
}

//Funkcija vaicājumam  "b" DAY
void routesDay(const vector<Route> &routes, const string &day) {
    for (const auto &route : routes) {
        if (route.day == day) {
            cout << route.start << " " << route.end << " " << route.day << " " << route.time << " " << fixed << setprecision(2) << route.price << endl;
        }
    }
}

//Funkcija vaicājumam "c" PRICE
void routesPrice(const vector<Route> &routes, double maxPrice) {
    for (const auto &route : routes) {
        if (route.price <= maxPrice) { //izvada visu zem ievadītās cenas
            cout << route.start << " " << route.end << " " << route.day << " " << route.time << " " << fixed << setprecision(2) << route.price << endl;
        }
    }
}

//Funkcija vaicājumam "d" ERROR FILE
void printErrorFile(const string &errorFile) {
    ifstream file(errorFile);
    string line;
    while (getline(file, line)) {
        cout << line << endl; //jauna līnija
    }
}

int main() {
    const string dbFile = "db.csv";
    const string errorFile = "err.txt"; //

    vector<Route> routes = loadRoutes(dbFile, errorFile);    //Ielādē maršrutus
    string input;  // ievadei
    char command; 

    do {
        cout << "Ievadiet komandu (a, b, c, d, e): ";
        getline(cin, input);  // ievades līnija
        command = tolower(input[0]); // tolower
        if (input.size() > 1) {  // izveidots, lai izvairītos no kļūdām kuras veidojās ievadot vārdus kas sākas ar a,b,c,d,e.
            cout << "Komanda neder" << endl;
            continue;
        }
        if (command == 'a') {
            string start, end;
            cout << "Ievadiet sākuma maršrutu un beigu maršrutu: ";
            cin >> start >> end;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignorē nepareizu ievadi
            cout << "result:" << endl;
            routesStops(routes, start, end);
        } else if (command == 'b') {
            string day;
            cout << "Ievadiet dienu: (Pr),(Ot),(Tr),(Ce),(Pt),(St),(Sv): ";
            cin >> day;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "result:" << endl;
            routesDay(routes, day);
        } else if (command == 'c') {
            double maxPrice;
            // kļūdu apstrāde
            bool validInput = false; // validInput
            while (!validInput) {
                cout << "Ievadiet maksimālo cenu biļetei: ";
                cin >> maxPrice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                if (cin.fail()) { //ja ievada string tipu double tipa vietā
                    cin.clear(); // nodzēš kļūdas
                    cout << "Nepareiza cena, ievadiet derīgu cenu.\n";
                } else {
                    validInput = true;  // cikls beidzas
                }
            }
            // izvade ar pareiuz ievadi tikai
            cout << "result:" << endl;
            routesPrice(routes, maxPrice); 
        } else if (command == 'd') {
            cout << "result:" << endl;
            printErrorFile(errorFile);
        } else if (command == 'e') {
            cout << endl;
        } else {
            cout  << "Nepareiza komanda,ievadiet a, b, c, d vai e: ."  << endl;
        }
    } while (command != 'e');

    return 0;
}