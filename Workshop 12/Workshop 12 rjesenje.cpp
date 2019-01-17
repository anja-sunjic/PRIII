#include<iostream>
#include<vector>
#include<string>
#include <algorithm>
#include <iomanip>

using namespace std;


char * AlocirajNizKaraktera(const char * text)
{
	if (text == nullptr)
		return nullptr;
	int size = strlen(text) + 1;
	char * novi = new char[size];
	strcpy_s(novi, size, text);

	return novi;
}
class Term //Pojam
{
	char* _key;
	char* _value;
public:
	//Konstruktor
	Term(const char * key, const char * value)
	{
		_key = AlocirajNizKaraktera(key);
		_value = AlocirajNizKaraktera(value);
	}
	//sa funkcijom push_back elementi se dodaju koristeci konstruktor kopije
	Term(const Term& t)
	{
		_key = AlocirajNizKaraktera(t._key);
		_value = AlocirajNizKaraktera(t._value);
	}

	Term& operator=(const Term& t)
	{
		delete[] _key;
		delete[] _value;

		_key = AlocirajNizKaraktera(t._key);
		_value = AlocirajNizKaraktera(t._value);

		return *this;
	}
	//Destruktor
	~Term()
	{
		delete[] _key; _key = nullptr;
		delete[] _value; _value = nullptr;
	}

	char * getKey() { return _key; }
	char * getValue() { return _value; }
	//Operator za ispis
	friend ostream& operator<<(ostream& cout, const Term& t)
	{
		cout <<left<<setw(20)<< t._key << "\t" << setw(10) << t._value << endl;
		return cout;
	}
	//Operatori poređenja == i != (koristiti samo vrijednost atributa _key)
	friend bool operator==(const Term& t1, const Term& t2)
	{
		return strcmp(t1._key, t2._key) == 0;
	}

	friend bool operator!=(const Term& t1, const Term& t2)
	{
		return strcmp(t1._key, t2._key) != 0;
	}

	friend bool operator<(const Term& t1, const Term& t2)
	{
		return strcmp(t1._key, t2._key) < 0;
	}
};

class Dictionary //Riječnik pojmova
{
	string _title;
	int _publicationYear;
	//Ključne riječi bosanskog jezika u atribut _key
	//Odgovarajući prevod na engleskom jeziku u atribut _value
	vector<Term> _terms;
public:
	//Konstruktor
	Dictionary(const char * naslov, int godina)
	{
		_title = naslov;
		_publicationYear = godina;
	}

	//Operator () za dodavanje novih pojmova u riječnik (u paru key-value).
	//Spriječiti dodavanje istih vrijednosti u kolekciju (baciti izuzetak).
	//Pojmovi u svakom momentu treba da budu sortirani abecedno (koristiti funkciju sort iz biblioteke algorithm).
	Dictionary& operator()(const char * key, const char * value)
	{
		Term novi(key, value);
		for (int i = 0; i < _terms.size(); i++)
		{
			if (novi==_terms[i])
				throw exception("Vec postoji taj pojam. ");
		}
		_terms.push_back(novi);
		sort(_terms.begin(), _terms.end());
		return *this;
	}
	//Operator -= za uklanjanje pojma iz riječnika na osnovu atributa _key.
	Dictionary& operator-=(const char * key)
	{
		bool found = false;
		vector<Term>::iterator it = _terms.begin();
		while (it != _terms.end())
		{
			if (strcmp(it->getKey(), key) == 0)
			{
				found = true;
				_terms.erase(it);
				return *this;
			}
			it++;
		}
		if (!found)
			throw exception("Ne postoji taj pojam u rjecniku!");
	}

	//Funkciju koja vraća trenutni broj pojmova u riječniku.
	int getBrojPojmova() { return _terms.size(); }

	vector<Term>& GetNiz() { return _terms; }

	//operator[] koji vraća vrijednost na osnovu ključne riječi (povratna vrijednost je engleska riječ-value, 
	//na osnovu bosanske-key)
	char* operator[](const char * key)
	{
		bool found = false;
		for (int i = 0; i < _terms.size(); i++)
		{
			if (strcmp(_terms[i].getKey(), key) == 0)
			{
				found = true;
				return _terms[i].getValue();
			}
		}

		if (!found)
			throw exception("Ne postoji taj pojam u rjecniku!");
	}

	//Funkciju koja vraća kolekciju ključeva (bosanskih riječi) generisanih slučajnim odabirom.
	//Generisanje slučajnog broja između min i max: int randNum = rand()%(max-min + 1) + min;
	//Funkcija kao parametar prima broj riječi koje je potrebno generisati.
	vector<string> GetRijeci(int broj)
	{
		if (broj > _terms.size())
			throw exception("Nema toliko rijeci u rjecniku!");
		vector<string> rijeci;
		
		while (rijeci.size() < broj)
		{
			srand(time(NULL));
			int randBroj = rand() % (_terms.size());
			bool vecDodan = false;
			for (int j = 0; j < rijeci.size(); j++)
			{
				if (strcmp(_terms[randBroj].getKey(), rijeci[j].c_str()) == 0)
					vecDodan = true;
			}
			if (!vecDodan)
				rijeci.push_back(_terms[randBroj].getKey());
		}
		return rijeci;
	}

	string GetTitle() { return _title; }
	int GetYear() { return _publicationYear; }
};

//Osmisliti i implementirati klasu Competitor (Takmičar) na način da se upotrebom objekta klase Dictionary
//svakom takmičaru dodijele određena pitanja (riječi bosanskog jezika) - koristiti implementiranu funkciju klase Dictionary.
//Svakom pitanju je potrebno pridružiti odgovor takmičara (kolekcija _odgovori).
//Obavezno implementirati funkciju koja vrši kalkulaciju osvojenih bodova (1 tačan odgovor = 1 bod).
//Po potrebi dopuniti klasu dodatnim atributima.
class Competitor //Takmičar
{
	string _ime;
	string _prezime;
	vector<string> _pitanja;
	vector<string*> _odgovori;

	//rjecnik cemo postaviti kao static var u klasi da je svi objekti mogu koristiti, a da ne pravi svaki svoju kopiju
	static Dictionary& d;
	static int brojPitanja;
public:
	Competitor(string ime, string prezime)
	{
		_ime = ime;
		_prezime = prezime;
		_pitanja = d.GetRijeci(brojPitanja);
	}
	string GetImePrezime() { return _ime + ' ' + _prezime; }
	int IzracunajBodove()
	{
		int bodovi = 0;
		for (int i = 0; i < brojPitanja; i++)
			if (d[_pitanja[i].c_str()] == *_odgovori[i])
				bodovi++;
		return bodovi;
	}
	void PokreniTest()
	{
		cout << "Ime i prezime: " << _ime + ' ' + _prezime << endl << endl;
		cout << "Test poznavanja rijeci iz rjecnika: " << d.GetTitle() << endl << endl;
		cout << "Uputa: Unesite prijevod rijeci na engleski jezik!" << endl << endl;
		for (int i = 0; i < brojPitanja; i++)
		{
			cout << _pitanja[i] << endl;
			string temp;
			getline(cin, temp);
			_odgovori.push_back(new string(temp));
		}

		Evaluacija();
	}

	void Evaluacija()
	{
		for (int i = 0; i < brojPitanja; i++)
		{
			cout << "Pitanje " << i+1<<" :\t"<<_pitanja[i] << endl<<endl;
			cout << "Vas odgovor: " << *_odgovori[i] << endl;
			cout << "Tocan odgovor: " << d[_pitanja[i].c_str()]<<endl<<endl;
		}

		cout << "\n\tOsvojili ste: " << IzracunajBodove() << " od " << brojPitanja << " bodova." << endl;
	}
	

};
vector<Competitor> GetNajbolji(Competitor * niz[], int size, int& maxScore)
{
	vector<Competitor> x;
	for (int i = 0; i < size; i++)
		if (niz[i]->IzracunajBodove() > maxScore)
			maxScore = niz[i]->IzracunajBodove();

	for (int i = 0; i < size; i++)
		if (niz[i]->IzracunajBodove() == maxScore)
			x.push_back(*niz[i]);

	return x;
}
Dictionary rjecnik("Automotive dictionary", 2019);
Dictionary& Competitor::d = rjecnik;
int Competitor::brojPitanja = 5;

int main()
{
	//Kreirati riječnik pojmova.
	rjecnik("kompresor", "turbocharger");
	rjecnik("bregasta osovina", "camshaft");
	rjecnik("klip", "piston");
	rjecnik("ventil", "valve");
	rjecnik("unutarnje izgaranje", "internal combustion");
	rjecnik("svjecica", "spark plug");
	rjecnik("kvacilo", "clutch");
	rjecnik("mjenjac", "gearbox");
	rjecnik("auspuh", "exhaust");
	rjecnik("rashladna tecnost", "coolant");
	rjecnik("filter ulja", "oil filter");
	rjecnik("prijenos", "transmission");
	rjecnik("nagib tockova", "camber angle");

	//try {
	//	rjecnik("svjecica", "spark plug");
	//}
	//catch (exception& e)
	//{
	//	cout<<e.what()<<endl;
	//}
	//try { rjecnik -= ("turbina"); }
	//catch (exception& e)
	//{
	//	cout << e.what() << endl;
	//}
	//rjecnik -= ("ventil");


	//Kreirati niz objekata tipa Competitor i simulirati takmičenje u poznavanju riječi engleskog jezika.
	//Pronaći najboljeg među takmičarima (najveći broj bodova).
	const int size = 3;
	Competitor * niz[size];
	niz[0] = new Competitor("Anja", "S");
	niz[1] = new Competitor("Luka", "S");
	niz[2] = new Competitor("Vedran", "B");
	for (int i = 0; i < size; i++)
		niz[i]->PokreniTest();
	int maxScore = -1;
	vector<Competitor> najbolji = GetNajbolji(niz, size, maxScore);
	cout << "\n Najbolji natjecatelj/i je/su:\n";
	for (int i = 0; i < najbolji.size(); i++)
	{
		cout << "\t" << najbolji[i].GetImePrezime() << endl;
	}
	cout << "\n sa osvojenih " << maxScore << " bodova.\n";
	system("pause");
	return 0;
}


