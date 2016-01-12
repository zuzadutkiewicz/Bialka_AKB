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
    string amino;
    int wagi[MAX_LICZ_ELEM];
    int liczElem;
};

struct sekwencja instacja[ROZM_TAB];
int **grafSkojarzen;

void odczytajZPliku(const char * nazwaPliku);
void porownaj();
int porownajAmin(int wiersz1, int  kolumna1, int wiersz2, int kolumna2);
int indexTablicy(int wiersz, int kolumna);
void utworzGrafSkojarzen();
void drukujGrafSkojarzen();

int okno = 5;
int wiarygodnosc = 10;

int main()
{
    odczytajZPliku("instacja.txt");
    utworzGrafSkojarzen();
    porownaj();
    drukujGrafSkojarzen();
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

        int liczAmin = 0;

        instacja[element].amino = lineAmin;
        liczAmin =  lineAmin.length() + 1;
        cout << "Amin: " << instacja[element].amino[liczAmin] << endl;
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

        liczAmin-=1;
        liczWag-=1;
        if(liczAmin != liczWag)
        {
            cout << "Rozna liczba elementow na pozycji: " << element << endl;
            cout << "Liczba Amin: " << liczAmin << endl;
            cout << "Liczba Wag: " << liczWag << endl;
            cout << "Wybieram mniejszą wartosc." << endl;
        }

        instacja[element].liczElem = liczAmin > liczWag ? liczWag : liczAmin;

        cout << endl << "Koniec instancji" << endl;

        cout << "Amin: " << element << "  " << lineAmin << endl;
        cout << "Wag: " << element << "  " << lineWag << endl;
        element++;
    }

    plik.close();
    printf("Odczyt z pliku %s zakonczony powodzeniem.\n", nazwaPliku);
}


void porownaj()
{
    for (int wiersz1 = 0; wiersz1 < ROZM_TAB; wiersz1++)
        for (int wiersz2 =0; wiersz2 < ROZM_TAB; wiersz2++)
        {
            if ( wiersz1 == wiersz2)
                continue;
            for (int kolumna1 = 0; kolumna1 < instacja[wiersz1].liczElem - okno; kolumna1++)
                for (int kolumna2 =0; kolumna2 < instacja[wiersz2].liczElem - okno; kolumna2++)
                {
                    int ret = porownajAmin(wiersz1, kolumna1, wiersz2, kolumna2);

                    if( ret == 1)
                    {
                        grafSkojarzen[indexTablicy(wiersz1, kolumna1)][indexTablicy(wiersz2, kolumna2)] =1;
                        grafSkojarzen[indexTablicy(wiersz2, kolumna2)][indexTablicy(wiersz1, kolumna1)] =1;
                        //    cout << " amino1 " << wiersz1 << " " << kolumna1 << " idx " << indexTablicy(wiersz1, kolumna1) << endl;
                        //    cout << " amino2 " << wiersz2 << " " << kolumna2 << " idx " << indexTablicy(wiersz2, kolumna2) << endl << endl;
                    }
                }
        }
}


int porownajAmin(int wiersz1, int kolumna1, int wiersz2, int kolumna2)
{
    string amino1 = instacja[wiersz1].amino.substr(kolumna1, okno);
    string amino2 = instacja[wiersz2].amino.substr(kolumna2, okno);
    if( amino1.compare(amino2) == 0)
    {
        return 1;
    }
    return 0;
}

int indexTablicy(int wiersz, int kolumna)
{
    return wiersz * MAX_LICZ_ELEM + kolumna;
}

void utworzGrafSkojarzen()
{
    grafSkojarzen = new int *[ROZM_TAB * MAX_LICZ_ELEM];
    for ( int i = 0; i < ROZM_TAB * MAX_LICZ_ELEM; ++i )
        grafSkojarzen[i] = new int [ROZM_TAB * MAX_LICZ_ELEM];
}

void drukujGrafSkojarzen()
{
    for(int i = 0; i < ROZM_TAB * MAX_LICZ_ELEM; i++)
        for(int j = 0; j < ROZM_TAB * MAX_LICZ_ELEM; j++)
            if(grafSkojarzen[i][j] == 1)
                cout << "grafSkojarzen[" << i << "][" << j << "]=" << grafSkojarzen[i][j] << endl;
}
