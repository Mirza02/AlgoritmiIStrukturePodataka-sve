#include <iostream>
using namespace std;

template <typename Tip>
class UsmjereniGraf{
    UsmjereniGraf(int a){}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova() = 0;
    virtual void dodajGranu(int a, int b, float t = 0) = 0;
    virtual void obrisiGranu(int a, int b) = 0;
    virtual bool postojiGrana(int a, int b) const = 0;
    virtual void postaviTezinuGrane(int a, int b, float t = 0) = 0;
    virtual float dajTezinuGrane(int a, int b) const = 0;
    virtual void postaviOznakuCvora(int a, Tip b) = 0;
    virtual Tip dajOznakuCvora(int a) const = 0;
    virtual void postaviOznakuGrane(int a, int b, Tip o) = 0;
    virtual Tip dajOznakuGrane(int a, int b) const = 0;
    virtual Grana<Tip> dajGranu(int a, int b) = 0;
    virtual Cvor<Tip> dajCvor(int a) = 0;
    virtual const Grana
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
