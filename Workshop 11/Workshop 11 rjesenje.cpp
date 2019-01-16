#include<iostream>
#include <exception>
#include <regex>
using namespace std;

class NedozvoljenaOperacija : public exception {
	int _linijaKoda;
public:
	NedozvoljenaOperacija(const char * error, int linija): exception(error)
	{
		_linijaKoda = linija;
	}
	void ErrorInfo()
	{
		cout << what() << " u liniji koda " << _linijaKoda << endl;
	}
};

bool validDate(const char * datum)
{
	//provjerava je li format dd.mm.yyyy. i jesu li validne vrijednosti brojeva, ali ne i br dana za svaki mjesec npr. 30.02. je valid
	string pravilo = "(0[1-9]|[12][0-9]|3[01])\\.(0[0-9]|1[012])\\.\\d{4}";
	regex reg(pravilo);
	return (regex_match(datum, reg));
}

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);

	return temp;
}

template<class T>
class Kolekcija
{
	T* _elementi;
	int _trenutno;
public:
	//Kolekciju dopuniti u skladu sa potrebama zadatka
	Kolekcija()
	{
		_elementi = nullptr;
		_trenutno = 0;
	}
	Kolekcija(int vel)
	{
		_trenutno = vel;
		_elementi = new T[_trenutno];
	}
	Kolekcija(const Kolekcija& k)
	{
		_trenutno = k._trenutno;
		_elementi = new T[_trenutno];
		for (int i = 0; i < _trenutno; i++)
			_elementi[i] = k._elementi[i];
	}
	~Kolekcija()
	{
		delete[] _elementi;
		_elementi = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k)
	{
		if (this != &k)
		{
			_trenutno = k._trenutno;
			delete[] _elementi;
			_elementi = new T[_trenutno];
			for (int i = 0; i < _trenutno; i++)
				_elementi[i] = k._elementi[i];
		}
		return *this;
	}

	void AddElement(const T& el)
	{
		T* temp = new T[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++)
		{
			temp[i] = _elementi[i];
		}

		delete[] _elementi;
		_elementi = temp;

		_elementi[_trenutno++] = el;
	}
	void RemoveElement(int index)
	{
		T* temp = new T[_trenutno - 1];
		int j = 0;
		for (int i = 0; i < _trenutno; i++)
		{
			if (i == index)
				continue;
			temp[j] = _elementi[i];
			j++;
		}
		delete[] _elementi;
		_elementi = temp;
		_trenutno--;
	}

	int GetTrenutno()const { return _trenutno; }

	T& operator[](int index) 
	{
		if (index < 0 || index >= _trenutno)
			throw NedozvoljenaOperacija("Indeks nije validan.", __LINE__);

		return _elementi[index];
	}
	T*& getNiz()
	{
		return _elementi;
	}
	void NulirajTrenutno()
	{
		_trenutno = 0;
	}
	friend ostream& operator<<<>(ostream&, const Kolekcija&);
};

template<class T>
ostream& operator<<<>(ostream& cout, const Kolekcija<T>& kol)
{
	for (int i = 0; i < kol._trenutno; i++)
	{
		cout << kol._elementi[i] << "\t";
	}

	return cout;
}

//Kataloška prodaja
class Proizvod
{
protected:
	char _sifra[10];
	char* _naziv;
	double _cijena;
	double _popust; //Izrazen procentualno
	//Dostupno proizvoda
	int _kolicina;
public:
	//Konstruktor i destruktor funkcije

	Proizvod(const char* sifra, const char* naziv, double cijena, int kolicina)
	{
		//U slučaju nedozvoljenih vrijednosti za cijenu i količinu baciti izuzetak
		if (cijena < 0 || kolicina < 0)
			throw NedozvoljenaOperacija("Cijena i/ili kolicina mora biti veca od 0!", __LINE__);

		strncpy_s(_sifra, sifra, _TRUNCATE);
		_naziv = AlocirajNizKaraktera(naziv);		
		_cijena = cijena;
		_kolicina = kolicina;
		_popust = 0;
	}

	Proizvod(const Proizvod& p)
	{
		strncpy_s(_sifra, p._sifra, _TRUNCATE);
		_naziv = AlocirajNizKaraktera(p._naziv);
		_cijena = p._cijena;
		_kolicina = p._kolicina;
		_popust = p._popust;
	}

	virtual ~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;
	}

	Proizvod& operator=(const Proizvod& p)
	{
		if (this != &p)
		{
			strncpy_s(_sifra, p._sifra, _TRUNCATE);
			delete[] _naziv;
			_naziv = AlocirajNizKaraktera(p._naziv);
			_cijena = p._cijena;
			_kolicina = p._kolicina;
			_popust = p._popust;
		}

		return *this;
	}
	//Implementirati potrebne funkcije za rad sa klasom
	virtual void Info()
	{
		
		cout << "\tSifra: " << _sifra << "\tNaziv: " << _naziv << endl;
		cout << "Cijena: " << _cijena << "\tTrenutna kolicina: " << _kolicina << "\tPopust: " << _popust << "%" <<endl<<endl;
	}
	friend bool operator==(const Proizvod& p1, const Proizvod& p2)
	{
		return (strcmp(p1._sifra, p2._sifra) == 0);
	}

	void SetPopust(double popust) { _popust = popust;}
	char * GetSifru() {	return _sifra; }
	int GetKolicina() { return _kolicina; }
	double GetPopust() { return _popust; }
	double GetCijenu() { return _cijena; }

	void PromijeniKolicinu(int i)
	{
		_kolicina += i;
	}
};

//Implementirati nasljeđivanje
class Odjeca: public Proizvod
{
	Kolekcija<char*> _velicine;
	Kolekcija<char*> _boje;
	char _kategorija[50];
public:
	//Potrebne konstruktor i destruktor funkcije
	Odjeca(const char* sifra, const char* naziv, double cijena, int kolicina, const char * kategorija)
		:Proizvod(sifra, naziv, cijena, kolicina)
	{
		strncpy_s(_kategorija, kategorija, _TRUNCATE);
	}

	Odjeca(const Odjeca& o):Proizvod(o)
	{
		for (int i = 0; i < o._velicine.GetTrenutno(); i++)
			_velicine.AddElement(AlocirajNizKaraktera(const_cast<Odjeca&>(o)._velicine[i]));
		for (int i = 0; i < o._boje.GetTrenutno(); i++)
			_boje.AddElement(AlocirajNizKaraktera(const_cast<Odjeca&>(o)._boje[i]));
		strncpy_s(_kategorija, o._kategorija, _TRUNCATE);		
	}

	Odjeca& operator=(const Odjeca& o)
	{
		if (this != &o)
		{
			(Proizvod&)(*this) = o;
			for (int i = 0; i < _velicine.GetTrenutno(); i++)
			{
				delete[] _velicine[i];
				_velicine[i] = nullptr;
			}
			delete[] _velicine.getNiz();
			_velicine.getNiz() = nullptr;
			_velicine.NulirajTrenutno();

			for (int i = 0; i < _boje.GetTrenutno(); i++)
			{
				delete[] _boje[i];
				_boje[i] = nullptr;
			}
			delete[] _boje.getNiz();
			_boje.getNiz() = nullptr;
			_boje.NulirajTrenutno();

			for (int i = 0; i < o._velicine.GetTrenutno(); i++)
				_velicine.AddElement(AlocirajNizKaraktera(const_cast<Odjeca&>(o)._velicine[i]));
			for (int i = 0; i < o._boje.GetTrenutno(); i++)
				_boje.AddElement(AlocirajNizKaraktera(const_cast<Odjeca&>(o)._boje[i]));

			strncpy_s(_kategorija, o._kategorija, _TRUNCATE);
		}
		return *this;
	}

	~Odjeca()
	{
		for (int i = 0; i < _velicine.GetTrenutno(); i++)
		{
			delete[] _velicine[i];
			_velicine[i] = nullptr;
		}
	
		for (int i = 0; i < _boje.GetTrenutno(); i++)
		{
			delete[] _boje[i];
			_boje[i] = nullptr;
		}
	}
	

	//Implementirati potrebne funkcije za rad sa klasom
	void Info()
	{
		Proizvod::Info();
		cout << "Kategorija: " << _kategorija << endl;
		cout << "Velicine: " << _velicine << endl;;
		cout << "Boje: " << _boje << endl << endl << endl;
	}

	void DodajVelicinu(const char * velicina)
	{
		_velicine.AddElement(AlocirajNizKaraktera(velicina));
	}

	void DodajBoju(const char * boja)
	{
		_boje.AddElement(AlocirajNizKaraktera(boja));
	}
};

class Katalog
{
	char* _naslov;
	char* _vrijediDo; //Datum u formatu dd.MM.yyyy
	Kolekcija<Proizvod*> _proizvodi;
public:
	//Potrebne konstruktor i destruktor funkcije
	Katalog(const char* naslov, const char* vrijediDo)
	{
		//U slučaju da datum ne ispunjava zadati format baciti izuzetak
		if (!validDate(vrijediDo))
			throw NedozvoljenaOperacija("Nije validan format datuma", __LINE__);
		
		_naslov = AlocirajNizKaraktera(naslov);
		_vrijediDo = AlocirajNizKaraktera(vrijediDo);
	}

	Katalog(const Katalog& k)
	{
		_naslov = AlocirajNizKaraktera(k._naslov);
		_vrijediDo = AlocirajNizKaraktera(k._vrijediDo);

		for (int i = 0; i < k._proizvodi.GetTrenutno(); i++)
		{
			Odjeca * pok = dynamic_cast<Odjeca*>(const_cast<Katalog&>(k)._proizvodi[i]);
			if (pok != nullptr)
				_proizvodi.AddElement(new Odjeca(*pok));
			else _proizvodi.AddElement(new Proizvod(*const_cast<Katalog&>(k)._proizvodi[i]));
		}
	}

	~Katalog()
	{
		delete[] _naslov;
		_naslov = nullptr;

		delete[] _vrijediDo;
		_vrijediDo = nullptr;

		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			delete _proizvodi[i];
			_proizvodi[i] = nullptr;
		}
	}

	//Kreirati funkciju za dodavanje proizvoda u katalog i pri tome provjeriti da li se radi
	//o objektu izvedene ili osnovne klase.
	
	bool DodajProizvod(const Proizvod& p)
	{
		//Onemogućiti dodavanje identičnih proizvoda u jedan katalog (proizvode razlikovati samo na osnovu šifre).
		//U slučaju pokušaja dodavanja istih proizvoda baciti izuzetak.
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
			if (*_proizvodi[i] == p)
				throw NedozvoljenaOperacija("Pokusavate dva puta dodati isti proizvod", __LINE__);
		
		Odjeca * pok = dynamic_cast<Odjeca*>(const_cast<Proizvod*>(&p));
		if (pok != nullptr)
			_proizvodi.AddElement(new Odjeca(*pok));
		else _proizvodi.AddElement(new Proizvod(p));
	}
	

	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu šifre.
	//Ukoliko se ne pronađe traženi proizvod baciti izuzetak.
	void UkloniProizvod(const char * sifra)
	{
		bool found = false;
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			if (strcmp(_proizvodi[i]->GetSifru(), sifra)==0)
			{
				_proizvodi.RemoveElement(i);
				found = true;
			}
		}
		if (!found)
			throw NedozvoljenaOperacija("Ne postoji proizvod s tom sifrom u katalogu!", __LINE__);
	}

	//Preklopiti operator za ispis kataloške ponude
	friend ostream& operator<<(ostream& cout, const Katalog& k)
	{
		cout << "\t" << k._naslov << endl << endl;
		cout << "Vrijedi do: " << k._vrijediDo << endl;
		for (int i = 0; i < k._proizvodi.GetTrenutno(); i++)
			const_cast<Katalog&>(k)._proizvodi[i]->Info();

		return cout;
	}
};

class Narudzba
{
	int _broj;
	char* _datumIsporuke; //Datum u formatu dd.MM.yyyy
	Kolekcija<Proizvod*> _naruceniProizvodi;
	bool otkazana;

	//static varijabla na osnovu koje cemo praviti broj narudzbe
	static int brojacNarudzbi;
public:
	//Potrebne konstruktor i destruktor funkcije
	//U slučaju da datum ne ispunjava zadati format baciti izuzetak.
	Narudzba(const char * datum)
	{
		if (!validDate(datum))
			throw NedozvoljenaOperacija("Nije validan format datuma", __LINE__);
		_broj = ++brojacNarudzbi;
		_datumIsporuke = AlocirajNizKaraktera(datum);
		otkazana = false;
	}
	Narudzba()
	{
		_datumIsporuke = nullptr;
		otkazana = false;
		_broj = ++brojacNarudzbi;
	}

	Narudzba(const Narudzba& n)
	{
		_broj = n._broj;
		_datumIsporuke = AlocirajNizKaraktera(n._datumIsporuke);
		otkazana = n.otkazana;

		_naruceniProizvodi = n._naruceniProizvodi;
	}

	Narudzba& operator=(const Narudzba& n)
	{
		_broj = n._broj;
		delete[] _datumIsporuke;
		_datumIsporuke = AlocirajNizKaraktera(n._datumIsporuke);
		otkazana = n.otkazana;
		_naruceniProizvodi = n._naruceniProizvodi;
		return *this;
	}
	~Narudzba()
	{
		delete[] _datumIsporuke;
	}

	//Kreirati funkciju za dodavanje proizvoda narudžbe sa povratnom vrijednošću tipa bool.
	//Ukoliko proizvod više nije dostupan baciti izuzetak.
	//Dozvoliti dodavanje istog proizvoda više puta.
	bool DodajProizvod(const Proizvod& p)
	{
		if (const_cast<Proizvod&>(p).GetKolicina() <= 0)
			throw NedozvoljenaOperacija("Proizvod vise nije dostupan!", __LINE__);
			
		_naruceniProizvodi.AddElement(&const_cast<Proizvod&>(p));
		//smanjujemo kolicinu za 1
		const_cast<Proizvod&>(p).PromijeniKolicinu(-1);
		return true;
	}

	//Funkciju za uklanjanje određenog proizvoda iz narudžbe. Voditi računa da narudžba može 
	//imati više puta isti proizvod.
	void UkloniProizvod(const char * sifra)
	{
		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++)
		{
			if (strcmp(_naruceniProizvodi[i]->GetSifru(), sifra) == 0)
			{
				//treba opet povecati kolicinu za jedan
				_naruceniProizvodi[i]->PromijeniKolicinu(1);
				_naruceniProizvodi.RemoveElement(i);
				//uklonit cemo samo jedan element s tom sifrom iz narudzbe, a ne sve
				return;
			}
		}

	}
	//Dodati funkciju za proračun ukupne cijene narudžbe.
	//Voditi računa o mogućnosti dodavanja više istih proizvoda (količina).
	//Uračunati popust u slučaju da je definisan.
	double UkupnaCijena()
	{
		double suma = 0;
		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++)
		{
			suma += (1 - _naruceniProizvodi[i]->GetPopust() / 100)*_naruceniProizvodi[i]->GetCijenu();
		}

		return suma;
	}

	//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom.
	friend ostream& operator<<(ostream& cout, const Narudzba& n)
	{
		cout << "Narudzba broj " << n._broj << endl;
		cout << "Datum isporuke: " << n._datumIsporuke << endl << endl;
		cout << "Otkazana: ";
		if (n.otkazana)
			cout << "da" << endl;
		else cout << "ne" << endl;
		for (int i = 0; i < n._naruceniProizvodi.GetTrenutno(); i++)
		{
			cout << i + 1 << ". ";
			const_cast<Narudzba&>(n)._naruceniProizvodi[i]->Info();
		}
		cout << "Ukupni iznos je: " << const_cast<Narudzba&>(n).UkupnaCijena() << "KM"<<endl;
		return cout;
	}

	void OtkaziNarudzbu()
	{
		otkazana = true;
	}

	int GetBroj() { return _broj; }
	bool JeLiOtkazana() { return otkazana; }
	char * GetDatum() { return _datumIsporuke; }
	int GetBrojProizvoda() { return _naruceniProizvodi.GetTrenutno(); }
};
int Narudzba::brojacNarudzbi = 0;

//U posljednjoj klasi samostalno prepoznati izuzetne situacije
class Prodavac
{

	char* _imePrezime;
	Kolekcija<Narudzba>* _narudzbe;
public:
	//Potrebne konstruktor i destruktor funkcije
	Prodavac(const char * imePrezime)
	{
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_narudzbe = new Kolekcija<Narudzba>;
	}

	~Prodavac()
	{
		delete[] _imePrezime;
		delete _narudzbe;
	}
	//Omogućiti dodavanje narudžbi za prodavača
	void DodajNarudzbu(const Narudzba& n)
	{
		_narudzbe->AddElement(n);
	}
	//Omogućiti otkazivanje narudžbe na osnovu njenog broja.
	void OtkaziNarudzbu(int broj)
	{
		bool found = false;
		for (int i = 0; i < _narudzbe->GetTrenutno(); i++)
		{
			if (_narudzbe->getNiz()[i].GetBroj() == broj)
			{
				found = true;
				_narudzbe->getNiz()[i].OtkaziNarudzbu();
			}
		}
		if (!found)
			throw NedozvoljenaOperacija("Nije pronadjena narudzba s tim brojem!", __LINE__);

	}
	//Ispis svih podataka o prodavaču i kratke informacije o
	//svim narudžbama (datum, broj proizvoda i cijena). Posebno izdvojiti otkazane narudžbe.
	void Info()
	{
		cout << "Ime i prezime: " << _imePrezime << endl;
		cout << "R.br.\tDatum\tBr.proizvoda\tIznos" << endl;
		int j = 1;
		for (int i = 0; i < _narudzbe->GetTrenutno(); i++)
		{
			if (!_narudzbe->getNiz()[i].JeLiOtkazana())
			{
				cout << j++ << ".\t";
				cout << _narudzbe->getNiz()[i].GetDatum() << "\t" << _narudzbe->getNiz()[i].GetBrojProizvoda() << "\t" << _narudzbe->getNiz()[i].UkupnaCijena() << endl;
			}
		}
		cout << "\t::OTKAZANE NARUDZBE::" << endl;
		j = 1;
		for (int i = 0; i < _narudzbe->GetTrenutno(); i++)
		{
			if (_narudzbe->getNiz()[i].JeLiOtkazana())
			{
				cout << j++ << ".\t";
				cout << _narudzbe->getNiz()[i].GetDatum() << "\t" << _narudzbe->getNiz()[i].GetBrojProizvoda() << "\t" << _narudzbe->getNiz()[i].UkupnaCijena() << endl;
			}
		}
	}
	
};

int main()
{
	Proizvod p1("123", "iPhone X", 2000, 20);
	Proizvod p2("234", "iPhone SE", 600, 35);
	//Bacit ce exception jer je cijena negativna
	try {
		Odjeca o1("246", "Kaput", -226.95, 10, "Zimski");
	}
	catch (NedozvoljenaOperacija & no)
	{
		no.ErrorInfo();
	}
	Odjeca o1("246", "Coat", 226.95, 10, "Winter jackets");
	o1.DodajBoju("Black");
	o1.DodajBoju("Toffee");
	o1.DodajVelicinu("34");
	o1.DodajVelicinu("36");
	o1.SetPopust(10);
	Odjeca o2("444", "Faux fur coat", 199.95, 10, "Winter jackets");
	o2.DodajBoju("Pink");
	o2.DodajBoju("Maroon");
	o2.DodajVelicinu("One size");
	o2.SetPopust(20);
	Odjeca o3("654", "Faux leather jacket", 99.95, 5, "Winter jackets");
	o3.DodajBoju("Emerald");
	o3.DodajVelicinu("M");

	//konstruktor kopije i operator= za Proizvod i Odjecu
	cout << "\t::Copy constructor i operator= za klase Proizvod i Odjeca::\n\n";
	Proizvod * kopija= new Proizvod(p1);
	kopija->Info();
	*kopija = p2;
	kopija->Info();
	delete kopija;
	kopija = new Odjeca(o1);
	kopija->Info();
	*dynamic_cast<Odjeca*>(kopija) = o2;
	kopija->Info();
	system("pause");
	system("cls");

	cout << "\t::Dodavanje proizvoda u katalog::\n\n";
	try {
		//format je dd.mm.yyyy bez tacke na kraju 
		Katalog k("Januar 2019", "01.02.2019.");
	}
	catch (NedozvoljenaOperacija & no)
	{
		no.ErrorInfo();
	}
	Katalog k("Januar 2019", "01.02.2019");
	k.DodajProizvod(p1);
	k.DodajProizvod(p2);
	k.DodajProizvod(o1);
	k.DodajProizvod(o2);
	k.DodajProizvod(o3);
	cout << k;
	system("pause");
	system("cls");
	//Pokusam dodati proizvod koji je vec dodan
	try {
		k.DodajProizvod(p1);
	}
	catch (NedozvoljenaOperacija& no)
	{
		no.ErrorInfo();
	}
	cout << "\t::Uklanjanje proizvoda iz kataloga::\n\n";
	k.UkloniProizvod("246");
	cout << k;
	try {
		//pokusat cemo opet ukloniti isti proizvod
		k.UkloniProizvod("246");
	}
	catch (NedozvoljenaOperacija& no)
	{
		no.ErrorInfo();
	}
	system("pause");
	system("cls");
	cout << "\t::Dodavanje i uklanjanje proizvoda - narudzba::\n\n";
	Narudzba n("15.01.2019");

	n.DodajProizvod(p1);
	n.DodajProizvod(p1);
	n.DodajProizvod(p2);
	n.DodajProizvod(o2);
	n.DodajProizvod(o2);
	
	n.UkloniProizvod("123");
	cout << n;
	system("pause"); 
	system("cls");

	Narudzba n2("16.01.2019");
	n2.DodajProizvod(p2);
	n2.DodajProizvod(o1);
	n2.DodajProizvod(p2);
	n2.DodajProizvod(p2);

	cout << "PRODAVAC:" << endl;
	Prodavac p("Elma");
	p.DodajNarudzbu(n);
	p.DodajNarudzbu(n2);
	try {
		p.OtkaziNarudzbu(3);
		//pokusam otkazati narudzbu koja ne postoji
	}
	catch (NedozvoljenaOperacija& no)
	{
		no.ErrorInfo();
	}
	p.OtkaziNarudzbu(1);
	p.Info();
	


	system("pause");
	return 0;
}



