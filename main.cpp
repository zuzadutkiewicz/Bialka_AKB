#include <iostream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#define ROZM_TAB 7
#define MAX_LICZ_ELEM 200

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
// int **grafSkojarzen;

void odczytajZPliku(const char * nazwaPliku);
int porownajAmin(int wiersz1, int  kolumna1, int wiersz2, int kolumna2);
int skojarzNast(int skojarzWiersz, int skojarzKol, int wiersz, int kolumna, struct skojarz * skojarzTmp);
void zerujSkojarzTablPoz();
int generujBialka();
int nastepnyPoziom(int poziom, int skojarzKol, int wiersz, int kolumna);
void drukujBialka();
int okno = 5;
int wiarygodnosc = 28;

int maxOpuszczone = 20;
int minLiczElem = 3;
int glebokoscSzukania = 3;

int main()
{
    odczytajZPliku("instacja.txt");
    int jest = generujBialka();
    if( jest == 1)
        drukujBialka();
    else
        cout << "Brak rozwiazan" << endl;
}


int generujBialka()
{
    int opuszczone = 0;
    int liczbaElem = 0;
    int skojarzKol = 0;
    int wiersz = 0;
    zerujSkojarzTablPoz();
    for(int kolumna = 0; kolumna < MAX_LICZ_ELEM; kolumna++)
    {
        int skojarzWiersz = 0;
        int znaleziony = nastepnyPoziom(skojarzWiersz, skojarzKol, wiersz, kolumna);
        if(znaleziony == 1 )
        {
            liczbaElem = liczbaElem + 1;
            skojarzTabl[skojarzWiersz][skojarzKol].wiersz = wiersz;
            skojarzTabl[skojarzWiersz][skojarzKol].kolumna = kolumna;
            if ( liczbaElem > minLiczElem )
                break;
        }
        else
            opuszczone = opuszczone + 1;

        if(opuszczone > maxOpuszczone)
        {
            // wyczysc tablice i szukaj dalsze elementy
            zerujSkojarzTablPoz();
            skojarzKol = 0;
        }
        // przejscie do nastepnej kolumny
        skojarzKol++;
    }
    if(liczbaElem >= minLiczElem)
        return 1;
    return 0;
}

int nastepnyPoziom(int skojarzWiersz, int skojarzKol, int wiersz, int kolumna)
{
    struct skojarz skojarzTmp;
    wiersz++;
    wiersz = wiersz % ROZM_TAB;
    skojarzWiersz++;
    if(wiersz > glebokoscSzukania)
        return 1;
    if(skojarzNast(skojarzWiersz, skojarzKol, wiersz, kolumna, &skojarzTmp) == 1)
        if( nastepnyPoziom(skojarzWiersz, skojarzKol, wiersz, kolumna) == 1)
        {
            skojarzTabl[skojarzWiersz][skojarzKol].wiersz = skojarzTmp.wiersz;
            skojarzTabl[skojarzWiersz][skojarzKol].kolumna = skojarzTmp.kolumna;
            return 1;
        }
    return 0;
}

void drukujBialka()
{
    for(int j = 0; j < MAX_LICZ_ELEM; j++)
    {
        for(int i = 0; i < ROZM_TAB; i++)
        {
            int wiersz = skojarzTabl[i][j].wiersz;
            int kolumn = skojarzTabl[i][j].kolumna;
            if(wiersz == -1 || kolumn == -1 )
                break;
            string amino  = instancja[wiersz].amino.substr(kolumn, okno);
            string aminoW = "";
            for(int i = 0; i < okno; i++)
            {
                if ( instancja[wiersz].wagi[kolumn + i] > wiarygodnosc)
                    aminoW = aminoW + amino.substr(i, 1);
            }

            if( skojarzTabl[i][j].wiersz > -1)
            {
                cout << " wiersz: " << wiersz
                     << " kolumn: " << kolumn
                     << " amino:"   << amino
                     << " aminoW: " << aminoW
                     << endl;
            }
        }
        // cout << endl;
    }
}


int skojarzNast(int skojarzWiersz, int skojarzKol, int wiersz, int kolumna, struct skojarz * skojarzTmp)
{
    for(int i = kolumna; i <= kolumna + maxOpuszczone; i++)
    {
        // na podstawie linia i kolumna ustalic poprzednie wartosci
        // w tablicy skojarzen i sprawdzic czy sa odpowiednie skojarzenia
        // w danym wierszu z ewentualnym przesuniecie o maxOpuszczone
        int wiersz1  = wiersz - 1;
        int kolumna1 = kolumna;
        int wiersz2 = wiersz;
        int kolumna2 = i;
        int jestRowne = porownajAmin(wiersz1, kolumna1, wiersz2, kolumna2);
        if( jestRowne == 1 )
        {
            skojarzTmp->wiersz = wiersz;
            skojarzTmp->kolumna = i;
            return 1;
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

    if ( aminoW1.compare("") == 0 || aminoW2.compare("") == 0 )
        return 1;

    size_t found1 = amino1.find(aminoW2);
    if ( found1 != string::npos )
        return 1;

    size_t found2 = amino1.find(aminoW2);
    if ( found2 != string::npos )
        return 1;


    return 0;
}


int indexTablicy(int wiersz, int kolumna)
{
    return wiersz * MAX_LICZ_ELEM + kolumna;
}


//
// odczytanie danych z pliku do tablicy
//
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

