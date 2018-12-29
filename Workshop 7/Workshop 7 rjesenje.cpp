#include<iostream>
using namespace std;

char * alociraj(const char * text)
{
	if (text == nullptr)
		return nullptr;
	int size = strlen(text) + 1;
	char * novi = new char[size];
	strcpy_s(novi, size, text);
	return novi;
}
template<class T1, class T2, int max>
class FITKolekcija
{
	int* _trenutno;
	T1 _elementi1[max];
	T2* _elementi2[max];
	bool Full()
	{
		return (*_trenutno) == max;
	}
	bool Empty()
	{
		return (*_trenutno) == 0;
	}
public:
	//Potrebne konstruktor i destruktor funkcije
	FITKolekcija()
	{
		_trenutno = new int(0);
		for (int i = 0; i < max; i++)
			_elementi2[i] = nullptr;
	}
	~FITKolekcija()
	{
		delete _trenutno; _trenutno = nullptr;
		for (int i = 0; i < max; i++)
		{
			delete _elementi2[i];
			_elementi2[i] = nullptr;
		}
	}

	//Kreirati konstruktor kopije za kolekciju
	FITKolekcija(const FITKolekcija& k)
	{
		_trenutno = new int(*k._trenutno);
		for (int i = 0; i < max; i++)
		{
			_elementi1[i] = k._elementi1[i];
			if (k._elementi2[i] != nullptr)
				_elementi2[i] = new T2(*k._elementi2[i]);
			else _elementi2[i] = nullptr;
		}
	}
	//operator dodjele
	FITKolekcija& operator=(const FITKolekcija& k)
	{
		if (this != &k)
		{
			*_trenutno = *k._trenutno;
			for (int i = 0; i < max; i++)
			{
				_elementi1[i] = k._elementi1[i];
				if (k._elementi2[i] != nullptr)
				{
					delete _elementi2[i];
					_elementi2[i] = new T2(*k._elementi2[i]);
				}
				else _elementi2[i] = nullptr;
			}
		}
		return *this;
	}

	//Preklopiti operator() na način da omogući dodavanje novih elemenata u kolekciju
	//Pri tome spriječiti dodavanja duplih elemenata (odnosi se i na T1 i na T2)
	void operator()(const T1& el1, const T2& el2)
	{
		if (Full())
		{
			cout << "Puna kolekcija!" << endl;
			return;
		}
		for (int i = 0; i < *_trenutno; i++)
			if (el1 == _elementi1[i] || (_elementi2[i]!=nullptr && el2 == *_elementi2[i]))
			{
				cout << "Pokusaj dodavanja duplog elementa!" << endl;
				return;
			}
		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = new T2(el2);

		(*_trenutno)++;
	}

	//Preklopiti operator[] na način da vraća element tipa T1 na osnovu njegove lokacije u nizu
	T1 operator[](int index)
	{
		if (index >= *_trenutno)
		{
			cout << "Indeks veci od broja elemenata u nizu!" << endl;
			return 0;
		}
		return _elementi1[index];
	}

	//Preklopiti operator-= na način da uklanja elemente iz kolekcije na osnovu rednog broja proslijeđenog kao parametar
	FITKolekcija& operator-=(int index)
	{
		if (Empty())
		{
			cout << "Prazna kolekcija!" << endl;
			return *this;
		}
		if (index >= *_trenutno)
		{
			cout << "Indeks veci od broja elemenata u nizu!" << endl;
			return *this;
		}
		(*_trenutno)--;
		for (int i = index; i < *_trenutno; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			*_elementi2[i] = *_elementi2[i + 1];
		}
		_elementi2[*_trenutno] = nullptr;
		return *this;
	}
	//Preklopiti operator za ispis. Implementaciju raditi izvan tijela klase.
	friend ostream& operator<<<>(ostream& cout, const FITKolekcija& k);

	int getTrenutno()
	{
		return *_trenutno;
	}
};
template<class T1, class T2, int max>
ostream& operator<<<>(ostream& cout, const FITKolekcija<T1, T2, max>& k)
{
	for (int i = 0; i < *k._trenutno; i++)
		cout << *k._elementi2[i] << " " << k._elementi1[i] << endl;
	
	cout << endl;
	return cout;
}
class Anketa
{
	char* _pitanje;
	int _trajanje; //izraženo u danima
	//Kolekcija treba da pohrani moguće odgovore na pitanja, sa pripadajućim rednim brojem
	FITKolekcija<char*, int, 8> _odgovori;
public:
	//Kreirati potrebne konstruktor i destruktor funkcije
	Anketa(const char * pitanje, int trajanje)
	{
		_pitanje = alociraj(pitanje);
		_trajanje = trajanje;
	}
	Anketa(const Anketa& a):_odgovori(a._odgovori)
	{
		_pitanje = alociraj(a._pitanje);
		_trajanje = a._trajanje;
	}
	~Anketa()
	{
		delete[] _pitanje; _pitanje = nullptr;
	}
	Anketa& operator=(const Anketa& a)
	{
		_odgovori = a._odgovori;
		delete[] _pitanje;
		_pitanje = alociraj(a._pitanje);
		_trajanje = a._trajanje;
	}

	//Kreirati funkciju DodajOdgovor koja treba da doda novi odgovor za anketu.
	//Spriječiti dodavanje istog odgovora više puta.
	bool DodajOdgovor(const char * odgovor)
	{
		for (int i = 0; i < _odgovori.getTrenutno(); i++)
			if (strcmp(_odgovori[i], odgovor) == 0)
			{
				cout << "Odgovor vec dodan!" << endl;
				return false;
			}
		_odgovori(alociraj(odgovor), _odgovori.getTrenutno()+1);
		return true;
	}

	//Kreirati funkciju UkloniOdgovor koja uklanja odgovor na osnovu njegovog rednog broja.
	void UkloniOdgovor(int redniBroj)
	{
		_odgovori -= redniBroj;

	}

	//Preklopiti operator++ na način da povećava trajanje ankete za jedan dan
	//Pri tome voditi računa da se implementiraju dvije verzije ovog operatora (objekat++; ++objekat)
	Anketa& operator++()
	{
		_trajanje++;
		return *this;
	}

	Anketa operator++(int x)
	{
		Anketa nova(*this);
		_trajanje++;
		return nova;
	}


	//Preklopiti operator za ispis
	friend ostream& operator<<(ostream& cout, const Anketa& a)
	{
		cout << a._pitanje << endl;
		cout << "Vrijeme trajanja: " << a._trajanje << " dana" << endl;
		cout << a._odgovori;
		return cout;
	}
};

int main()
{
	FITKolekcija<int, int, 5> k;
	k(1, 2);
	k(2, 3);
	k(10, 11);
	k(2, 4);//ne treba dodati jer je dupli
	cout << k;
	k -= 0; //uklanja prvi i pomjera za jedno mjesto unatrag
	cout << k;
	cout << "Prvi element je " << k[0] << endl;
	system("pause");
	system("cls");
	
	Anketa a("No of hrs spent online daily?", 30);
	a.DodajOdgovor("less than 1");
	a.DodajOdgovor("1-3");
	a.DodajOdgovor("more then 3");
	cout << a;
	a.UkloniOdgovor(1);
	cout << a;
	a.UkloniOdgovor(4);
	system("pause");
	system("cls");
	
	cout << ++a;//povecava odmah
	cout << a++;//povecava u sljedecoj liniji
	cout << a;
	system("pause");
	return 0;
}


