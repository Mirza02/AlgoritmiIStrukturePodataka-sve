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

template <typename Tip>
class DvostrukaLista : public Lista<Tip>{
    struct Cvor{
        Tip element;
        Cvor *prethodni;
        Cvor *sljedeci;
        Cvor(Tip element, Cvor *prethodni = nullptr, Cvor *sljedeci = nullptr) :
                element(element), prethodni(prethodni), sljedeci(sljedeci){
            if(prethodni != nullptr) prethodni->sljedeci = this;
            if(sljedeci != nullptr) sljedeci->prethodni = this;
        }
        Cvor(Cvor *prethodni = nullptr, Cvor *sljedeci = nullptr){
            if(prethodni != nullptr) prethodni->sljedeci = this;
            if(sljedeci != nullptr) sljedeci->prethodni = this;
        }
    };
    Cvor *prvi;
    Cvor *zadnji;
    Cvor *tekuci;
    int duzina;
public:
    DvostrukaLista(){
        prvi = new Cvor(nullptr, nullptr);
        zadnji = new Cvor(prvi, nullptr);
        tekuci = prvi;
        duzina = 0;
    }
    ~DvostrukaLista(){
        while(prvi != zadnji){
            tekuci = prvi;
            prvi = prvi->sljedeci;
            delete tekuci;
        }
        delete zadnji;
    }
    DvostrukaLista(const DvostrukaLista &nova){
        duzina = nova.duzina;
        prvi = new Cvor(nullptr, nullptr);
        zadnji = new Cvor(prvi, nullptr);
        tekuci = prvi;
        Cvor *temp = nova.prvi->sljedeci;
        for(int i = 0; i < nova.brojElemenata(); i++){
            this->dodajIza(temp->element);
            tekuci = tekuci->sljedeci;
            temp = temp->sljedeci;
            duzina--;
        }
    }
    DvostrukaLista &operator=(const DvostrukaLista &nova){
        if(this == &nova) return *this;
        duzina = nova.duzina;
        prvi = new Cvor(nullptr, nullptr);
        zadnji = new Cvor(prvi, nullptr);
        tekuci = prvi;
        Cvor *temp = nova.prvi->sljedeci;
        for(int i = 0; i < nova.brojElemenata(); i++){
            this->dodajIza(temp->element);
            tekuci = tekuci->sljedeci;
            temp = temp->sljedeci;
            duzina--;
        }
        return *this;
    }
    int brojElemenata() const{
        return duzina;
    }
    Tip &trenutni(){
        return tekuci->element;
    }
    Tip trenutni() const{
        return tekuci->element;
    }
    bool prethodni(){
        if(tekuci->prethodni == prvi) return false;
        tekuci = tekuci->prethodni;
        return true;
    }
    bool sljedeci(){
        if(tekuci->sljedeci == zadnji) return false;
        tekuci = tekuci->sljedeci;
        return true;
    }
    void pocetak(){
        tekuci = prvi->sljedeci;
    }
    void kraj(){
        tekuci = zadnji->prethodni;
    }
    void obrisi(){
        tekuci->prethodni->sljedeci = tekuci->sljedeci;
        tekuci->sljedeci->prethodni = tekuci->prethodni;
        Cvor *temp = tekuci->sljedeci;
        delete tekuci;
        if(temp == zadnji) tekuci = zadnji->prethodni;
        else tekuci = temp;
        duzina--;
    }
    void dodajIspred(const Tip &el){
        if(duzina == 0){
            prvi->sljedeci = new Cvor(el, prvi, zadnji);
            tekuci = prvi->sljedeci;
        }
        else{
            Cvor *temp = tekuci->prethodni;
            tekuci->prethodni = new Cvor(el, temp, tekuci);
            temp->sljedeci = tekuci->prethodni;
        }
        duzina++;
    }
    void dodajIza(const Tip &el){
        if(duzina == 0){
            prvi->sljedeci = new Cvor(el, prvi, zadnji);
            tekuci = prvi->sljedeci;
        }
        else{
            Cvor *temp = tekuci->sljedeci;
            tekuci->sljedeci = new Cvor(el, tekuci, temp);
            temp->prethodni = tekuci->sljedeci;
        }
        duzina++;
    }
    Tip &operator[](int i){
        if(duzina == 0) throw "Lista je prazna";
        else if(i < 0 || i >= duzina) throw "Nepravilan indeks";
        Cvor *temp = prvi->sljedeci;
        for(int j = 0; j < i; j++){
            temp = temp->sljedeci;
        }
        return temp->element;
    }
    Tip const operator[](int i) const{
        if(duzina == 0) throw "Lista je prazna";
        else if(i < 0 || i >= duzina) throw "Nepravilan indeks";
        Cvor *temp = prvi->sljedeci;
        for(int j = 0; j < i; j++){
            temp = temp->sljedeci;
        }
        return temp->element;
    }
};


template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &n){
    Lista<Tip> l = n;
    l.pocetak();
    Tip maks = l.trenutni();
    l.sljedeci();
    for(int i = 1; i < l.brojElemenata(); i++){
        Tip temp = l.trenutni();
        if(temp > maks) maks = temp;
        l.sljedeci();
    }
    return maks;
}


int main(){
    DvostrukaLista<int> lista;
    for (int i(1); i<=5; i++)
        lista.dodajIspred(i);
    {
        // Konstruktor kopije
        DvostrukaLista<int> lista2(lista);
        DvostrukaLista<int> lista3;
        // Operator dodjele
        (lista3=lista).brojElemenata();
        // Destruktivna samododjela
        (lista=lista).brojElemenata();

        lista.obrisi();
        std::cout << lista2.brojElemenata() << " " << lista2[2];
        std::cout << " " << lista3.brojElemenata() << " " << lista3[2] << " ";
    }
    std::cout << lista.brojElemenata() << " " << lista[2];
}