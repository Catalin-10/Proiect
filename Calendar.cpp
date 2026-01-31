#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct Eveniment {
    int ane;
    int lunae;
    int zie;
    string text;
};

struct Sarbatoare {
    int luna;
    int zi;
    string nume;
};

int zileInLuna(int luna, int an) {
    switch (luna) {
    case 0: return 31;
    case 1: return ((an % 4 == 0 && an % 100 != 0) || (an % 400 == 0)) ? 29 : 28;
    case 2: return 31;
    case 3: return 30;
    case 4: return 31;
    case 5: return 30;
    case 6: return 31; 
    case 7: return 31;
    case 8: return 30;
    case 9: return 31;
    case 10: return 30;
    case 11: return 31;
    default: return 0;
    }
}

bool areEveniment(int an, int luna, int zi, const vector <Eveniment>& Evenimente) {
    for (const auto& e : Evenimente) {
        if (e.ane == an && e.lunae == luna && e.zie == zi) {
            return true;
        }
    }
    return false;
}

vector<Sarbatoare> sarbatoriRO() {
    return {
        {0, 1, "Anul Nou"},
        {0, 2, "Anul Nou"},
        {0, 24, "Unirea Principatelor Romane"},
        {4, 1, "Ziua Muncii"},
        {5, 1, "Ziua Copilului"},
        {7, 15, "Adormirea Maicii Domnului"},
        {10, 30, "Sf. Andrei"},
        {11, 1, "Ziua Nationala"},
        {11, 25, "Craciun"},
        {11, 26, "Craciun"}
    };  
}

bool esteSarbatoare(int luna, int zi, const vector<Sarbatoare>& sarbatori) {
    for (const auto& s : sarbatori) {
        if (s.luna == luna && s.zi == zi) {
            return true;
        }
    }
    return false;
}

int ZileLucratoare(int an, int luna,const vector<Sarbatoare>& sarbatori) {
    tm data = {};
    data.tm_year = an - 1900;
    data.tm_mon = luna;
    int total = 0;
    int zile = zileInLuna(luna, an);
    for (int zi = 1; zi <= zile; zi++) {
        data.tm_mday = zi;
        mktime(&data);
        bool weekend = (data.tm_wday == 0 || data.tm_wday == 6);
        bool sarbatoare = esteSarbatoare(luna, zi, sarbatori);
        if (!weekend && !sarbatoare) {
            total++;
        }
    }
    return total;
}

int ZileLibere(int an, int luna, const vector<Sarbatoare>& sarbatori) {
    tm data = {};
    data.tm_year = an - 1900;
    data.tm_mon = luna;
    int total = 0;
    int zile = zileInLuna(luna, an);
    for (int zi = 1; zi <= zile; zi++) {
        data.tm_mday = zi;
        mktime(&data);
        bool weekend = (data.tm_wday == 0 || data.tm_wday == 6);
        bool sarbatoare = esteSarbatoare(luna, zi, sarbatori);
        if (weekend || sarbatoare) {
            total++;
        }
    }
    return total;   
}

void exportText(int an, int luna, const vector<Eveniment>& ev, const string& file) {
    ofstream out(file);
    if (!out) {
        return;
    }
    out << "Calendar " << luna + 1 << "/" << an << "\n";
    out << "Lun Mar Mie Joi Vin Sam Dum\n";
    tm d = {};
    d.tm_year = an - 1900;
    d.tm_mon = luna;
    d.tm_mday = 1;
    mktime(&d);
    int spatii = (d.tm_wday + 6) % 7;
    for (int i = 0; i < spatii; i++) {
        out << "    ";
    }
    int col = spatii;
    for (int zi = 1; zi <= zileInLuna(luna, an); zi++) {
        if (areEveniment(an, luna, zi, ev))
            out << "*" << setw(2) << zi << "*";
        else if (col >= 5)
            out << "[" << setw(2) << zi << "]";
        else
            out << " " << setw(2) << zi << " ";

        col++;
        if (col > 6) {
            out << "\n";
            col = 0;
        }
    }
    out << "\n";
    out.close();
}

void CalendarComplet(int an, int luna, const vector<Eveniment>& Evenimente) {
    static const string luni[] = {
        "Ianuarie", "Februarie", "Martie", "Aprilie",
        "Mai", "Iunie", "Iulie", "August",
        "Septembrie", "Octombrie", "Noiembrie", "Decembrie"
    };
    tm data = {};
    data.tm_year = an - 1900;
    data.tm_mon = luna;
    data.tm_mday = 1;
    mktime(&data);
    int tm_wday = data.tm_wday;
    int spatii = (tm_wday + 6) % 7;
    int zile_luna = zileInLuna(luna, an);
    cout << "\n=====================================\n";
    cout << luni[luna] << " " << an << endl;
    cout << "=====================================\n";
    cout << "Lun Mar Mie Joi Vin Sam Dum\n";
    for (int i = 0; i < spatii; i++) {
        cout << "    ";
    }
    int col = spatii;
    for (int zi = 1; zi <= zile_luna; zi++) {
        bool event = areEveniment(an, luna, zi, Evenimente);
        if (event)
            cout << "*" << setw(2) << zi << "*";
        else if (col >= 5)
            cout << "[" << setw(2) << zi << "]";
        else
            cout << " " << setw(2) << zi << " ";
        col++;
        if (col > 6) {
            cout << endl;
            col = 0;
        }
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    int an = 0;
    int luna = 0;
    vector<Eveniment> Evenimente;
    bool viewFull = false;
    bool afiseazaSarbatori = false;
    string exportFormat = "", outputFile = "";
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--year" && i + 1 < argc) {
            an = stoi(argv[++i]);
        }
        else if (arg == "--month" && i + 1 < argc) {
            luna = stoi(argv[++i]) - 1;
        }
        else if (arg == "--view" && i + 1 < argc) {
            if (string(argv[++i]) == "full") {
                viewFull = true;
            }
        }
        else if (arg == "--export") {
            exportFormat = argv[++i];
        }
        else if (arg == "--output") {
            outputFile = argv[++i];
        }
        else if (arg == "--add_event" && i + 2 < argc) {
            string data = argv[++i];
            string text = argv[++i];
            Eveniment e;
            e.ane = stoi(data.substr(0, 4));
            e.lunae = stoi(data.substr(5, 2)) - 1;
            e.zie = stoi(data.substr(8, 2));
            e.text = text;
            Evenimente.push_back(e);
        }
        else if(arg=="--holidays") {
            afiseazaSarbatori = true;
        }
    }
    vector<Sarbatoare> sarbatori = sarbatoriRO();
    if (afiseazaSarbatori) {
        cout << "Sarbatori legale Romania " << an << ":\n";
        for (const auto& s : sarbatori) {
            cout << s.zi << " - " << s.nume << endl;
        }
        cout << "Total: " << sarbatori.size() << " zile libere legale\n";
        return 0;
    }
    if (viewFull) {
        for (int l = 0; l < 12; l++) {
            CalendarComplet(an, l, Evenimente);
        }
        return 0;
    }
    if (exportFormat == "text" && !outputFile.empty()) {
        exportText(an, luna, Evenimente, outputFile);
        cout << "Export realizat in " << outputFile << endl;
        return 0;
    }
    tm data = {};
    data.tm_year = an - 1900;
    data.tm_mon = luna;
    data.tm_mday = 1;
    mktime(&data);
    int tm_wday = data.tm_wday;
    int spatii = (tm_wday + 6) % 7;
    int zile_luna = zileInLuna(luna, an);
    cout << "Lun Mar Mie Joi Vin Sâm Dum\n";
    for (int i = 0; i < spatii; i++) cout << "    ";
    int col_zi = spatii;
    for (int zi = 1; zi <= zile_luna; zi++) {
        bool event = areEveniment(an, luna, zi, Evenimente); 
        if (event) {
            cout << "*" << setw(2) << zi << "*";
        }
        else if (col_zi >= 5) {
            cout << "[" << setw(2) << zi << "]";
        }
        else {
            cout << " " << setw(2) << zi << " ";
        }
        col_zi++;
        if (col_zi > 6) {
            cout << endl;
            col_zi = 0;
        }
    }
    cout << endl;
    if (!Evenimente.empty()) {
        cout << "Evenimente in aceasta luna:\n";
        for (const auto& e : Evenimente) {
            if (e.ane == an && e.lunae == luna) {
                cout << e.zie << " - " << e.text << endl;
            }
        }
    }
    cout << "Zile lucratoare: " << ZileLucratoare(an, luna,sarbatori) << endl;
    cout << "Zile libere: " << ZileLibere(an, luna, sarbatori) << endl;
    return 0;
}
