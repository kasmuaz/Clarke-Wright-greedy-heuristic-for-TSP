#include <iostream>
#include <vector>
#include <chrono>


#include "graf.h"


using namespace std;
using namespace std::chrono;

int main() {
  Graf G("qatar.txt");
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
     cout << "Duzina ture: "<<najvecaUsteda(G.udaljenosti)<<endl;
    //cout << "Duzina ture: "<<turaNajbliziSusjed(G.udaljenosti,0)<<endl;
    //cout << "Duzina ture: "<<G.turaNajmanjaGrana()<<endl;
    chrono::high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>( t2 - t1 ).count();

    cout <<"Vrijeme izvrsavanja:" <<duration/1000000.f << endl;

  return 0;
}
