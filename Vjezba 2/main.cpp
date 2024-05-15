#include <iostream>

using namespace std;

template <typename Tip>
class Iterator;

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
class Iterator{
    const NizLista<Tip> *niz;
    const JednostrukaLista<Tip> *lista;
    int trenutniNiz;
    typename JednostrukaLista<Tip>::Cvor *trenutniLista;
public:
    Iterator(const NizLista<Tip> &l){
        niz = &l;
        lista = nullptr;
        trenutniLista = nullptr;
        trenutniNiz = 0;
    }
    Iterator(const JednostrukaLista<Tip> &l){
        lista = &l;
        niz = nullptr;
        trenutniLista = l->prvi;

    }
    Tip trenutni(){
        if(niz != nullptr){
            return niz[trenutniNiz];
        }
        else{
            return trenutniLista->element;
        }
    }
    Tip sljedeci(){
        if(niz != nullptr){
            trenutniNiz++;
        }
        else{
            trenutniLista = trenutniLista->sljedeci;
        }
    }
    Tip prethodni(){
        if(niz != nullptr){
            if(trenutniNiz > 0){
                trenutniNiz--;
            }
            else{
                throw "Iterator je na nultom elementu";
            }
        }
        else{
            lista->pocetak();
            Cvor *temp = lista->trenutni();
            while(temp->sljedeci != trenutniLista){
                temp = temp->sljedeci;
            }
            trenutniLista = temp;
            return trenutniLista->element;
        }
    }
    Tip pocetak(){
        if(niz != nullptr){
            trenutniNiz = 0;
        }
        else{
            lista->pocetak();
            trenutniLista = lista->trenutni();
        }
    }
    Tip kraj(){
        if(niz != nullptr){
            trenutniNiz = niz->brojElemenata() - 1;
        }
        else{
            lista->kraj();
            trenutniLista = lista->trenutni();
        }
    }
};

template <typename Tip>
Tip dajMaksimum(const NizLista<Tip> &n){
    Iterator<Tip> iterator = n;
    Tip maks = iterator.trenutni();
    while(iterator.trenutni() != iterator.kraj()){

    }
}

template <typename Tip>
Tip dajMaksimum(const JednostrukaLista<Tip> &l){
    Iterator<Tip> iterator = l;
}

int main() {
    JednostrukaLista<int> l;
    for(int i = 0; i < 5; i++){
        l.dodajIza(i);
    }
    std::cout << dajMaksimum(l);
    return 0;
}
