#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename Tip> class Lista {
public:
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip &trenutni() = 0;
    virtual Tip trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip &el) = 0;
    virtual void dodajIza(const Tip &el) = 0;
    virtual Tip &operator[](int i) = 0;
    virtual const Tip operator[](int i) const = 0;
};

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
        Cvor(Tip element, Cvor *sljedeci = nullptr)
                : element(element), sljedeci(sljedeci){};
        Cvor(){};
    };
    int duzina;
    Cvor *prvi;
    Cvor *tekuci;
    Cvor *zadnji;
public:
    JednostrukaLista() {
        duzina = 0;
        prvi = nullptr;
        tekuci = nullptr;
        zadnji = nullptr;
    }
    ~JednostrukaLista() {
        while (prvi != nullptr) {
            tekuci = prvi;
            prvi = prvi->sljedeci;
            delete tekuci;
        }
        prvi = tekuci = nullptr;
    }
    JednostrukaLista(const JednostrukaLista &nova) {
        prvi = nullptr;
        tekuci = nullptr;
        duzina = nova.duzina;
        if (duzina != 0) {
            prvi = new Cvor(nova.prvi->element, nova.prvi->sljedeci);
            Cvor *temp = prvi;
            while (temp->sljedeci != nullptr) {
                temp->sljedeci = new Cvor(*temp->sljedeci);
                temp = temp->sljedeci;
            }
            zadnji = temp;
        }
        tekuci = prvi;
    }
    JednostrukaLista &operator=(const JednostrukaLista &nova) {
        if (this == &nova)
            return *this;
        this->~JednostrukaLista();
        duzina = nova.duzina;
        if (duzina != 0) {
            prvi = new Cvor(nova.prvi->element, nova.prvi->sljedeci);
            Cvor *temp = prvi;
            while (temp->sljedeci != nullptr) {
                temp->sljedeci = new Cvor(*temp->sljedeci);
                temp = temp->sljedeci;
            }
            zadnji = temp;
        }
        tekuci = prvi;
        return *this;
    }

    int brojElemenata() const { return duzina; }
    Tip &trenutni() {
        if (duzina == 0)
            throw "Lista je prazna";
        return tekuci->element;
    }
    Tip trenutni() const {
        if (duzina == 0)
            throw "Lista je prazna";
        return tekuci->element;
    }
    bool prethodni() {
        if (duzina == 0)
            throw "Lista je prazna";
        if (prvi == tekuci)
            return false;
        Cvor *temp = prvi;
        while (temp->sljedeci != tekuci) {
            temp = temp->sljedeci;
        }
        tekuci = temp;
        return true;
    }
    bool sljedeci() {
        if (duzina == 0)
            throw "Lista je prazna";
        if (tekuci->sljedeci == nullptr)
            return false;
        tekuci = tekuci->sljedeci;
        return true;
    }
    void pocetak() {
        if (duzina == 0)
            throw "Lista je prazna";
        tekuci = prvi;
    }
    void kraj() {
        tekuci = zadnji;
    }
    void obrisi() {
        if (duzina == 0)
            throw "Lista je prazna";
        if (prvi == tekuci) {
            prvi = prvi->sljedeci;
            delete tekuci;
            tekuci = prvi;
        } else if (tekuci == zadnji) {
            this->prethodni();
            delete zadnji;
            zadnji = tekuci;
            tekuci->sljedeci = nullptr;
        } else {
            Cvor *temp = prvi;
            while (temp->sljedeci != tekuci) {
                temp = temp->sljedeci;
            }
            temp->sljedeci = tekuci->sljedeci;
            delete tekuci;
            tekuci = temp->sljedeci;
        }
        duzina--;
    }
    void isprazni(){
        while (prvi != nullptr) {
            tekuci = prvi;
            prvi = prvi->sljedeci;
            delete tekuci;
        }
        prvi = tekuci = nullptr;
        duzina = 0;
    }
    void dodajIspred(const Tip &el) {
        if (duzina == 0) {
            prvi = new Cvor(el);
            tekuci = prvi;
            zadnji = tekuci;
        } else if (tekuci == prvi) {
            prvi = new Cvor(el, tekuci);
        } else {
            Cvor *temp = prvi;
            while (temp->sljedeci != tekuci) {
                temp = temp->sljedeci;
            }
            temp->sljedeci = new Cvor(el, tekuci);
        }
        duzina++;
    }
    void dodajIza(const Tip &el) {
        if (duzina == 0) {
            prvi = new Cvor(el);
            tekuci = prvi;
            zadnji = prvi;
        } else {
            Cvor *temp = tekuci->sljedeci;
            tekuci->sljedeci = new Cvor(el, temp);
            if(tekuci->sljedeci->sljedeci == nullptr) zadnji = tekuci->sljedeci;
        }
        duzina++;
    }
    Tip &operator[](int i) {
        if (duzina == 0)
            throw "Lista je prazna";
        if (i < 0 || i > duzina)
            throw "Nepravilan indeks";
        Cvor *temp = prvi;
        for (int j = 0; j < i; j++) {
            temp = temp->sljedeci;
        }
        return temp->element;
    }
    Tip const operator[](int i) const {
        if (duzina == 0)
            throw "Lista je prazna";
        if (i < 0 || i > duzina)
            throw "Nepravilan indeks";
        Cvor *temp = prvi;
        for (int j = 0; j < i; j++) {
            temp = temp->sljedeci;
        }
        return temp->element;
    }
};

template <typename Tip>
class Stek{
    JednostrukaLista<Tip> *l;
public:
    Stek(){
        l = new JednostrukaLista<Tip>();
    }
    ~Stek(){
        delete l;
    }
    Stek(const Stek &novi){
        l = new JednostrukaLista<Tip>(*(novi.l));
    }
    Stek &operator=(const Stek &novi){
        if(this == &novi) return *this;
        this->~Stek();
        l = new JednostrukaLista<Tip>(*(novi.l));
        return *this;
    }
    void brisi(){
        l->isprazni();
    }
    void stavi(const Tip &el){
        l->dodajIspred(el);
        l->pocetak();
    }
    Tip skini() {
        try {
            Tip result = l->trenutni();
            l->obrisi();
            if(l->brojElemenata() != 0){
                l->pocetak();
            }
            return result;
        }
        catch (...) {
            throw "Stek je prazan";
        }
    }
    Tip &vrh(){
        try{
            Tip &result = l->trenutni();
            return result;
        }
        catch(...){
            throw "Stek je prazan";
        }
    }
    int brojElemenata(){
        return l->brojElemenata();
    }
};

void pretraga(Stek<std::vector<int>> &s, int trazeni) {
    if(s.brojElemenata() == 0){
        std::cout << "Nema elementa";
        return;
    }
    std::vector<int> v = s.skini();
    if(v.size() == 0 || trazeni < v[0]){
        pretraga(s, trazeni);
    }
    else if(trazeni > v[v.size() - 1]) std::cout << "Nema elementa";
    else{
        int dno = 0, vrh = v.size() - 1;
        bool pronadjen = false;
        while(!pronadjen && vrh >= dno){
            int srednji = (vrh + dno) / 2;
            if(v[srednji] == trazeni){
                std::cout << srednji << " " << s.brojElemenata();
                pronadjen = true;
            }
            else if(v[srednji] <= trazeni) dno = srednji + 1;
            else vrh = srednji - 1;
        }
        if(pronadjen == false) std::cout << "Nema elementa";
    }
    s.stavi(v);
}

void pretragaTest1(){           // testiranje kada je pravilan unos te kada je u prvom i u zadnjem vektoru trazeni broj
    Stek<std::vector<int>> stektest1;
    stektest1.stavi({1, 5, 7, 8, 9});
    stektest1.stavi({10, 11});
    stektest1.stavi({15, 301, 564});
    std::cout << "Pretraga za broj 7: ";
    pretraga(stektest1, 7);
    std::cout << std::endl << "Pretraga za broj 301: ";
    pretraga(stektest1, 301);
    std::cout << std::endl << "Broj elemenata u steku nakon pretraga: " << stektest1.brojElemenata();
    std::cout << std::endl;
}

void pretragaTest2(){           // testiranje slucaja kada u steku nema elementa koji se trazi
    Stek<std::vector<int>> stektest2;
    stektest2.stavi({1, 7, 8, 11, 46});
    stektest2.stavi({50, 51});
    stektest2.stavi({60, 87, 98, 532});
    stektest2.stavi({1000});
    pretraga(stektest2, 1001);
    std::cout << std::endl;
    pretraga(stektest2, 6);
    std::cout << std::endl << "Broj elemenata u steku nakon pretrage: " << stektest2.brojElemenata();
}

void pretragaTest3(){           // testiranje na praznom steku, istom steku kada je dodan element i kada je vektor gdje se nalazio trazeni element uklonjen
    Stek<std::vector<int>> stektest3;
    std::cout << "Pretraga praznog vektora: ";
    pretraga(stektest3, 100);
    stektest3.stavi({2, 4, 12, 15});
    stektest3.stavi({21, 26, 36});
    stektest3.stavi({55, 64, 88, 94, 100, 103, 1102});
    std::cout << std::endl << "Pretraga za broj 100: ";
    pretraga(stektest3, 100);
    stektest3.skini();
    std::cout << std::endl << "Pretraga za broj 100 nakon sto je uklonjen posljednji vektor: ";
    pretraga(stektest3, 100);
}

int main() {
    pretragaTest1();
    pretragaTest2();
    pretragaTest3();
}
