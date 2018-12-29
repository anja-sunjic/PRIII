#include<iostream>
using namespace std;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template <class T1, class T2, int max>
class Kolekcija
{
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	//Implementirati potrebne funkcije za rad sa kolekcijom
	Kolekcija() : _trenutno(0) {}

	void AddElement(T1 el1, T2 el2)
	{
		if (_trenutno == max)
			return;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno++] = el2;

	}
	int GetTrenutno() { return _trenutno; }
	T1& GetElement1(int i) { return _elementi1[i]; }
	T2& GetElement2(int i) { return _elementi2[i]; }
	
	friend ostream& operator<<<>(ostream&, const Kolekcija&);

	void SetElement2(T2 x, int i) { _elementi2[i] = x; }
};

template <class T1, class T2, int max>
ostream& operator<< <>(ostream& cout, const Kolekcija<T1, T2, max>& k)
{
	for (size_t i = 0; i < k._trenutno; i++)
	{
		cout << k._elementi1[i] << " ";
		cout << k._elementi2[i]<<endl;
	}

	return cout;
}

class Ispit
{
protected:
	char* _predmet;
	char _datumOdrzavanja[12];
	char _vrijemeOdrzavanja[6]; // početak ispita
	int _trajanje; //trajanje ispita u minutama
	int _maxBrojBodova;
	double _rezultat; //procentualni rezultat ispita
public:
	Ispit()
	{
		_predmet = nullptr;
		strcpy_s(_datumOdrzavanja, "-");
		strcpy_s(_vrijemeOdrzavanja, "-");
		_trajanje = _maxBrojBodova = _rezultat = 0;
	}

	Ispit(const char* predmet, const char* datumOdrzavanja, const char* vrijemeOdrzavanja, int trajanje)
	{
		_predmet = AlocirajNizKaraktera(predmet);

		strncpy_s(_datumOdrzavanja, datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = trajanje;
		_maxBrojBodova = 0;
		_rezultat = 0;
	}

	Ispit(const Ispit& i)
	{
		_predmet = AlocirajNizKaraktera(i._predmet);
		strncpy_s(_datumOdrzavanja, i._datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, i._vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = i._trajanje;
		_maxBrojBodova = i._maxBrojBodova;
		_rezultat = i._rezultat;
	}

	Ispit& operator=(const Ispit& i)
	{
		delete[] _predmet;
		_predmet = AlocirajNizKaraktera(i._predmet);
		strncpy_s(_datumOdrzavanja, i._datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, i._vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = i._trajanje;
		_maxBrojBodova = i._maxBrojBodova;
		_rezultat = i._rezultat;
		return *this;
	}

	~Ispit()
	{
		delete[] _predmet;
		_predmet = nullptr;
	}

	virtual void Info()
	{
		cout << "Predmet: " << _predmet << endl;
		cout << "Termin ispita: " << _datumOdrzavanja << " u " << _vrijemeOdrzavanja << endl;
		cout << "Trajanje: " << _trajanje << endl;
		cout << "Maksimalan broj bodova: " << _maxBrojBodova << endl;
		cout << "Rezultat: " << _rezultat *100<<"%"<< endl;
	}

};


class PismeniIspit : public Ispit
{
	Kolekcija<char*, int, 50>* _pitanja; //kolekcija pitanja sa brojem bodova
	Kolekcija<char*, int, 50> _odgovori; //kolekcija odgovora sa brojem osvojenih bodova
	int _brTacnihOdgovora;
	int _brOsvojenihBodova;
public:

	//Konstruktor i destruktor funkcije
	PismeniIspit()
	{
		_pitanja = new Kolekcija<char*, int, 50>;
		_brTacnihOdgovora = 0;
		_brOsvojenihBodova = 0;
	}
	PismeniIspit(const char* predmet, const char* datumOdrzavanja, const char* vrijemeOdrzavanja, int trajanje)
		:Ispit(predmet, datumOdrzavanja, vrijemeOdrzavanja, trajanje)
	{
		_pitanja = new Kolekcija<char*, int, 50>;
		_brTacnihOdgovora = 0;
		_brOsvojenihBodova = 0;
	}
	~PismeniIspit()
	{
		delete _pitanja; _pitanja = nullptr;
	}

	//Konstruktor kopije
	PismeniIspit(const PismeniIspit& p):Ispit(p)
	{
		_pitanja = new Kolekcija<char*, int, 50>;
		*_pitanja = *p._pitanja;
		_odgovori = p._odgovori;
	}
	//Operator dodjele 
	PismeniIspit& operator=(const PismeniIspit& p)
	{
		if (this != &p)
		{
			(Ispit&)(*this) = p;
			delete _pitanja;
			_pitanja = new Kolekcija<char*, int, 50>;
			*_pitanja = *p._pitanja;
			_odgovori = p._odgovori;
		}
		return *this;
	}
	//Operator() za dodavanje pitanja u kolekciju uz pripadajući broj bodova.
	//Ispit ne smije sadržavati ista pitanja. Broj bodova na jednom pitanju ne smije biti negativan.
	PismeniIspit& operator()(const char * textPitanja, int bodovi)
	{
		if (bodovi <= 0)
		{
			cout << "Ne moze se dodati pitanje sa negativnim brojem bodova." << endl;
			return *this;
		}
		for (int i = 0; i < _pitanja->GetTrenutno() ; i++)
		{
			if (strcmp(_pitanja->GetElement1(i), textPitanja) == 0)
			{
				cout << "Vec postoji to pitanje!" << endl;
				return *this;
			}
		}
		_maxBrojBodova += bodovi;
		_pitanja->AddElement(AlocirajNizKaraktera(textPitanja), bodovi);
		return *this;
	}
	//Operator+= za dodavanje odgovora na pitanje. Odgovori se dodaju u redoslijedu dodavanja pitanja.
	//Broj osvojenih bodova se inicijalno postavlja na 0. Moguće je dodati i prazan tekst kao odgovor.
	PismeniIspit& operator+=(const char * textOdgovora)
	{
		_odgovori.AddElement(AlocirajNizKaraktera(textOdgovora), 0);
		return *this;
	}
	//Funkciju za ocjenu tačnog odgovora koja kao parametar prima redni broj pitanja u kolekciji i tačan odgovor. 
	//Funkcija treba da pronađe odgovor na traženo pitanje, te da taj odgovor uporedi sa tačnim odgovorom i postavi
	//osvojene bodove. Student može osvojiti 0 ili maksimalan broj bodova na pitanju. U skladu sa rezultatom
	//provjere postaviti vrijednosti odgovarajućih atributa.
	void ProvjeraOdgovoraNaPitanje(int redniBrojPitanja, const char * odgovor)
	{
		redniBrojPitanja--;
		if (redniBrojPitanja<0 || redniBrojPitanja>_pitanja->GetTrenutno())
		{
			cout << "Ne postoji pitanje pod tim rednim brojem!" << endl;
			return;
		}
		if (strcmp(_odgovori.GetElement1(redniBrojPitanja), odgovor) == 0)
		{
			_odgovori.SetElement2(_pitanja->GetElement2(redniBrojPitanja), redniBrojPitanja);
			_brTacnihOdgovora++;
			_brOsvojenihBodova += _odgovori.GetElement2(redniBrojPitanja);
		}
		_rezultat = double(_brOsvojenihBodova) / _maxBrojBodova;
	}

	void IspisiPitanja()
	{
		cout << *_pitanja << endl;
	}
	void Info()
	{
		Ispit::Info();
		cout << *_pitanja;
		cout << _odgovori;
		cout << "Broj tacnih odgovora: " << _brTacnihOdgovora << endl;
		cout << "Broj osvojenih bodova: " << _brOsvojenihBodova << endl;
		
	}
};

class PrakticniIspit : public Ispit
{
	Kolekcija<char*, char*, 3> _alati; //potrebni alati i njihove verzije, npr. alat "Visual Studio", verzija "2017"
	char* _zadatak; //tekst zadatka
	bool _zadovoljeno; //true ako je student zadovoljio na ispitu, u suprotnom false
public:
	//Na osnovu prethodne implementacije klase PismeniIspit osmisliti i implementirati
	//potrebne funkcije za rad sa klasom PrakticniIspit.
	PrakticniIspit() 
	{
		_zadatak = nullptr;
		_zadovoljeno = false;
	}
	PrakticniIspit(const char * textZadatka, const char* predmet, const char* datumOdrzavanja, const char* vrijemeOdrzavanja, int trajanje)
		:Ispit(predmet, datumOdrzavanja, vrijemeOdrzavanja, trajanje)
	{
		_zadatak = AlocirajNizKaraktera(textZadatka);
		_zadovoljeno = false;
	}
	PrakticniIspit(const PrakticniIspit& p):Ispit(p)
	{
		_alati = p._alati;
		_zadatak = AlocirajNizKaraktera(p._zadatak);
		_zadovoljeno = p._zadovoljeno;
	}
	PrakticniIspit& operator=(const PrakticniIspit& p)
	{
		if (this != &p)
		{
			(Ispit&)(*this) = p;
			_alati = p._alati;
			delete[] _zadatak;
			_zadatak = AlocirajNizKaraktera(p._zadatak);
			_zadovoljeno = p._zadovoljeno;
		}
		return *this;
	}
	~PrakticniIspit()
	{
		delete[] _zadatak;
		_zadatak = nullptr;
	}

	void DodajAlate(const char * naziv, const char * verzija)
	{
		for (int i = 0; i < _alati.GetTrenutno(); i++)
		{
			if (strcmp(_alati.GetElement1(i), naziv) == 0)
				if (strcmp(_alati.GetElement2(i), verzija) == 0)
				{
					cout << "Vec je dodana ta verzija alata!" << endl;
					return;
				}
		}
		_alati.AddElement(AlocirajNizKaraktera(naziv), AlocirajNizKaraktera(verzija));
	}
	void setZadovoljeno()
	{
		_zadovoljeno = true;
		_rezultat = 1;
	}
};

int main()
{
	//Test za klasu ispit
	/*Ispit ispit1("PR3", "24.01.2018.", "10:00", 90);
	Ispit ispit2;
	//Ispit ispit2(ispit1);
	ispit2 = ispit1;
	ispit2.Info();*/

	//Testirati sve dostupne funkcionalnosti putem datih kolekcija.
	//Drugi parametar kolekcije predstavlja broj indeksa studenta.
	PismeniIspit ispit1("PR3", "24.01.2019.", "10:00", 90);
	Kolekcija<PismeniIspit, char*, 5> _pismeniIspiti;
	_pismeniIspiti.AddElement(ispit1, AlocirajNizKaraktera("160173"));
	_pismeniIspiti.GetElement1(0)("When did WWI begin?", 10);
	_pismeniIspiti.GetElement1(0)("How long did 100 year war last", 15);
	_pismeniIspiti.GetElement1(0)("Who led Frech army in the Battle of Waterloo?", 15);
	_pismeniIspiti.GetElement1(0) += "1914";
	_pismeniIspiti.GetElement1(0) += "116";
	_pismeniIspiti.GetElement1(0) += "Napoleon";
	_pismeniIspiti.GetElement1(0).ProvjeraOdgovoraNaPitanje(1, "1914");
	_pismeniIspiti.GetElement1(0).ProvjeraOdgovoraNaPitanje(2, "116");
	_pismeniIspiti.GetElement1(0).ProvjeraOdgovoraNaPitanje(3, "Churchill");
	static_cast<Ispit>(_pismeniIspiti.GetElement1(0)).Info();

	Kolekcija<PrakticniIspit, char*, 100> _prakticniIspiti;
	PrakticniIspit ispit2("Označite ivice koje pripadaju stablu minimalnog raspona formiranog pomoću Prim algorima", "ASP", "1.2.2019.", "10:00", 90);
	_prakticniIspiti.AddElement(ispit2, AlocirajNizKaraktera("160173"));
	_prakticniIspiti.GetElement1(0).DodajAlate("VS", "2017");
	_prakticniIspiti.GetElement1(0).setZadovoljeno();
	static_cast<Ispit>(_prakticniIspiti.GetElement1(0)).Info();

	system("pause");
	return 0;
}

