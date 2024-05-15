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

int main() {
    NizMapa<int, int> nizmapa;
    BinStabloMapa<int, int> binmapa;
    time_t current_time = time(NULL);
    std::srand(current_time);
    for(int i = 0; i < 100000; i++){
        int random = std::rand() % 10000;
        nizmapa[random] = i;
        binmapa[random] = i;
    }
    clock_t nizmapaVrijemeDodavanja1 = clock();
    nizmapa[100000] = 0;
    clock_t nizmapaVrijemeDodavanja2 = clock();
    clock_t nizmapaVrijemePristupa1 = clock();
    int l = nizmapa[99999];
    clock_t nizmapaVrijemePristupa2 = clock();
    std::cout << "NizMapa:" << std::endl << "Vrijeme dodavanja: " << nizmapaVrijemeDodavanja2 - nizmapaVrijemeDodavanja1
    << std::endl << "Vrijeme pristupa: " << nizmapaVrijemePristupa2 - nizmapaVrijemePristupa1 << std::endl;
    clock_t binmapaVrijemeDodavanja1 = clock();
    binmapa[100000] = 0;
    clock_t binmapaVrijemeDodavanja2 = clock();
    clock_t binmapaVrijemePristupa1 = clock();
    l = binmapa[99999];
    clock_t binmapaVrijemePristupa2 = clock();
    std::cout << "BinMapa:" << std::endl << "Vrijeme dodavanja: " << binmapaVrijemeDodavanja2 - binmapaVrijemeDodavanja1
    << std::endl << "Vrijeme pristupa: " << binmapaVrijemePristupa2 - binmapaVrijemePristupa1;
    // operacije pretrage i pristupa imaju mnogo bolje performanse u BinStabloMapi nego u NizMapi
    return 0;
}
