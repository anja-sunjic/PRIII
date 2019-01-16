#include<iostream>
using namespace std;

template<class T1, class T2>
class Kolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	//defaultni
	Kolekcija()
	{
		_trenutno = 0;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}

	//konstruktor kopije
	Kolekcija(const Kolekcija& k)
	{
		_trenutno = k._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}

	~Kolekcija()
	{
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k)
	{
		if (this != &k)
		{
			delete[] _elementi1;
			delete[] _elementi2;
			_trenutno = k._trenutno;
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];

			for (int i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];

			}
		}
		return *this;

	}

	void DodajElemente(const T1& el1, const T2& el2)
	{
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		for (int i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}

	int GetTrenutno() const { return _trenutno; };

	T1& GetElement1(int i) const { return _elementi1[i]; }

	T2& GetElement2(int i) const { return _elementi2[i]; }

	friend ostream& operator<<<>(ostream&, const Kolekcija&);
};

template<class T1, class T2>
ostream& operator<<<>(ostream& cout, const Kolekcija<T1, T2>& kol)
{
	for (size_t i = 0; i < kol._trenutno; i++)
		cout << kol._elementi1[i] << "\t" << kol._elementi2[i] << endl;

	return cout;
}

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

//U programu implementirati nasljeðivanje izmeðu odgovarajuæih klasa
//Klasu Osoba deklarisati kao apstraktnu.
class Osoba
{
	char* _ime;
	char* _prezime;
	char* _adresa;
	char _telefon[15];
public:
	Osoba()
	{
		_ime = _prezime = _adresa = nullptr;
		strcpy_s(_telefon, "---");
	}

	Osoba(const char* ime, const char* prezime, const char* adresa, const char* telefon)
	{
		_ime = AlocirajNizKaraktera(ime);
		_prezime = AlocirajNizKaraktera(prezime);
		_adresa = AlocirajNizKaraktera(adresa);
		strncpy_s(_telefon, telefon, _TRUNCATE);
	}

	Osoba(const Osoba& o)
	{
		_ime = AlocirajNizKaraktera(o._ime);
		_prezime = AlocirajNizKaraktera(o._prezime);
		_adresa = AlocirajNizKaraktera(o._adresa);
		strncpy_s(_telefon, o._telefon, _TRUNCATE);
	}

	Osoba& operator=(const Osoba& o)
	{
		delete[] _ime;
		delete[] _prezime;
		delete[] _adresa;
		_ime = AlocirajNizKaraktera(o._ime);
		_prezime = AlocirajNizKaraktera(o._prezime);
		_adresa = AlocirajNizKaraktera(o._adresa);
		strncpy_s(_telefon, o._telefon, _TRUNCATE);

		return *this;
	}

	virtual ~Osoba() = 0
	{
		delete[] _ime;
		_ime = nullptr;
		delete[] _prezime;
		_prezime = nullptr;
		delete[] _adresa;
		_adresa = nullptr;
	}

	virtual void Info()
	{
		cout << "Ime i prezime: " << _ime << " " << _prezime << endl;
		cout << "Adresa: " << _adresa << endl;
		cout << "Telefon: " << _telefon << endl;
	}

	char * GetIme() { return _ime; }
	char * GetPrezime() { return _prezime; }

};

class Proizvod
{
	char* _naziv;
	char* _opis;
	double _cijena;
public:
	Proizvod()
	{
		_naziv = _opis = nullptr;
		_cijena = 0;
	}

	Proizvod(const char* naziv, const char* opis, double cijena)
	{
		_naziv = AlocirajNizKaraktera(naziv);
		_opis = AlocirajNizKaraktera(opis);
		_cijena = cijena;
	}

	Proizvod(const Proizvod& p)
	{
		_naziv = AlocirajNizKaraktera(p._naziv);
		_opis = AlocirajNizKaraktera(p._opis);
		_cijena = p._cijena;
	}

	Proizvod& operator=(const Proizvod& p)
	{
		delete[] _naziv;
		delete[] _opis;
		_naziv = AlocirajNizKaraktera(p._naziv);
		_opis = AlocirajNizKaraktera(p._opis);
		_cijena = p._cijena;

		return *this;
	}

	~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;

		delete[] _opis;
		_opis = nullptr;
	}

	double GetCijenu() { return _cijena; }

	void SetCijenu(double novaCijena) { _cijena = novaCijena; }

	friend ostream& operator<<(ostream&, const Proizvod&);

	friend ostream& operator<<(ostream&, const Proizvod*);

};

ostream& operator<<(ostream& cout, const Proizvod& p)
{
	cout << "Naziv: " << p._naziv << endl;
	cout << "Opis: " << p._opis << endl;
	if (p._cijena == 0)
		cout << "Gratis!" << endl;
	else cout << "Cijena: " << p._cijena << endl;

	return cout;
}

ostream& operator<<(ostream& cout, const Proizvod* p)
{
	cout << "Naziv: " << p->_naziv << endl;
	cout << "Opis: " << p->_opis << endl;
	if (p->_cijena == 0)
		cout << "Gratis!" << endl;
	else cout << "Cijena: " << p->_cijena << endl;


	return cout;
}

class Narudzba {
	char _datum[12]; //Format: dd.MM.yyyy.
	Kolekcija<Proizvod*, int> _proizvodi; //Podaci o naruèenim proizvodima i kolièinama
public:
	//Potrebne osnovne funkcije za rad sa klasom
	Narudzba(const char * datum)
	{
		strncpy_s(_datum, datum, _TRUNCATE);
	}
	Narudzba()
	{

	}
	Narudzba(const Narudzba& n)
	{
		strncpy_s(_datum, n._datum, _TRUNCATE);
		_proizvodi = n._proizvodi;
	}

	void DodajProizvod(const Proizvod& p, int kolicina)
	{
		_proizvodi.DodajElemente(new Proizvod(p), kolicina);
	}
	//Funkciju GetIznosNarudzbe koja vraæa ukupan iznos narudžbe na osnovu
	//podataka o cijenama proizvoda i naruèenim kolièinama. Funkcija prima neobavezni parametar popust
	//koji inicijalno ima vrijednost 0, što znaèi da ne postoji popust na postojeæe cijene proizvoda.
	double GetIznosNarudzbe(float popust = 0) const
	{
		double suma = 0;
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			suma += _proizvodi.GetElement1(i)->GetCijenu()*_proizvodi.GetElement2(i)*(1 - (popust / 100));
		}
		return suma;
	}

	int GetMjesec()
	{
		int mjesec = 0;
		if (_datum[3] == '1')
			mjesec = 10;
		mjesec += _datum[4] - 48;//in ascii code digits start from 48
		return mjesec;
	}
	//Funkciju za ispis svih vrijednosti atributa klase.
	friend ostream& operator<<(ostream&, const Narudzba&);
};

ostream& operator<<(ostream& cout, const Narudzba& n)
{
	cout << "Datum narudzbe: " << n._datum << endl;
	cout << n._proizvodi << endl;
	return cout;
}

int brojacID = 0;

class Klijent : public Osoba
{
protected:
	int _id;
	char* _email;
	Kolekcija<Narudzba, double> _narudzbe; //Podaci o narudžbama sa ukupnim utrošenim iznosom u KM
public:
	//Potrebne konstruktor i destruktor funkcije.
	Klijent()
	{
		_id = 0;
		_email = nullptr;
	}

	Klijent(const char* ime, const char* prezime, const char* adresa, const char* telefon, const char * email)
		:Osoba(ime, prezime, adresa, telefon), _id(++brojacID)
	{
		_email = AlocirajNizKaraktera(email);
	}

	Klijent(const Klijent& k) :Osoba(k), _id(k._id)
	{
		_email = AlocirajNizKaraktera(k._email);
		_narudzbe = k._narudzbe;
	}

	Klijent& operator=(const Klijent& k)
	{
		(Osoba&)(*this) = k;
		_id = k._id;
		delete[] _email;
		_email = AlocirajNizKaraktera(k._email);
		_narudzbe = k._narudzbe;
		return *this;
	}

	virtual ~Klijent()
	{
		delete[] _email; _email = nullptr;
	}

	//Funkciju za evidenciju nove narudžbe.
	virtual void DodajNarudzbu(const Narudzba& n, double popust = 0)
	{
		_narudzbe.DodajElemente(n, n.GetIznosNarudzbe(popust));
	}

	//Funkciju koja vraæa logièku vrijednost true ukoliko je klijent za odabrani mjesec u godini imao
	//potrošnju veæu od zadate. Prototip funkcije: bool IsPotrosnjaVeca(int mjesec, double potrosnja);
	//Za pronalazak potrosnje po mjesecima koristiti podatak o datumu narudžbe. 
	bool IsPotrosnjaVeca(int mjesec, double potrosnja)
	{
		for (int i = 0; i < _narudzbe.GetTrenutno(); i++)
			if (_narudzbe.GetElement1(i).GetMjesec() == mjesec)
			{
				if (_narudzbe.GetElement1(i).GetIznosNarudzbe() > potrosnja)
					return true;
			}
		return false;
	}

	//Funkciju za izmjenu e-mail adrese.
	void setEmail(const char * email)
	{
		delete[] _email;
		_email = AlocirajNizKaraktera(email);
	}

	//Funkciju za ispis svih vrijednosti atributa klase.
	virtual void Info()
	{
		Osoba::Info();
		cout << "Id klijenta: " << _id << endl;
		cout << "Email: " << _email << endl;
		cout << "\nNarudzbe:\n";
		cout << _narudzbe << endl;
	}
};

class VIPKlijent :public Klijent
{
	char* _parkingOznaka;
	double _popust;
public:
	//Potrebne konstruktor i destruktor funkcije
	VIPKlijent()
	{
		_parkingOznaka = nullptr;
		_popust = 0;
	}
	VIPKlijent(const char* parkingOznaka, double popust, const char* ime, const char* prezime, const char* adresa, const char* telefon, const char * email)
		:Klijent(ime, prezime, adresa, telefon, email)
	{
		_parkingOznaka = AlocirajNizKaraktera(parkingOznaka);
		_popust = popust;
	}
	VIPKlijent(const VIPKlijent& v) :Klijent(v)
	{
		_parkingOznaka = AlocirajNizKaraktera(v._parkingOznaka);
		_popust = v._popust;
	}
	VIPKlijent& operator=(const VIPKlijent& v)
	{
		(Klijent&)(*this) = v;
		delete[] _parkingOznaka;
		_parkingOznaka = AlocirajNizKaraktera(v._parkingOznaka);
		_popust = v._popust;

		return *this;
	}
	virtual ~VIPKlijent()
	{
		delete[] _parkingOznaka; _parkingOznaka = nullptr;
	}
	//Funkciju za evidenciju nove narudžbe sa zaraèunatim popustom na sve proizvode.
	virtual void DodajNarudzbu(const Narudzba& n)
	{
		Klijent::DodajNarudzbu(n, _popust);
	}

	//Funkciju koja posljednjoj narudžbi klijenta dodaje gratis proizvod proslijeðen kao parametar.
	void DodajGratisProizvod(Proizvod p)
	{
		p.SetCijenu(0);
		if (_narudzbe.GetTrenutno() == 0)
		{
			cout << "Klijent nema nijednu naruzbu!" << endl;
			return;
		}
		_narudzbe.GetElement1(_narudzbe.GetTrenutno() - 1).DodajProizvod(p, 1);
	}
	//Funkciju za ispis svih vrijednosti atributa klase
	virtual void Info()
	{
		Klijent::Info();
		cout << "Parking oznaka: " << _parkingOznaka << endl;
		cout << "Iznos popusta: " << _popust << endl;
	}

};

//Funkcija treba da pronaðe sve one klijente koji su za odabrani mjesec ostvarili potrošnju veæu
//od zadate. Ispisati njihove podatke i vratiti ukupan broj pronaðenih klijenata.
int PronadjiKlijentePoPotrosnji(Osoba* niz[], int max, double zadanaPotrosnja, int zadaniMjesec)
{
	int brojac = 0;
	for (int i = 0; i < max; i++)
	{
		if (dynamic_cast<Klijent*>(niz[i])->IsPotrosnjaVeca(zadaniMjesec, zadanaPotrosnja))
		{
			cout << i + 1 << ".";
			niz[i]->Info();
		}
	}
	return brojac;
}

//Pronaæi sve VIP klijente, te im dodati gratis proizvod proslijeðen kao parametar.
void UkljuciGratisProizvodVIPKlijentima(Osoba* niz[], int max, const Proizvod& p)
{
	VIPKlijent * pok = nullptr;
	for (int i = 0; i < max; i++)
	{
		pok = dynamic_cast<VIPKlijent*>(niz[i]);
		if (pok != nullptr)
			dynamic_cast<VIPKlijent*>(niz[i])->DodajGratisProizvod(p);
	}
}

//Svim klijentima postaviti mail adresu u formatu: ime.prezime@fit.ba.
void GenerisiMailAdreseKlijentima(Osoba* niz[], int max)
{
	Klijent * pok = nullptr;
	for (int i = 0; i < max; i++)
	{
		pok = dynamic_cast<Klijent*>(niz[i]);
		if (pok != nullptr)
		{
			char temp[40];
			strcpy_s(temp, 40, niz[i]->GetIme());
			strcat_s(temp, 40, ".");
			strcat_s(temp, 40, niz[i]->GetPrezime());
			strcat_s(temp, 40, "@fit.ba");

			dynamic_cast<Klijent*>(niz[i])->setEmail(AlocirajNizKaraktera(temp));
		}
	}
}

//Omoguæiti pretragu klijenata po imenu ili prezimenu (ispisati informacije
//o svim klijentima koji sadrže uneseni tekst u dijelu imena ili prezimena.
void PronadjiKlijentePoImenuPrezimenu(Osoba* niz[], int max, const char * imePrezime)
{
	bool found = false;
	for (int i = 0; i < max; i++)
	{
		if (strstr(niz[i]->GetIme(), imePrezime) != nullptr || strstr(niz[i]->GetPrezime(), imePrezime) != nullptr)
		{
			niz[i]->Info();
			found = true;
		}
	}
	if (!found)cout << "Nije pronadjen klijent s tim imenom!" << endl;
}

int main()
{
	//Testirati sve dostupne funkcionalnosti uz obaveznu provjeru globalnih funkcija.

	Proizvod p1("Soy milk", "vegan", 5);
	Proizvod p2("Tempeh", "vegan", 10.5);
	Proizvod p3("Seitan", "vegan", 13.99);
	Proizvod p4("Roquefort", "vegetarian", 20.50);

	Kolekcija<Proizvod, int> k;
	k.DodajElemente(p1, 2);
	k.DodajElemente(p2, 1);
	cout << k;
	Kolekcija<Proizvod, int> k2 = k;
	cout << k2;
	system("pause");
	system("cls");

	Narudzba n("21.12.2018");
	n.DodajProizvod(p1, 1);
	n.DodajProizvod(p4, 3);
	cout << n;
	cout << "Iznos narudzbe je " << n.GetIznosNarudzbe() << endl;
	system("pause");
	system("cls");

	Klijent k1("Anja", "Sunjic", "xxx", "123", "xxx");
	k1.DodajNarudzbu(n);
	k1.Info();
	if (k1.IsPotrosnjaVeca(12, 30))
		cout << "Potrosnja u 12. mjesecu je veca od 30KM." << endl;
	system("pause");
	system("cls");

	cout << "Izmjena email" << endl;
	k1.setEmail("anja.s");
	k1.Info();
	system("pause");
	system("cls");

	//VIP klijentima sve narudžbe kreirati uz zaraèunati popust
	VIPKlijent vk1("ABC", 10, "Luka", "Sunjic", "xyz", "345", "xxx");
	vk1.DodajNarudzbu(n);
	vk1.DodajGratisProizvod(p4);
	vk1.Info();
	system("pause");
	system("cls");

	Osoba * niz[2];
	niz[0] = new Klijent(k1);
	niz[1] = new VIPKlijent(vk1);
	cout << "Klijenti koji su 12 mjesecu potrosili vise od 30 KM:" << endl;
	PronadjiKlijentePoPotrosnji(niz, 2, 30, 12);
	system("pause");
	system("cls");

	UkljuciGratisProizvodVIPKlijentima(niz, 2, p3);
	GenerisiMailAdreseKlijentima(niz, 2);
	niz[0]->Info();//nije dodan gratis poizvod jer nije VIPKlijent, generisan email
	niz[1]->Info();//dodan gratis proizvod, generisan email
	system("pause");
	system("cls");

	PronadjiKlijentePoImenuPrezimenu(niz, 2, "Anja");
	system("pause");
	return 0;
}
