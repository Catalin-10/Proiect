#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>
using namespace std;

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

int main(int argc, char* argv[]) {
    int an = 0;
    int luna = 0;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--year" && i + 1 < argc) {
            an = stoi(argv[++i]);
        }
        else if (arg == "--month" && i + 1 < argc) {
            luna = stoi(argv[++i]) - 1;
        }
    }
    if (an == 0 || luna < 0 || luna > 11) {
        cout << "Usage: --month <1-12> --year <YYYY>\n";
        return 1;
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
        if (col_zi >= 5) cout << "[" << setw(2) << zi << "]";
        else cout << " " << setw(2) << zi << " ";
        col_zi++;
        if (col_zi > 6) {
            cout << endl;
            col_zi = 0;
        }
    }
    cout << endl;
    return 0;
}
