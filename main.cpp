#include <iostream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#define ROZM_TAB 7
#define MAX_LICZ_ELEM 150

using namespace std;



struct sekwencja
{
    char amino[MAX_LICZ_ELEM];
    int wagi[MAX_LICZ_ELEM];
    int liczElem;
};

struct sekwencja instacja[ROZM_TAB];

void odczytajZPliku(const char * nazwaPliku);

int main()
{
odczytajZPliku("instacja.txt");
}


void odczytajZPliku(const char * nazwaPliku)
{
    fstream plik;
    string lineAmin;
    string lineWag;
    int element = 0;
    plik.open(nazwaPliku, ios::in);
    if(plik.good() != true)
    {
        printf("Nie mozna otworzyc pliku %s", nazwaPliku);
        exit(1);
    }

    while(getline(plik,lineAmin))
    {


        const char * buf = lineAmin.c_str();

        int liczAmin = 0;

        while(buf[liczAmin] !='\0')
        {
           instacja[element].amino[liczAmin] = buf[liczAmin];
           cout << instacja[element].amino[liczAmin];
           liczAmin++;

        }
        cout << endl;
        getline(plik,lineWag);

        const char * bufWag = lineWag.c_str();
        int i = 0;
        int j = 0;
        int liczWag = 0;
        char buflicz[10];
        //int liczKolumn = 0;
        while(true)
        {
            if(bufWag[i] != ' ' && bufWag[i] !='\0')
            {
                buflicz[j]=bufWag[i];
                j++;
            }
            else
            {
                int dana = 0;
                buflicz[j] = '\0';
                dana = atoi(buflicz);
                instacja[element].wagi[liczWag] = dana;
                cout << instacja[element].wagi[liczWag] << " ";
                liczWag++;
                j = 0;
            }
            if(bufWag[i] == '\0')
            break;
            i++;
        }



        cout << endl << "Koniec instancji" << endl;

        cout << "Amin: " << element << "  " << lineAmin << endl;
        cout << "Wag: " << element << "  " << lineWag << endl;
        element++;
    }

    plik.close();
    printf("Odczyt z pliku %s zakonczony powodzeniem.\n", nazwaPliku);
}


