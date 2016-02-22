#include <iostream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#define ROZM_TAB 7
#define MAX_LICZ_ELEM 1000

using namespace std;


struct sekwencja
{
    string amino;
    int wagi[MAX_LICZ_ELEM];
    int liczElem;
};

struct wierzcholek
{
    int wiersz;
    int kolumna;
};

struct klik
{
    struct wierzcholek p1;
    struct wierzcholek p2;
    int liczElem;
} ;

struct klik kliki[MAX_LICZ_ELEM][MAX_LICZ_ELEM];

struct grupaKlik
{
    struct wierzcholek wKlik;
    int liczbaWierszy;
    int liczbaElementow;
    struct wierzcholek listaWierzch[100];
};




struct grupaKlik grupyKlik[MAX_LICZ_ELEM];
int skojarzMaxOdl = 0;
int skojarzGlebokosc = 0;

struct sekwencja instancja[ROZM_TAB];
int **grafSkojarzen;

void odczytajZPliku(const char * nazwaPliku);
void porownaj();
int porownajNukleotydy(unsigned int wiersz1, unsigned int  kolumna1, unsigned int wiersz2, unsigned int kolumna2);
int indexTablicy(int wiersz, int kolumna);
int dajiWylaczSkos(unsigned int wiersz, unsigned int kolumna);
void utworzGrafSkojarzen();
void drukujGrafSkojarzen();
void zerujSkojarzTablPoz();
void szukajKlik();
void drukujKlik();
void grupujKlik();
void zerujGrupyKlik();

// wielkosc okna
int okno = 4;
// wiarygodnosc
int wiarygodnosc = 5;
// maksymalna liczba opuszczonych elementow
int maxOpuszczone = 2;



int main()
{

    printf("Podaj wartosc okna: ");
    scanf("%d", &okno);
    while (okno < 3  || 8 < okno)
    {
        printf("Niepoprawny zakres liczby.\n");
        printf("Podaj liczbe z zakresu 4 -7. \n");
        printf("Podaj wartosc okna: ");
        scanf("%d", &okno);
    }
    printf("Podaj wartosc wiarygodnosci: ");
    scanf("%d", &wiarygodnosc);


    odczytajZPliku("instacja.txt");
    utworzGrafSkojarzen();

    porownaj();
    drukujGrafSkojarzen();
    szukajKlik();
    drukujKlik();
    grupujKlik();
}

void szukajKlik()
{
    int xKlik = 0;
    int yKlik = 0;

    for(int i = 0; i < ROZM_TAB * MAX_LICZ_ELEM; i++)
    {
        int pklik = 0;
        for(int j = 0; j < ROZM_TAB * MAX_LICZ_ELEM; j++)
            if(grafSkojarzen[i][j] == 1)
            {
                int ileSkos = dajiWylaczSkos(i, j);
                kliki[xKlik][yKlik].p1.wiersz = (unsigned int) i / MAX_LICZ_ELEM;
                kliki[xKlik][yKlik].p1.kolumna= (unsigned int) i % MAX_LICZ_ELEM;
                kliki[xKlik][yKlik].p2.wiersz = (unsigned int) j / MAX_LICZ_ELEM;
                kliki[xKlik][yKlik].p2.kolumna= (unsigned int)j % MAX_LICZ_ELEM;
                kliki[xKlik][yKlik].liczElem = ileSkos;
                pklik = 1;
                yKlik++;
            }
        if( pklik == 1)
        {
            xKlik++;
            pklik = 0;
        }
        if(xKlik >= MAX_LICZ_ELEM || yKlik >= MAX_LICZ_ELEM)
        {
            cout << " Za duza liczba elementow w tablicy klik" << xKlik << " " << yKlik;
            return;
        }
    }
}


void drukujKlik()
{
    cout << " Wydruk klik" << endl;
    for(int i = 0; i < MAX_LICZ_ELEM; i++)
        for(int j = 0; j < MAX_LICZ_ELEM; j++)
        {
            if(kliki[i][j].liczElem == 0)
                continue;
            cout << " w1="  << kliki[i][j].p1.wiersz << "," << " k1=" << kliki[i][j].p1.kolumna
                 << " w2="  << kliki[i][j].p2.wiersz << "," << " k2=" << kliki[i][j].p2.kolumna
                 << " liczbaEl=" << kliki[i][j].liczElem
                 << " Amino1:" << instancja[kliki[i][j].p1.wiersz].amino.substr(kliki[i][j].p1.kolumna, okno + kliki[i][j].liczElem - 1)
                 << " Amino2:" << instancja[kliki[i][j].p2.wiersz].amino.substr(kliki[i][j].p2.kolumna, okno + kliki[i][j].liczElem - 1)
                 << endl;
        }
}

void grupujKlik()
{
    cout << " Wydruk klik" << endl;
    int popWiersz = 0;
    int popKol = 0;
    int popMinWar = 0;
    int maxWiersz = 0;
    int maxKol = 0;
    int maxElem = 0;
    for(int i = 0; i < MAX_LICZ_ELEM; i++)
        for(int j = 0; j < MAX_LICZ_ELEM; j++)
        {
            if(kliki[i][j].liczElem == 0)
                continue;
            if( popWiersz != kliki[i][j].p1.wiersz ||
                    popKol != kliki[i][j].p1.kolumna)
            {
                if( maxElem < popMinWar)
                {
                    maxElem   = popMinWar;
                    maxWiersz = popWiersz;
                    maxKol    = popKol;
                }

                cout << " w="  << popWiersz << "," << " k=" << popKol
                     << " elem=" << popMinWar
                     << " Amino:" << instancja[popWiersz].amino.substr(popKol, okno + popMinWar - 1)
                     << endl;
                popMinWar = 0;
            }
            popWiersz = kliki[i][j].p1.wiersz;
            popKol    = kliki[i][j].p1.kolumna;
            if(popMinWar == 0)
                popMinWar = kliki[i][j].liczElem;
            else
                popMinWar = popMinWar < kliki[i][j].liczElem ? popMinWar : kliki[i][j].liczElem;
        }
    cout << " w="  << popWiersz << "," << " k=" << popKol
         << " elem=" << popMinWar
         << " Amino:" << instancja[popWiersz].amino.substr(popKol, okno + popMinWar - 1)
         << endl;

    cout << "Najdluzszy element:" << endl;
     cout << " w="  << maxWiersz << "," << " k=" << maxKol
         << " elem=" << maxElem
         << " Amino:" << instancja[maxWiersz].amino.substr(maxKol, okno + maxElem - 1)
         << endl;


}


int dajiWylaczSkos(unsigned int wiersz, unsigned int kolumna)
{
    int wWiersz = wiersz;
    int wKol = kolumna;
    int liczElem = 0;
    int pomin = 0;
    while(grafSkojarzen[wWiersz][wKol] == 1 || pomin <= maxOpuszczone)
    {
        if(grafSkojarzen[wWiersz][wKol] != 1 )
            pomin++;

        liczElem++;
        if(grafSkojarzen[wWiersz][wKol] == 1)
            grafSkojarzen[wWiersz][wKol] = 2;
        wWiersz++;
        wKol++;
    }
    // cofniecie pominietych elementow
    if(grafSkojarzen[wWiersz -1 ][wKol -1] != 1)
        liczElem = liczElem - pomin;

    return liczElem;
}


void zerujSkojarzTablPoz()
{
    for(int i = 0; i < MAX_LICZ_ELEM; i++)
        for(int j = 0; j < MAX_LICZ_ELEM; j++)
        {
            kliki[i][j].p1.wiersz = -1;
            kliki[i][j].p1.kolumna = -1;
            kliki[i][j].p2.wiersz = -1;
            kliki[i][j].p2.kolumna = -1;
            kliki[i][j].liczElem = 0;
        }
}


void porownaj()
{
    for (unsigned int wiersz1 = 0; wiersz1 < ROZM_TAB; wiersz1++)
        for (unsigned int wiersz2 =0; wiersz2 < ROZM_TAB; wiersz2++)
        {
            if ( wiersz1 == wiersz2)
                continue;
            for (int kolumna1 = 0; kolumna1 < instancja[wiersz1].liczElem - okno; kolumna1++)
                for (int kolumna2 =0; kolumna2 < instancja[wiersz2].liczElem - okno; kolumna2++)
                {
                    int ret = porownajNukleotydy(wiersz1, kolumna1, wiersz2, kolumna2);

                    if( ret == 1)
                    {
                        grafSkojarzen[indexTablicy(wiersz1, kolumna1)][indexTablicy(wiersz2, kolumna2)] = 1;
                        //    grafSkojarzen[indexTablicy(wiersz2, kolumna2)][indexTablicy(wiersz1, kolumna1)] = 1;
                    }
                    else
                    {
                        grafSkojarzen[indexTablicy(wiersz1, kolumna1)][indexTablicy(wiersz2, kolumna2)] = 0;
                        //    grafSkojarzen[indexTablicy(wiersz2, kolumna2)][indexTablicy(wiersz1, kolumna1)] = 0;

                    }
                }
        }
}


int porownajNukleotydy(unsigned int wiersz1, unsigned int kolumna1, unsigned int wiersz2, unsigned int kolumna2)
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
        return 0;

    size_t found1 = amino1.find(aminoW2);
    if ( found1 != string::npos )
        return 1;

    size_t found2 = amino2.find(aminoW1);
    if ( found2 != string::npos )
        return 1;

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
            cout << "Wybieram mniejsz¹ wartosc." << endl;
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
