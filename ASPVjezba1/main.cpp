#include <iostream>
#include <cmath>
#include <ctime>

bool DaLiJeProst(int n){
    if(n == 1) return true;
    for(int i = 2; i <= sqrt(n); i++){
        if(n % i == 0) return false;
    }
    return true;
}

void Goldbach(int n, int &p, int &q){
    if(n <= 2 || n % 2 != 0) throw std::range_error("Van opsega");
    for(int i = 1; i < n; i++){
        if(DaLiJeProst(i)){
            int u = n - i;
            if(DaLiJeProst(u)){
                p = i;
                q = n - i;
                return;
            }
        }
    }
    throw std::domain_error("Netacna hipoteza");
}

int main() {
    std::cout << "Unesite n: ";
    int n;
    std::cin >> n;
    int p, q;
    clock_t vrijeme1 = clock();
    try{
        Goldbach(n, p, q);
    }
    catch(std::range_error opseg){
        std::cout << opseg.what();
        return 2;
    }
    catch(std::domain_error hip){
        std::cout << hip.what();
        return 1;
    }
    clock_t vrijeme2 = clock();
    int a = CLOCKS_PER_SEC;
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Vrijeme izvrsenja: " << ukvrijeme << "ms." << std::endl;
    std::cout << "p = " << p << " q = " << q;
    return 0;
}
