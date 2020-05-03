#include <iostream>
#include <set>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;
ifstream f("TEMA2.in");
ofstream g("tema2.out");
queue <int> Q;
char alfa, alf[101];
int n,m,q0,nr,x,y,tranz,var,a,T,nrelem,aux[101],NR;
int mat[101][101][101],inchidere[101][101][101];
int inchidereL[101][101],inchidereelem[101][101];
bool viz[101],OK,parcurs[101],stergere[101],finall[101],finalnou[101];
void citire()
{
    f>>n;
    f>>m;
    for (int i=0; i<m; i++)
        f>>alf[i];
    f>>q0;
    f>>nr;
    for (int i=0; i<nr; i++)
    {
        f>>x;
        finall[x] = 1;
    }
    f>>tranz;
    for (int i=0; i<tranz; i++)
    {
        f>>x>>alfa>>y;
        if (alfa != '$')
            var = alfa - 'a';
        else
            var = 40;
        mat[x][y][var] = 1;
    }
}
void start_x(int start,int val) ///vizitam toate starile in care putem ajunge plecand in starea start
/// si folosind val+'a'(care reprezinta o litera din alfabet. pe 40 se alfa lambda)
{
    if(val==40)
        viz[start]=1;
    parcurs[start]=1;
    for(int i=0; i<n; i++)
        if(mat[start][i][40]==1 and mat[start][i][val]==0)///daca putem trece cu lambda
            start_x(i,val);
        else if(mat[start][i][val]==1)
            viz[i]=1;
    for(int i=0; i<n; i++)
        if(viz[i]==1 and parcurs[i]==0) ///daca starea a fost vizitata,dar nu si parcursa
        {
            start_x(i,val);
            parcurs[i]=1;
        }
    ///vectorul de parcurgere este pentru a verifica daca din toate starile pe care le-am vizitat am plecat,adica starea
    ///respectiva sa fie stare de start si sa vedem unde putem ajunge cu caracterul respectiv si tot asa
}

void lambdainchiderea_automat(int elemalfabet,int M[100][101])///reuniunea starilor accesibile cu caracterul (val+'a')
///din fiecare din starile din λ-inchidere
{
    for(int i=0; i<=n; i++)///pentru fiecare stare determinam  toate starile in care putem ajunge plecand din ea cu
        ///caracterul elemalfabet care e de tip int pt ca i am scazut un 'a';
    {
        int stareactuala=i;
        for(int cnt=0; cnt<n; cnt++)
        {
            viz[cnt]=0;
            parcurs[cnt]=0;
        }
        int nrelem=0;
        start_x(stareactuala,elemalfabet);
        ///formam o matrice in care avem in prima dimensiune starea din care pleaca,iar in cea de-a doua dimensiune pe
        ///pozitia 0 nr de stari in care poate ajunge cu caracterul elemalfabet,iar de la 1 pana la elemetul mat[i][0]
        /// starile in care poate ajunge starea i
        for(int cnt=0; cnt<n; cnt++)
            if(viz[cnt]==1)
                M[i][++nrelem]=cnt;
        M[i][0]=nrelem;
    }
}

void lambdanfa_to_nfa()
{
    ///Matricea inchidereL reprezinta λ-inchiderea starii de pe linia respectiva
    lambdainchiderea_automat(40,inchidereL);
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<=inchidereL[i][0]; j++)
            g<<inchidereL[i][j]<<" ";
        g<<endl;
    }
    ///stabilim noile stari finale
    ///• Starile finale vor fi toate starile care contin o stare finala din automatul initial in λ-inchidere
    for(int i=0; i<n; i++)
    {
        int gasit=0;
        for(int j=0; j<=inchidereL[i][0]; j++)
            if(finall[inchidereL[i][j]]==1)
            {
                gasit=1;
                break;
            }
        if(gasit==1)
            finalnou[i]=1;
        if(gasit==0 and finall[i]==1 )
            finalnou[i]=0;
    }
    for(int i=0; i<n; i++)
        finall[i]=finalnou[i];

    for (int cnt=0; cnt<m; cnt++) ///parcurgem alfabetul
    {
        int inchidereelem[101][101];
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                inchidereelem[i][j]=0;
        g<<endl<<alf[cnt]<<endl;
        int elem=alf[cnt]-'a';
        lambdainchiderea_automat(elem,inchidereelem);///pt  litera din alfabet ii formam matricea starilor
        /// λ-inchiderea multimii a reuniunii starilor
        /// accesibile cu caracterul (elemalfabet+'a')  din fiecare din starile din λ-inchidere

        for(int i=0; i<n; i++)///formam a λ-inchiderea multimii din a doua dimensiune a matricei inchierelem.
        ///pe care o adaugam intr o matrice tridimensionala unde prima dimensiune e litera din alfabet cu care
        ///pleaca,in cea de-a doua dimensiune starea i, iar in a 3-a pe  pozitia 0  se afla nr de stari in care poate
        ///ajunge cu caracterul elemalfabet,iar de la 1 pana la elemetul mat[i][0] starile in care poate ajunge
        /// starea i
        {
            for(int l=0; l<n; l++)
                viz[l]=0;
            for(int j=1; j<=inchidereelem[i][0]; j++)
            {
                int indice=inchidereelem[i][j];
                for(int l=1; l<=inchidereL[indice][0]; l++)
                    viz[inchidereL[indice][l]]=1;
            }
            int nrelem=0;
            for(int l=0; l<n; l++)
                if(viz[l]==1)
                    inchidere[cnt][i][++nrelem]=l;
            inchidere[cnt][i][0]=nrelem;
        }
        for(int i=0; i<n; i++)
        {
            g<<i<<": ";
            for(int j=0; j<=inchidere[cnt][i][0]; j++)
                g<<inchidere[cnt][i][j]<<" ";
            g<<endl;
        }
    }
    for(int i=0; i<n; i++)///stabilim starile redundate
    {
        for(int l=i+1; l<n ; l++)
            if(finall[i]==finall[l] and inchidere[0][i][0]==inchidere[0][l][0]) ///daca sunt stari finale amandoua
            ///si daca au nr de elemente egal
            {
                int OK=1;
                for(int cnt=0; cnt<m and OK==1; cnt++)
                    if( inchidere[cnt][i][0]==inchidere[cnt][l][0])
                    {
                        for(int j=0; j<=inchidere[cnt][l][0]; j++)
                            if(inchidere[cnt][i][j]!=inchidere[cnt][l][j])
                            {
                                OK=0;
                                break;
                            }
                    }
                    else
                        OK=0;
                if(OK==1)///daca am gasit o linie identica o stergem.
                {
                    if(a==0)
                        T=i,a=1;
                    stergere[l]=1;
                }
            }

    }
    for(int cnt=0; cnt<m; cnt++)///inlocuim ce trebuie sters cu starea ramasa
        for(int i=0; i<n; i++)
            if(stergere[i]==1)
            {
                int val=1+inchidere[cnt][i][0];
                inchidere[cnt][i][val]=T;
                inchidere[cnt][i][0]=1+inchidere[cnt][i][0];
            }

    ///eliminare_stari_redundate();
    for(int cnt=0; cnt<m; cnt++)
        for(int i=0; i<n; i++)
            if(stergere[i]==0)
            {
                for(int j=0; j<n; j++)
                    viz[j]=0;
                for(int j=1; j<=inchidere[cnt][i][0]; j++)
                {
                    if(stergere[inchidere[cnt][i][j]]==1)
                        inchidere[cnt][i][j]=T;
                    viz[inchidere[cnt][i][j]]=1;
                }
                int nrelem=0;
                for(int j=0; j<n; j++)
                    if(viz[j]==1)
                        inchidere[cnt][i][++nrelem]=j;
                inchidere[cnt][i][0]=nrelem;
            }
            else
                inchidere[cnt][i][0]=-1;
    g<<"LAMBDANFA->NFA"<<endl;
}
/*
void adaugare_dfa()
{
    int stareactuala=Q.front();
    Q.pop();
    while(!Q.empty())
    {
        for(int cnt=0; cnt<m; cnt++)///parcurgem literele din alfabet
            for(int i=0; i<NR; i++)///parcurgem starile
                if(parcurs[i]==0)///daca nu a mai fost parcursa starea
                {
                    for(int j=1; j<=inchidere[cnt][i][0]; j++)///vizitam toate elementele
                    {
                        for(int l=0; l<n; l++)
                            viz[l]=0,aux[l]=0;

                        x=inchidere[cnt][i][j];
                        for(int l=1; l<=inchidere[cnt][x][0]; l++)
                            viz[inchidere[cnt][x][l]]=1;

                    }
                    nrelem=0;
                        for(int l=0; l<n; l++)
                            if(viz[l]==1)
                                aux[++nrelem]=l;
                        aux[0]=nrelem;
                    ///cautam aux in matrice in a 2a dimensiune
                    int gasit=0;
                    for(int l=0; l<NR and gasit==0; l++)
                        for(int k=0; k<=inchidere[cnt][l][0]; k++)
                            if(inchidere[cnt][l][k]!=aux[k])
                            {
                                gasit=1;
                                break;
                            }
                    if(gasit==0)///daca nu am gasit vectorul aux  realizam o noua stare
                    {
                        NR++;
                        Q.push(NR);
                        for(int k=0; k<=inchidere[cnt][NR][0]; k++)
                            inchidere[cnt][NR][k]=aux[k];
                    }
                    else
                        parcurs[i]=1;

                    parcurs[i]=0;
                }
    }
}
void nfa_to_dfa()
{
    for(int cnt=0; cnt<m; cnt++)
        for(int i=0; i<n; i++)
            inchidere[cnt][0][i]=1;
    for(int cnt=0; cnt<m; cnt++)///Construim o matrice tridimensionala unde prima dimensiune reprezinta o litera
        ///a alfabetului pe care o citim, a doua dimensiune reprezinta o stare, iar in cea de-a treia o sa fie o multime
        ///cu toate starile in care putem ajunge folosind litera alfabetului
    {

        int inchidereelem[101][101];
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                inchidereelem[i][j]=0;
        g<<endl<<alf[cnt]<<endl;
        int elem=alf[cnt]-'a';
        lambdainchiderea_automat(elem,inchidereelem);
        for(int i=0; i<n; i++)
        {
            nrelem=0;
            for(int j=1; j<=inchidereelem[i][0]; j++)
                inchidere[cnt][i][++nrelem]=inchidereelem[i][j];
            inchidere[cnt][i][0]=nrelem;
        }
        for(int i=0; i<n; i++)
        {
            g<<i<<": ";
            for(int j=1; j<=inchidere[cnt][i][0]; j++)
                g<<inchidere[cnt][i][j]<<" ";
            g<<endl;
        }
    }
    for(int i=0; i<n; i++)
        parcurs[i]=0;

    Q.push(q0);
    NR=n;
}

*/
void afisare()
{
    g<<endl;
    for(int cnt=0; cnt<m; cnt++)
    {
        g<<alf[cnt]<<endl;
        for(int i=0; i<n; i++)
            if(inchidere[cnt][i][0]!=-1)
            {
                g<<i<<" {";
                for(int j=1; j<inchidere[cnt][i][0]; j++)
                    g<<inchidere[cnt][i][j]<<",";
                if(inchidere[cnt][i][0]!=0)
                    g<<inchidere[cnt][i][inchidere[cnt][i][0]];
                g<<"} "<<endl;
            }
    }

}
int main()
{
    citire();
    lambdanfa_to_nfa();
    ///nfa_to_dfa();
    afisare();
    return 0;
}
