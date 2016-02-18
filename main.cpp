/*
Copyright (C) 2016  Zuzanna Dutkiewicz

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <iostream>
#include <sstream>
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

struct zasieg
{
    string przed;
    string po;
};

struct zasieg zasiegTabl[ROZM_TAB][MAX_LICZ_ELEM];

struct sekwencja instancja[ROZM_TAB];

string koniecTabl[ROZM_TAB];

void wpiszKoniecTabl();
void drukujKoniectabl();
void wpiszZasiegTabl(string poziom);

void odczytajZPliku(const char * nazwaPliku);
int porownajAmin(int wiersz1, int  kolumna1, int wiersz2, int kolumna2);
int skojarzNast(int skojarzWiersz, int skojarzKol, int wiersz, int kolumna, struct skojarz * skojarzTmp);
void zerujSkojarzTablPoz();
void czyscZasiegTabl();
int generujBialka();
int nastepnyPoziom(int poziom, int skojarzKol, int wiersz, int kolumna);
void drukujBialka();

// parametry
int okno = 5;
int wiarygodnosc = 26;

int maxOpuszczone = 20;
int minLiczElem = 3;
int glebokoscSzukania = 2;


int main()
{
    printf("Podaj wartosc okna: ");
    scanf("%d", &okno);
    if ( 3 < okno > 8)
    {
        printf("Niepoprawny zakres liczby.\n");
        printf("Podaj liczbe z zakresu 4 -7. \n");
        printf("Podaj wartosc okna: ");
        scanf("%d", &okno);
    }
    printf("Podaj wartosc wiarygodnosci: ");
    scanf("%d", &wiarygodnosc);

    odczytajZPliku("instacja.txt");
    generujBialka();
    wpiszKoniecTabl();
    drukujKoniectabl();
}


int generujBialka()
{

    for(int wierszX = 6; wierszX < 7; wierszX++)
    {
        int opuszczone = 0;
        int liczbaElem = 0;
        int skojarzKol = 0;
        int znalezione = 0;
        int poziomZasiegu = 0;
        int wiersz = wierszX % ROZM_TAB;
        zerujSkojarzTablPoz();

        for(int kolumna = 0; kolumna < MAX_LICZ_ELEM - okno; kolumna++)
        {
            int skojarzWiersz = 0;
            int jest = nastepnyPoziom(skojarzWiersz, skojarzKol, wiersz, kolumna);
            if(jest == 1 )
            {
                liczbaElem = liczbaElem + 1;
                skojarzTabl[skojarzWiersz][skojarzKol].wiersz = wiersz;
                skojarzTabl[skojarzWiersz][skojarzKol].kolumna = kolumna;
                skojarzKol++;
            }
            else
                opuszczone = opuszczone + 1;

            if(opuszczone > maxOpuszczone)
            {
                // drukuj i wyczysc tablice i szukaj dalsze elementow
                if(liczbaElem >= minLiczElem)
                {
                    cout << "Znalezione elementy:" << endl;
                    drukujBialka();
                    poziomZasiegu++;
                    ostringstream ss;
                    ss << "(" << wierszX << ")" << poziomZasiegu;
                    string str = ss.str();
                    wpiszZasiegTabl(str );

                    znalezione = 1;
                }
                zerujSkojarzTablPoz();
                liczbaElem = 0;
                skojarzKol = 0;
            }
            // przejscie do nastepnej kolumny
            // skojarzKol++;
        }
        if(liczbaElem >= minLiczElem)
        {
            drukujBialka();
        }
        if( znalezione == 0)
            cout << " Brak rozwiazan." << endl;

    }

    return 0;
}


int nastepnyPoziom(int skojarzWiersz, int skojarzKol, int wiersz, int kolumna)
{
    struct skojarz skojarzTmp;
    wiersz++;
    wiersz = wiersz % ROZM_TAB;
    skojarzWiersz++;
    if(skojarzWiersz >= glebokoscSzukania)
        return 1;
    if(skojarzNast(skojarzWiersz, skojarzKol, wiersz, kolumna, &skojarzTmp) == 1)
    {
        if( nastepnyPoziom(skojarzWiersz, skojarzKol, skojarzTmp.wiersz, skojarzTmp.kolumna) == 1)
        {
            skojarzTabl[skojarzWiersz][skojarzKol].wiersz  = skojarzTmp.wiersz;
            skojarzTabl[skojarzWiersz][skojarzKol].kolumna = skojarzTmp.kolumna;
            return 1;
        }
    }
    return 0;
}




int skojarzNast(int skojarzWiersz, int skojarzKol, int wiersz, int kolumna, struct skojarz * skojarzTmp)
{
    // na podstawie linia i kolumna ustalic poprzednie wartosci
    // w danym wierszu z ewentualnym przesuniecie o maxOpuszczone
    int kolumnaTmp = 0;

    if(skojarzKol > 0)
        kolumnaTmp = skojarzTabl[skojarzWiersz][skojarzKol - 1].kolumna + 1;

    // nie moze przekraczac maksymalnej liczby elementow
    int kolumnaMax = kolumnaTmp + maxOpuszczone > MAX_LICZ_ELEM ? MAX_LICZ_ELEM : kolumnaTmp + maxOpuszczone;

    for(int i = kolumnaTmp; i <= kolumnaMax; i++)
    {
        // znajdz poprzedni wiersz
        int wierszp = wiersz == 0 ? ROZM_TAB -1 : wiersz - 1;

        int jestRowne = porownajAmin(wierszp, kolumna, wiersz, i);
        if( jestRowne == 1 )
        {
            skojarzTmp->wiersz = wiersz;
            skojarzTmp->kolumna = i;
            return 1;
        }
    }
    return 0;
}



int porownajAmin(int wiersz1, int kolumna1, int wiersz2, int kolumna2)
{
    if(kolumna1 + okno > instancja[wiersz1].amino.length() )
        return 0;
    if(kolumna2 + okno > instancja[wiersz2].amino.length() )
        return 0;

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

    size_t found2 = amino2.find(aminoW1);
    if ( found2 != string::npos )
        return 1;

    return 0;
}


void wpiszZasiegTabl(string poziom)
{
    for(int i = 0; i < ROZM_TAB; i++)
    {
        int wiersz = -1;
        int kolumn = -1;
        int kolMin = MAX_LICZ_ELEM;
        int kolMax = -1;
        for(int j = 0; j < MAX_LICZ_ELEM; j++)
        {
            if( skojarzTabl[i][j].wiersz != -1)
                wiersz = skojarzTabl[i][j].wiersz;
            kolumn = skojarzTabl[i][j].kolumna;
            if(kolumn > -1 && kolMax < kolumn)
                kolMax = kolumn;
            if(kolumn > -1 && kolMin > kolumn)
                kolMin = kolumn;
        }
        if( wiersz != -1)
        {
            kolMax = kolMax + okno - 1;
            cout << " wiersz: " << wiersz
                 << " kolMin: " << kolMin
                 << " kolMax: " << kolMax << endl;
            zasiegTabl[wiersz][kolMin].przed =
                zasiegTabl[wiersz][kolMin].przed + "[" + poziom + ">";
            zasiegTabl[wiersz][kolMax].po    =
                "<" + poziom + "]" + zasiegTabl[wiersz][kolMax].po;
        }
    }
    cout << endl;
}


void drukujBialka()
{
    int drukPrzerwy = 0;
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
                drukPrzerwy = 1;
            }
        }
        if( drukPrzerwy == 1)
        {
            cout << endl;
            drukPrzerwy = 0;
        }
    }
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
        // cout << "Amin: " << instancja[element].amino[liczAmin] << endl;
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
                // cout << instancja[element].wagi[liczWag] << " ";
                liczWag++;
                j = 0;
            }
            if(bufWag[i] == '\0')
                break;
            i++;
        }

        liczAmin-=1;
        liczWag-=1;

        instancja[element].liczElem = liczAmin > liczWag ? liczWag : liczAmin;


        cout << "Amin: " << element << "  " << lineAmin << endl;
        cout << "Wag: " << element << "  " << lineWag << endl << endl;
        element++;
    }

    plik.close();
    printf("Odczyt z pliku %s zakonczony powodzeniem.\n\n", nazwaPliku);
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

void czyscZasiegTabl()
{
    for(int i = 0; i < ROZM_TAB; i++)
        for(int j = 0; j < MAX_LICZ_ELEM; j++)
        {
            zasiegTabl[i][j].przed = "";
            zasiegTabl[i][j].po = "";
        }
}

void wpiszKoniecTabl()
{
    for(int i = 0; i < ROZM_TAB; i++)
    {
        unsigned int len = instancja[i].amino.length();
        for( unsigned int j = 0; j < len; j++)
        {
            koniecTabl[i] = koniecTabl[i] +
                            zasiegTabl[i][j].przed +
                            instancja[i].amino.substr(j,1) +
                            zasiegTabl[i][j].po;

        }
    }
}

void drukujKoniectabl()
{
    for(int i = 0; i < ROZM_TAB; i++)
        cout <<  " wiersz: " << i << " Amino:" << koniecTabl[i] << endl;

}
