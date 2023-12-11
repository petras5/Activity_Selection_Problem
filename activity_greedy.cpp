#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <iomanip>
#include <fstream> //datoteke
#include <sstream> //manipulacije stingovima
#include <cstdlib>
#include <algorithm>
#include <chrono>

using namespace std;

struct Aktivnost
{
	double pocetak;
	double kraj;
};

vector<Aktivnost> generirajRandomAktivnosti(int n)
{
	vector<Aktivnost> aktivnosti;
	int i;

	mt19937 rng(static_cast<unsigned int>(time(nullptr)));
	uniform_real_distribution<double> pocetnoVrijemeDist(0.0, 23.0); // max pocetno vrijeme = 23.0

	for (int i = 0; i < n; ++i)
	{
		Aktivnost aktivnost;
		aktivnost.pocetak = pocetnoVrijemeDist(rng);
		aktivnost.kraj = aktivnost.pocetak + uniform_real_distribution<double>(0.01, 24.0 - aktivnost.pocetak)(rng);
		aktivnosti.push_back(aktivnost);
	}

	return aktivnosti;
}

bool usporediKrajAktivnosti(const Aktivnost &a, const Aktivnost &b)
{
	return a.kraj < b.kraj;
}

void greedy_activity_selector(vector<double> &s, vector<double> &f, int n, vector<Aktivnost> &odabraneAktivnosti)
{
	int m, k = 0;
	Aktivnost a;
	for (m = 1; m < n; m++)
	{
		if (s[m] >= f[k])
		{
			a.pocetak = s[m];
			a.kraj = f[m];
			odabraneAktivnosti.push_back(a);
			k = m;
		}
	}
	return;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Potrebno je unijeti broj aktivnosti." << endl;
		exit(1);
	}
	int i, n, brojOdabranihAktivnosti;
	n = atoi(argv[1]);
	if (n < 2)
	{
		cout << "Potrebno je unijeti pozitivan broj aktivnosti jednak barem 2 jer je broj odabranih aktivnosti za 1 aktivnost uvijek 1." << endl;
		exit(1);
	}
	vector<Aktivnost> aktivnosti = generirajRandomAktivnosti(n);
	vector<Aktivnost> odabraneAktivnosti;
	vector<double> s;
	vector<double> f;

	sort(aktivnosti.begin(), aktivnosti.end(), usporediKrajAktivnosti);
	for (i = 0; i < n; i++)
	{
		s.push_back(aktivnosti[i].pocetak);
		f.push_back(aktivnosti[i].kraj);
	}

	cout << "\n Generirane sortirane aktivnosti: " << endl;
	for (i = 0; i < n; i++)
	{
		cout << i + 1 << ".aktivnost, pocetak: " << setprecision(2) << fixed << aktivnosti[i].pocetak << " kraj: " << setprecision(2) << fixed << aktivnosti[i].kraj << endl;
	}

	auto start = chrono::high_resolution_clock::now();

	odabraneAktivnosti.push_back(aktivnosti[0]);
	greedy_activity_selector(s, f, n, odabraneAktivnosti);

	auto end = chrono::high_resolution_clock::now();

	brojOdabranihAktivnosti = odabraneAktivnosti.size();

	cout << "\n broj odabranih aktivnosti = " << brojOdabranihAktivnosti << endl;
	cout << "Odabrane aktivnosti: " << endl;
	for (i = 0; i < brojOdabranihAktivnosti; i++)
	{
		cout << i + 1 << ".aktivnost, pocetak: " << setprecision(2) << fixed << odabraneAktivnosti[i].pocetak << " kraj: " << setprecision(2) << fixed << odabraneAktivnosti[i].kraj << endl;
	}
	auto trajanje = chrono::duration_cast<chrono::microseconds>(end - start);

	cout << "Vrijeme izvrsavanja: " << trajanje.count() << " microseconds." << endl;

	// Generira jedinstveno ime datoteke na temelju vrijednosti varijable n
	stringstream datotekaImeStream;
	datotekaImeStream << "IT_vrijeme_izvrsavanja_n" << n << ".csv";
	string datotekaIme = datotekaImeStream.str();

	// Pise vrijeme izvrsavanja u mikrosekundama u CSV datoteku u append modu
	ofstream izlaznaDatoteka(datotekaIme, ios::app);
	if (izlaznaDatoteka.is_open())
	{
		izlaznaDatoteka << trajanje.count() << "\n";
		izlaznaDatoteka.close();
		cout << "Vrijeme izvrsavanja zapisano u '" << datotekaIme << "'" << endl;
	}
	else
	{
		cerr << "Greska: Nije moguce otvoriti CSV datoteku za pisanje." << endl;
	}

	return 0;
}