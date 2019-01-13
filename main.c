#include <stdio.h>

//pour le srand (aléatoire)
#include <stdlib.h>
#include <time.h>

//APPEL DES TYPES CRÉÉS
#include "case.h"
#include "coord.h"

//PROTOTYPES
int Lire_Entier(int inf, int sup);
void Lire_Parametres(int *N, int *M, int *R, int *Next, int *Niveau);
void Init_Grille(int N, int M, int R, Case *T);
int Contient_Pion(int CoN, int CoM, int R, Case *T);
void Affiche_Grille(int N, int M, int R, Case *T);
int nimberFct(int N, int M, int CoN, int CoM);
int Nim_Addition(int R, Case *T);
int Tab_Voisines(int N, int M, int CoN, int CoM, Coord *TabVois);
int Hasard(int sup);
void Maj_Grille(int N, int M, int *R, Case *T);
void Move_Joueur(int N, int M, int R, Case *T, Coord *TabVois);
void Move_Hasard(int N, int M, int R, Case *T, Coord *TabVois);
void Move_Gagnant(int N, int M, int R, Case *T, Coord *TabVois);

//MAIN
int main()
{
    //Déclaration des variables
    Coord TabVois[4];
    int N, M, R, Next, Niveau, fin = 0;

    //Appel des fonctions
    printf("~ JEU DE NIM ~\n\n");
    Lire_Parametres(&N,&M,&R,&Next,&Niveau);
    Case T[R]; //On déclare T ici pour pouvoir utiliser R (saisi dans Lire_Parametres) comme taille

    //Déroulement de la partie
    Init_Grille(N,M,R,T); //Début de la partie
    do
    {
        switch(Next)
        {
            case 1: //Ordinateur
            {
                int x, prob = Hasard(10);
                switch(Niveau)
                {
                    case 1: x = 9;  break; //Probabilité 0.9 de faire un coup au hasard
                    case 2: x = 5;  break; //Probabilité 0.5 de faire un coup au hasard
                    case 3: x = 1;  break; //Probabilité 0.1 de faire un coup au hasard
                    default : printf("Erreur : x pas initialisé"); break; //Vérification : ne sort normalement pas
                }
                if(prob<=x) //Probabilité de jouer un coup au hasard
                {
                    Move_Hasard(N,M,R,T,TabVois);
                    Maj_Grille(N,M,&R,T);
                }
                else //Probabilité de jouer un coup gagnant
                {
                    Move_Gagnant(N,M,R,T,TabVois);
                    Maj_Grille(N,M,&R,T);
                }
                if((R == 1) && (T[0].Co.coordN == N) && (T[0].Co.coordM == M)) fin = 1; //Cas où le dernier pion arrive
                else Next = 2;
                break;
            }
            case 2: //Joueur
            {
                Move_Joueur(N,M,R,T,TabVois);
                Maj_Grille(N,M,&R,T);
                if((R==1) && (T[0].Co.coordN == N) && (T[0].Co.coordM == M)) fin = 2;
                else Next = 1;
                break;
            }
            default : printf("Erreur : Next pas initialisé"); break; //Vérification : ne sort normalement pas
        }
    }
    while(fin == 0);

    //Message de fin
    if(fin == 1) printf("C'est termine, TU AS PERDU !");
    if(fin == 2) printf("C'est termine, TU AS GAGNE !");
}


//FONCTIONS

int Lire_Entier(int inf, int sup) //Permet de saisir un entier entre 2 bornes préalablement définies en paramètres
{
    int x = 0;
    while (x<inf||x>sup)
    {
        scanf("%d",&x); //Saisie d'un entier
        if(x<inf||x>sup) printf("erreur ! - Veuillez resaisir un nombre entre %d et %d :\n--> ", inf , sup);
        while (getchar()!='\n'); //Vidage du buffer
    }
    return x; //On retourne l'entier saisi
}

//On utilise des pointeurs dans la fction pour modifier les variables initialisés dans le main
void Lire_Parametres(int *N, int *M, int *R, int *Next, int *Niveau) //Permet de saisir les différentes variables nécessaires au prgm
{
    printf("Parametre du jeu \n");
    printf("----------------- \n");

    printf("Nombre de lignes   : ");
    *N = Lire_Entier(3,30);

    printf("Nombre de colonnes : ");
    *M = Lire_Entier(3,30);

    printf("Nombre de pions    : ");
    *R = Lire_Entier(1,*N);

    printf("Niveau de 1 a 3    : ");
    *Niveau = Lire_Entier(1,3);

    printf("Qui commence ?\nL'ordinateur (1) ou le joueur (2) : ");
    *Next = Lire_Entier(1,2);
}

void Init_Grille(int N, int M, int R, Case *T) //Fonction permettant d'initialiser la grille (et donc les pions)
{
    int i;
    for(i=0;i<R;i++) //On place tous les pions (on réitère donc R fois l'opération)
    {
        //On remplit le tableau T des cases occupées
        T[i].Co.coordN=i+1; //Permet d'accéder suucessivement aux coordonnées lignes de chaque case
        T[i].Co.coordM=1; //Idem pour colonne
        /*printf("%d : %d \n",T[i].Co.coordN, T[i].Co.coordM); //Vérification*/
        T[i].nimber = nimberFct(N,M,i+1,1); //Permet d'accéder suucessivement au nimber de chaque case
    }
    Affiche_Grille(N,M,R,T);
}

int Contient_Pion(int CoN, int CoM, int R, Case *T) //Fonction indiquant si une case contient un pion
{
    int i, g = 0; //g constitue un bouléen (faux : 0)
    for(i=0;i<R;i++)
    {
        if((T[i].Co.coordN == CoN) && (T[i].Co.coordM == CoM)) g = 1; //Si condition vérifiée, g vrai (1) -> il y a un pion à cette case
    }
    return g;
}

void Affiche_Grille(int N, int M, int R, Case *T) //Fonction affichant le jeu en mode console
{
    int i, j, k;
    printf("\n  ");
    for(i=1;i<=M;i++) printf("%4d",i); //Permet d'afficher les indicateurs de colonne
    printf("\n");
    //On parcourt toutes les cases du tableau avec j et k
    for(j=1;j<=N;j++)
    {
        printf("%2d |",j);//Permet d'afficher les indicateurs de lignes
        for(k=1;k<=M;k++)
        {
            //Si case occupée 0 sinon -
            if(Contient_Pion(j,k,R,T)) printf(" 0 |");
            else printf(" - |");
        }
        printf("\n");
    }
    printf("\n");
}

int nimberFct(int N, int M, int CoN, int CoM) //Calcule le nimber d'une case
{
    return ((N-CoN)%3)^((M-CoM)%3); //On fait %3 car on retrouve toujous le même carré de nimbers
}

int Nim_Addition(int R, Case *T) //Effectue la nim-addition de l'ensemble des cases occupées
{
    int i, nimAdd = 0;
    for(i=0;i<R;i++)
        nimAdd ^= T[i].nimber; //nim-addition
    return nimAdd;
}

int Tab_Voisines(int N, int M, int CoN, int CoM, Coord *TabVois) //Fonction ajoutant les voisins d'une case donnée dans un tableau et retournant le nbre de voisin
{
    int i = 0;
    //On regarde quels voisins existent pour une case donnée
    if(CoM+1<=M) //1er voisin à droite
    {
        TabVois[i].coordM = CoM+1;
        TabVois[i].coordN = CoN;
        i++; //Itère le nombre de voisin
    }
    if(CoM+2<=M) //2ème voisin à droite
    {
        TabVois[i].coordM = CoM+2;
        TabVois[i].coordN = CoN;
        i++;
    }
    if(CoN+1<=N) //1er voisin en bas
    {
        TabVois[i].coordM = CoM;
        TabVois[i].coordN = CoN+1;
        i++;
    }
    if(CoN+2<=N) //2ème voisin en bas
    {
        TabVois[i].coordM = CoM;
        TabVois[i].coordN = CoN+2;
        i++;
    }
    return i; //i=nbre de voisins
}

int Hasard(int sup) //Retourne un nombre au hasard entre 1 et une borne donnée
{
    srand((unsigned int)time(NULL));
    //Retourne un résultat entre 1 et une borne choisie (sup)
    return rand()%sup+1;
}

void Maj_Grille(int N, int M, int *R, Case *T) //Fonction mettant le jeu à jour
{
    int i;
    for (i=0;i<*R;i++)
    {
        if((*R>1) && (T[i].Co.coordN == N) && (T[i].Co.coordM == M)) //Condition correspondant à l'arrivée au puit
        {
            T[i] = T[*R-1]; //On prend la dernière valeur de T que l'on place à T[i]
            (*R)--; //Met à jour le nombre de pions restants
        }
    }
    Affiche_Grille(N,M,*R,T); //On affiche la grille avec les valeurs mises à jour
}

void Move_Joueur(int N, int M, int R, Case *T, Coord *TabVois) //Saisie + réalisation d'un coup par le joueur
{
    int i, j, chxPion, chxMvmt, nbVois;
    if(R == 1) printf("! Plus qu'un seul pion en (%d,%d)\n\n",T[0].Co.coordN,T[0].Co.coordN);
    printf("A toi de jouer !\n");

    //Choix du pion par le joueur
    printf("Choisir un pion: ");
    for(i=0;i<R;i++) printf("%d:(%d,%d) ",i+1,T[i].Co.coordN,T[i].Co.coordM); //sorte de menu des pions possibles
    printf("\n---> ");
    chxPion = Lire_Entier(1,R); //Saisie du pion

    //Choix du mouvement par le joueur
    nbVois = Tab_Voisines(N,M,T[chxPion-1].Co.coordN,T[chxPion-1].Co.coordM,TabVois); //nbVois prend pour valeur le nombre de voisin du pion choisi
    printf("Choisir la destination: ");
    for(j=0;j<nbVois;j++) printf("%d:(%d,%d) ",j+1,TabVois[j].coordN,TabVois[j].coordM); // sorte de menu des destinations possibles
    printf("\n---> ");
    chxMvmt = Lire_Entier(1,nbVois); //Saisie de la destination

    //Application des mouvements
    T[chxPion-1].nimber = nimberFct(N,M,T[chxMvmt-1].Co.coordN,T[chxMvmt-1].Co.coordM); //On calcule le nouveau nimber
    T[chxPion-1].Co = TabVois[chxMvmt-1]; //Les coordonnées initiales deviennent celles de la destination choisie
}

void Move_Hasard(int N, int M, int R, Case *T, Coord *TabVois) //Effectue un coup au hasard
{
    int chxPion, chxMvmt, nbVois;
    //Choix émulé de l'ordi du pion et du mouvement
    chxPion = Hasard(R);
    nbVois = Tab_Voisines(N,M,T[chxPion-1].Co.coordN,T[chxPion-1].Co.coordM,TabVois);
    chxMvmt = Hasard(nbVois);

    //Affichage de l'action choisie
    printf("L'ordinateur joue (%d,%d) ---> (%d,%d)\n",T[chxPion-1].Co.coordN,T[chxPion-1].Co.coordM,TabVois[chxMvmt-1].coordN,TabVois[chxMvmt-1].coordM);

    //Application des mouvements
    T[chxPion-1].nimber = nimberFct(N,M,TabVois[chxMvmt-1].coordN,TabVois[chxMvmt-1].coordM);
    T[chxPion-1].Co = TabVois[chxMvmt-1];
}

void Move_Gagnant(int N, int M, int R, Case *T, Coord *TabVois) //Effetue un coup gagnant
{
    int i, j, nbVois, p = Nim_Addition(R,T);
    if(Nim_Addition(R,T) == 0) Move_Hasard(N,M,R,T,TabVois); //Pions en position nulle
    else
        for(i=0;i<R;i++)
        {
            nbVois=Tab_Voisines(N,M,T[i].Co.coordN,T[i].Co.coordM,TabVois);
            for(j=0;j<nbVois;j++)
                if(nimberFct(N,M,TabVois[j].coordN,TabVois[j].coordM) == (T[i].nimber^p)) //Stratégie gagnante pour passer d'une positionnon nulle à une position nulle
                {
                    //Affichage de l'action qui va être faite
                    printf("L'ordinateur joue (%d,%d) ---> (%d,%d)\n",T[i].Co.coordN,T[i].Co.coordM,TabVois[j].coordN,TabVois[j].coordM);

                    //Application des mouvements
                    T[i].nimber = nimberFct(N,M,TabVois[j].coordN,TabVois[j].coordM);
                    T[i].Co = TabVois[j];

                    //Sorti de la double boucle for
                    nbVois = 0;
                    i = R;
                }
        }
}
