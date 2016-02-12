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

struct skojarz
{
    int wiersz;
    int kolumna;
} ;

struct skojarz skojarzTabl[ROZM_TAB][MAX_LICZ_ELEM];
int skojarzMaxOdl = 0;
int skojarzGlebokosc = 0;

struct sekwencja instancja[ROZM_TAB];
int **grafSkojarzen;

void odczytajZPliku(const char * nazwaPliku);
void porownaj();
int porownajAmin(int wiersz1, int  kolumna1, int wiersz2, int kolumna2);
int indexTablicy(int wiersz, int kolumna);
void utworzGrafSkojarzen();
void drukujGrafSkojarzen();
int skojarzNast(int wiersz, int kolumna, struct skojarz skojarzTmp);
void zerujSkojarzTablPoz();
int generujBialka();
int nastepnyPoziom(int wiersz, int kolumna);
void drukujBialka();
int okno = 5;
int wiarygodnosc = 0;

int maxOpuszczone = 3;
int minLiczElem = 4;
int glebokoscSzukania = 4;

int main()
{
    odczytajZPliku("instacja.txt");
    utworzGrafSkojarzen();
    porownaj();
    drukujGrafSkojarzen();
    int jest = generujBialka();
    if( jest == 1)
    {
        drukujBialka();
    }
}

int generujBialka()
{
    int opuszczone = 0;
    int liczbaElem = 0;
    for(int i = 0; i < MAX_LICZ_ELEM; i++)
    {
        if( skojarzTabl[0][i].wiersz == -1)
            break;
        int znaleziony = nastepnyPoziom(0,i);
        if(znaleziony == 1)
            liczbaElem = liczbaElem + 1;
        else
            opuszczone = opuszczone + 1;

        if(opuszczone > maxOpuszczone)
            break;
    }
    if(liczbaElem >= minLiczElem)
        return 1;
    return 0;
}

int nastepnyPoziom(int wiersz, int kolumna)
{
    struct skojarz skojarzTmp;
    wiersz++;
    if(wiersz > glebokoscSzukania)
        return 1;
    if(skojarzNast(wiersz, kolumna, skojarzTmp) == 1)
        if( nastepnyPoziom(wiersz, kolumna) == 1)
    {
        // TO_DO wpisac do tablicy znalezionych
        return 1;
    }
    return 0;
}

void drukujBialka()
{
    for(int i = 0; i < ROZM_TAB; i++)
    {
        for(int j = 0; j < MAX_LICZ_ELEM; j++)
        {
            int wiersz = skojarzTabl[i][j].wiersz;
            int kolumn = skojarzTabl[i][j].kolumna;
            string amino  = instancja[wiersz].amino.substr(kolumn, okno);
            if( skojarzTabl[i][j].wiersz > -1)
            {
            cout << " wiersz: " << wiersz
                 << " kolumn: " << kolumn
                 << " amino:"   << amino
                 << endl;
            }
        }
        cout << endl;
    }
}


int skojarzNast(int linia, int kolumna, struct skojarz skojarzTmp)
{
    for(int i = kolumna; i <= kolumna + maxOpuszczone; i++)
    {
        // na podstawie linia i kolumna ustalic poprzednie wartosci
        // w tablicy skojarzen i sprawdzic czy sa odpowiednie skojarzenia
        // w danym wierszu z ewentualnym przesuniecie o maxOpuszczone
        if( grafSkojarzen[linia][i] == 1 )
        {
        }
    }
        return 0;
}
void zerujSkojarzTablPoz()
{
    for(int i = 0; i < ROZM_TAB; i++)
        for(int j = 0; j < MAX_LICZ_ELEM; j++)
        {
            skojarzTabl[i][j].wiersz = -1;
            skojarzTabl[i][j].kolumna = -1;
        }
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

        instancja[element].amino = lineAmin;
        liczAmin =  lineAmin.length() + 1;
        cout << "Amin: " << instancja[element].amino[liczAmin] << endl;
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
                instancja[element].wagi[liczWag] = dana;
                cout << instancja[element].wagi[liczWag] << " ";
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

        instancja[element].liczElem = liczAmin > liczWag ? liczWag : liczAmin;

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
            for (int kolumna1 = 0; kolumna1 < instancja[wiersz1].liczElem - okno; kolumna1++)
                for (int kolumna2 =0; kolumna2 < instancja[wiersz2].liczElem - okno; kolumna2++)
                {
                    int ret = porownajAmin(wiersz1, kolumna1, wiersz2, kolumna2);

                    if( ret == 1)
                    {
                        grafSkojarzen[indexTablicy(wiersz1, kolumna1)][indexTablicy(wiersz2, kolumna2)] = 1;
                        grafSkojarzen[indexTablicy(wiersz2, kolumna2)][indexTablicy(wiersz1, kolumna1)] = 1;
                        //    cout << " amino1 " << wiersz1 << " " << kolumna1 << " idx " << indexTablicy(wiersz1, kolumna1) << endl;
                        //    cout << " amino2 " << wiersz2 << " " << kolumna2 << " idx " << indexTablicy(wiersz2, kolumna2) << endl << endl;
                    }
                    else
                    {
                        grafSkojarzen[indexTablicy(wiersz1, kolumna1)][indexTablicy(wiersz2, kolumna2)] = 0;
                        grafSkojarzen[indexTablicy(wiersz2, kolumna2)][indexTablicy(wiersz1, kolumna1)] = 0;

                    }
                }
        }
}


int porownajAmin(int wiersz1, int kolumna1, int wiersz2, int kolumna2)
{
    string amino1 = instancja[wiersz1].amino.substr(kolumna1, okno);
    string amino2 = instancja[wiersz2].amino.substr(kolumna2, okno);
    string aminoW1 = "";
    string aminoW2 = "";

    if( amino1.compare(amino2) == 0)
        return 1;

    for(int i = 0; i < okno; i++)
    {
        if ( instancja[wiersz1].wagi[kolumna1 + i] > wiarygodnosc)
            aminoW1 = aminoW1 + amino1.substr(i, 1);
        if ( instancja[wiersz2].wagi[kolumna2 + i] > wiarygodnosc)
            aminoW2 = aminoW2 + amino2.substr(i, 1);
    }

    if ( aminoW1.compare("") != 0 && aminoW2.compare("") != 0 )
    {
        size_t found1 = amino1.find(aminoW2);
        if ( found1 != string::npos )
            return 1;

        size_t found2 = amino1.find(aminoW2);
        if ( found2 != string::npos )
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
    {
        grafSkojarzen[i] = new int [ROZM_TAB * MAX_LICZ_ELEM];
        for(int j = 0; j < ROZM_TAB * MAX_LICZ_ELEM; j++)
                grafSkojarzen[i][j] = -1;
    }
}


void drukujGrafSkojarzen()
{
    for(int i = 0; i < ROZM_TAB * MAX_LICZ_ELEM; i++)
        for(int j = 0; j < ROZM_TAB * MAX_LICZ_ELEM; j++)
            if(grafSkojarzen[i][j] == 1)
                cout << "grafSkojarzen[" << i << "][" << j << "]=" << grafSkojarzen[i][j] << endl;
}

