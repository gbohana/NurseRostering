#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<cmath>
using namespace std;
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

// for((i = 1; i<11; i++)); do ./a.out 40 $i; done
// ./a.out
pair<double,double> makeCoordenada(){
    pair <double,double> coordenada;
    double r = rand()%31;
    double theta = -M_PI + ((double)rand() / (RAND_MAX + 1.0))*(2.0*M_PI);
    double x = r*cos(theta);
    double y = r*sin(theta);
    coordenada = make_pair(x,y);
    return coordenada;
}

int distancia(pair<int,int> a, pair<int,int> b){
    return sqrt(pow(a.first - b.first,2.0) + pow(a.second - b.second,2.0));
}

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

int main(int argc, char* argv[]){

    srand(time(0));

    if(argc < 3){
        cout<<"Informe:\n - numero de veiculos [3, 5 ou 10];\n - numero de pacientes [10,20,30];"<<endl;
        cout<<" - densidade de serviço [0-10];\n - numero de sequencia [0-29]"<<endl;
        cout<<"Exemplo: ./a.out 3 10 0 1"<<endl;
        return 0;
    }

    string argv1(argv[1]);
    string argv2(argv[2]);
    string argv3(argv[3]);
    string argv4(argv[4]);

    string output_filename("Instanz_HCSRP_"+argv1+"_"+argv2+"_"+argv3+"_"+argv4+".dzn");
    ofstream saida(output_filename.c_str(),ios::out);
    int nbNodes = atoi(argv[2]) + 2;
    int nbVehi = atoi(argv[1]);
    int nbServi = 5;
    int sequencia = atoi(argv[3]);
    /*
    switch (atoi(argv[1]))
    {
        case 10:
             nbNodes = 12;
             nbVehi = 3;
            break;
        case 20:
            nbNodes=22;
            nbVehi=5;
            break;
        case 30:
             nbNodes = 32;
             nbVehi = 10;
            break;
        default:
            break;
    }*/

    saida<<"nbNodes = "<<nbNodes<<";"<<endl;
    saida<<"nbVehi = "<<nbVehi<<";"<<endl;
    saida<<"nbServi = "<<nbServi<<";"<<endl;

    int e[nbServi][nbNodes];
    int l[nbServi][nbNodes];

    for(int i = 0; i < nbServi; i++)
        for (int j = 0; j < nbNodes; j++)
        {
            if(j == 0 || j == nbNodes-1){
                e[i][j] = 0;
                l[i][j] = 480;
            }else{
            e[i][j] = rand()%481;
            l[i][j] = e[i][j]+30+rand()%31;
            }
        }

    // Inicio da impressao do e
    saida<<"\ne = \n [|";
    for(int i = 0; i < nbServi; i++)
    {
        for (int j = 0; j < nbNodes; j++)
        {
            saida<<setw(5)<< e[i][j]<<",";
        }
        saida<<endl<<" |";

    }
    saida<< "];\n"<<endl;
    // Fim da impressao do e

    // Inicio da impressao do l
    saida<<"l = \n [|";
    for(int i = 0; i < nbServi; i++)
    {
        for (int j = 0; j < nbNodes; j++)
            saida<<setw(4)<< l[i][j]<<",";
        saida<<endl<<" |";
    }
    saida<< "];\n";
    // Fim da impressao do l

    int a[nbVehi][nbServi];
        int minimo = min(nbVehi,nbServi);
        //Garantir que tenha pelo menos uma enfermeira para cada serviço
        for(int i = 0; i < nbVehi; i++)
        {
            for(int j = 0; j < nbServi; j++)
            {
                a[i][j] = 0;
                if(i%minimo == j%minimo)
                    a[i][j] = 1;
                else
                {
                    if(rand()%100 < sequencia*10)
                        a[i][j] = 1;
                }
            }
        }
        // Inicio da impressao da tabela a
        saida<<"\na = \n [|";
        for(int i = 0; i < nbVehi; i++){
            for(int j = 0; j < nbServi; j++)
                saida<<setw(3)<< a[i][j]<<",";
            saida<<endl<<" |";
        }
        saida<< "];\n";

    // }

    int p[nbNodes][nbVehi][nbServi];
    for(int i = 0; i < nbNodes; i++)
    {
        for(int j = 0; j < nbVehi; j++)
        {
            for(int k = 0; k < nbServi; k++)
            {
                if( i == 0 || i == nbNodes-1){
                    p[i][j][k] = 0;
                }
                else
                {
                    //Calcular a menor janela de tempo - minTimeWindow
                    int minTimeWindow = 480;
                    for(int serv = 0; serv < nbServi; serv++){
                        minTimeWindow = min(minTimeWindow,l[serv][i]-e[serv][i]);
                    }
                    //p[i][j][k] = min(10+rand()%(int(double(minTimeWindow)*((11-sequencia)/10.0))),minTimeWindow);
                    p[i][j][k] = min(5+rand()%25,minTimeWindow);
                
                }

            }
        }
    }


    saida<<"\np = array3d(1..nbNodes,1..nbVehi,1..nbServi,\n[";
    for(int i = 0; i < nbNodes; i++)
    {
        for(int j = 0; j < nbVehi; j++)
        {
            for(int k = 0; k < nbServi; k++)
            {
                saida<<setw(3)<<p[i][j][k]<<",";
            }
            saida<<"\t";
        }
        saida<<endl;
    }
    saida<< "]);\n";


	/***********************     paciente - dia - servico  **************************/

	string rs[nbNodes][nbServi];
	int minimoPaciente = min(5, nbServi);

	for (int i = 0; i < nbNodes; i++)
	{
		for (int k = 0; k < nbServi; k++)
			{
				rs[i][k] = "false";

				if (i == 0 || i == nbNodes - 1) {
					rs[i][k] = "true";
				}
		}
	}

    for (int i = 0; i < nbNodes; i++)
		for (int j = 0; j < 3; j++){
            rs[i][rand()%nbServi] = "true";
            break;
        }

	saida << "\nrs = \n[|";
	for (int i = 0; i < nbNodes; i++)
	{
		    for (int k = 0; k < nbServi; k++)
			{
				saida << setw(3) << rs[i][k] << ",";
			}
			saida<<endl<<" |";
    }
	saida<< "];\n"<<endl;

    //DEBUGAR AQUI!!!
    int matriz[nbNodes][nbNodes];
    vector<pair<int,int> > local;
    pair<int,int> origem;
    origem.first = 0;
    origem.second = 0;
    /*for(int i=0; i < nbNodes; i++)
        local.push_back(makeCoordenada());
    	*/
    for(int i=0; i < nbNodes; i++) {
        pair<int, int> loc = makeCoordenada();
	   if(i>0){
	   	for (int j = 0; j < local.size(); j++){
		   	while (distancia(loc,local[j])<=5 && distancia(loc,origem)<=5){
				loc = makeCoordenada();
			}
		 
	   	}
	   }
	 local.push_back(loc);
    }	

    local[0].first = 0;
    local[0].second = 0;
    local[nbNodes-1] = local[0];
  	
    for(int i=0; i < nbNodes; i++){
        for(int j=0; j < nbNodes; j++){
            matriz[i][j] = distancia(local[i],local[j]);
        }
    }

    saida<<"d=[";
    for(int i=0; i < nbNodes; i++){
        saida<<"|";
        for(int j=0; j < nbNodes; j++){
            saida<<int(matriz[i][j])<<", ";
        }
        saida<<endl;
    }
    saida<<"|];"<<endl;

    //coordenadas para a visualização dos resultados
    saida<<"\n%coord=[ ";
    for(int i=0; i < nbNodes; i++){
        saida<<"("<<local[i].first<<",";
        saida<<local[i].second<<"), ";
        //cout<<"("<<local[i].first<<","<<local[i].second<<")"<<endl;
    }
    saida<< "];\n"<<endl;


    //um par de serviços é selecionado para ter precedência
    /*int prec[nbServi][nbServi];
    for(int i=0; i < nbNodes; i++){
        for(int j=0; j < nbNodes; j++){
            prec[i][j] = 0;
        }
    }

    int g = rand()%nbServi;
    int f = rand()%nbServi;
    if (g!=f){
        prec[g][f] = 1;
    } else {
        while (g!=f){
            int g = rand()%nbServi;
            int f = rand()%nbServi;
        }
        prec[g][f] = 1;
    }

    saida<<"\nprec=[";
    for(int i=0; i < nbServi; i++){
        saida<<"|";
        for(int j=0; j < nbServi; j++){
            saida<<prec[i][j]<<", ";
        }
        saida<<endl;
    }
    saida<<"|];"<<endl;
    // */

}
