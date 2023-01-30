#include <iostream>
#include <vector>

using namespace std;
/**
Przyjmowane dane:
W - szerokosc obszaru/bloku;
H - wysokosc obszaru/bloku;
n - liczba blokow;
**/
/**
----------Sekcja: Klasy----------
**/
//Klasa bazowa Rectangle, z ktorej pola i funkcje
//dziedzicza pozostale klasy
class Rectangle {
public:
    //Zmienne przechowujace szerokosc W i wysokosc H obiektu
    int W = 0;
    int H = 0;
    int x_0 = 0; //pozycja wypelnienia blokiem (szerokosc)
    int y_0 = 0; //pozycja wypelnienia blokiem (wysokosc)
    //Gettery i Settery
    int getW(){
        return W;
    }
    void setW(int width){
        W = width;
    }
    int getH(){
        return H;
    }
    void setH(int height){
        H = height;
    }
    int getX0(){
        return x_0;
    }
    void setX0(int x){
        x_0 = x;
    }
    int getY0(){
        return y_0;
    }
    void setY0(int y){
        y_0 = y;
    }
};
//Klasa Block reprezentujaca obiekty blokow
class Block: public Rectangle {
};
//Klasa Area reprezentujaca obszar i podobszary
class Area: public Rectangle {
public:
    //Funkcja pozwalajaca na wycinanie blokow z podobszarow
    int cutBlock(Block block) {
        //Zmienna R przechowujaca ilosc obszaru pozostalego po wycieciu bloku
        int R = block.getH()*block.getW();
        //Jesli podobszar ma wysokosc wieksza od szerokosci,
        //wtedy skracana jest wysokosc, w przeciwnym wypadku
        //szerokosc
        if (getH() > getW() && getY0() >= block.getH()) {
            setY0(getY0() - block.getH());
        } else if  (getH() <= getW() && getX0() >= block.getW()) {
            setX0(getX0() - block.getW());
        }
        return R;
    }
};
/**
----------Sekcja: Funkcje----------
**/
//Wycinanie pierwszego bloku: obszar przy krawedzi pionowej bloku
Area cutBlockH(Area area, Block block) {
    area.setW(area.getW() - block.getW());
    area.setH(block.getH());
    area.setY0(area.getH());
    return area;
}
//Wycinanie pierwszego bloku: obszar przy krawedzi poziomej bloku
Area cutBlockW(Area area, Block block) {
    area.setH(area.getH() - block.getH());
    area.setX0(area.getW());
    return area;
}
//Podzial na dwa obszary po wycieciu pierwszego bloku
vector<Area> divideIntoAreas(Area area, Block block) {
    vector<Area> aList;
    aList.push_back(cutBlockW(area, block)); //dodawanie obszaru przy poziomej krawedzi bloku
    aList.push_back(cutBlockH(area, block)); //dodawanie obszaru przy pionowej krawedzi bloku
    return aList;
}
//Obliczanie pomocniczej powierzchni zmarnowanego materialu R2
//(powierzchnia przy krawedzi poziomej bloku)
int countR1(int R1, Area area, Block block) {
    //Zmienna flag przechowuje stan petli while - okresla, czy obszar
    //zostal maksymalnie wypelniony
    bool flag = true;
    //Jesli nie znaleziono optymalnego bloku, caly obszar uwaza sie
    //za zmarnowany
    if (block.getH() == 0) {
        return area.getH() * area.getW();
    }
    //Petla wycinajaca bloki. Wycina bloki tak dlugo, az rozmiar
    //obszaru nie pozwoli na dalsze wyciecie pelnego bloku
    while (flag) {
        flag = false;
        if (area.getX0() >= block.getW()) {
            flag = true;
            area.cutBlock(block);
            R1 -= block.getH() * block.getW();
        }
    }
    return R1;
}
//Obliczanie pomocniczej powierzchni zmarnowanego materialu R2
//(powierzchnia przy krawedzi pionowej bloku)
int countR2(int R2, Area area, Block block) {
    //Zmienna flag przechowuje stan petli while - okresla, czy obszar
    //zostal maksymalnie wypelniony
    bool flag = true;
    //Jesli nie znaleziono optymalnego bloku, caly obszar uwaza sie
    //za zmarnowany
    if (block.getW() == 0) {
        return area.getW() * area.getH();
    }
    //Petla wycinajaca bloki. Wycina bloki tak dlugo, az rozmiar
    //obszaru nie pozwoli na dalsze wyciecie pelnego bloku
    while (flag) {
        flag = false;
        if (area.getY0() >= block.getH()) {
            flag = true;
            area.cutBlock(block);
            R2 -= block.getW() * block.getH();
        }
    }
    return R2;
}
/**
----------Glowna funkcja programu----------
**/
//Obliczanie calkowitej powierzchni zmarnowanego materialu R
int countR(Area area, vector<Block> blockList) {
    //Zmienna przechowujaca ilosc zmarnowanego materialu R
    int R = 0;
    //Lista obszarow powstajaca po wycieciu pierwszego bloku
    vector<Area> aList;
    //Dzielenie obszaru glownego na dwa podobszary i zapisanie ich na liscie
    //Zmienna aList przechowuje te liste
    aList = divideIntoAreas(area, blockList.front());
    //Deklaracja zmiennych R1 i R2 przechowujacych ilosc zmarnowanego materialu
    //z podobszarow
    int R1 = aList.front().getW() * aList.front().getH(); //obszar przy poziomej krawedzi bloku
    int R2 = aList.back().getW() * aList.back().getH(); //obszar przy pionowej krawedzi bloku
    //Deklaracja blokow bR1 i bR2
    Block bR1; //blok odpowiedni do wypelnienia obszaru przy poziomej krawedzi bloku
    Block bR2; //blok odpowiedni do wypelnienia obszaru przy pionowej krawedzi bloku
    //Jesli w danych wejsciowych podano wiecej niz 1 blok
    if (blockList.size() > 1) {
        //Petla for przechodzaca wszystkie pozycje listy blokow blockList za wyjatkiem pierwszego
        //(usunietego juz) bloku, aby wyciac je z powstalych podobszarow
        //Jednoczesnie w tej samej petli odbywa sie poszukiwanie bloku optymalnego do pozniejszego
        //wycinania podobszarow (bR1 i bR2)
        for (int i = 1; i < blockList.size(); i++) {
            //Zmienna flag, ktora przechowuje stan instrukcji warunkowej if
            bool flag = false;
            //Jesli zostanie znaleziony blok o wymiarach 1x1, program moze zakonczyc dzialanie
            //zwracajac wartosc 0, poniewaz wiadomo, ze mozna wtedy wypelnic tym blokiem caly
            //dostepny obszar nie marnujac zadnej powierzchni
            if (blockList.at(i).getH() == 1 && blockList.at(i).getW() == 1) {
                return 0;
            }
            //Jesli pierwszy i drugi warunek zostana spelnione, to po wykonaniu instrukcji z pierwszego
            //warunku zmienna zmienia swoja wartosc na true, aby uniknac odejmowania powierzchni bloku z
            //drugiego obszaru (poniewaz ten sam blok zostal juz wyciety wczesniej). Instrukcja musi byc
            //podzielona na dwa if'y, z uwagi na to, ze ten sam blok moze byc optymalny dla obu podobszarow
            if (blockList.at(i).getH() % (aList.front().getH() + 1) > bR1.getH()
                && blockList.at(i).getH() <= aList.front().getY0()) {
                flag = true;
                bR1 = blockList.at(i);
                R1 -= aList.front().cutBlock(blockList.at(i));
            }
            if (blockList.at(i).getW() % (aList.back().getW() + 1) > bR2.getW()
                && blockList.at(i).getW() <= aList.back().getX0()) {
                bR2 = blockList.at(i);
                if (!flag) {
                    R2 -= aList.back().cutBlock(blockList.at(i));
                }
            }
        }
    }
    //W przypadku, gdy lista blokow zawiera tylko jeden blok, ktory zostal wyciety,
    //ale jest on na tyle niewielki, ze mozna go wyciac z podobszarow, bloki bR1 i
    //bR2 przyjmuja go jako wartosc
    else if (blockList.size() == 1) {
        bR1 = blockList.at(0);
        bR2 = blockList.at(0);
    }
    //Obliczanie obszarow skladowych zmarnowanego materialu (R1 i R2)
    R1 = countR1(R1, aList.front(), bR1); //obliczanie obszaru zmarnowanego materialu R1
    R2 = countR2(R2, aList.back(), bR2); //obliczanie obszaru zmarnowanego materialu R2
    cout << "Area 1 position (" << aList.front().getX0() << "x" << aList.front().getY0() << ");" << endl;
    cout << "Area 2 position (" << aList.back().getX0() << "x" << aList.back().getY0() << ");" << endl;
    //Sumowanie wyniku
    R = R1 + R2;
    return R;
}
/**
----------Sekcja: Metoda main----------
**/
int main(int argc, char const *argv[])
{
    int W; //szerokosc obszaru/bloku
    int H; //wysokosc obszaru/bloku
    int n; //liczba blokow
    Area a1; //obszar bazowy, z ktorego wycinamy ("plaszczyzna")
    vector<Block> bL; //lista wszystkich blokow
    Block b1; //pojedynczy blok pomocny do zapisywania blokow

    cout << "Podaj szerokosc plaszczyzny: " << endl;
    cin >> W; //input: szerokosc obszaru
    cout << "Podaj wysokosc plaszczyzny: " << endl;
    cin >> H; //input: wysokosc obszaru
    cout << "Podaj liczbe blokow: " << endl;
    cin >> n; //input: ilosc blokow
    a1.setW(W);
    a1.setH(H);
    for(int i = 0; i < n; i++) {
        cout << "Podaj szerokosc bloku: " << endl;
        cin >> W; //input: szerokosc bloku
        cout << "Podaj wysokosc bloku: " << endl;
        cin >> H; //input: wysokosc bloku
        //ustawienie rozmiarow bloku
        b1.setW(W); //szerokosc bloku
        b1.setH(H); //wysokosc bloku
        //dodanie bloku do listy
        bL.push_back(b1);
    }
    //wypisanie wyniku dzialania programu
    cout << countR(a1, bL) << ";" << endl;
    //oczekiwanie na wcisniecie przycisku przez uzytkownika
    system("pause");
    return 0;
}
