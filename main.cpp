#include <iostream>
#include <string>

using namespace std;

int m;

struct Cvor {
	int broj;
	Cvor** sinovi;
	Cvor* otac;
	int c;
	Cvor(int br = 0, int stepen = m) :broj(br) {
		sinovi = new Cvor * [stepen]; otac = nullptr;
		for (int i = 0; i < stepen; i++) {
			sinovi[i] = nullptr;
			c = 0;
		}
	}
	~Cvor() { delete[] sinovi; otac = nullptr; sinovi = nullptr; }

};

class Red {
private:
	struct Elem {
		Cvor* pok;
		Elem* sled;
		Elem(Cvor* p, Elem* sl = nullptr) :pok(p), sled(sl) {};
	};
	Elem* head, * tale;
	void kopiraj(const Red&);
	void premesti(Red&);
public:
	void brisi();
	Red() { head = tale = nullptr; }
	Red(const Red& red) { kopiraj(red); }
	Red& operator=(const Red& red) {
		if (this != &red) { brisi(); kopiraj(red); }
		return *this;
	}
	Red& operator=(Red&& red) {
		if (this != &red) { brisi(); premesti(red); }
		return *this;
	}
	Red(Red&& red) { premesti(red); }
	~Red() { brisi(); }
	Red& dodaj(Cvor*);
	Cvor* uzmi();
	bool prazan();
};



class Stablo {
private:
	int brojacIN = 0;
	Cvor* prvi; Cvor* tek;
	Red redIN, redOUT;
	int brCvorova = 0;
public:
	void brisi();
	//int otacCvora(int a) const;
	Stablo() { prvi = tek = nullptr; }
	Stablo(int br, int stepen = m) {
		prvi = tek = new Cvor(br, stepen); brCvorova++;
		redOUT.dodaj(prvi);
	}
	~Stablo() { brisi(); }
	Stablo(const Stablo&) = delete;
	Stablo& operator=(const Stablo&) = delete;
	Stablo& operator+=(int);
	friend ostream& operator<<(ostream&, Stablo&);
	int dohvatiBrCvorova() const;
	int vrednostOca(int) const;
};



int main()
{
	cout << "Kojeg stepena zelite da bude ovo stablo?";
	cin >> m;
	cout << "Izabrali ste da stablo bude " << m << ". stepena" << endl;
	cout << "Iz menija izaberite koje operacije zelite da se izvrse." << endl;
	string c; int broj;
	Stablo stablo;
	cout << "Unesite opciju.(Ako izaberete opciju koja nije definisana, program ce se prekinuti.)" << endl;
	cout << "1.Umetanje novog elementa u stablo " << endl;
	cout << "2.Ispis stabla u level order poretku." << endl;
	cout << "3.Brisanje stabla." << endl;
	cout << "4.Otac cvora ciji cvor nosi broj:" << endl;
	cout << "5.Broj cvorova u ovom stablu." << endl;
	while (1) {
		cout << "Unesite opciju." << endl;
		cin >> c;
		if (c == "1") {
			cout << "Opcija 1.Unesite novi cvor stabla." << endl;
			cin >> broj; stablo += broj;

		}
		else if (c == "2") {
			cout << "Opcija 2.Ispis stabla u level order poretku" << endl;
			cout << stablo << endl;

		}
		else if (c == "3") {
			cout << "Opcija 3.Brisanje stabla" << endl;
			stablo.brisi();

		}
		else if (c == "4") {
			cout << "Opcija 4.Unesite cvor cijeg oca trazite." << endl;
			cin >> broj;
			cout << "Otac cvora " << broj << " je " << stablo.vrednostOca(broj) << endl;

		}
		else if (c == "5") {
			cout << "Opcija 5.Broj cvorova u ovom stablu" << endl;
			cout << "Broj cvorova u stablu je:" << stablo.dohvatiBrCvorova() << endl;

		}
		else {
			cout << "Kraj programa." << endl;
			break;
		}
	}

	return 0;
}

void Red::brisi()
{
	while (head) {
		Elem* stari = head; head = head->sled; delete stari;
	}
	head = tale = nullptr;
}

void Red::kopiraj(const Red& red)
{
	Elem* tek = red.head;
	while (tek) {
		Elem* novi = new Elem(tek->pok);
		if (head == nullptr) head = novi;
		else tale->sled = novi;
		tale = novi;
		tek = tek->sled;
	}
}

void Red::premesti(Red& red)
{
	head = red.head; tale = red.tale;
	red.head = red.tale = nullptr;
}



Red& Red::dodaj(Cvor* p)
{
	Elem* novi = new Elem(p);
	if (head == nullptr) head = novi;
	else tale->sled = novi;
	tale = novi;
	return *this;
}

Cvor* Red::uzmi()
{
	if (head == nullptr) exit(1);
	Elem* stari = head; head = head->sled; Cvor* vred = stari->pok; delete stari;
	if (head == nullptr) tale = nullptr;
	return vred;
}

bool Red::prazan()
{
	if (head == nullptr) return true;
	else return false;
}


void Stablo::brisi()
{
	Red red; redIN.brisi(); red.dodaj(prvi); redOUT.brisi(); Cvor* pom; int provera = 1; brojacIN = 0; brCvorova = 0;//Sve vratimo u ispravno prazno stanje
	if (prvi == nullptr) { cout << "Pokusavate da obrisete prazno stablo." << endl; provera = 0; }
	else {
		while (provera) {
			if (red.prazan() == false) {
				pom = red.uzmi();
			}
			else {
				pom = nullptr;
				provera = 0;
			}
			if (pom != nullptr) {
				for (int i = 0; i < m; i++) {
					if (pom->sinovi[i] != nullptr) red.dodaj(pom->sinovi[i]);
				}
				delete pom; pom = nullptr;
			}
		}
	}
	prvi = tek = nullptr;
}


Stablo& Stablo::operator+=(int a)
{
	if (prvi == nullptr) { prvi = tek = new Cvor(a, m); redOUT.dodaj(prvi);  brCvorova++; return *this; }
	while (1) {
		if (brojacIN == m) {
			if (redIN.prazan() == false) {
				tek = redIN.uzmi(); brojacIN = 0;
			}
			else {
				tek = nullptr; return *this;
			}
		}
		while (brojacIN < m) {
			if (tek->sinovi[brojacIN] == nullptr) {
				tek->sinovi[brojacIN] = new Cvor(a, m); tek->sinovi[brojacIN]->otac = tek;
				brCvorova++;
				redIN.dodaj(tek->sinovi[brojacIN]); redOUT.dodaj(tek->sinovi[brojacIN]);
				brojacIN++;
				return *this;
			}

		}
	}
}

int Stablo::dohvatiBrCvorova() const
{
	return brCvorova;
}

int Stablo::vrednostOca(int a) const
{
	Cvor* pom; Red red; red.dodaj(prvi); int provera = 1;
	if (prvi == nullptr) { cout << "Prazno stablo" << endl; return -1; }
	else {
		while (1) {
			if (red.prazan() == false) pom = red.uzmi();
			else pom = nullptr;
			if (pom == nullptr) { cout << "Uneli ste nepostojeci element." << endl; return -2; }
			else {
				for (int i = 0; i < m; i++) {
					if (pom->sinovi[i] != nullptr) {
						if (pom->sinovi[i]->broj == a) return pom->broj;
						if (pom->sinovi[i]->sinovi[0] != nullptr) red.dodaj(pom->sinovi[i]);
					}
				}
			}
		}
	}
}

ostream& operator<<(ostream& it, Stablo& stablo)
{
	Cvor* pom = nullptr; Red red = stablo.redOUT;
	if (stablo.prvi == nullptr) { cout << "Hocete da ispisete prazno stablo" << endl; return it; }
	do {
		if (red.prazan() == false) {
			pom = red.uzmi();
		}
		else pom = nullptr;

		if (pom == nullptr) return it;
		else {
			it << pom->broj << " ";
		}

	} while (pom != nullptr);


	return it;

}
