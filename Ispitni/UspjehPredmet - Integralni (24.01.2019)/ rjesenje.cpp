#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <regex>
#include <thread>
#include <cmath>
#include <iterator>

using namespace std;

const char *crt = "\n-------------------------------------------\n";
enum GodinaStudija { PRVA = 1, DRUGA, TRECA };
const char * charGodinaStudija[] = { "PRVA", "DRUGA", "TRECA" };

char * Alociraj(const char * sadrzaj) 
{
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Dictionary {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
	void ProsiriNiz()
	{
		T1 * temp1 = new T1[*_trenutno + 1];
		T2 * temp2 = new T2[*_trenutno + 1];

		for (int i = 0; i < *_trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		
		_elementi1 = temp1;
		_elementi2 = temp2;
	}
public:
	Dictionary() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	Dictionary(const Dictionary& d)
	{
		_trenutno = new int(*d._trenutno);
		_elementi1 = new T1[*_trenutno];
		_elementi2 = new T2[*_trenutno];
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = d._elementi1[i];
			_elementi2[i] = d._elementi2[i];
		}
	}
	Dictionary& operator=(const Dictionary& d)
	{
		if (this == &d)
			return *this;
		*_trenutno = *d._trenutno;
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = new T1[*_trenutno];
		_elementi2 = new T2[*_trenutno];
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = d._elementi1[i];
			_elementi2[i] = d._elementi2[i];
		}
		return *this;
	}
	~Dictionary() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}
	void AddElement(const T1& el1, const T2& el2)
	{
		ProsiriNiz();
		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;

		(*_trenutno)++;
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }
	friend ostream& operator<< (ostream &COUT, const Dictionary &obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	Dictionary<int, int> getRange(int poc, int kraj)
	{
		if (poc<0 || poc>(*_trenutno) - 1 || kraj<0 || kraj>(*_trenutno) - 1 || poc >= kraj)
			throw exception("Izvan opsega kolekcije");

		Dictionary<int, int> d;
		for (int i = poc; i <= kraj; i++)
			d.AddElement(_elementi1[i], _elementi2[i]);
		cout << "Dodavanje elemenata od " << poc << " do " << kraj << " pozicije." << endl;
		return d;
	}
};

class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme& dv)
	{
		_dan = new int(*dv._dan);
		_mjesec = new int(*dv._mjesec);
		_godina = new int(*dv._godina);
		_sati = new int(*dv._sati);
		_minuti = new int(*dv._minuti);
	}
	DatumVrijeme& operator=(const DatumVrijeme& dv)
	{
		if (this == &dv)
			return *this;
		*_dan = *dv._dan;
		*_mjesec = *dv._mjesec;
		*_godina = *dv._godina;
		*_sati = *dv._sati;
		*_minuti = *dv._minuti;

		return *this;
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	const char * ToCharArray()
	{
		stringstream s;
		if (*_dan < 10)
			s << "0";

		s << *_dan << "/";

		if (*_mjesec < 10)
			s << "0";

		s<<*_mjesec << "/" << *_godina << " " << *_sati << ":" << *_minuti;
		string * stringArray = new string(s.str());
		return stringArray->c_str();
	}
	friend ostream& operator<< (ostream &COUT, const DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
	int brojMinuta() const
	{
		return *_minuti + 60 * *_sati + 1440 * * _dan + 43200 * *_mjesec + 518400 * *_godina;
	}
	friend int razlikaMinuta(const DatumVrijeme& d1, const DatumVrijeme& d2) 
	{
		return abs(d1.brojMinuta() - d2.brojMinuta());
	}
	
	friend bool operator<(const DatumVrijeme& d1, const DatumVrijeme& d2)
	{
		if (*d1._godina < *d2._godina)
			return true;
		if (*d1._godina == *d2._godina)
		{
			if (*d1._mjesec < *d2._mjesec)
				return true;
			if (*d1._mjesec == *d2._mjesec)
				if (*d1._dan < *d2._dan)
					return true;
		}
		return false;
	}
	
};

class Predmet {
	char * _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char * naziv = "", int ocjena = 0, string napomena = "") {
		_naziv = Alociraj(naziv);
		_ocjena = ocjena;
		_napomena = napomena;
	}
	Predmet(const Predmet& p)
	{
		_naziv = Alociraj(p._naziv);
		_ocjena = p._ocjena;
		_napomena = p._napomena;
	}
	Predmet& operator=(const Predmet& p)
	{
		if (this == &p)
			return *this;
		delete[] _naziv;
		_naziv = Alociraj(p._naziv);
		_ocjena = p._ocjena;
		_napomena = p._napomena;

		return *this;
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Predmet &obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	string GetNapomena() const { return _napomena; }
	char * GetNaziv() const { return _naziv; }
	int GetOcjena() { return _ocjena; }

	void DodajNapomenu(string napomena)
	{
		_napomena += " " + napomena;
	}
	friend bool operator==(const Predmet& p1, const Predmet& p2)
	{
		return strcmp(p1.GetNaziv(), p2.GetNaziv()) == 0;
	}
};

class Uspjeh {
	GodinaStudija * _godina;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Dictionary<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh(GodinaStudija godina) 
	{
		_godina = new GodinaStudija(godina);
	}
	Uspjeh(const Uspjeh& u)
	{
		_godina = new GodinaStudija(*u._godina);
		_predmeti = u._predmeti;
	}
	Uspjeh& operator=(const Uspjeh& u)
	{
		if (this == &u)
			return *this;
		delete[] _godina;
		_godina = new GodinaStudija(*u._godina);
		_predmeti = u._predmeti;
		return *this;
	}
	~Uspjeh() { delete _godina; _godina = nullptr; }

	Dictionary<Predmet, DatumVrijeme> * GetPredmeti() { return &_predmeti; }
	GodinaStudija * GetGodinaStudija() { return _godina; }
	friend ostream& operator<< (ostream &COUT, const Uspjeh &obj) {
		COUT <<"\nGodina studija: "<< *obj._godina << "\n " << obj._predmeti << endl;
		return COUT;
	}
	bool AddPredmet(const Predmet& p, const DatumVrijeme& d)
	{
		//provjera vremena dodavanja
		if(_predmeti.getTrenutno()>0)
			if (razlikaMinuta(_predmeti.getElement2(_predmeti.getTrenutno()-1), d) < 60)
			{
				cout << "Nije proslo sat vremena od posljednjeg dodavanja!" << endl;
				return false;
			}
		//provjera je li vec dodan taj predmet
		for (int i = 0; i < _predmeti.getTrenutno(); i++)
			if (_predmeti.getElement1(i) == p)
			{
				cout << "Taj predmet je vec dodan u okviru te godine!" << endl;
				return false;
			}
		_predmeti.AddElement(p, d);
		return true;
	}

	double GetUspjeh()
	{
		double suma = 0;
		int ukupno = _predmeti.getTrenutno();
		for (int i = 0; i < ukupno; i++)
		{
			suma += _predmeti.getElement1(i).GetOcjena();
		}
		return suma / ukupno;
	}

};

bool validEmail(string email)
{
	/*
email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
za provjeru koristiti regex
*/
	string pravilo = "[a-zA-Z]+[-][a-zA-Z]+[@]((hotmail.com)|(outlook.com)|(fit.ba))";
	return regex_match(email, regex(pravilo));
}

class Student {
	char * _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;
public:

	Student(const char * imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = Alociraj(imePrezime);
		if(validEmail(emailAdresa))
			_emailAdresa = emailAdresa;
		else _emailAdresa = "notSet@fit.ba";
		_brojTelefona = brojTelefona;
		for (int i = 1; i < 4; i++)
		{
			_uspjeh.push_back(GodinaStudija(i));
		}
	}
	Student(const Student& s)
	{
		_imePrezime = Alociraj(s._imePrezime);
		_emailAdresa = s._emailAdresa;
		_brojTelefona = s._brojTelefona;
		_uspjeh = s._uspjeh;
	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Student &obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;

		ostream_iterator<Uspjeh, char> out_iter(cout, "\n");
		copy(obj._uspjeh.begin(), obj._uspjeh.end(), out_iter);

		return COUT;
	}
	vector<Uspjeh> * GetUspjeh() { return &_uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char * GetImePrezime() { return _imePrezime; }

	bool AddPredmet(const Predmet& p, GodinaStudija g, const DatumVrijeme& d)
	{

		if (_uspjeh[g - 1].AddPredmet(p, d))
		{
			thread nit1(&Student::PosaljiEmail,this, g);

			double uspjeh = _uspjeh[g - 1].GetUspjeh();
			if (uspjeh > 8)
			{
				thread nit2(&Student::PosaljiSMS, this, g, uspjeh);
				nit2.join();
			}

			nit1.join();

			return true;
		}
		else return false;
	
	}

	void PosaljiEmail(GodinaStudija g)
	{
		string email = string("FROM:info@fit.ba\nTO:") + GetEmail() + "\nPostovani " + GetImePrezime() + ", evidentirali ste uspjeh za " + to_string(GodinaStudija(g)) + " godinu studija.\n\n";
		email += "Pozdrav\nFIT Team.\n";
		
		cout << email;	
	}

	void PosaljiSMS(GodinaStudija g, double uspjeh)
	{
		string sms = string("Svaka cast za uspjeh ") + to_string(uspjeh) + " ostvaren u " + to_string(GodinaStudija(g)) + " godini studija.\n\n";
	
		cout << sms;
	}
	
	//vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	int BrojPonavljanjaRijeci(string rijec)
	{
		int broj=0;
		const regex reg(rijec);

		for (int i = 0; i < _uspjeh.size(); i++)
		{
			for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{
				string * napomena = new string(_uspjeh[i].GetPredmeti()->getElement1(j).GetNapomena());
				
				sregex_iterator it_poc(napomena->cbegin(), napomena->cend(), reg);
				sregex_iterator it_end;

				while (it_poc != it_end)
				{
					broj++;
					it_poc++;
				}

			}
		}

		return broj;
	}

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> operator()(DatumVrijeme* d1, DatumVrijeme* d2)
	{
		vector<Predmet> answer;
		for (int i = 0; i < _uspjeh.size(); i++)
			for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				if (*d1 < _uspjeh[i].GetPredmeti()->getElement2(j) && _uspjeh[i].GetPredmeti()->getElement2(j) < *d2)
					answer.push_back(_uspjeh[i].GetPredmeti()->getElement1(j));

		return answer;
	}

	Uspjeh * operator[](const char * godina)
	{
		for(int i=0; i<3;i++)
		{
			if (strcmp(godina, charGodinaStudija[i]) == 0)
			{
				return &_uspjeh[i];
			}
		}
		return nullptr;
	}


};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/

	DatumVrijeme temp,
		datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);
	//funkcija ToCharArray vraca datum i vrijeme kao char *. konverziju izvrsiti koristeci stringstream objekat.
	//voditi racuna o tome da se izmedju datuma i vremena nalazi samo jedan razmak, te da su vrijednosti dana i mjeseca iskazane kao dvije cifre
	cout << datum19062019_1015.ToCharArray() << endl;//treba ispisati: 19/06/2019 10:15
	temp = datum05072019_1231;
	cout << temp.ToCharArray() << endl;//treba ispisati: 05/07/2019 12:31

	cout << "Razlika u minutama izmedju ta dva je " << razlikaMinuta(datum19062019_1015, temp);
	const int DictionaryTestSize = 9;
	Dictionary<int, int> Dictionary1;
	for (size_t i = 0; i < DictionaryTestSize; i++)
		Dictionary1.AddElement(i + 1, i*i);

	try {
		//vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
		//funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne
		Dictionary<int, int> opseg = Dictionary1.getRange(2, 7);
		cout << opseg << endl;
		Dictionary1.getRange(7, 11);
	}
	catch (exception& err) {
		cout << err.what() << endl;
	}
	cout << Dictionary1 << endl;

	Dictionary<int, int> Dictionary2 = Dictionary1;
	cout << Dictionary2 << crt;

	Dictionary<int, int> Dictionary3;
	Dictionary3 = Dictionary1;
	cout << Dictionary3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet MAT("Matematika", 7, "Ucesce na takmicenju"),
		UIT("Uvod u informacijske tehnologije", 9),
		RM("Racunarske mreze", 8),
		EN("Engleski jezik", 6);
	UIT.DodajNapomenu("Pohvala za ostvareni uspjeh");
	cout << MAT << endl;

	/*
	email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
	za provjeru koristiti regex
	*/

	Student jasmin("Jasmin Azemovic", "jasmin.azemovic@hotmail.com", "033 281 172");
	Student adel("Adel Handzic", "adel.handzic@fit.ba", "033 281 170");
	Student emailNotValid("Ime Prezime", "korisnik@lazna.ba", "033 281 170");

	/*
	uspjeh se dodaje za svaki predmet na nivou godine studija.
	tom prilikom onemoguciti:
	- dodavanje istoimenih predmeta na nivou jedne godine,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
	godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
	Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(UIT, DRUGA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(RM, DRUGA, datum30062019_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(EN, PRVA, datum19062019_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(MAT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati MAT jer je vec dodana u prvoj godini
	if (jasmin.AddPredmet(MAT, PRVA, datum05072019_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati UIT jer nije prosao 1 sat od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(UIT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	/*nakon evidentiranja uspjeha na bilo kojem predmetu tj. prilikom uspjesno izvrsene funkcije AddPredmet, Studentu se salje email sa sadrzajem:
	FROM:info@fit.ba
	TO: emailStudenta
	Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
	Pozdrav.
	FIT Team.

	ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email-a, salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X ostvaren u X godini studija".
	slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/
	cout << "USPJEH ISPISATI KORISTEĆI OSTREAM_ITERATOR" << endl<<endl;
	cout << jasmin << endl;

	//vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	cout << "\nPredmeti evidentirani izmedju 18.06.2019 i 21.06.2019." << endl;
	vector<Predmet> jasminUspjeh = jasmin(new DatumVrijeme(18, 06, 2019, 10, 15), new DatumVrijeme(21, 06, 2019, 10, 10));
	for (Predmet u : jasminUspjeh)
		cout << u << endl;

	Uspjeh * uspjeh_I_godina = jasmin["PRVA"];//vraca uspjeh Studenta ostvaren u prvoj godini studija
	if (uspjeh_I_godina != nullptr)
		cout << *uspjeh_I_godina << endl;

	cin.get();
	system("pause>0");
}
