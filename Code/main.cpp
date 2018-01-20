#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include <fstream>

using namespace std;

typedef vector <unsigned> CVLine; // un type représentant une ligne de la grille
typedef vector <CVLine> CMat; // un type représentant la grille
typedef pair <unsigned, unsigned> CPosition; // une position dans la girlle

/* AFFICHAGE */
const string CReset   ("0");

void Couleur (const string & coul);                                                     //Permet d'afficher des couleurs sur la console
void ClearScreen ();                                                                    //Vide la console
void ClearBuf ();                                                                       //Vide le tampon de cin
void InitGrid (CMat & Grid, unsigned Size, unsigned KNbCandies);                        //Initialise la grille sans combinaison
bool ReplaceEmpty (CMat & Grid, unsigned KNbCandies);                                   //Remplace les 0 par des nombres aléatoires
void DisplayGrid (const CMat & Grid);                                                   //Affiche la grille
void DownGrid (CMat & Grid);                                                            //Fait tomber les cases quand il y a un trou en dessous d'eux
void MakeAMove (CMat & Grid, const CPosition & Pos, char Direction);                    //Permet d'effectuer un mouvement de case
bool AtLeastThreeInAColumn  (const CMat & Grid, CPosition & Pos, unsigned & Howmany);   //Permet de trouver une combinaison de Howmany en colonnes
bool AtLeastThreeInARow  (const CMat & Grid, CPosition & Pos, unsigned & Howmany);      //Permet de trouver une combinaison de Howmany en lignes
void RemovalInColumn (CMat & Grid, const CPosition & Pos, unsigned  Howmany);           //Remplace les combinaisons en colonne par des 0
void RemovalInRow (CMat & Grid, const CPosition & Pos, unsigned  Howmany);              //Remplace les combinaisons en ligne par des 0

void Couleur (const string & coul)
{
    cout << "\033[7;" << coul <<"m";
} // Couleur()

void ClearScreen ()
{
    cout << "\033[H\033[2J";
} // ClearScreen ()

void InitGrid (CMat & Grid, unsigned Size, unsigned KNbCandies)
{
    Grid.resize (Size, vector <unsigned> (Size));
    srand(time(NULL));
    for (unsigned i (0); i < Grid.size(); ++i)
    {
        for (unsigned j(0); j < Grid[i].size(); ++j)
        {
            Grid [i][j] = rand()%KNbCandies+1;
        }
    }
    CPosition Pos;
    unsigned Howmany;
    do
    {
        while (AtLeastThreeInAColumn (Grid, Pos, Howmany))
        {
            RemovalInColumn (Grid, Pos, Howmany);
        }
        while (AtLeastThreeInARow  (Grid, Pos, Howmany))
        {
            RemovalInRow (Grid, Pos, Howmany);
        }
    } while (ReplaceEmpty(Grid, KNbCandies));
} // InitGrid ()

bool ReplaceEmpty (CMat & Grid, unsigned KNbCandies)
{
    srand(time(NULL));
    bool check = false;
    for (unsigned i (0); i < Grid.size(); ++i)
    {
        for (unsigned j(0); j < Grid[i].size(); ++j)
        {
            if (Grid [i][j] == 0)
            {
                Grid [i][j] = rand()%KNbCandies+1;
                check = true;
            }

        }
    }
    return check;
} // ReplaceEmpty ()

void DisplayGrid (const CMat & Grid)
{
    ClearScreen ();
    Couleur(CReset);
    unsigned line = 1;
    unsigned columns = 1;
    cout << "  ";
    while (columns <= Grid.size())
    {
        cout << columns;
        if (columns <= 9)
            cout << ' ';
        ++columns;
    }
    cout << endl;
    for (unsigned i (0); i < Grid.size(); ++i)
    {
        Couleur(CReset);
        cout << line;
        if (line <= 9)
            cout << ' ';
        ++line;
        for (unsigned j(0); j < Grid[i].size(); ++j)
        {
            Couleur (to_string(Grid [i][j] + 29));
            if (Grid [i][j] == 0)
                Couleur(CReset);
            cout << Grid [i][j] << ' ';
        }
        cout << endl;
    }
    Couleur(CReset);
    cout << endl;
} // DisplayGrid ()

void DownGrid (CMat & Grid)
{
    CPosition Posbis;
    for (unsigned i (0); i < Grid.size(); ++i)
    {
        for (unsigned j(0); j < Grid[i].size(); ++j)
        {
            unsigned ibis = i;
            if (Grid [i][j] == 0)
            {
                while (ibis > 0)
                {
                    swap (Grid[ibis-1][j], Grid[ibis][j]);
                    ibis--;
                }
            }
        }
    }
} //DownGrid ()

void ClearBuf ()
{
    cin.clear();
    string Buf;
    getline(cin, Buf);
} //ClearBuf ()

void MakeAMove (CMat & Grid)
{
    bool check = false;
    CPosition Pos;
    unsigned x;
    unsigned y;
    unsigned Size;
    char Direction;
    while (true)
    {
        Size = Grid.size();
        //RECUPERATION DE X
        do {
            for(;;)
            {
                cout << "Entrez un numero de colonne entre 1 et " << Size << endl;
                cin >> x;
                if (cin) break;
                if(cin.fail())
                {
                    cout << "Veuillez entrer un nombre !" << endl;
                    ClearBuf ();
                }
            }
            if(x <= Size && x != 0) break;
            cout << "Entrez  un chiffre entre 1 et " << Size << endl << endl;
        }while(true);
        ClearBuf ();
        //RECUPERATION DE Y
        do{
            for(;;)
            {
                cout << "Entrez un numero de ligne entre 1 et " << Size << endl;
                cin >> y;
                if (cin) break;
                if(cin.fail())
                {
                    cout << "Veuillez entrer un nombre !" << endl;
                    ClearBuf ();
                }
            }
            if(y <= Size && y != 0) break;
            cout << "Entrez  un chiffre entre 1 et " << Size << endl << endl;
        }while(true);
        Pos.first = y-1;
        Pos.second = x-1;
        //RECUPERATION DE DIRECTION
        ClearBuf ();
        while(true)
        {
            cout << "Entrez la direction (Z = HAUT ; S = BAS ; Q = GAUCHE ; D = DROITE)" << endl;
            string DirectionTemp;
            getline(cin, DirectionTemp);
            if(DirectionTemp.size() == 1)
            {
                Direction = DirectionTemp[0];
                Direction = tolower(Direction);
            }
            else
            {
                Direction = 'p';
            }
             if(Direction == 'z' || Direction == 'q' || Direction == 's' || Direction == 'd') break;
                cout << "Commande invalide !" << endl;

        }
        switch (Direction)
        {
            case 'z' : if(Pos.first != 0 && //Sortie de grille
                          (Grid[Pos.first-1][Pos.second] != 0) && (Grid[Pos.first][Pos.second] != 0)) //On ne bouge pas un 0
                        /*(Grid[Pos.first-1][Pos.second] != 6) && (Grid[Pos.first][Pos.second] != 6))*/   //On ne bouge pas un mur
                {
                    swap (Grid[Pos.first-1][Pos.second], Grid[Pos.first][Pos.second]);
                    check = true;
                }
            break;
            case 's' : if((Pos.first != Grid.size()-1) && //Sortie de grille
                          (Grid[Pos.first+1][Pos.second] != 0) && (Grid[Pos.first][Pos.second] != 0)) //On ne bouge pas un 0
                        /*(Grid[Pos.first+1][Pos.second] != 6) && (Grid[Pos.first][Pos.second] != 6))*/  //On ne bouge pas un mur
                {
                    swap (Grid[Pos.first+1][Pos.second], Grid[Pos.first][Pos.second]);
                    check = true;
                }
            break;
            case 'd' : if((Pos.second != Grid.size()-1) && //Sortie de grille
                          (Grid[Pos.first][Pos.second+1] != 0) && (Grid[Pos.first][Pos.second] != 0)) //On ne bouge pas un 0
                        /*(Grid[Pos.first][Pos.second+1] != 6) && (Grid[Pos.first][Pos.second] != 6))*/   //On ne bouge pas un mur
                {
                    swap (Grid[Pos.first][Pos.second+1], Grid[Pos.first][Pos.second]);
                    check = true;
                }
            break;
            case 'q' : if(Pos.first != 0 && //Sortie de grille
                          (Grid[Pos.first][Pos.second-1] != 0) && (Grid[Pos.first][Pos.second] != 0)) //On ne bouge pas un 0
                        /*(Grid[Pos.first][Pos.second-1] != 6) && (Grid[Pos.first][Pos.second] != 6))*/   //On ne bouge pas un mur
                {
                    swap (Grid[Pos.first][Pos.second-1], Grid[Pos.first][Pos.second]);
                    check = true;
                }
            break;
            default:
                cout << "Choix incorect, z=haut / q=gauche / s=bas / d=droite" << endl;
                cin >> Direction;
        }
        if (check)
            break;
        cout << "Erreur:" << endl
             << "Vous ne pouvez pas déplacer une case dans une case une vide ou hors de la grille" << endl;
    }
} //MakeAMove()

bool AtLeastThreeInAColumn (const CMat & Grid, CPosition & Pos, unsigned & Howmany)
{
    for (unsigned i (0); i < Grid.size() - 2; ++i)
    {
        for (unsigned j(0); j < Grid[i].size(); ++j)
        {
            if (Grid[i][j] == Grid [i+1][j] && Grid[i][j] == Grid [i+2][j]) //3 affilés
            {
                if (Grid[i][j] == 0) break;
                Pos.first = j + 1;
                Pos.second = i + 1;
                Howmany = 3;
                while (Grid[i][j] == Grid[i + Howmany][j])
                    Howmany++;
                return true;
            }
        }
    }
    return false;
} //AtLeastThreeInAColumn()

bool AtLeastThreeInARow  (const CMat & Grid, CPosition & Pos, unsigned & Howmany)
{
    for (unsigned i (0); i < Grid.size(); ++i)
    {
        for (unsigned j(0); j < Grid[i].size() - 2; ++j)
        {
            if (Grid[i][j] == Grid [i][j+1] && Grid[i][j] == Grid [i][j+2])
            {
                if (Grid[i][j] == 0) break;
                Pos.first = j + 1;
                Pos.second = i + 1;
                Howmany = 3;
                while (Grid[i][j] == Grid[i][j + Howmany])
                    ++Howmany;
                return true;
            }
        }
    }
    return false;
} //AtLeastThreeInARow()

void RemovalInColumn (CMat & Grid, const CPosition & Pos, unsigned  Howmany)
{
    unsigned Cpt = 0;
    while (Howmany != 0)
    {
        Howmany--;
        Grid[Pos.second + Cpt - 1][Pos.first - 1] = 0;
        Cpt++;
    }
} //RemovalInColumn()

void RemovalInRow (CMat & Grid, const CPosition & Pos, unsigned  Howmany)
{
    unsigned Cpt = 0;
    while (Howmany != 0)
    {
        Howmany--;
        Grid[Pos.second - 1][Pos.first - 1 + Cpt] = 0;
        Cpt++;
    }
} //RemovalInRow()

void DisplayFile (const string & File)
{
    ifstream ifs;
    ifs.open (File);
    if (!(ifs.is_open ()))
    {
        if (File == "logo.txt")
        {
            cout << "NumberCrush" << endl;
            return;
        }
        if (File == "menu.txt")
        {
            cout << "Menu" << endl
                 << "1 - Jouer" << endl
                 << "2 - Options" << endl
                 << "3 - Règles" << endl
                 << "4 - Quitter" << endl;
            return;
        }
    }
    string str;
    while (!(ifs.eof()))
    {
        getline (ifs, str);
        cout << str << endl;
    }
}// DisplayFile ()
int main()
{
    CMat Grid;
    unsigned Size;
    unsigned KNbCandies = 7;
    unsigned Score = 0;
    CPosition Pos;
    unsigned Coup;
    unsigned Howmany;
    DisplayFile  ("logo.txt");
    DisplayFile  ("menu.txt");

    char choix = '0';
    while (choix != '1' && choix != '2')
    {
        cin >> choix;
    }
    ClearBuf ();
    switch (choix)
    {
        case '1':
            cout << "Taille de la grille ?" << endl;
            cin >> Size;
            cout << "Nombre de coup max ?" << endl;
            cin >> Coup;
            break;
        case '2':
            Size = 8;
            Coup = 10;
            break;
    }
    //INITIALISATION
    InitGrid (Grid, Size, KNbCandies);
    while (true)
    {
        DisplayGrid (Grid);
        cout << "Il vous reste " << Coup << " coup(s) !" << endl
             << "Score actuel : " << Score << endl << endl;
        MakeAMove (Grid);
        DisplayGrid (Grid);
        sleep(1);
        do
        {
            while (AtLeastThreeInAColumn (Grid, Pos, Howmany))
            {
                cout << "Une combinaison de " << Howmany << " bonbons a ete trouve dans les colonnes en coordonnées " << Pos.first << " ; " << Pos.second  << endl;
                RemovalInColumn (Grid, Pos, Howmany);   //REMPLACE PAR DES 0
                DisplayGrid (Grid);                     //AFFICHE
                sleep(1);
                DownGrid (Grid);                        //REMONTE LES 0
                Score += 10;
            }
            while (AtLeastThreeInARow  (Grid, Pos, Howmany))
            {
                cout << "Une combinaison de " << Howmany << " bonbons a ete trouve dans les lignes en coordonnées " << Pos.first << " ; " << Pos.second  << endl;
                RemovalInRow (Grid, Pos, Howmany);      //REMPLACE PAR DES 0
                DisplayGrid (Grid);                     //AFFICHE
                sleep(1);
                DownGrid (Grid);                        //REMONTE LES 0
                Score += 10;
            }
        }  while (ReplaceEmpty (Grid, KNbCandies));

        --Coup;
        if (Coup == 0)
        {
            cout << "Partie termine !" << endl
                 << "Vous avez realise un score de " << Score << " !" << endl;
            break;
        }
    }

    return 0;
} //main()
