#include <iostream>
#include <iomanip>

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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevi;
        Cvor *desni;
        Cvor *roditelj;
        int balans;
        Cvor(TipKljuca kljuc, TipVrijednosti vrijednost = TipVrijednosti(), Cvor *lijevi = nullptr, Cvor *desni = nullptr, Cvor *roditelj = nullptr) :
                kljuc(kljuc), vrijednost(vrijednost), lijevi(lijevi), desni(desni), roditelj(roditelj){
            balans = 0;
        }
    };
    Cvor *korijen;
    int brojCvorova;

    void obrisiStablo(Cvor *cvor){
        if(cvor != nullptr){
            obrisiStablo(cvor->lijevi);
            obrisiStablo(cvor->desni);
            delete cvor;
            cvor = nullptr;
        }
        korijen = nullptr;
    }
    void prekopirajStablo(Cvor *&c1, Cvor *c2, Cvor *r) {
        if (c2 != nullptr) {
            c1 = new Cvor(c2->kljuc, c2->vrijednost, nullptr, nullptr, r);
            prekopirajStablo(c1->lijevo, c2->lijevo, c1);
            prekopirajStablo(c1->desno, c2->desno, c1);
            brojCvorova++;
        }
    }

public:
    AVLStabloMapa(){
        korijen = nullptr;
        brojCvorova = 0;
    }
    ~AVLStabloMapa(){
        obrisiStablo(korijen);
        brojCvorova = 0;
    }
    void rotacijaLijevo(Cvor *kriticni){
        Cvor *temp = kriticni->desni;
        if(kriticni->roditelj != nullptr && kriticni->roditelj->lijevi == temp){
            kriticni->roditelj->lijevi = temp;

        }
        else if(kriticni->roditelj != nullptr && kriticni->roditelj->desni == temp){
            kriticni->roditelj->desni = temp;
        }
        bool desno = false;
        if(temp->lijevi != nullptr){
            kriticni->desni = temp->lijevi;
            temp->lijevi->roditelj = kriticni;
            temp->lijevi = nullptr;
            desno = true;
        }
        temp->roditelj = kriticni->roditelj;
        if(kriticni->roditelj != nullptr){
            kriticni->roditelj = temp;
        }
        else{
            kriticni->roditelj = temp;
            korijen = temp;
        }
        temp->lijevi = kriticni;
        kriticni->balans = 0;
        temp->balans = 0;
        if(!desno) kriticni->desni = nullptr;
    }
    void rotacijaDesno(Cvor *kriticni){
        Cvor *temp = kriticni->lijevi;
        if(kriticni->roditelj != nullptr && kriticni->roditelj->lijevi == temp){
            kriticni->roditelj->lijevi = temp;
        }
        else if(kriticni->roditelj != nullptr && kriticni->roditelj->desni == temp){
            kriticni->roditelj->desni = temp;
        }
        bool lijevo = false;
        if(temp->desni != nullptr){
            kriticni->lijevi = temp->desni;
            temp->desni->roditelj = kriticni;
            temp->desni = nullptr;
            lijevo = true;
        }
        temp->roditelj = kriticni->roditelj;
        if(kriticni->roditelj != nullptr){
            kriticni->roditelj = temp;
        }
        else{
            kriticni->roditelj = temp;
            korijen = temp;
        }
        temp->desni = kriticni;
        kriticni->balans = 0;
        temp->balans = 0;
        if(!lijevo){
            kriticni->lijevi = nullptr;
        }
    }
    void azuriranjeBalansa(Cvor *&c) {
        if (c->roditelj == nullptr)
            return;
        if (!(c->lijevi && c->desni && c->balans == 0)) {
            if (c->roditelj->lijevi == c)
                c->roditelj->balans++;
            else
                c->roditelj->balans--;
        }
        bool azur = true;
        Cvor *c1 = c->roditelj;
        Cvor *c2 = c;
        if (c->balans > 0 && c->roditelj->balans < -1) {
            rotacijaDesno(c2);
            rotacijaLijevo(c1);
            azur = false;
        } else if (c->balans < 0 && c->roditelj->balans > 1) {
            rotacijaLijevo(c2);
            rotacijaDesno(c1);
            azur = false;
        } else if (c->roditelj->balans < -1) {
            rotacijaLijevo(c1);
            azur = false;
        } else if (c->roditelj->balans > 1) {
            rotacijaDesno(c1);
            azur = false;
        }
        if (azur == true)
            azuriranjeBalansa(c->roditelj);
    }
    void ubaci(Cvor *novi){
        Cvor *p = korijen;
        Cvor *rod = nullptr;
        while(p != nullptr){
            rod = p;
            if(novi->kljuc > p->kljuc){
                p = p->desni;
            }
            else{
                p = p->lijevi;
            }
        }
        bool postoji = false;
        if(rod == nullptr){
            korijen = novi;
            postoji = true;
        }
        else{
            if(novi->kljuc < rod->kljuc){
                rod->lijevi = novi;
                novi->roditelj = rod;
                if(rod->desni != nullptr){
                    postoji = true;
                    rod->balans = 0;
                }
            }
            else{
                rod->desni = novi;
                novi->roditelj = rod;
                if(rod->lijevi != nullptr){
                    postoji = true;
                    rod->balans = 0;
                }
            }
        }
        if(postoji == false) azuriranjeBalansa(novi);
        brojCvorova++;
    }
    void dodaj(TipKljuca kljuc){
        Cvor *novi = new Cvor(kljuc);
        ubaci(novi);
        return novi->vrijednost;
    }
    AVLStabloMapa(const AVLStabloMapa<TipKljuca, TipVrijednosti> &nova){
        korijen = nullptr;
        brojCvorova = 0;
        kopiraj(*this, nova.korijen);
    }
    int brojElemenata() const{
        return brojCvorova;
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
        Cvor *dodaj = new Cvor(kljuc);
        ubaci(dodaj);
        return dodaj->vrijednost;
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
    void obrisi(){
        obrisiStablo(korijen);
        brojCvorova = 0;
    }
    void obrisi(const TipKljuca &k) {
        Cvor *t = korijen;
        Cvor *pomocni = nullptr;
        while (t != nullptr && t->kljuc != k) {
            pomocni = t;
            if (k < t->kljuc)
                t = t->lijevi;
            else
                t = t->desni;
        }
        if (t == nullptr)
            throw;
        Cvor *pom1 = nullptr, *pom2 = nullptr, *temp = nullptr;
        if (t->desni == nullptr)
            pom1 = t->lijevi;
        else if (t->lijevi == nullptr)
            pom1 = t->desni;
        else {
            pom2 = t;
            pom1 = t->lijevi;
            temp = pom1->desni;
            while (temp != nullptr) {
                pom2 = pom1;
                pom1 = temp;
                temp = pom1->desni;
            }
            if (pom2 != t) {
                pom2->desni = pom1->lijevi;
                pom1->lijevi = t->lijevi;
                pom2->roditelj = pom1;
                pom1->roditelj = pomocni;
                if (pom2->desni) {
                    pom2->desni->roditelj = pom2;
                }
            }
            pom1->desni = t->desni;
            t->desni->roditelj = pom1;
        }
        if (pomocni == nullptr) {
            korijen = pom1;
            if (korijen) {
                korijen->roditelj = nullptr;
            }
        } else if (t == pomocni->lijevi) {
            pomocni->lijevi = pom1;
            if (pom1)
                pom1->roditelj = pomocni;
        } else {
            pomocni->desni = pom1;
            if (pom1)
                pom1->roditelj = pomocni;
        }
        if (t->roditelj && t->roditelj->lijevi == t) {
            t->roditelj->balans--;
        } else if (t->roditelj) {
            t->roditelj->balans++;
        }
        delete t;
        brojCvorova--;
    }
    void Preorder(Cvor *cvor){
        if(cvor != nullptr){
            dodaj(cvor->kljuc) = cvor->vrijednost;
            Preorder(cvor->lijevi);
            Preorder(cvor->desni);
        }
    }
    AVLStabloMapa &operator=(const AVLStabloMapa &avl) {
        if (this == &avl)
            return *this;
        obrisiSve(korijen);
        korijen = nullptr;
        brojCvorova = 0;
        prekopirajStablo(korijen, avl.korijen, nullptr);
        return *this;
    }
};

int main() {
    // Osnovni test funkcionalnosti AVLStabloMapa
    AVLStabloMapa<std::string,std::string> m;
    m["Sarajevo"] = "BiH";
    m["Zagreb"] = "Hrvatska";
    cout << m.brojElemenata() << " " << m["Zagreb"] << " '"; // 2 Hrvatska

// Test praznih elemenata
    cout << m["Beograd"] <<"' '"; // ''
    cout << m["zagreb"] << "' "; // ''
    m.obrisi("Zagreb");

// Potpuno korektna AVLStabloMapa nece kreirati objekte "Beograd" i "zagreb"
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
