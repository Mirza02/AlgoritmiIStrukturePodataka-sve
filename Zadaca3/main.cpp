#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void radixSort(std::vector<int> &a){
    int max = a[0];
    for(int i = 1; i < a.size(); i++){
        if(a[i] > max) max = a[i];
    }
    int stepen = 1;
    while(max / stepen > 0){
        std::vector<std::vector<int> > Q;
        Q.resize(10);
        for(int i = 0; i < a.size(); i++){
            Q[(a[i] / stepen) % 10].push_back(a[i]);
        }
        int k = 0;
        for(int i = 0; i < Q.size(); i++){
            for(int j = 0; j < Q[i].size(); j++){
                a[k] =  Q[i][j];
                k++;
            }
        }
        stepen *= 10;
    }
}

int roditelj(int i){
    return (i - 1) / 2;
}

int lijevoDijete(int i){
    return 2 * i + 1;
}

int desnoDijete(int i){
    return 2 * i + 2;
}

bool jeLiList(std::vector<int> &a, int i, int velicina){
    return (i >= velicina / 2) && (i < velicina);
}

void popraviDole(std::vector<int> &a, int i, int velicina){
    while(!jeLiList(a, i, velicina)){
        int veci = lijevoDijete(i);
        int dd = desnoDijete(i);
        if(dd < velicina && a[dd] > a[veci]){
            veci = dd;
        }
        if(a[i] > a[veci]){
            return;
        }
        std::swap(a[i], a[veci]);
        i = veci;
    }
}

void stvoriGomilu(std::vector<int> &a){
    for(int i = (a.size() / 2) - 1; i >= 0; i--){
        popraviDole(a, i, a.size());
    }
}

void popraviGore(std::vector<int> &a, int i){
    while(i != 0 && a[i] > a[roditelj(i)]){
        std::swap(a[i], a[roditelj(i)]);
        i = roditelj(i);
    }
}

void umetniUGomilu(std::vector<int> &a, int umetnuti, int &velicina){
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, velicina - 1);
}

int izbaciPrvi(std::vector<int> &a, int &velicina){
    if(velicina == 0){
        throw "Gomila je prazna!";
    }
    velicina--;
    std::swap(a[0], a[velicina]);
    if(velicina != 0){
        popraviDole(a, 0, velicina);
    }
    return a[velicina];
}

void gomilaSort(std::vector<int> &a){
    stvoriGomilu(a);
    int velicina = a.size();
    for(int i = 0; i <= a.size() - 2; i++){
        izbaciPrvi(a, velicina);
    }
}

int main() {
}
