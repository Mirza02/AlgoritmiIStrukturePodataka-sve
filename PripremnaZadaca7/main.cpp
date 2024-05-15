#include <iostream>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
    virtual ~Mapa(){};
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti &operator[](const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti operator[](const TipKljuca &kljuc) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti> **nizmapa;
    int duzina, kapacitet;

public:
    NizMapa() {
        kapacitet = 10;
        duzina = 0;
        nizmapa = new std::pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
    }
    ~NizMapa() {
        for (int i = 0; i < duzina; i++) {
            delete nizmapa[i];
        }
        delete[] nizmapa;
    }
    NizMapa(const NizMapa &nova) {
        duzina = nova.duzina;
        kapacitet = nova.kapacitet;
        nizmapa = new std::pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
        for (int i = 0; i < duzina; i++) {
            nizmapa[i] = new std::pair<TipKljuca, TipVrijednosti>(
                    nova.nizmapa[i]->first, nova.nizmapa[i]->second);
        }
    }
    NizMapa &operator=(const NizMapa &nova) {
        if (this == &nova)
            return *this;
        this->obrisi();
        duzina = nova.duzina;
        kapacitet = nova.kapacitet;
        for (int i = 0; i < duzina; i++) {
            nizmapa[i] = new std::pair<TipKljuca, TipVrijednosti>(
                    nova.nizmapa[i]->first, nova.nizmapa[i]->second);
        }
        return *this;
    }
    int brojElemenata() const{
        return duzina;
    }
    void obrisi() {
        for (int i = 0; i < duzina; i++) {
            delete nizmapa[i];
        }
        duzina = 0;
        kapacitet = 10;
        delete[] nizmapa;
        nizmapa = new std::pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
    }
    void obrisi(const TipKljuca &kljuc) {
        for (int i = 0; i < duzina; i++) {
            if (nizmapa[i]->first == kljuc) {
                delete nizmapa[i];
                nizmapa[i] = nullptr;
                for (int j = i; j < duzina - 1; j++) {
                    nizmapa[j] = nizmapa[j + 1];
                }
                duzina--;
                return;
            }
        }
        throw "Kljuc ne postoji";
    }
    TipVrijednosti operator[](const TipKljuca &kljuc) const {
        for (int i = 0; i < duzina; i++) {
            if (nizmapa[i]->first == kljuc)
                return nizmapa[i]->second;
        }
        return TipVrijednosti();
    }
    TipVrijednosti &operator[](const TipKljuca &kljuc) {
        for (int i = 0; i < duzina; i++) {
            if (nizmapa[i]->first == kljuc)
                return nizmapa[i]->second;
        }
        if (duzina == kapacitet) {
            kapacitet *= 2;
            std::pair<TipKljuca, TipVrijednosti> **temp =
                    new std::pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
            for (int i = 0; i < duzina; i++) {
                temp[i] = new std::pair<TipKljuca, TipVrijednosti>(nizmapa[i]->first,
                                                                   nizmapa[i]->second);
            }
            for (int i = 0; i < duzina; i++) {
                delete nizmapa[i];
            }
            delete[] nizmapa;
            nizmapa = temp;
        }
        duzina++;
        nizmapa[duzina - 1] = new std::pair<TipKljuca, TipVrijednosti>;
        nizmapa[duzina - 1]->first = kljuc;
        return nizmapa[duzina - 1]->second;
    }
};

void mapaTest1(){               // testiranje dodavanja i operatora [] za mapu <int, double>
    std::cout << "MAPA TEST 1: " << std::endl;
    NizMapa<int, double> testMapa1;
    testMapa1[3] = 5.25;
    testMapa1[1] = 3.21;
    testMapa1[-1] = 9.821;
    for(int i = 3; i >= -1; i-=2){
        std::cout << testMapa1[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void mapaTest2(){               // testiranje dodavanja, operatora [] i metode brojElemenata() za mapu <string, int>
    std::cout << "MAPA TEST 2: " << std::endl;
    NizMapa<string, int> testMapa2;
    testMapa2["Sarajevo"] = 4000;
    testMapa2["Zenica"] = 71200;
    testMapa2["Tuzla"] = 90192;
    std::cout << testMapa2["Zenica"] << std::endl;
    std::cout << "Broj elemenata u mapi: " << testMapa2.brojElemenata();
    std::cout << std::endl << std::endl;
}

void mapaTest3(){           // testiranje metode brisi za brisanje jednog elementa i metode brisi za brisanje svih podataka za mapu <double, string>
    std::cout << "MAPA TEST 3: " << std::endl;
    NizMapa<double, string> testMapa3;
    testMapa3[4.21] = "Olovka";
    testMapa3[1.25] = "Zadacnica";
    testMapa3[7.975] = "Knjiga";
    std::cout << "Broj elemenata prije brisanja: " << testMapa3.brojElemenata() << std::endl;
    testMapa3.obrisi(7.975);
    std::cout << "Broj elemenata poslije brisanja jednog elementa: " << testMapa3.brojElemenata() << std::endl;
    testMapa3.obrisi();
    std::cout << "Broj elemenata poslije brisanja svih podataka: " << testMapa3.brojElemenata();
    std::cout << std::endl << std::endl;
}

void mapaTest4(){          // testiranja kopije i operatora dodjele za mapu <int, char>
    std::cout << "MAPA TEST 4: " << std::endl;
    NizMapa<int, char> testMapa4;
    testMapa4['a'] = 1;
    testMapa4['d'] = 4;
    NizMapa<int, char> testMapaKopija(testMapa4);
    testMapaKopija['e'] = 5;
    testMapaKopija['j'] = 10;
    NizMapa<int, char> testMapaDodjela;
    testMapaDodjela = testMapa4;
    testMapaDodjela.obrisi('d');
    std::cout << "Broj elemenata prve mape: " << testMapa4.brojElemenata() << std::endl;
    std::cout << "Broj elemenata kopirane mape: " << testMapaKopija.brojElemenata() << std::endl;
    std::cout << "Broj elemenata dodijeljene mape: " << testMapaDodjela.brojElemenata() << std::endl;
    std::cout << std::endl << std::endl;
}

void mapaTest5(){           // testiranje mape pri unosu vece vrijednosti podataka za mapu <int, double>
    std::cout << "MAPA TEST 5: " << std::endl;
    NizMapa<int, double> testMapa5;
    for(int i = 0; i < 10000; i++){
        testMapa5[i] = i * 0.25;
    }
    std::cout << "Broj elemenata u mapi: " << testMapa5.brojElemenata() << std::endl;
    for(int i = 9999; i > 5000; i--){
        testMapa5.obrisi(i);
    }
    std::cout << "Broj elemenata u mapi nakon brisanja: " << testMapa5.brojElemenata();
}

int main() {
    mapaTest1();
    mapaTest2();
    mapaTest3();
    mapaTest4();
    mapaTest5();
}
