#include <iostream.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream.h>
#include <TFile.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH1.h>
#include <time.h>
#include <unistd.h> 
#include <iomanip>


int qgainalltohist(double min = 0, double max=1000, int chperbin=1, int salvar=0, double thr = 0){
	float vol;
	char temp[100];
	double voltagePMT, ledmVol, ledpVol;
	char* histName = new char[100];
	std::cout << "threshold: " << thr << endl;
	int bin = (max-min)/chperbin;
	int chx;
	sprintf(histName,"gain_all_channels");
	TCanvas *can = new TCanvas(histName,"Canvas",2,10,2600,700);
	can->Divide(6,3);
	for(chx=0;chx<17;chx++)
	{
	//	sprintf(histName,"gain_ch%d", chx);
	//	TCanvas *can = new TCanvas(histName,"Canvas",200,10,400,350);
		can->cd(chx+1);
		sprintf(histName,"MPPC station 2 gain ch%d", chx);
		TH1F *hist = new TH1F("histo",histName,bin,min,max);
		ifstream file;
		char* buffer = new char[100];
		sprintf(buffer,"/home/utfsm/rimsky/acquisition/testgain_minNimpulse/prueba_gain_ch%02d.dat",chx);
		std::cout << buffer << endl;
		file.open(buffer,ios::in);
		if(!file.is_open()){ std::cerr << "ERROR: no se pudo abrir el archivo de lectura";
					return 0;}
		file.getline(temp,200); //basura
		while(file.good()){
			file.read(temp,chx*sizeof(short));
			file >> vol;	
			file.getline(temp,200);	
			if(vol>thr)
				hist->Fill(vol);	
			}
	
		file.close();
		
	
		hist->Draw();
		
//		std::cout << "hist saved as image"<<(int)getch();
		if(salvar){
			sprintf(histName,"./acquisition/testgain_minNimpulse/test_gain_ch%02d.png", chx);
/*	
			std::cout << "enter PMTvoltage LED - voltage LED + voltage" << std::endl;
			scanf("%d %d %d",&voltagePMT, &ledmVol, &ledpVol);
			std::cin >> ledmVol >> ledpVol >> voltagePMT;
			TPaveText *pt = new TPaveText(0.78,0.5,0.98,0.81, "NDC"); // NDC sets coords
        		pt->SetLabel("obs");                                      			// relative to pad dimensions
			pt->SetFillColor(0); // text is black on white
			pt->SetTextSize(0.04); 
			pt->SetTextAlign(12);
			sprintf(buffer,"PMT voltage: %.0f",voltagePMT);
			pt->AddText(buffer);
			sprintf(buffer,"LED - voltage: %.1f",ledmVol);
			pt->AddText(buffer);
			sprintf(buffer,"LED + voltage: %.1f", ledpVol);
			pt->AddText(buffer);
			pt->Draw(); 
		      //to draw your text object
*/	
		can->SaveAs(histName);
			std::cout << "hist saved as image in "<< histName << std::endl;
		}
		else 	{std::cout << "any file was saved "<< histName << std::endl;}
	
		//delete buffer;
	}	
	return 0;
}	

int testhist(int chx, int test, double min = 0, double max=1000,int chperbin=1, int salvar=0, double thr = 0){
	float vol;
	char temp[100];
	double voltagePMT, ledmVol, ledpVol;
	char* histName = new char[100];
	std::cout << "threshold: " << thr << endl;
	double mean, rms;
	int bin = (max-min)/chperbin;
	sprintf(histName,"can_ch%d_test%02d", chx,test);
	TCanvas *can = new TCanvas(histName,"Canvas",650,400,700,350);
	sprintf(histName,"MPPC station 2 ch%d test%02d", chx,test);
	TH1F *hist = new TH1F("histo",histName,bin,min,max);
	ifstream file;
	char* buffer = new char[100];
	sprintf(buffer,"/home/utfsm/rimsky/acquisition/prueba_test%02d.dat",test);
	hist->SetXTitle("Canales");
	hist->SetYTitle("Cuentas");
	std::cout << buffer << endl;
	file.open(buffer,ios::in);
	if(!file.is_open()){ std::cerr << "ERROR: no se pudo abrir el archivo de lectura";
				return 0;}
	file.getline(temp,200); //basura
	while(file.good()){
		file.read(temp,chx*sizeof(short));
		file >> vol;	
		file.getline(temp,200);	
	//	if(vol>thr)
	//		hist->Fill(vol);	
		}
	mean = hist->GetMean();
	rms = hist->GetRMS();
	std::cout << "mean is: " << mean << "\nRMS is: " << rms << std::endl;
	file.close();
	

	hist->Draw();
	
//	std::cout << "hist saved as image"<<(int)getch();
	if(salvar){
		sprintf(histName,"./acquisition/testgain_minNimpulse/hist_ch%02d_test%02d.png", chx, test);

		can->SaveAs(histName);
		std::cout << "hist saved as image in "<< histName << std::endl;

	}
	else 	{std::cout << "any file was saved "<< histName << std::endl;}

	//delete buffer;
	return 0;
}




int testallhist(int test, double min = 0, double max=1000, int chperbin=1, int salvar=0, double thr = 0){
	float vol;
	char temp[100];
//	double voltagePMT, ledmVol, ledpVol;
	double mean[17], rms[17];
	char* histName = new char[100];
	char* histTitle = new char[100];
	std::cout << "threshold: " << thr << endl;
	int bin = (max-min)/chperbin;	
	int j;
	sprintf(histName,"can_test%02d", test);
	TCanvas *can = new TCanvas(histName,"Canvas",2,10,1360,700);
	can->Divide(6,3);
	ifstream file;
	char* buffer = new char[150];
	sprintf(buffer,"/home/utfsm/rimsky/acquisition/prueba_test%02d.dat",test);
	std::cout << buffer << endl;
	file.open(buffer,ios::in);
	if(!file.is_open()){ std::cerr << "ERROR: no se pudo abrir el archivo de lectura";
		return 0;}
	file.getline(temp,200); //basura
	std::cout << "archivo listeilor\n";
	TH1F *hist[18];

	char* rootFileName = new char[100];
	sprintf(rootFileName,"test%02d.root",test);
	TFile *rootFile = new TFile(rootFileName,"RECREATE");
	TPaveText *pt[18]; 
	for(j=0;j<17;j++)
	{	
		sprintf(histName,"histoTest%d_ch%d",test, j);
		sprintf(histTitle,"MPPC station 2 ch%d", j);
		hist[j] = new TH1F(histName,histTitle,bin,min,max);
		pt[j] = new TPaveText(0.65,0.75,0.99,0.99, "NDC");	// NDC sets coords
	
	}
	while(file.good()){
		for(j=0;j<17;j++)
		{
			file >> vol;	
			//file.getline(temp,200);	
			if(vol>thr)
				hist[j]->Fill(vol);	
		}
	}
	file.close();
	ofstream fileout;
	sprintf(buffer,"/home/utfsm/rimsky/acquisition/prueba_test%02dinfo.dat",test);
	fileout.open(buffer,ios::out);
	std::cout<<buffer;
	if(!fileout.is_open()){ std::cerr << "ERROR: no se pudo abrir el archivo de escritura";
		return 0;}
	
       

	for(j=0;j<17;j++)
	{	
		pt[j]->SetLabel("histo");                                      			// relative to pad dimensions
		pt[j]->SetFillColor(0); // text is black on white
		pt[j]->SetTextSize(0.05); 
		pt[j]->SetTextAlign(12);
			
		mean[j] = hist[j]->GetMean();
		rms[j] = hist[j]->GetRMS();

		sprintf(buffer,"mean = %02.2f",mean[j]);
		pt[j]->AddText(buffer);
		sprintf(buffer,"rms = %02.2f",rms[j]);
		pt[j]->AddText(buffer);
	//	fileout << setprecision (4) << mean << "\t" << setprecision (3) << rms << std::endl;
		fileout << mean[j] << "\t" << rms[j] << std::endl;
		can->cd(j+1);
		hist[j]->Draw();
		pt[j]->Draw();
                hist[j]->Write();	
		
	}
	can->cd(18);
	pt[18] = new TPaveText(0.15,0.35,0.99,0.97, "NDC");	// NDC sets coords
	pt[18]->SetLabel("INFO");                                      			// relative to pad dimensions
	pt[18]->SetFillColor(0); // text is black on white
	pt[18]->SetTextSize(0.05); 
	pt[18]->SetTextAlign(12);
	sprintf(buffer,"MPPC Nº 39 right position");
	pt[18]->AddText(buffer);
	sprintf(buffer,"events: 10000");
	pt[18]->AddText(buffer);
	sprintf(buffer,"MPPC bias Voltage = 71.36[V]");
	pt[18]->AddText(buffer);
	sprintf(buffer,"PMT Voltage = 1000[V]");
	pt[18]->AddText(buffer);
	sprintf(buffer,"Pulse generator: 1kHz width:10ns");
	pt[18]->AddText(buffer);
	sprintf(buffer,"LED Voltage: 3.22[V]");
	pt[18]->AddText(buffer);
	pt[18]->Draw(); 
	
	if(salvar){
		sprintf(histName,"./acquisition/prueba_test%02d.png", test);
		can->SaveAs(histName);
		std::cout << "hist saved as image in "<< histName << std::endl;
		fileout.close();
		rootFile->Close();		
		delete rootFile;
	}
	else 
		{std::cout << "any file was saved " << std::endl;
		fileout.close();
		//rootFile->Close();
		}
	
	
	return 0;
}

double N_photoelectron(double mean,double rms){
	return ((mean-ped)/rms)*((mean-ped)/rms);
}
double square(double num){
	return num*num;}

int Graph_NG_vs_V(int startpoint, int NG=0, int salvar=0){
	double MEAN[17][17];		// MEAN[N_measure][N_ch] se considera el primero como pedestal
	double RMS[17][17];
	double Npe[17][17];
	double Gain[17][17];
	double Volt[17];
	char *RootFilename = new char[100];
	char *Histname = new char[100];
	TFile *RootFile;
	TH1F *hist;
	TCanvas *can = new TCanvas("graficos","Experiment",2,10,1360,700);
	can->Divide(6,3);
	TGraph *gr[2][17]; //gr[x][ch] x=0:Number Phel x=1:Gain
	
	for (int i=0;i<17;i++)	{
		sprintf(RootFilename,"test%d.root",i + startpoint);
		RootFile = new TFile(RootFilename);
		for (int j=0;j<17;j++) {
			
			sprintf(Histname,"histoTest%d_ch%d",i + startpoint,j);
			
			hist = (TH1F*) RootFile->Get(Histname);
			MEAN[i][j] = hist->GetMean();
			RMS[i][j] = hist->GetRMS();
			if(i!=0){
				Npe[j][i] = square(((MEAN[i][j]-MEAN[0][j])/RMS[i][j]));
				Gain[j][i] = square(RMS[i][j])/(MEAN[i][j]-MEAN[0][j]);
			
			}
			delete hist;
		}
		Volt[i] = 71.36 + i*0.1;
		
		RootFile->Close();
		delete RootFile;
	}
	for (int i=0;i<17;i++)	{
		can->cd(i+1);		
		if(NG) {
			sprintf(Histname,"Nº PhEl vs bias Voltage ch%d",i);
			gr[0][i] = new TGraph(16,&Volt[1],&Npe[i][1]);
			gr[0][i]->SetTitle(Histname);
			gr[0][i]->Draw("AL-");
		}else {
			sprintf(Histname,"Gain vs bias Voltage ch%d",i);
			gr[1][i] = new TGraph(17,Volt,Gain[i]);
			gr[1][i]->SetLineColor(kBlue);
			gr[1][i]->SetTitle(Histname);
			gr[1][i]->Draw("AL-");
		}
	
	}
	if(salvar){
			sprintf(Histname,"/home/utfsm/rimsky/prueba_test_Uniformity_graph%d.png",NG+1);
			can->SaveAs(Histname);
			std::cout << "hist saved as image in "<< Histname << std::endl;
	}
	else 	{std::cout << "any file was saved "<< std::endl;}
}

int Graph_G_vs_V{

}

int waitPMT(int howmuch)
{	
	int t1=(int)time(0);
	int t0=(int)time(0);
	while((t1-t0)!=howmuch)
	{	
		//if(time())
		t1=time(0);
		printf("\tPMT is not ready yet, elapsed time is: %0.1d\r",(t1-t0));
		fflush(NULL);
		system("sleep 1s");
	}
	
	printf("\n\nPMT is ready\n");
	return 0;
	
}

int Graph_means(int num, int salvar = 0)
{	
	int cantidad = 0;
	double mean[20], rms[20], par[9];
	char * histName = new char[100];	
	sprintf(histName,"can_hist%02d", num);
	TCanvas *can = new TCanvas(histName,"Experiment",100,10,600,400);
	ifstream inputfile;
	char* buffer = new char[100];
	sprintf(buffer,"/home/utfsm/rimsky/acquisition/prueba_test%02dinfo.dat",num);
	std::cout << "input file: "<< buffer << std::endl;
	inputfile.open(buffer,ios::in);
	if(!inputfile.is_open()){ std::cerr << "ERROR: no se pudo abrir el archivo de lectura";
		return 0;}
	int i = 0;
	double ex[20];
	double xrimsky[20];
	while(inputfile.good() && i<20){
		inputfile >> mean[i] >> rms[i];
		xrimsky[i] = (double)i;
		ex[i] = 0;
		
		std::cout << i <<  "\t" << (int)xrimsky[i] <<  "\t" << mean[i] << "\t" << rms[i] << std::endl;	
		i++;
	}
	
	TGraphErrors *gr1 = new TGraphErrors(i-1,xrimsky,mean,ex,rms);
//	gr1->Draw("LAP");
	gr1->SetMarkerColor(kBlue);
   	gr1->SetMarkerStyle(21);
   	gr1->Draw("LAP");
	TPaveText *pt = new TPaveText(0.70,0.85,0.99,0.96, "NDC");
	pt->SetLabel("INFO");                                      			// relative to pad dimensions
	pt->SetFillColor(0); // text is black on white
	pt->SetTextSize(0.04); 
	pt->SetTextAlign(12);
	sprintf(buffer,"LED Voltage: 3.1[V]");
	pt->AddText(buffer);
	pt->Draw();
//	std::cout << "a crear fitFcn\n";
//	TF1 *fitFcn = new TF1("fitFcn","pol1",0,600,2);
//	std::cout << "a hacer el fit\n";
//	gr->Fit(fitFcn,"R");
	//gr->Fit("pol1");
//	std::cout << "a obtener parametros\n";
//	fitFcn.GetParameters(par);
//	fitFcn->Draw("AL*");
//	std::cout << "\nploteado" << "par1: " << par[0] << "par2: " <<par[1]<< std::endl;
	if(salvar){
			sprintf(histName,"/home/utfsm/rimsky/acquisition/prueba_test%02dgraph.png",num);
			can->SaveAs(histName);
			std::cout << "hist saved as image in "<< histName << std::endl;
		}
		else 	{std::cout << "any file was saved "<< std::endl;}
	return 0;
}


int Graph_file(int num, int salvar = 0)
{	
	int cantidad = 0;
	int i = 0;
	double x[20], y[20],par[9];
	char * histName = new char[100];	
	sprintf(histName,"can_ch%d", num);
	TCanvas *can = new TCanvas("can1","Experiment",200,10,600,400);
	ifstream inputfile;
	char* buffer = new char[100];
	sprintf(buffer,"/home/utfsm/rimsky/Experiment%02d.dat",num);
	std::cout << "input file: "<< buffer << std::endl;
	inputfile.open(buffer,ios::in);
	while(inputfile.good()){
		inputfile >> x[i] >> y[i];
		i++;		
		}
	TGraph *gr = new TGraph(i-1,x,y);
	std::cout << "a crear fitFcn\n";
	TF1 *fitFcn = new TF1("fitFcn","pol1",0,600,2);
	std::cout << "a hacer el fit\n";
	gr->Fit(fitFcn,"R");
	//gr->Fit("pol1");
	std::cout << "a obtener parametros\n";
	fitFcn.GetParameters(par);
	fitFcn->Draw("AL*");
	std::cout << "\nploteado" << "par1: " << par[0] << "par2: " <<par[1]<< std::endl;
	if(salvar){
			sprintf(histName,"./Experiment%02d.png", chx);
			can->SaveAs(histName);
			std::cout << "hist saved as image in "<< histName << std::endl;
		}
		else 	{std::cout << "any file was saved "<< histName << std::endl;}
	return 0;
} 
int main(int argc,char *argv[]){
	if(argc<3){
		std::cout<<"llamado a funcion: ./camactohist numevents archivo_destino"		
		return 1;}

	std::string fileName = argv[2];
	std::ostringstream nombre,command,histo;
	std::cout << argv[3] << " <-Numero de veces que se correra el script" << std::endl;
	double amp[17],desv[17];
	
	
	int max_i = atoi(argv[3]);
	for (int i=1;i<=max_i;i++){
		command<<"./spectra "<<argv[1]<<" "<<fileName<<i<<".dat"<<std::endl;
		std::cout<< "command: " << command.rdbuf()->str() << std::endl;
		std::cout<<"Iteracion numero :"<<i<<std::endl;		
		system(command.rdbuf()->str().c_str());
		command.rdbuf()->str(""); //borrar el buffer
		testhist(0,16+i);
/// histograma
		int test=i;int chx=0;
		float vol;
		char temp[100];
		double voltagePMT, ledmVol, ledpVol;
		char* histName = new char[100];
		std::cout << "threshold: " << thr << endl;
		int bin = (max-min)/chperbin;
		sprintf(histName,"can_ch%d_test%02d", chx,test);
		TCanvas *can = new TCanvas(histName,"Canvas",200,10,700,350);
		sprintf(histName,"MPPC station 2 ch%d test%02d", chx,test);
		TH1F *hist = new TH1F("histo",histName,bin,min,max);
		ifstream file;
		char* buffer = new char[100];
		sprintf(buffer,"/home/utfsm/rimsky/acquisition/prueba_test%02d.dat",test);
		hist->SetXTitle("Canales");
		hist->SetYTitle("Cuentas");
		std::cout << buffer << endl;
		file.open(buffer,ios::in);
		if(!file.is_open()){ std::cerr << "ERROR: no se pudo abrir el archivo de lectura";
					return 0;}
		file.getline(temp,200); //basura
		while(file.good()){
			file.read(temp,chx*sizeof(short));
			file >> vol;	
			file.getline(temp,200);	
			if(vol>thr)
				hist->Fill(vol);	
			}
	

	}

}


