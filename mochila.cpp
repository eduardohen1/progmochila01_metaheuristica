#include<fstream>
#include<iostream>
#include <dirent.h> //ler diretorio de arquivos
#include <ctime>
#include <iomanip> 
#include <cstdlib>
#include <string.h>
#include <algorithm>

using namespace std;

//estrutura para armazenar as informações dos itens 
struct tipoItem

{
    float peso; //peso do item
    float valor; //valor do item
};

//estrutura para armazenar soluções da mochila
struct tipoSolucao
{
    int n;
    float peso;
    float valor;
    int *item;
    float capacidade;
    float otimo;
};

//zera as variáveis da solução e aloca de forma dinâmica o vetor binário de solução
void criaSolucao(tipoSolucao &s,int n,float capacidade, float otimo)
{
    s.n=n;
    s.peso=0;
    s.valor=0;
    s.capacidade=capacidade;
    s.otimo=otimo;
    s.item= new int[n];
    for(int i=0;i<n;i++)
        s.item[i]=0;
}

//copiar nova solução:
void atribuiSolucao(tipoSolucao &s1, tipoSolucao s2){
    s1.n = s2.n;
    s1.peso = s2.peso;
    s1.valor = s2.valor;
    s1.capacidade = s2.capacidade;
    s1.otimo = s2.otimo;
    for(int i=0;i<s2.n;i++)
        s1.item[i]=s2.item[i];
}

//apaga o vetor binário de solução que foi criado dinamicamente.
void apagaSolucao(tipoSolucao &s)
{
    delete s.item;
}

//função lê o diretório com as instâncias e abre um menu de seleção e retorna a string contendo o nome do arquivo da instância
//e do nome do arquivo com a resposta. 
void selecionaInstancia(char nomeArq1[], char nomeArq2[], char nome[])
{
    DIR *d;
    int op;
    struct dirent *dir;
    
    cout<<"\nTipo de instancia\n================\n[1] Large Scale\n[2] Low Dimensional\n\nDigite uma opcao: ";
    cin>>op;

    switch(op)
    {
        case 1: 
            strcpy(nomeArq1,"instances_01_KP\\large_scale");
            strcpy(nomeArq2,"instances_01_KP\\large_scale-optimum");  
            break;
        case 2:
            strcpy(nomeArq1,"instances_01_KP\\low-dimensional");
            strcpy(nomeArq2,"instances_01_KP\\low-dimensional-optimum"); 
            break;
        default:
            cout<<"Opcao invalida"; 
            exit(0);            
    }

    d = opendir(nomeArq1);
    if (d)
    {
        cout<<"\nInstancias\n=========\n";
        int c=0;
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0)
            {
                c++;
                cout<<"["<<setw(2)<<c<<"] "<<dir->d_name<<endl;
            }
        }
        cout<<"\nDigite uma opcao: ";
        cin>>op;
        if(op<=0 || op >c)
        {
            cout<<"Opcao invalida"; 
            closedir(d);
            exit(0);
        }
        rewinddir(d);

        c=0;
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0)
            {
                c++;
                if(c==op)
                {
                    strcat(nomeArq1,"\\");
                    strcat(nomeArq1,dir->d_name);
                    strcat(nomeArq2,"\\");
                    strcat(nomeArq2,dir->d_name);
                    strcpy(nome,dir->d_name);
                    break;
                }
            }
        }

        closedir(d);
    }
}

//Lê do arquivo os dados das instância 
 void carregaDados(tipoItem **itens, float &capacidade,int &n,float &otimo, char nome[])
{
    char nomeArq1[300],nomeArq2[300];
    ifstream fin;
    
    selecionaInstancia(nomeArq1, nomeArq2,nome);

    cout<<"\nArquivo da instancia escolhida: "<<nomeArq1<<endl;
    cout<<"\nArquivo contendo a solucao otima da instancia: "<<nomeArq2<<endl;

    fin.open(nomeArq2);
    if(!fin.is_open())
    {
        cout<<"Erro ao abrir o arquivo \""<<nomeArq2<<"\""<<endl;
        exit(0);
    }
    fin>>otimo;
    //cout<<"\nValor Otimo: "<<otimo<<endl;      
    fin.close();

    fin.open(nomeArq1);
    if(!fin.is_open())
    {
        cout<<"Erro ao abrir o arquivo \""<<nomeArq1<<"\""<<endl;
        exit(0);
    }

    fin>>n>>capacidade;
    //cout<<"numero de itens: "<<n<<" capacidade: "<<capacidade<<endl;
    *itens = new tipoItem[n];
    for(int i=0;i<n;i++)
    {
        fin>>(*itens)[i].valor>>(*itens)[i].peso;
       //cout<<"item "<<i+1<<" -- valor: "<<(*itens)[i].valor<<" peso: "<<(*itens)[i].peso<< endl;
    }
    fin.close();
 
}

//imprime o vetor com os itens
void imprimeItens(tipoItem itens[], int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<"Item "<<setw(4)<<i+1;
        cout<<"   |  Peso: "<<setw(5) <<itens[i].peso;
        cout<<"   |  Valor: "<<setw(5) <<itens[i].valor<<endl;
    }
    cout<<endl;
}

//imprime os dados de uma solução
void imprimeSolucao( tipoSolucao s,tipoItem itens[] )
{
    cout<<" Solucao\n";
    cout<<"=======================================\n";
    cout<<"Valor Otimo:"<<s.otimo<<endl;
    cout<<"Capacidade Maxima: "<<s.capacidade<<endl;
    cout<<"=======================================\n";
    cout<<"Valor Atual: "<<s.valor<<endl;
    cout<<"Peso Atual: "<<s.peso<<endl<<endl;

   // cout<<"Escolhidos:\n";
    int cont=0;
    for(int i=0;i<s.n;i++)
    {
        if(s.item[i]==1)
        {
            cont++;
            //cout<<"Item "<<setw(4)<<i+1;
            //cout<<"   |  Peso: "<<setw(5) <<itens[i].peso;
            //cout<<"   |  Valor: "<<setw(5) <<itens[i].valor<<endl;
        }
    }
    
    cout<<"Itens escolhidos: "<<cont<<endl<<endl;    
    cout<<endl;
}

//cria uma solução aleatória
void solucaoInicial(tipoSolucao &s,tipoItem itens[])
{
    int *marca=new int[s.n];
    int i,cont=0;
    for(i=0;i<s.n;i++)
    {
        marca[i]=0;
    }

    while(s.peso<s.capacidade && cont<s.n)
    {
        i=rand()%s.n;
        while(marca[i]==1)
            i=(i+1)%s.n;
                   
        if(s.peso+itens[i].peso<=s.capacidade)
        {
            s.item[i]=1;
            s.peso=s.peso+ itens[i].peso;
            s.valor=s.valor+itens[i].valor;
        }
        marca[i]=1;
        cont++;        
    }
        
    delete marca;
        
}

//avalia a vizinhança de uma solução e retorna true se encontrou um vizinho melhor.
bool avaliaVizinhanca(tipoSolucao &s, tipoItem itens[], int d)
{
    int i,j, pos1=-1, pos2=-1, posd1=-1;
    float mValor=s.valor, mPeso=s.peso;
    float peso_aux, valor_aux,peso_aux2, valor_aux2;
    bool melhorou=false;
    
    switch(d)
    {
        case 1:
            for(i=0;i<s.n;i++)
            {
               if(s.item[i]==0)
               {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor;         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor;
                }
                if(peso_aux<=s.capacidade && valor_aux>mValor)
                //if(peso_aux<=s.capacidade && valor_aux-mValor>0.01)
                {
                    pos1=i;
                    mValor=valor_aux;
                    mPeso=peso_aux;
                }
            }
            if(pos1!=-1)
            {
                s.item[pos1]= (s.item[pos1]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            return false;
        break;

       case 2:
           //d=1
            for(i=0;i<s.n;i++)
            {
               if(s.item[i]==0)
               {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor;         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor;
                }
                if(peso_aux<=s.capacidade && valor_aux>mValor)
                {
                    posd1=i;
                    mValor=valor_aux;
                    mPeso=peso_aux;

                }
            }
            if(posd1!=-1)
            {
                melhorou=true;
            }

            //d=2
            for(i=0;i<s.n-1;i++)
            {
                if(s.item[i]==0)
                {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor;         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor;
                }
                            
                for(j=i+1;j<s.n;j++)
                {
                
                    if(s.item[j]==0)
                    {   
                        peso_aux2=peso_aux+itens[j].peso;
                        valor_aux2=valor_aux+itens[j].valor;         
                    }
                    else
                    {
                        peso_aux2=peso_aux-itens[j].peso;
                        valor_aux2=valor_aux-itens[j].valor;
                    }

                    if(peso_aux2<=s.capacidade && valor_aux2>mValor)
                    {
                        pos1=i;
                        pos2=j;
                        mValor=valor_aux2;
                        mPeso=peso_aux2;
                    }
                }

            } 
            if(pos1!=-1)
            {
                s.item[pos1]=(s.item[pos1]+1)%2;
                s.item[pos2]=(s.item[pos2]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            if(melhorou)
            {
                s.item[posd1]= (s.item[posd1]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            return false;
        break;
  
    }
    return false;
}


void buscaLocal(tipoSolucao &s, tipoItem itens[], char nome[])
{
    
    bool teste=true;
    int d=2;//distancia
    int i=0;
    
    ofstream fout;
    char nomearq[100];

    /*-------------------Usado para criar o arquivo de saída--------------------------*/
    strcpy(nomearq,nome);
    strcat(nomearq,"_saida.txt");
    fout.open(nomearq);
    if(!fout.is_open())
    {
        cout<<"Erro ao criar o arquivo saida.txt"<<endl;
        exit(0);
    }
    fout<<nome<<"\n";
    fout<<s.otimo<<"\n";
    /*--------------------------------------------------------------------------------*/

    solucaoInicial(s,itens);
    imprimeSolucao(s,itens);

    fout<<i<<" "<<s.valor<<"\n"; //gravando a solução inicial
   
    while(teste)
    {
        teste=avaliaVizinhanca(s,itens,d);
        if(teste)
        {
            i++;
            imprimeSolucao(s,itens);
            fout<<i<<" "<<s.valor<<"\n"; //gravando a solução atual
        }
        
    }
    fout.close(); //fechar o arquivo de saída
}

void buscaLocalVND(tipoSolucao &s, tipoItem itens[], char nome[])
{
    
    bool teste=true;
    int d=2;//distancia
    int i=0;
    
    ofstream fout;
    char nomearq[100];

    /*-------------------Usado para criar o arquivo de saída--------------------------*/
    strcpy(nomearq,nome);
    strcat(nomearq,"_saida.txt");
    fout.open(nomearq);
    if(!fout.is_open())
    {
        cout<<"Erro ao criar o arquivo saida.txt"<<endl;
        exit(0);
    }
    fout<<nome<<"\n";
    fout<<s.otimo<<"\n";
    /*--------------------------------------------------------------------------------*/

    /*solucaoInicial(s,itens);*/
    imprimeSolucao(s,itens);

    fout<<i<<" "<<s.valor<<"\n"; //gravando a solução inicial
   
    while(teste)
    {
        teste=avaliaVizinhanca(s,itens,d);
        if(teste)
        {
            i++;
            imprimeSolucao(s,itens);
            fout<<i<<" "<<s.valor<<"\n"; //gravando a solução atual
            d = 1;
        }else{
            d++;
        }
        
    }
    fout.close(); //fechar o arquivo de saída
}

void buscaNovaSolucaoInicial(tipoSolucao s, tipoSolucao &ss, int d, tipoItem itens[]){
    
    int newN = s.n + d;
    int *marca = new int[newN];
    int i, count, numRam = 0;
    for(i=0;i<newN;i++){
        marca[i] = 0;
    }
    while(ss.peso<ss.capacidade && count < newN){
        i=rand()%newN;
        while(marca[i]==1)
            i=(i+1)%newN;
                   
        if(ss.peso+itens[i].peso<=ss.capacidade)
        {
            ss.item[i]=1;
            ss.peso=ss.peso+ itens[i].peso;
            ss.valor=ss.valor+itens[i].valor;
        }
        marca[i]=1;
        count++; 
    }
    delete marca;
}

void buscaVNS(tipoSolucao &s, tipoItem itens[], char nome[]){
    int d = 2;
    int i = 0;
    int vnsOtima = 0;
    bool teste = true;
    tipoSolucao ss;

    solucaoInicial(s,itens);
    buscaLocal(s, itens, nome);

    buscaNovaSolucaoInicial(s, ss, d, itens);
    
    vnsOtima = 0;
    for(i=0; i < d; i++){
        buscaLocalVND(ss, itens, nome);
        if(vnsOtima < ss.valor){
            vnsOtima = ss.valor;
            i = 0;
        }        
    }

}

bool avaliaVizinhancaTabu(tipoSolucao &s, tipoItem itens[], int d, int *tabu, int iteracao, float melhorValor)
{
    int i,j, pos1=-1, pos2=-1, posd1=-1;
    //float mValor=s.valor, mPeso=s.peso;
    float mValor=0, mPeso=0;
    float peso_aux, valor_aux,peso_aux2, valor_aux2;
    bool melhorou=false;
    
    switch(d)
    {
        case 1:
            for(i=0;i<s.n;i++)
            {
               if(s.item[i]==0)
               {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor;         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor;
                }
                if(peso_aux<=s.capacidade 
                        && valor_aux>mValor 
                        && (tabu[i]<iteracao || valor_aux > melhorValor)) //aspiração e tabu
                //if(peso_aux<=s.capacidade && valor_aux-mValor>0.01)
                {
                    pos1=i;
                    mValor=valor_aux;
                    mPeso=peso_aux;
                }
            }
            if(pos1!=-1)
            {
                s.item[pos1]= (s.item[pos1]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;

                tabu[pos1] = iteracao + 7; //tempo tabu

                return true;
            }
            return false; //implementar aspiração default
        break;
       /*
       case 2:
           //d=1
            for(i=0;i<s.n;i++)
            {
               if(s.item[i]==0)
               {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor;         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor;
                }
                if(peso_aux<=s.capacidade && valor_aux>mValor)
                {
                    posd1=i;
                    mValor=valor_aux;
                    mPeso=peso_aux;

                }
            }
            if(posd1!=-1)
            {
                melhorou=true;
            }

            //d=2
            for(i=0;i<s.n-1;i++)
            {
                if(s.item[i]==0)
                {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor;         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor;
                }
                            
                for(j=i+1;j<s.n;j++)
                {
                
                    if(s.item[j]==0)
                    {   
                        peso_aux2=peso_aux+itens[j].peso;
                        valor_aux2=valor_aux+itens[j].valor;         
                    }
                    else
                    {
                        peso_aux2=peso_aux-itens[j].peso;
                        valor_aux2=valor_aux-itens[j].valor;
                    }

                    if(peso_aux2<=s.capacidade && valor_aux2>mValor)
                    {
                        pos1=i;
                        pos2=j;
                        mValor=valor_aux2;
                        mPeso=peso_aux2;
                    }
                }

            } 
            if(pos1!=-1)
            {
                s.item[pos1]=(s.item[pos1]+1)%2;
                s.item[pos2]=(s.item[pos2]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            if(melhorou)
            {
                s.item[posd1]= (s.item[posd1]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            return false;
        break;
        */
    }
    return false;
}

void buscaTabu(tipoSolucao &s, tipoItem itens[], char nome[])
{    
    bool teste=true;
    int d=1;//distancia
    int i=0;
    int *tabu = new int[s.n];
    tipoSolucao sMelhorSolucao;
    
    //zerando lista tabu
    for(int j = 0; j<s.n;j++){
        tabu[j] = 0;
    }
    criaSolucao(sMelhorSolucao, s.n, s.capacidade,s.otimo);

    ofstream fout;
    char nomearq[100];

    /*-------------------Usado para criar o arquivo de saída--------------------------*/
    strcpy(nomearq,nome);
    strcat(nomearq,"_saida.txt");
    fout.open(nomearq);
    if(!fout.is_open())
    {
        cout<<"Erro ao criar o arquivo saida.txt"<<endl;
        exit(0);
    }
    fout<<nome<<"\n";
    fout<<s.otimo<<"\n";
    /*--------------------------------------------------------------------------------*/

    solucaoInicial(s,itens);
    imprimeSolucao(s,itens);

    atribuiSolucao(sMelhorSolucao, s);

    fout<<i<<" "<<s.valor<<"\n"; //gravando a solução inicial
   
    while(i<300 && teste == true) //numero de iterações
    {
        i++;
        teste=avaliaVizinhancaTabu(s,itens,d, tabu, i, sMelhorSolucao.valor);
        if(teste)
        {
            i++;
            imprimeSolucao(s,itens);
            fout<<i<<" "<<s.valor<<"\n"; //gravando a solução atual
            if(s.valor > sMelhorSolucao.valor)
                atribuiSolucao(sMelhorSolucao, s);
        }
        
    }
    fout.close(); //fechar o arquivo de saída
    atribuiSolucao(s, sMelhorSolucao);
}

//-- aula dia 20/04/2021
bool avaliaVizinhancaPenalizada(tipoSolucao &s, tipoItem itens[], int d, float lambda, int p[], int k)
{
    int i,j, pos1=-1, pos2=-1, posd1=-1;
    float mValor=s.valor, mPeso=s.peso;
    float peso_aux, valor_aux,peso_aux2, valor_aux2;
    bool melhorou=false;
    
    float pen = 0;
    float pen_aux = 0;
    /*
    for (i=0; i<s.n;i++){
        pen = pen+p[i]*s.item[i];
    }
    */
    s.valor = s.valor - (lambda*p[k]);
    mValor = s.valor;

    switch(d)
    {
        case 1:
            for(i=0;i<s.n;i++)
            {
               if(s.item[i]==0)
               {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor - (lambda *p[i]);
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor + (lambda *p[i]);
                }
                if(peso_aux<=s.capacidade && valor_aux>mValor)
                //if(peso_aux<=s.capacidade && valor_aux-mValor>0.01)
                {
                    pos1=i;
                    mValor=valor_aux;
                    mPeso=peso_aux;
                }
            }
            if(pos1!=-1)
            {
                s.item[pos1]= (s.item[pos1]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            return false;
        break;

       case 2:
           //d=1
            for(i=0;i<s.n;i++)
            {
               if(s.item[i]==0)
               {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor - (lambda *p[i]);         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor + (lambda *p[i]);
                }
                if(peso_aux<=s.capacidade && valor_aux>mValor)
                {
                    posd1=i;
                    mValor=valor_aux;
                    mPeso=peso_aux;
                }
            }
            if(posd1!=-1)
            {
                melhorou=true;
            }

            //d=2
            for(i=0;i<s.n-1;i++)
            {
                if(s.item[i]==0)
                {
                    peso_aux=s.peso+itens[i].peso;
                    valor_aux=s.valor+itens[i].valor - (lambda *p[i]);         
                }
                else
                {
                    peso_aux=s.peso-itens[i].peso;
                    valor_aux=s.valor-itens[i].valor + (lambda *p[i]);
                }
                            
                for(j=i+1;j<s.n;j++)
                {
                
                    if(s.item[j]==0)
                    {   
                        peso_aux2=peso_aux+itens[j].peso;
                        valor_aux2=valor_aux+itens[j].valor - (lambda *p[j]);         
                    }
                    else
                    {
                        peso_aux2=peso_aux-itens[j].peso;
                        valor_aux2=valor_aux-itens[j].valor + (lambda *p[j]);
                    }

                    if(peso_aux2<=s.capacidade && valor_aux2>mValor)
                    {
                        pos1=i;
                        pos2=j;
                        mValor=valor_aux2;
                        mPeso=peso_aux2;
                    }
                }

            } 
            if(pos1!=-1)
            {
                s.item[pos1]=(s.item[pos1]+1)%2;
                s.item[pos2]=(s.item[pos2]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            if(melhorou)
            {
                s.item[posd1]= (s.item[posd1]+1)%2;
                s.valor=mValor;
                s.peso=mPeso;
                return true;
            }
            return false;
        break;
  
    }
    return false;
}

void buscaLocalPenalizada(tipoSolucao &s, tipoItem itens[], char nome[], float lambda, int p[], int k)
{
    
    bool teste=true;
    int d=2;//distancia
    int i=0;
    
    ofstream fout;
    char nomearq[100];

    while(teste)
    {
        teste=avaliaVizinhancaPenalizada(s,itens,d, lambda, p, k);
        if(teste)
            i++;
    }    
}

//função para retornar a maior utilidade
int maxPos(float u[], int n){
    float max=u[0];
    int p = 0;

    for(int i=1;i<n;i++){
        if(u[i]>max){
            max = u[i];
            p=i;
        }
    }
    return p;
}

void buscaLocalGuiada(tipoSolucao &s, tipoItem itens[], char nome[])
{    
    bool teste=true;
    int d=2;//distancia
    int i=0;
    int j=0;
    float *u = new float[s.n];
    float *c = new float[s.n];
    int *p = new int[s.n];
    float lambda;
    float aux;
    
    //zerando o p
    for(i=0;i<s.n;i++){
        p[i] = 0;
        c[i] = itens[i].valor/itens[i].peso;
    }

    ofstream fout;
    char nomearq[100];

    /*-------------------Usado para criar o arquivo de saída--------------------------*/
    strcpy(nomearq,nome);
    strcat(nomearq,"_saida.txt");
    fout.open(nomearq);
    if(!fout.is_open())
    {
        cout<<"Erro ao criar o arquivo saida.txt"<<endl;
        exit(0);
    }
    fout<<nome<<"\n";
    fout<<s.otimo<<"\n";
    /*--------------------------------------------------------------------------------*/

    solucaoInicial(s,itens);
    imprimeSolucao(s,itens);
    
    lambda = s.valor/2; //definir o lambda com metade do valor

    i = 0;
    fout<<i<<" "<<s.valor<<"\n"; //gravando a solução inicial
    j = 0;    
    i = 1;
    //100 iterações
    while(i<=30)
    {
        buscaLocalPenalizada(s, itens, nome, lambda, p, j);
        imprimeSolucao(s,itens);
        aux = 0;
        for(int k=0;k<s.n;k++)
            aux = aux + s.item[k]*itens[k].valor;
        fout<<i<<" "<<aux<<"\n"; //gravando a solução sem penalização
        for(j=0;j<s.n;j++)
            u[j] = s.item[j]*(c[j]/(1+p[j]));
        
        j=maxPos(u,s.n);
        p[j]=p[j]+1;
        i++;
    }
    fout.close(); //fechar o arquivo de saída
}

//--------------------

int main()
{
    tipoItem *itens; //itens
    float otimo;
    float capacidade;
    int n;
    tipoSolucao sOtima;
    char nome[100];
    
    //criando a semente dos números aleatório
    unsigned seed = time(NULL);
    srand(seed);
    
    //escolhe e carrega os dados das instâncias
    carregaDados(&itens,capacidade,n,otimo,nome);
    //imprimeItens(itens,n);
    
    //cria a solução (zera e aloca o vetor binário)
    criaSolucao(sOtima,n,capacidade,otimo);
    
    //função de otimização
    //buscaLocal(sOtima, itens,nome);

    //função busca VNS:
    //buscaVNS(sOtima, itens,nome);

    //buscaTabu(sOtima,itens,nome);

    buscaLocalGuiada(sOtima, itens, nome);
    
    //melhor solução
    cout<<"\n\n Melhor Solucao:";
    imprimeSolucao(sOtima,itens);

    cout<<"Arquivo de saida criado: "<<nome<<"_saida.txt";
    
    //apaga o vetor binário criado de forma dinâmica
    apagaSolucao(sOtima);
    
    //apaga o vetor de itens;
    delete itens;
    return 0;
}