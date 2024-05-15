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
void NizListaTest1() {          // testiranje praznog konstruktora, metoda dodajIspred(), kraj(), trenutni()
    // brojElemenata() i operatora []
    std::cout << "NIZLISTA TEST 1: " << std::endl;
    NizLista<int> lista;
    for (int i = 0; i < 5; i++) {
        lista.dodajIspred(i);
    }
    lista.kraj();
    std::cout << "Zadnji element liste: " << lista.trenutni() << std::endl;
    std::cout << "Svi elementi liste: ";
    for(int i = 0; i < lista.brojElemenata(); i++){
        std::cout << lista[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void NizListaTest2(){           // testiranje nepraznog konstruktora, ubacivanja vise elemenata od provbitne velicine niza,
    // metoda pocetak(), trenutni() kada se koristi kao referenca na objekat, dodajIza() i operatora []
    std::cout << "NIZLISTA TEST 2: " << std::endl;
    NizLista<int> lista(5);
    for(int i = 0; i < 10; i++){
        lista.dodajIza(i);
    }
    lista.pocetak();
    std::cout << "Prvi element liste: " << lista.trenutni() << std::endl;
    lista.trenutni() = 100;
    std::cout << "Svi elementi liste (prvi promijenjen): " << std::endl;
    for(int i = 0; i < lista.brojElemenata(); i++){
        std::cout << lista[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void NizListaTest3(){           // testiranje liste za double vrijednosti, kopirajuceg konstruktora, metoda prethodni()
    // i trenutni()
    std::cout << "NIZLISTA TEST 3: " << std::endl;
    NizLista<double> lista1;
    for(int i = 0; i < 10; i++){
        lista1.dodajIspred(i * 1.25);
    }
    NizLista<double> lista2(lista1);
    std::cout << "Elementi prve liste (obrnut poredak): ";
    for(int i = 0; i < lista1.brojElemenata(); i++){
        std::cout << lista1.trenutni() << " ";
        lista1.prethodni();
    }
    std::cout << std::endl << "Elementi druge liste: ";
    for(int i = 0; i < lista2.brojElemenata(); i++){
        std::cout << lista2[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void NizListaTest4(){           // testiranje operatora dodjele, metoda obrisi(), dodajIza(), kraj() i operatora []
    std::cout << "NIZLISTA TEST 4: " << std::endl;
    NizLista<int> lista1;
    for(int i = 0; i < 5; i++){
        lista1.dodajIza(i * 50);
    }
    NizLista<int> lista2;
    lista2 = lista1;
    lista2.kraj();
    lista2.obrisi();
    lista2.obrisi();
    lista2[0] = 10;
    std::cout << "Elementi prve liste: ";
    for(int i = 0; i < lista1.brojElemenata(); i++){
        std::cout << lista1[i] << " ";
    }
    std::cout << std::endl << "Elementi druge liste: ";
    for(int i = 0; i < lista2.brojElemenata(); i++){
        std::cout << lista2[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void NizListaTest5(){           // testiranje NizListe sa char tipom, metoda pocetak(), sljedeci(), trenutni()
    std::cout << "NIZLISTA TEST 5: " << std::endl;
    NizLista<char> lista1;
    for(int i = 0; i < 5; i++){
        lista1.dodajIspred('a' + i);
    }
    lista1.pocetak();
    lista1.sljedeci();
    lista1.trenutni() = 'z';
    lista1.pocetak();
    std::cout << "Elementi liste: ";
    for(int i = 0; i < lista1.brojElemenata(); i++){
        std::cout << lista1.trenutni() << " ";
        lista1.sljedeci();
    }
    std::cout << std::endl << std::endl;
}


void JednostrukaListaTest1(){       // testiranje konstruktora, metoda dodajIspred(), kraj(), i operatora []
    std::cout << "JEDNOSTRUKALISTA TEST 1:" << std::endl;
    JednostrukaLista<int> lista1;
    for(int i = 0; i < 10; i++){
        lista1.dodajIspred(i);
    }
    lista1.kraj();
    if(!lista1.sljedeci()) std::cout << "Dostignut je kraj liste" << std::endl;
    std::cout << "Elementi liste: ";
    for(int i = 0; i < lista1.brojElemenata(); i++){
        std::cout << lista1[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void JednostrukaListaTest2(){       // testiranje kopirajuceg konstruktora, metoda pocetak(), trenutni() i obrisi()
    std::cout << "JEDNOSTRUKALISTA TEST 2: " << std::endl;
    JednostrukaLista<double> lista1;
    for(int i = 0; i < 15; i++){
        lista1.dodajIza(i * 0.45);
    }
    std::cout << "Elementi liste 1 : ";
    lista1.pocetak();
    std::cout << lista1.trenutni() << " ";
    while(lista1.sljedeci()){
        std::cout << lista1.trenutni() << " ";
    }
    for(int i = 0; i < 5; i++){
        lista1.obrisi();
    }
    JednostrukaLista<double> lista2(lista1);
    std::cout << std::endl << "Elementi liste 2: ";
    for(int i = 0; i < lista2.brojElemenata(); i++){
        std::cout << lista2[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void JednostrukaListaTest3(){           // testiranje operatora dodjele, metoda kraj(), prethodni() i operatora []
    std::cout << "JEDNOSTRUKA LISTA TEST 3: " << std::endl;
    JednostrukaLista<int> lista1;
    for(int i = 5; i < 10000; i *= 5){
        lista1.dodajIspred(i);
    }
    lista1.kraj();
    lista1.trenutni() = 1;
    lista1.prethodni();
    std::cout << "Predzadnji element: " << lista1.trenutni();
    lista1[lista1.brojElemenata() - 2] = 2;
    JednostrukaLista<int> lista2;
    lista2 = lista1;
    std::cout << std::endl << "Elementi nove liste: ";
    for(int i = 0; i < lista2.brojElemenata(); i++){
        std::cout << lista2[i] << " ";
    }
}

int main() {
    NizListaTest1();
    NizListaTest2();
    NizListaTest3();
    NizListaTest4();
    NizListaTest5();

    JednostrukaListaTest1();
    JednostrukaListaTest2();
    JednostrukaListaTest3();
}
