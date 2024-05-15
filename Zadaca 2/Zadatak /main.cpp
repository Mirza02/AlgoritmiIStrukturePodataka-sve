#include <iostream>

template <typename Tip>
class DvostraniRed{
    struct Cvor{
        Tip element;
        Cvor *sljedeci;
        Cvor *prethodni;
        Cvor() : sljedeci(nullptr), prethodni(nullptr){}
        Cvor(Tip el, Cvor *sljedeci = nullptr,Cvor *prethodni = nullptr): element(el),
                                                      sljedeci(sljedeci), prethodni(prethodni){}
    };
    Cvor *prvi;
    Cvor *zadnji;
    int duzina;
public:
    DvostraniRed() : prvi(nullptr), zadnji(nullptr), duzina(0){}
    ~DvostraniRed(){
        if(duzina == 0) return;
        while(prvi != zadnji){
            Cvor *temp = prvi;
            prvi = prvi->sljedeci;
            delete temp;
        }
        delete prvi;
    }
    DvostraniRed(const DvostraniRed &r){
        prvi = nullptr;
        zadnji = nullptr;
        duzina = 0;
        Cvor *rtemp = r.prvi;
        for(int i = 0; i < r.duzina; i++){
            this->staviNaCelo(rtemp->element);
            rtemp = rtemp->sljedeci;
        }
    }
    DvostraniRed &operator=(const DvostraniRed &r){
        if(this == &r) return *this;
        this->~DvostraniRed();
        prvi = nullptr;
        zadnji = nullptr;
        duzina = 0;
        Cvor *rtemp = r.prvi;
        for(int i = 0; i < r.duzina; i++){
            this->staviNaCelo(rtemp->element);
            rtemp = rtemp->sljedeci;
        }
        return *this;
    }
    void staviNaVrh(Tip el){
        if(duzina == 0){
            prvi = new Cvor(el, nullptr, nullptr);
            zadnji = prvi;
            prvi->sljedeci = zadnji;
            prvi->prethodni = zadnji;
            duzina++;
        }
        else{
            Cvor *temp = new Cvor(el, prvi, zadnji);
            prvi->prethodni = temp;
            zadnji->sljedeci = temp;
            prvi = temp;
            duzina++;
        }
    }

    Tip skiniSaVrha(){
        if(duzina == 0) throw "Red je prazan";
        Cvor *temp = prvi;
        prvi = prvi->sljedeci;
        zadnji->sljedeci = prvi;
        prvi->prethodni = zadnji;
        Tip el = temp->element;
        delete temp;
        duzina--;
        return el;
    }

    void staviNaCelo(Tip el){
        if(duzina == 0){
            prvi = new Cvor(el, nullptr, nullptr);
            zadnji = prvi;
            prvi->sljedeci = zadnji;
            prvi->prethodni = zadnji;
            duzina++;
        }
        else{
            Cvor *temp = new Cvor(el, prvi, zadnji);
            zadnji->sljedeci = temp;
            prvi->prethodni = temp;
            zadnji = temp;
            duzina++;
        }
    }

    Tip skiniSaCela(){
        if(duzina == 0) throw "Red je prazan";
        Cvor *temp = zadnji;
        zadnji = zadnji->prethodni;
        zadnji->sljedeci = prvi;
        prvi->prethodni = zadnji;
        Tip el = temp->element;
        delete temp;
        duzina--;
        return el;
    }

    void brisi(){
        this->~DvostraniRed();
        prvi = nullptr;
        zadnji = nullptr;
        duzina = 0;
    }

    Tip vrh(){
        if(duzina == 0) throw "Red je prazan";
        return prvi->element;
    }

    Tip celo(){
        if(duzina == 0) throw "Red je prazan";
        return zadnji->element;
    }

    int brojElemenata(){
        return duzina;
    }
};


void DvRedTest1(){           // test konstruktora, metode staviNaVrh() i brojElemenata()
    std::cout << "Dvostruki red test 1: " << std::endl;
    DvostraniRed<int> red1;
    for(int i = 0; i < 10; i++){
        red1.staviNaVrh(i);
    }
    std::cout << red1.brojElemenata() << std::endl << std::endl;
}

void DvRedTest2(){          // test metoda staviNaCelo() i vrh()
    std::cout << "Dvostruki red test 2: " << std::endl;
    DvostraniRed<int> red2;
    for(int i = 0; i < 100; i++){
        red2.staviNaCelo(i);
    }
    std::cout << red2.vrh() << std::endl << std::endl;
}

void DvRedTest3(){          // test metoda skiniSaVrha() i celo()
    std::cout << "Dvostruki red test 3: " << std::endl;
    DvostraniRed<int> red3;
    for(int i = 0; i < 5; i++){
        red3.staviNaCelo(i);
    }
    std::cout << red3.celo() << std::endl;
    for(int i = 0; i < 5; i++){
        std::cout << red3.skiniSaVrha() << " ";
    }
    std::cout << std::endl << std::endl;
}

void DvRedTest4(){          // test konstruktora kopije i metode brisi()
    std::cout << "Dvostruki red test 4: " << std::endl;
    DvostraniRed<int> red4;
    for(int i = 100; i < 200; i += 3){
        red4.staviNaVrh(i);
    }
    DvostraniRed<int> redkop(red4);
    DvostraniRed<int> redjed;
    redjed = redkop;
    red4.brisi();
    for(int i = 0; i < 4; i++){
        redjed.skiniSaVrha();
    }
    std::cout << "Broj elemenata prvog reda: " << red4.brojElemenata() << std::endl;
    std::cout << "Broj elemenata drugog reda: " << redkop.brojElemenata() << std::endl;
    std::cout << "Broj elemenata treceg reda: " << redjed.brojElemenata() << std::endl << std::endl;
}

int main() {
    DvRedTest1();
    DvRedTest2();
    DvRedTest3();
    DvRedTest4();
}
