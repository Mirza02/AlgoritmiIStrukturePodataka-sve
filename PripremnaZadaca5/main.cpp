#include <iostream>

int fib_petlja (int n) {
    if (n<=1) return n; // 0 i 1
    int pretprosli(0), prosli(1), rezultat;
    for (int i(2); i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}

int fib2_0(int n, int prosli = 1, int pretprosli = 0, int rezultat = 0){
    if(n <= 1) return rezultat;
    rezultat = pretprosli + prosli;
    pretprosli = prosli;
    prosli = rezultat;
    return fib2_0(n - 1, prosli, pretprosli, rezultat);
}



int nzd(int x, int y){
    if(y == 0) return x;
    int t;
    if(x < y){
        t = nzd(x, (y % x));
    }
    else if(y < x){
        t = nzd(y, (x % y));
    }
    return t;
}

int main() {

    return 0;
}
