#include <iostream>

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

template <typename Tip> class NizLista : public Lista<Tip> {
    int kapacitet = 0;
    int duzina = 0;
    int tekuci;
    Tip **elementi;

public:
    NizLista(int size = 10) {
        duzina = 0;
        kapacitet = size;
        tekuci = 0;
        elementi = new Tip *[kapacitet] {};
    }
    ~NizLista() {
        for (int i = 0; i < duzina; i++) {
            delete elementi[i];
        }
        delete[] elementi;
    }
    NizLista(const NizLista &nova) {
        if (duzina > 0) {
            for (int i = 0; i < kapacitet; i++) {
                delete elementi[i];
            }
            delete[] elementi;
        }
        kapacitet = nova.kapacitet;
        duzina = nova.duzina;
        tekuci = nova.tekuci;
        elementi = new Tip *[kapacitet] {};
        for (int i = 0; i < duzina; i++) {
            elementi[i] = new Tip(*nova.elementi[i]);
        }
    }
    NizLista &operator=(const NizLista &nova) {
        if (this == &nova)
            return *this;
        for (int i = 0; i < kapacitet; i++) {
            delete elementi[i];
        }
        delete[] elementi;
        kapacitet = nova.kapacitet;
        duzina = nova.duzina;
        tekuci = nova.tekuci;
        elementi = new Tip *[kapacitet] {};
        for (int i = 0; i < duzina; i++) {
            elementi[i] = new Tip(*nova.elementi[i]);
        }
        return *this;
    }
    int brojElemenata() const { return duzina; }
    Tip &trenutni() {
        if (duzina == 0)
            throw "Lista je prazna";
        return *elementi[tekuci];
    }
    Tip trenutni() const {
        if (duzina == 0)
            throw "Lista je prazna";
        return *elementi[tekuci];
    }
    bool prethodni() {
        if (duzina == 0)
            throw "Lista je prazna";
        if (tekuci == 0)
            return false;
        tekuci--;
        return true;
    }
    bool sljedeci() {
        if (duzina == 0)
            throw "Lista je prazna";
        if (tekuci == (duzina - 1))
            return false;
        tekuci++;
        return true;
    }
    void pocetak() {
        if (duzina == 0)
            throw "Lista je prazna";
        tekuci = 0;
    }
    void kraj() {
        if (duzina == 0)
            throw "Lista je prazna";
        tekuci = duzina - 1;
    }
    void obrisi() {
        if (duzina <= 0)
            throw "Lista je prazna";
        delete elementi[tekuci];
        for (int i = tekuci; i < duzina - 1; i++) {
            elementi[i] = elementi[i + 1];
        }
        duzina--;
        if (tekuci == duzina)
            tekuci--;
    }
    void produzi() {
        kapacitet *= 2;
        Tip **temp = elementi;
        elementi = new Tip *[kapacitet] {};
        for (int i = 0; i < duzina; i++) {
            elementi[i] = temp[i];
        }
        delete[] temp;
    }
    void dodajIspred(const Tip &el) {
        if (duzina == 0) {
            elementi[0] = new Tip(el);
            tekuci = 0;
        } else {
            if (duzina + 1 > kapacitet) {
                this->produzi();
            }
            for (int i = duzina; i > tekuci; i--) {
                elementi[i] = elementi[i - 1];
            }
            elementi[tekuci] = new Tip(el);
            tekuci++;
        }
        duzina++;
    }
    void dodajIza(const Tip &el) {
        if (duzina == 0) {
            elementi[0] = new Tip(el);
            tekuci = 0;
        } else {
            if (duzina > kapacitet) {
                this->produzi();
            }
            for (int i = duzina; i > tekuci + 1; i--) {
                elementi[i] = elementi[i - 1];
            }
            elementi[tekuci + 1] = new Tip(el);
        }
        duzina++;
    }
    virtual Tip &operator[](int i) {
        if (duzina == 0)
            throw "Lista je prazna";
        else if (i < 0 || i >= duzina)
            throw "Netacan indeks";
        return *elementi[i];
    }
    virtual const Tip operator[](int i) const {
        if (duzina == 0)
            throw "Lista je prazna";
        else if (i < 0 || i >= duzina)
            throw "Netacan indeks";
        return *elementi[i];
    }
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


void stekTest1(){             // testiranje konstruktora, metoda stavi(), broj_elemenata() i vrh()
    std::cout << "STEK TEST 1: " << std::endl;
    Stek<int> stek_int;
    for(int i = 0; i < 5; i++){
        stek_int.stavi(i);
    }
    std::cout << "Broj elemenata u steku: " << stek_int.brojElemenata() << std::endl;
    std::cout << "Element na vrhu steka: " << stek_int.vrh() << std::endl;
    std::cout << "Broj elemenata u steku nakon koristenja metode vrh(): " << stek_int.brojElemenata();
    std::cout << std::endl << std::endl;
}

void stekTest2(){           // testiranje operatora =, metoda skini() i brojElemenata()
    std::cout << "STEK TEST 2: " << std::endl;
    Stek<double> stek_double;
    for(int i = 5; i < 10; i++){
        stek_double.stavi(i * 1.5);
    }
    Stek<double> stek2;
    stek2 = stek_double;
    std::cout << "Element na vrhu steka 2: " << stek2.skini() << std::endl;
    std::cout << "Broj elemenata prvog steka: " << stek_double.brojElemenata() << std::endl;
    std::cout << "Broj elemenata drugog steka nakon metode .skini(): " << stek2.brojElemenata();
    std::cout << std::endl << std::endl;
}

void stekTest3(){           // testiranje kopirajuceg konstruktora i metode brisi()
    std::cout << "STEK TEST 3: " << std::endl;
    Stek<int> stek_int;
    for(int i = 5; i < 10; i++){
        stek_int.stavi(i);
    }
    Stek<int> stek2(stek_int);
    stek_int.brisi();
    std::cout << "Broj elemenata prvog steka nakon metode brisi(): " << stek_int.brojElemenata() << std::endl;
    std::cout << "Broj elemenata drugog steka nakon metode brisi(): " << stek2.brojElemenata();
}

int main() {
    stekTest1();
    stekTest2();
    stekTest3();

    return 0;
}
