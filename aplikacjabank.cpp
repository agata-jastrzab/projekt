#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Klient {
protected:
    int id;
    string imie;
    string nazwisko;
    double saldo;
    double oprocentowanie;

public:
    Klient(int i, string im, string nazw, double s, double op)
        : id(i), imie(im), nazwisko(nazw), saldo(s), oprocentowanie(op) {}

    virtual ~Klient() {}

    int getId() const { return id; }

    virtual void naliczOprocentowanie() {
        saldo += saldo * (oprocentowanie / 100.0);
    }

    virtual void pokaz() const {
        cout << "ID: " << id << ", " << imie << " " << nazwisko
             << ", Saldo: " << saldo << ", Oprocentowanie: " << oprocentowanie << "%" << endl;
    }

    void wplata(double kwota) { saldo += kwota; }

    void wyplata(double kwota) {
        if (kwota <= saldo)
            saldo -= kwota;
        else
            cout << "Brak srodków!" << endl;
    }

    void przelew(Klient& odbiorca, double kwota) {
        if (kwota <= saldo) {
            saldo -= kwota;
            odbiorca.wplata(kwota);
        } else {
            cout << "Za malo srodków na przelew!" << endl;
        }
    }
};

class KlientVIP : public Klient {
    double dodatkoweOprocentowanie;

public:
    KlientVIP(int i, string im, string nazw, double s, double op, double dop)
        : Klient(i, im, nazw, s, op), dodatkoweOprocentowanie(dop) {}

    void naliczOprocentowanie() override {
        saldo += saldo * ((oprocentowanie + dodatkoweOprocentowanie) / 100.0);
    }

    void pokaz() const override {
        cout << "[VIP] ID: " << id << ", " << imie << " " << nazwisko
             << ", Saldo: " << saldo << ", Oprocentowanie: " << oprocentowanie
             << " + " << dodatkoweOprocentowanie << "%" << endl;
    }
};
// mennu

int znajdzKlienta(const vector<Klient*>& klienci, int id) {
    for (int i = 0; i < klienci.size(); ++i) {
        if (klienci[i]->getId() == id)
            return i;
    }
    return -1;
}

int main() {
    vector<Klient*> klienci;
    int opcja;

    do {
        cout << "\n=== MENU ===\n";
        cout << "1. Dodaj klienta\n";
        cout << "2. Dodaj klienta VIP\n";
        cout << "3. Wplata\n";
        cout << "4. Wyplata\n";
        cout << "5. Przelew\n";
        cout << "6. Naliczenie odsetek\n";
        cout << "7. Wyswietl wszystkich\n";
        cout << "0. Wyjscie\n";
        cout << "Opcja: ";
        cin >> opcja;

        if (opcja == 1) {
            int id;
            string imie, nazwisko;
            double saldo, oprocentowanie;
            cout << "ID: "; cin >> id;
            cout << "Imie: "; cin >> imie;
            cout << "Nazwisko: "; cin >> nazwisko;
            cout << "Saldo: "; cin >> saldo;
            cout << "Oprocentowanie: "; cin >> oprocentowanie;
            klienci.push_back(new Klient(id, imie, nazwisko, saldo, oprocentowanie));
        }

        else if (opcja == 2) {
            int id;
            string imie, nazwisko;
            double saldo, oprocentowanie, dodatkowe;
            cout << "ID: "; cin >> id;
            cout << "Imie: "; cin >> imie;
            cout << "Nazwisko: "; cin >> nazwisko;
            cout << "Saldo: "; cin >> saldo;
            cout << "Oprocentowanie: "; cin >> oprocentowanie;
            cout << "Dodatkowe oprocentowanie: "; cin >> dodatkowe;
            klienci.push_back(new KlientVIP(id, imie, nazwisko, saldo, oprocentowanie, dodatkowe));
        }

        else if (opcja == 3) {
            int id;
            double kwota;
            cout << "ID klienta: "; cin >> id;
            cout << "Kwota: "; cin >> kwota;
            int index = znajdzKlienta(klienci, id);
            if (index != -1) klienci[index]->wplata(kwota);
            else cout << "Nie znaleziono klienta.\n";
        }

        else if (opcja == 4) {
            int id;
            double kwota;
            cout << "ID klienta: "; cin >> id;
            cout << "Kwota: "; cin >> kwota;
            int index = znajdzKlienta(klienci, id);
            if (index != -1) klienci[index]->wyplata(kwota);
            else cout << "Nie znaleziono klienta.\n";
        }

        else if (opcja == 5) {
            int id1, id2;
            double kwota;
            cout << "ID nadawcy: "; cin >> id1;
            cout << "ID odbiorcy: "; cin >> id2;
            cout << "Kwota: "; cin >> kwota;
            int i1 = znajdzKlienta(klienci, id1);
            int i2 = znajdzKlienta(klienci, id2);
            if (i1 != -1 && i2 != -1)
                klienci[i1]->przelew(*klienci[i2], kwota);
            else
                cout << "Nie znaleziono klientow.\n";
        }

        else if (opcja == 6) {
            int id;
            cout << "ID klienta: "; cin >> id;
            int index = znajdzKlienta(klienci, id);
            if (index != -1) klienci[index]->naliczOprocentowanie();
            else cout << "Nie znaleziono klienta.\n";
        }

        else if (opcja == 7) {
            for (auto k : klienci)
                k->pokaz();
        }

    } while (opcja != 0);

    
    for (auto k : klienci)
        delete k;

    return 0;
}
