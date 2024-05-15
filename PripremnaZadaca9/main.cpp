#include <iostream>
#include <algorithm>
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

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevi;
        Cvor *desni;
        Cvor *roditelj;
        Cvor(TipKljuca kljuc) : kljuc(kljuc){
            lijevi = nullptr;
            desni = nullptr;
            roditelj = nullptr;
            vrijednost = TipVrijednosti();
        }
    };
    Cvor *korijen;
    int brojCvorova;
    void obrisiStablo(Cvor *korijen){
        if(korijen != nullptr){
            obrisiStablo(korijen->lijevi);
            obrisiStablo(korijen->desni);
            delete korijen;
            korijen = nullptr;
        }
    }
    TipVrijednosti &ubaciKljucUStablo(TipKljuca kljuc){
        Cvor *p = korijen;
        Cvor *rod = nullptr;
        while(p != nullptr){
            rod = p;
            if(kljuc > p->kljuc){
                p = p->desni;
            }
            else{
                p = p->lijevi;
            }
        }
        Cvor *temp = new Cvor(kljuc);
        if(rod == nullptr){
            korijen = temp;
        }
        else{
            if(kljuc < rod->kljuc){
                rod->lijevi = temp;
            }
            else{
                rod->desni = temp;
            }
            temp->roditelj = rod;
        }
        brojCvorova++;
        return temp->vrijednost;
    }
    void kopiraj(BinStabloMapa<TipKljuca, TipVrijednosti> &mapa, Cvor *cvor){
        if(cvor != nullptr){
            mapa[cvor->kljuc] = cvor->vrijednost;
            kopiraj(mapa, cvor->lijevi);
            kopiraj(mapa, cvor->desni);
        }
    }
public:
    BinStabloMapa(){
        korijen = nullptr;
        brojCvorova = 0;
    }
    ~BinStabloMapa(){
        obrisiStablo(korijen);
        brojCvorova = 0;
    }
    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &nova){
        korijen = nullptr;
        brojCvorova = 0;
        kopiraj(*this, nova.korijen);
    }
    BinStabloMapa<TipKljuca, TipVrijednosti> &operator=(BinStabloMapa<TipKljuca, TipVrijednosti> nova){
        if(this == &nova) return *this;
        std::swap(korijen, nova.korijen);
        std::swap(brojCvorova, nova.brojCvorova);
        return *this;
    }
    int brojElemenata() const{
        return brojCvorova;
    }
    void obrisi(){
        obrisiStablo(korijen);
        korijen = nullptr;
        brojCvorova = 0;
    }
    void obrisi(const TipKljuca &kljuc){
        Cvor *p = korijen;
        Cvor *rod = nullptr;
        while(p != nullptr && kljuc != p->kljuc){
            rod = p;
            if(kljuc < p->kljuc){
                p = p->lijevi;
            }
            else{
                p = p->desni;
            }
        }
        Cvor *m;
        if(p->lijevi == nullptr){
            m = p->desni;
        }
        else{
            Cvor *pm, *temp;
            if(p->desni == nullptr){
                m = p->lijevi;
            }
            else{
                pm = p;
                m = p->lijevi;
                temp = m->desni;
                while(temp != nullptr){
                    pm = m;
                    m = temp;
                    temp = m->desni;
                }
                if(pm != p){
                    pm->desni = m->lijevi;
                    m->lijevi = p->lijevi;
                }
                m->desni = p->desni;
            }
        }
        if(rod == nullptr){
            korijen = m;
        }
        else{
            if(p == rod->lijevi){
                rod->lijevi = m;
            }
            else{
                rod->desni = m;
            }
        }
        delete p;
        brojCvorova--;
    }
    TipVrijednosti &operator[](const TipKljuca &kljuc){
        Cvor *k = korijen;
        while(k != nullptr && k->kljuc != kljuc){
            if(kljuc < k->kljuc){
                k = k->lijevi;
            }
            else{
                k = k->desni;
            }
        }
        if(k != nullptr){
            return k->vrijednost;
        }
        return ubaciKljucUStablo(kljuc);
    }
    TipVrijednosti operator[](const TipKljuca &kljuc) const{
        Cvor *k = korijen;
        while(k != nullptr && k->kljuc != kljuc){
            if(kljuc < k->kljuc){
                k = k->lijevi;
            }
            else{
                k = k->desni;
            }
        }
        if(k != nullptr){
            return k->vrijednost;
        }
        return TipVrijednosti();
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class Pair{
public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    bool brisanje;
    Pair(){
        brisanje = false;
    }
    Pair(TipKljuca k, TipVrijednosti v, bool b = false){
        kljuc = k;
        vrijednost = v;
        brisanje = b;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti>{
    int velicina;
    int kapacitet;
    Pair<TipKljuca, TipVrijednosti> **niz, **kraj;
    std::function<unsigned int(const TipKljuca &, unsigned int)> h;

    unsigned int hashLin(const TipKljuca &k, unsigned int i) const{
        return (h(k, kapacitet) + i) % kapacitet;
    }
    int ubaciHash(const TipKljuca &kljuc){
        int br = 0;
        int i;
        do{
            i = hashLin(kljuc, br);
            if(niz[i] == nullptr || niz[i]->brisanje == true){
                if(niz[i] != nullptr){
                    delete niz[i];
                }
                niz[i] = new Pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
                velicina++;
                return i;
            }
            else{
                br++;
            }
        }while(br < kapacitet);
        return -1;
    }
    int traziHash(const TipKljuca &kljuc) const{
        unsigned int pozicija;
        unsigned int br = 0;
        do{
            pozicija = hashLin(kljuc, br);
            if(niz[pozicija] != nullptr && niz[pozicija]->brisanje == false && niz[pozicija]->kljuc == kljuc){
                return pozicija;
            }
            else{
                br++;
            }
        }while(niz[pozicija] != nullptr && br != kapacitet);
        return -1;
    }
    bool obrisiHash(const TipKljuca &kljuc){
        int i = traziHash(kljuc);
        if(i != -1){
            velicina--;
            niz[i]->brisanje = true;
            return true;
        }
        return false;
    }
    void povecaj(){
        Pair<TipKljuca, TipVrijednosti> **t = new Pair<TipKljuca, TipVrijednosti> *[velicina + 2]();
        Pair<TipKljuca, TipVrijednosti> **p1, **p2;
        for(p1 = niz, p2 = t; p1 != kraj; p1++){
            if(*p1 != nullptr){
                *p2++ = new Pair<TipKljuca, TipVrijednosti>(*p1++);
            }
        }
        int novavelicina = velicina * 2;
        obrisi();
        delete[] niz;
        kapacitet = novavelicina;
        kraj = p2;
        niz = t;
    }
public:
    HashMapa(){
        velicina = 0;
        kapacitet = 1000000;
        niz = new Pair<TipKljuca, TipVrijednosti> *[kapacitet]{};
        kraj = niz + kapacitet;
    }
    HashMapa(const HashMapa &hashMapa){
        velicina = hashMapa.velicina;
        kapacitet = hashMapa.kapacitet;
        niz = hashMapa.niz;
        kraj = hashMapa.kraj;
        h = hashMapa.h;
        hashMapa.kraj = nullptr;
        hashMapa.niz = nullptr;
    }
    int brojElemenata() const{
        return velicina;
    }
    void obrisi(){
        Pair<TipKljuca, TipVrijednosti> **temp;
        for(temp = niz; temp != kraj; temp++){
            delete *temp;
            *temp = nullptr;
        }
        velicina = 0;
    }
    void obrisi(const TipKljuca &kljuc){
        if(obrisiHash(kljuc) == false){
            throw "Kljuc nije pronadjen";
        }
    }
    ~HashMapa(){
        obrisi();
        delete[] niz;
    }
    HashMapa &operator=(const HashMapa &hm) {
        if (this == &hm)
            return *this;
        obrisi();
        delete[] niz;
        velicina = hm.vel;
        kapacitet = hm.kapacitet;
        niz = new Pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
        kraj = niz + kapacitet;
        h = hm.h;
        for (int i = 0; i < hm.kapacitet; i++) {
            if (hm.niz[i]) {
                niz[i] = new Pair<TipKljuca, TipVrijednosti>(*hm.niz[i]);
            }
        }
        return *this;
    }
    HashMapa &operator=(HashMapa &&hm) {
        if (this == &hm)
            return *this;
        obrisi();
        delete[] niz;
        velicina = hm.vel;
        kapacitet = hm.kapacitet;
        niz = hm.niz;
        kraj = hm.kraj;
        h = hm.h;
        hm.niz = nullptr;
        hm.kraj = nullptr;
        return *this;
    }
    TipVrijednosti &operator[](const TipKljuca &k) {
        if (!h)
            throw "Nije definisana hash funkcija.";
        int poz = traziHash(k);
        if (poz != -1) {
            return niz[poz]->vrijednost;
        } else {
            return niz[ubaciHash(k)]->vrijednost;
        }
    }
    TipVrijednosti operator[](const TipKljuca &k) const {
        if (!h)
            throw "Nije definisana hash funkcija.";
        int poz = traziHash(k);
        if (poz != -1) {
            return niz[poz]->vrijednost;
        } else {
            return TipVrijednosti();
        }
    }
    void definisiHashFunkciju(
            std::function<unsigned int(const TipKljuca &, unsigned int)> h) {
        this->h = h;
    }
};

unsigned int hashfunkcija(int input, unsigned int max) { return input % max; }

#include <string>
unsigned int djbhash(std::string ulaz, unsigned int max) {
    unsigned int suma=5381;
// 5381 je pocetna vrijednost
// koja poboljsava distribuciju
    for (int i(0); i<ulaz.length(); i++)
        suma = suma*33 + ulaz[i];
    return suma % max;
}

int main() {
// Osnovni test funkcionalnosti HashMapa
    HashMapa<std::string,std::string> m;
    m.definisiHashFunkciju(djbhash);
    m["Sarajevo"] = "BiH";
    m["Zagreb"] = "Hrvatska";
    cout << m.brojElemenata() << " " << m["Zagreb"] << " '"; // 2 Hrvatska

// Test praznih elemenata
    cout << m["Beograd"] <<"' '"; // ''
    cout << m["zagreb"] << "' "; // ''
    m.obrisi("Zagreb");

// Potpuno korektna HashMapa nece kreirati objekte "Beograd" i "zagreb"
// jer oni nemaju vrijednosti pa ce se ispod ispisati 1
// Ali to zahtijeva implementaciju "Pristupnika" pa cemo priznavati
// rjesenje koje ispise broj 3
    cout << m.brojElemenata(); // 3 ili 1

// Test brisanja
    cout << " '" << m["Zagreb"] << "' " ; // ''
    m.obrisi();
    cout << m.brojElemenata(); // 0
    cout << " '" << m["Sarajevo"] << "' "; // ''
}
