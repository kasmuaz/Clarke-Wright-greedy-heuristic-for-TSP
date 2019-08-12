#ifndef GRAF_H
#define GRAF_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <ctime>

#include "netezinski_graf.h"

using namespace std;

struct Grad
{
    int indeks;
    double latitude;
    double longitude;
    Grad(int indeks, double latitude, double longitude):indeks(indeks), latitude(latitude), longitude(longitude) {}
};

class Graf
{

    struct Grana
    {
        int tezina;
        int pocetak;
        int kraj;
        Grana(int tezina, int pocetak, int kraj):tezina(tezina),pocetak(pocetak),kraj(kraj) {}
        friend bool operator <(const Grana& prva, const Grana& druga)
        {
            return prva.tezina < druga.tezina;
        }
    };

public:
    int n;
    vector <Grad> gradovi;
    vector <vector <int> > udaljenosti;
    vector<Grana> grane;

    int udaljenost(double x1, double y1, double x2, double y2)
    {
        return round(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
    }

    Graf(int n);
    Graf(string filename);

    vector<int> vratiTuraNajmanjaGrana();

    int turaNajmanjaGrana();
};

Graf::Graf(int N) {
  n = N;
  const double PI = 4 * atan(1);
  vector<int> indeksi;
  for(int i=0;i<n;i++)
    indeksi.push_back(i);
  //random_shuffle(indeksi.begin(),indeksi.end());

  for(int i=0;i<n;i++) {
    int indeks = i;

    double latitude = 1000 * n * sin((indeksi[i]/double(n)) * 2 * PI);
    double longitude = 1000 * n * cos((indeksi[i]/double(n)) * 2 * PI);
    gradovi.push_back(Grad(indeks,latitude,longitude));
  }
  udaljenosti.resize(n);
    for(int i=0; i<n; i++)
        udaljenosti[i].resize(n);
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            udaljenosti[i][j] = udaljenost(gradovi[i].latitude,gradovi[i].longitude,gradovi[j].latitude,gradovi[j].longitude);
            if(i<j)
                grane.push_back(Grana(udaljenosti[i][j],i,j));
        }
}

Graf::Graf(string filename)
{
    ifstream unos(filename);
    while(!unos.eof())
    {
        int indeks;
        double latitude;
        double longitude;
        unos>>indeks;
        unos.get();
        unos>>latitude;
        unos.get();
        unos>>longitude;
        unos.get();
        gradovi.push_back(Grad(indeks,latitude,longitude));
    }
    unos.close();
    n = gradovi.size();
    udaljenosti.resize(n);
    for(int i=0; i<n; i++)
        udaljenosti[i].resize(n);
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            udaljenosti[i][j] = udaljenost(gradovi[i].latitude,gradovi[i].longitude,gradovi[j].latitude,gradovi[j].longitude);
            if(i<j)
                grane.push_back(Grana(udaljenosti[i][j],i,j));
        }
     
}

int duzinaTure(vector<vector<int>> &udaljenosti, vector<int>& tura)
{
    int duzina_ture = 0;
    int n = tura.size();

    for(int i=0; i < n; i++)
        duzina_ture += udaljenosti[tura[i]][tura[(i+1)%n]];

    return duzina_ture;
}

void ispisiTuru(vector<int>& tura)
{
    for(int i=0; i<tura.size(); i++)
        cout<<tura[i]<<" ";
    cout<<endl;
}

bool provjeriTuru(vector<int>& tura)
{
    int n = tura.size();
    vector<bool> posjecen(n,false);
    if(tura.size() != n)
        return false;
    else
    {
        for(int i=0; i<n; i++)
        {
            if(tura[i]<0 || tura[i]>=n || posjecen[tura[i]] == true)
                return false;
            else
                posjecen[tura[i]] = true;
        }
    }
    return true;
}

int turaNajbliziSusjed(vector<vector<int>> &udaljenosti, int pocetni)
{

    vector<int> tura;
    int n = udaljenosti.size();
    vector<bool> posjecen(n,false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for(int i=0; i<n-1; i++)
    {
        int trenutni = tura[tura.size()-1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for(int j=0; j<n; j++)
        {
            if(!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg)
            {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if(provjeriTuru(tura))
        return duzinaTure(udaljenosti,tura);
    else
        throw "Greska";
}

vector<int> turaNS(vector<vector<int>> &udaljenosti, int pocetni)
{
    vector<int> tura;
    int n = udaljenosti.size();
    vector<bool> posjecen(n,false);
    posjecen[pocetni] = true;
    tura.push_back(pocetni);
    for(int i=0; i<n-1; i++)
    {
        int trenutni = tura[tura.size()-1];
        int udaljenostNajblizeg = INT_MAX;
        int najblizi;
        for(int j=0; j<n; j++)
        {
            if(!posjecen[j] && udaljenosti[trenutni][j] < udaljenostNajblizeg)
            {
                udaljenostNajblizeg = udaljenosti[trenutni][j];
                najblizi = j;
            }
        }
        posjecen[najblizi] = true;
        tura.push_back(najblizi);
    }
    if(provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}

int Graf::turaNajmanjaGrana()
{
    Ruta ruta(n);
    sort(grane.begin(),grane.end());
    vector<int> stepeni(n,0);
    int trenutna = 0;
    for(int i=0; i<n-1; i++)
    {
        while(true)
        {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if(stepeni[pocetak] == 2 || stepeni[kraj] == 2)
            {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak,kraj);
            if(ruta.imaLiCiklusa(pocetak))
            {
                ruta.obrisiGranu(pocetak,kraj);
                trenutna++;
                continue;
            }
            else
            {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }

        }
    }

    vector<int> tura = ruta.vratiTuru();
    if(provjeriTuru(tura))
        return duzinaTure(udaljenosti,tura);
    else
        throw "Greska";
}

vector<int> Graf::vratiTuraNajmanjaGrana()
{
    Ruta ruta(n);
    sort(grane.begin(),grane.end());
    vector<int> stepeni(n,0);
    int trenutna = 0;
    for(int i=0; i<n-1; i++)
    {
        while(true)
        {
            Grana trenutnaGrana = grane[trenutna];
            int pocetak = trenutnaGrana.pocetak;
            int kraj = trenutnaGrana.kraj;
            if(stepeni[pocetak] == 2 || stepeni[kraj] == 2)
            {
                trenutna++;
                continue;
            }
            ruta.dodajGranu(pocetak,kraj);
            if(ruta.imaLiCiklusa(pocetak))
            {
                ruta.obrisiGranu(pocetak,kraj);
                trenutna++;
                continue;
            }
            else
            {
                stepeni[pocetak]++;
                stepeni[kraj]++;
                trenutna++;
                break;
            }

        }
    }

    vector<int> tura = ruta.vratiTuru();
    if(provjeriTuru(tura))
        return tura;
    else
        throw "Greska";
}



bool comp (vector<int> x, vector<int> y)
{
    return (x[2] > y[2] );
}

int najvecaUsteda(vector<vector<int>> &udaljenosti)
{
    int n = udaljenosti.size();
    vector<int> stepeni(n,0);
    vector<vector<int>> ustede;
    Ruta ruta(n);

    for(int i = 1; i < n; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            vector<int> v;
            v.push_back(i);
            v.push_back(j);
            v.push_back(udaljenosti[0][i] + udaljenosti[0][j] - udaljenosti[i][j]);
            ustede.push_back(v);
        }

    }
    sort(ustede.begin(),ustede.end(),comp);
     int b = 0;

  for(int k = 0; k < ustede.size(); k++)
     {
         if(b == n-2) break;
           if(stepeni[ustede[k][0]] == 2 || stepeni[ustede[k][1]] == 2)
           {
               continue;
           }
            ruta.dodajGranu(ustede[k][0],ustede[k][1]);
            bool flag =false;
            if(ruta.imaLiCiklusaNova()){
                ruta.obrisiGranu(ustede[k][0],ustede[k][1]);
                flag=true;
            }
         if(!flag){
            stepeni[ustede[k][0]]++;
            stepeni[ustede[k][1]]++;
            b++;
         }
    }
     for(int i = 1; i < n; i++)
     {
         if(stepeni[i] == 2) continue;
         ruta.dodajGranu(0,i);
     }

     vector<int> t = ruta.vratiTuru0();
     return duzinaTure(udaljenosti,t);
}

vector<int> vratiTuruNajvecaUsteda(vector<vector<int>> &udaljenosti)
{
    int n = udaljenosti.size();
    vector<int> stepeni(n,0);
    vector<vector<int>> ustede;
    Ruta ruta(n);

    for(int i = 1; i < n; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            vector<int> v;
            v.push_back(i);
            v.push_back(j);
            v.push_back(udaljenosti[0][i] + udaljenosti[0][j] - udaljenosti[i][j]);
            ustede.push_back(v);
        }

    }
    sort(ustede.begin(),ustede.end(),comp);

    int b = 0;

  for(int k = 0; k < ustede.size(); k++)
     {
         if(b == n-2) break;
           if(stepeni[ustede[k][0]] == 2 || stepeni[ustede[k][1]] == 2)
           {
               continue;
           }
            ruta.dodajGranu(ustede[k][0],ustede[k][1]);
            bool flag =false;
            if(ruta.imaLiCiklusaNova()){
                ruta.obrisiGranu(ustede[k][0],ustede[k][1]);
                flag=true;
            }
         if(!flag){
            stepeni[ustede[k][0]]++;
            stepeni[ustede[k][1]]++;
            b++;
         }
    }
     for(int i = 1; i < n; i++)
     {
         if(stepeni[i] == 2) continue;
         ruta.dodajGranu(0,i);
     }

     return ruta.vratiTuru0();

}


int najvecaUstedaM(vector<vector<int>> &udaljenosti)
{
    int n = udaljenosti.size();
    vector<int> stepeni(n,0);
    vector<vector<int>> ustede;
    Ruta ruta(n);

    for(int i = 1; i < n; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            vector<int> v;
            v.push_back(i);
            v.push_back(j);
            v.push_back(udaljenosti[0][i] + udaljenosti[0][j] - udaljenosti[i][j]);
            ustede.push_back(v);
        }

    }
    sort(ustede.begin(),ustede.end(),comp);
     int b = 0;

  for(int k = 0; k < ustede.size(); k++)
     {
         if(b == n-2) break;
           if(stepeni[ustede[k][0]] == 2 || stepeni[ustede[k][1]] == 2)
           {
               continue;
           }
            ruta.dodajGranu(ustede[k][0],ustede[k][1]);
            bool flag =false;
            if(ruta.imaLiCiklusa(stepeni[ustede[k][0]]) || ruta.imaLiCiklusa(stepeni[ustede[k][1]]) ){
                ruta.obrisiGranu(ustede[k][0],ustede[k][1]);
                flag=true;
            }
         if(!flag){
            stepeni[ustede[k][0]]++;
            stepeni[ustede[k][1]]++;
            b++;
         }
    }
     for(int i = 1; i < n; i++)
     {
         if(stepeni[i] == 2) continue;
         ruta.dodajGranu(0,i);
     }

     vector<int> t = ruta.vratiTuru0();
     return duzinaTure(udaljenosti,t);
}


#endif // GRAF_H
