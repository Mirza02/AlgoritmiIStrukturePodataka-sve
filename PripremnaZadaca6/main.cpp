#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;


template <typename Tip>
void bubble_sort(Tip* niz, int vel){
    int p = vel;
    do{
        int i = p - 1;
        p = 0;
        for(int j = 1; j <= i; j++){
            if(niz[j - 1] > niz[j]){
                Tip temp = niz[j - 1];
                niz[j - 1] = niz[j];
                niz[j] = temp;
                p = j;
            }
        }
    } while( p != 0);
}

template <typename Tip>
void selection_sort(Tip* niz, int vel){
    for(int i = 0; i <= vel - 2; i++){
        Tip min = niz[i];
        int pmin = i;
        for(int j = i + 1; j <= vel - 1; j++){
            if(niz[j] < min){
                min = niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }
}

template <typename Tip>
void merge(Tip* niz, int l, int p, int q, int u){
    int i = 0;
    int j = q - l;
    int k = l;
    Tip *temp = new Tip[u - l + 1];
    for(int m = 0; m <= u - l; m++){
        temp[m] = niz[l + m];
    }
    while(i <= p - l && j <= u - l){
        if(temp[i] < temp[j]){
            niz[k] = temp[i];
            i++;
        }
        else{
            niz[k] = temp[j];
            j++;
        }
        k++;
    }
    while(i <= p - l){
        niz[k] = temp[i];
        k++;
        i++;
    }
    while(j <= u - l){
        niz[k] = temp[j];
        k++;
        j++;
    }
    delete[] temp;
}

template <typename Tip>
void merge_sort_recursive(Tip* niz, int l, int u){
    if(u > l){
        int p = (l + u - 1) / 2;
        int q = p + 1;
        merge_sort_recursive(niz, l, p);
        merge_sort_recursive(niz, q, u);
        merge(niz, l, p, q, u);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel){
    merge_sort_recursive(niz, 0, vel - 1);
}

template <typename Tip>
int particija(Tip* niz, int prvi, int zadnji){
    Tip pivot = niz[prvi];
    int p = prvi + 1;
    while(p <= zadnji && niz[p] < pivot){
        p++;
    }
    for(int i = p + 1; i <= zadnji; i++){
        if(niz[i] < pivot){
            Tip temp = niz[i];
            niz[i] = niz[p];
            niz[p] = temp;
            p++;
        }
    }
    Tip temp = niz[prvi];
    niz[prvi] = niz[p - 1];
    niz[p - 1] = temp;
    return p - 1;
}

template <typename Tip>
void quick_sort_recursive(Tip* niz, int prvi, int zadnji){
    if(prvi < zadnji){
        int j = particija(niz, prvi, zadnji);
        quick_sort_recursive(niz, prvi, j - 1);
        quick_sort_recursive(niz, j + 1, zadnji);
    }
}

template <typename Tip>
void quick_sort(Tip* niz, int vel){
    quick_sort_recursive(niz, 0, vel - 1);
}

void ucitaj(std::string filename, int*& niz, int &vel){
    std::ifstream ulaz(filename);
    vel = -1;
    int element;
    while(ulaz >> element){
        vel++;
    }
    ulaz.close();
    niz = new int[vel];
    ulaz.open(filename);
    for(int i = 0; i < vel; i++){
        ulaz >> niz[i];
    }
}

void generisi(std::string filename, int vel){
    std::ofstream izlaz(filename);
    time_t current_time = time(NULL);
    std::srand(current_time);
    for(int i = 0; i < vel; i++){
        izlaz << std::rand() % 100 + 1 << " ";
    }
}

void sortiranje(std::string filename){
    int vel;
    int* niz;
    ucitaj("nesortirani_brojevi.txt", niz, vel);
    std::cout << "1. Bubble sort" << std::endl << "2. Selection sort" << std::endl
    << "3. Quick sort" << std::endl << "4. Merge sort" << std::endl;
    int n;
    std::cin >> n;
    clock_t vrijeme1, vrijeme2;
    switch(n){
        case 1 :
            vrijeme1 = clock();
            bubble_sort(niz, vel);
            vrijeme2 = clock();
            break;
        case 2 :
            vrijeme1 = clock();
            selection_sort(niz, vel);
            vrijeme2 = clock();
            break;
        case 3 :
            vrijeme1 = clock();
            quick_sort(niz, vel);
            vrijeme2 = clock();
            break;
        case 4 :
            vrijeme1 = clock();
            merge_sort(niz, vel);
            vrijeme2 = clock();
            break;
        default :
            std::cout << "Unijeli ste pogresnu vrijednost!";
            return;
    }
    int vrijeme_izvrsenja = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Vrijeme izvrsenja: " << vrijeme_izvrsenja << "s" << std::endl;
    std::ofstream izlaz("sortirani_brojevi.txt");
    for(int i = 0; i < vel; i++){
        izlaz << niz[i] << " ";
    }
    izlaz.close();
}

int main() {
    int n;
    std::cout << "Unesite broj elemenata: ";
    std::cin >> n;
    generisi("nesortirani_brojevi.txt", n);
    sortiranje("nesortirani_brojevi.txt");
    return 0;
}
