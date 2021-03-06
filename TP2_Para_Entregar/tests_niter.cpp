#include <iostream>
#include <fstream>
//#include <chrono>
#include <vector>
#include "ImagePredictor.h"
#include <random>       // std::default_random_engine
#include <chrono> 

using namespace std;

std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
std::chrono::duration<double, std::milli> elapsed_seconds;



vector<string> levantarArchivosDesdeTest(string testFilesPath){
	ifstream inFile;
	inFile.open(testFilesPath);
	string filePath;
	string previousFilePath = "";
	inFile >> filePath;
	string tag;
	inFile >> tag;

	vector<string> filePaths;

		
	while (filePath != previousFilePath){
		filePaths.push_back(filePath.substr(0, filePath.size()-1));
		previousFilePath = filePath;
		inFile >> filePath;
		inFile >> tag;

	}

	inFile.close();

	return filePaths;
}




vector<double> restaEntreVectores(vector<double> v1, vector<double> v2 ){
        vector<double> res ;
        for(int i = 0 ; i < v1.size(); i ++){
            res.push_back(v1[i] - v2[i]);
        }
        return res;
    }

double distanceBetweenVector(vector<double> v1, vector<double> v2 ){
        std::vector<double> v3 = restaEntreVectores(v1,v2);
        sort(v3.begin(), v3.end());
        
        double ac = 0 ;
        for(int i = 0 ; i < v3.size(); i ++){
            ac += v3[i]*v3[i];
        }
        return ac;
    }


void testNiter(string entrenamientoFilesPath){
    vector<string> filePathsEntrenamiento = levantarArchivosDesdeTest(entrenamientoFilesPath);
    ImagePredictor predictor = ImagePredictor();
    predictor.loadImagesFromFileDataSet(filePathsEntrenamiento);

    vector<vector<double> > matrizImagenesOriginal = predictor.basicImagePixelMatrix;
    int n = matrizImagenesOriginal.size();
    int m = matrizImagenesOriginal[0].size();
    

	//LA TRANSFORMO EN CUADRADA
        int currentAlfa = 100;

        vector<double> vectorEsperanza(m, 0.0);
         for (int i = 0; i < m; i++) {
            vectorEsperanza[i] = 0.0;
            for (int j = 0; j < n; j++) {
                vectorEsperanza[i] += matrizImagenesOriginal[j][i];
            }
            vectorEsperanza[i] = vectorEsperanza[i] / n;
            //En U[i] tengo el promedio del pixel i
        }

        
        //Creo la Matriz X donde X_i = (X_i - u ) / sqrt(n-1)
        vector<vector<double> > matrizRestadoEsperanza(n, vector<double>(m, 0.0));
        for (int i = 0 ;  i < m; i++){ //itero columnas
            for (int j = 0; j < n; j++) { //itero filas
                matrizRestadoEsperanza[j][i] = (matrizImagenesOriginal[j][i] - vectorEsperanza[i]) / sqrt(n-1) ;
            }
        }
        //(X^t)
        vector<vector<double> > matrizRestadoEsperanzaTraspuesta(m, vector<double>(n, 0.0));
        trasponerMatriz(matrizRestadoEsperanza, matrizRestadoEsperanzaTraspuesta);

        //Creo M matriz U de nxn. M = [ (X)*(X^t) ]
        
        vector<vector<double> > matrizU(n, vector<double>(n, 0.0));
        multiplicarMatricesDouble(matrizRestadoEsperanza,matrizRestadoEsperanzaTraspuesta, matrizU );
        
	//LA TRANSFORMO EN CUADRADA


    matrizImagenesOriginal = matrizU;

    vector<vector<double> > matrizAutovalores700(m, vector<double>(100, 0.0));
   	vector<vector<double> > matrizAutovalores500(m, vector<double>(100, 0.0));
    vector<vector<double> > matrizAutovalores300(m, vector<double>(100, 0.0));
    vector<vector<double> > matrizAutovalores100(m, vector<double>(100, 0.0));
    vector<vector<double> > matrizAutovalores50(m, vector<double>(100, 0.0));
    vector<vector<double> > matrizAutovalores25(m, vector<double>(100, 0.0));
    vector<vector<double> > matrizAutovalores10(m, vector<double>(100, 0.0));

    //autovalores.
    vector<double> autovaloresDe700(100, 0.0);
    vector<double> autovaloresDe500(100, 0.0);
    vector<double> autovaloresDe300(100, 0.0);
    vector<double> autovaloresDe100(100, 0.0);
    vector<double> autovaloresDe50(100, 0.0);
    vector<double> autovaloresDe25(100, 0.0);
    vector<double> autovaloresDe10(100, 0.0);
 



    fstream tiemposNiter("tiemposNiter", ios::out);
    tiemposNiter<<"niter,tiempo" << endl;
    
    //calculo niter 700

    vector<vector<double> > matrizImagenesPara700 = matrizImagenesOriginal;
    startTime = std::chrono::system_clock::now();
    predictor.obtenerAutovectoresDeflacion(matrizImagenesPara700, matrizAutovalores700,autovaloresDe700,100, 700);
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime-startTime;
	tiemposNiter <<700 <<","<< elapsed_seconds.count() << endl;




    //primero calculo con los niter 500.

    vector<vector<double> > matrizImagenesPara500 = matrizImagenesOriginal;
    startTime = std::chrono::system_clock::now();
    predictor.obtenerAutovectoresDeflacion(matrizImagenesPara500, matrizAutovalores500,autovaloresDe500,100, 500);
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime-startTime;
	tiemposNiter <<500 <<","<< elapsed_seconds.count() << endl;



    // calculo con los niter 300.
    vector<vector<double> > matrizImagenesPara300 = matrizImagenesOriginal;
    startTime = std::chrono::system_clock::now();
    predictor.obtenerAutovectoresDeflacion(matrizImagenesPara300, matrizAutovalores300,autovaloresDe300,100, 300);
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime-startTime;
	tiemposNiter <<300 <<","<< elapsed_seconds.count() << endl;

    
    //calculo para niter = 100;
    vector<vector<double> > matrizImagenesPara100 = matrizImagenesOriginal;
    startTime = std::chrono::system_clock::now();
    predictor.obtenerAutovectoresDeflacion(matrizImagenesPara100, matrizAutovalores100,autovaloresDe100,100, 100);
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime-startTime;
	tiemposNiter <<100 <<","<< elapsed_seconds.count() << endl;

    //calculo para niter = 50;
    vector<vector<double> > matrizImagenesPara50 = matrizImagenesOriginal;
    startTime = std::chrono::system_clock::now();
    predictor.obtenerAutovectoresDeflacion(matrizImagenesPara50, matrizAutovalores50,autovaloresDe50,100, 50);
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime-startTime;
	tiemposNiter <<50 <<","<< elapsed_seconds.count() << endl;

    //calculo para niter = 25;
    vector<vector<double> > matrizImagenesPara25 = matrizImagenesOriginal;
    startTime = std::chrono::system_clock::now();
    predictor.obtenerAutovectoresDeflacion(matrizImagenesPara25, matrizAutovalores25,autovaloresDe25,100, 25);
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime-startTime;
	tiemposNiter <<25 <<","<< elapsed_seconds.count() << endl;


    //calculo para niter = 10;
    vector<vector<double> > matrizImagenesPara10 = matrizImagenesOriginal;
    startTime = std::chrono::system_clock::now();
    predictor.obtenerAutovectoresDeflacion(matrizImagenesPara10, matrizAutovalores10,autovaloresDe10,100, 10);
    endTime = std::chrono::system_clock::now();
    elapsed_seconds = endTime-startTime;
	tiemposNiter <<10 <<","<< elapsed_seconds.count() << endl;


	

    tiemposNiter.close();



    //Escribo resultados niter 700
    fstream sal700("niter700", ios::out);
    sal700 << "Autovector,Error" << endl;
    for (int i = 0; i < 100; ++i)
    {
        vector<double> autovectorIesimoDe700(n, 0.0);

        for (int j = 0; j < n; ++j)
        {
        	autovectorIesimoDe700[j]  = matrizAutovalores700[j][i];

        }

        vector<vector<double> > matrizImagenesPara700T = matrizImagenesOriginal;
        vector<double> Ax700(n, 0.0);
        multiplicarMatrizVectorDouble(matrizImagenesPara700T,autovectorIesimoDe700, Ax700);

       //Normalizo Ax
        for (int j = 0; j < n; ++j)
        {
        	if(Ax700[j] < 0){Ax700[j]= Ax700[j]*(-1);}
        }
       
       //Lx = 
       vector<double> Lx700(n, 0.0);
       for (int j = 0; j < n; ++j)
       {
       		double lxi = autovectorIesimoDe700[j] * autovaloresDe700[i]; 
       		if(lxi < 0){Lx700[j] = lxi*(-1);}else{Lx700[j] = lxi;} 
       }
       

       double diferencia = distanceBetweenVector(Ax700, Lx700);

       sal700 << i << "," << diferencia << endl;
    }
    sal700.close();





    //Escribo resultados niter 500
    fstream sal500("niter500", ios::out);
    sal500 << "Autovector,Error" << endl;
    for (int i = 0; i < 100; ++i)
    {
        vector<double> autovectorIesimoDe500(n, 0.0);

        for (int j = 0; j < n; ++j)
        {
        	autovectorIesimoDe500[j]  = matrizAutovalores500[j][i];

        }

        vector<vector<double> > matrizImagenesPara500T = matrizImagenesOriginal;
        vector<double> Ax500(n, 0.0);
        multiplicarMatrizVectorDouble(matrizImagenesPara500T,autovectorIesimoDe500, Ax500);

       //Normalizo Ax
        for (int j = 0; j < n; ++j)
        {
        	if(Ax500[j] < 0){Ax500[j]= Ax500[j]*(-1);}
        }
       
       //Lx = 
       vector<double> Lx500(n, 0.0);
       for (int j = 0; j < n; ++j)
       {
       		double lxi = autovectorIesimoDe500[j] * autovaloresDe500[i]; 
       		if(lxi < 0){Lx500[j] = lxi*(-1);}else{Lx500[j] = lxi;} 
       }
       

       double diferencia = distanceBetweenVector(Ax500, Lx500);

       sal500 << i << "," << diferencia << endl;
    }
    sal500.close();




    //Escribo resultados niter 300
    fstream sal300("niter300", ios::out);
    sal300 << "Autovector,Error" << endl;
    for (int i = 0; i < 100; ++i)
    {
        vector<double> autovectorIesimoDe300(n, 0.0);

        for (int j = 0; j < n; ++j)
        {
        	autovectorIesimoDe300[j]  = matrizAutovalores300[j][i];

        }

        vector<vector<double> > matrizImagenesPara300T = matrizImagenesOriginal;
        vector<double> Ax(n, 0.0);
        multiplicarMatrizVectorDouble(matrizImagenesPara300T,autovectorIesimoDe300, Ax);

       //Normalizo Ax
        for (int j = 0; j < n; ++j)
        {
        	if(Ax[j] < 0){Ax[j]= Ax[j]*(-1);}
        }
       
       //Lx = 
       vector<double> Lx(n, 0.0);
       for (int j = 0; j < n; ++j)
       {
       		double lxi = autovectorIesimoDe300[j] * autovaloresDe300[i]; 
       		if(lxi < 0){Lx[j] = lxi*(-1);}else{Lx[j] = lxi;} 
       }
       

       double diferencia = distanceBetweenVector(Ax, Lx);

       sal300 << i << "," << diferencia << endl;
    }
    sal300.close();


    //Escribo resultados niter 100
    fstream sal100("niter100", ios::out);
    sal100 << "Autovector,Error" << endl;
    for (int i = 0; i < 100; ++i)
    {
        vector<double> autovectorIesimoDe100(n, 0.0);

        for (int j = 0; j < n; ++j)
        {
        	autovectorIesimoDe100[j]= matrizAutovalores100[j][i];
        }

        vector<vector<double> > matrizImagenesPara100T = matrizImagenesOriginal;
        vector<double> Ax100(n, 0.0);
        multiplicarMatrizVectorDouble(matrizImagenesPara100T,autovectorIesimoDe100, Ax100);

       //Normalizo Ax
        for (int j = 0; j < n; ++j)
        {
        	if(Ax100[j] < 0){Ax100[j]= Ax100[j]*(-1);}
        }
       
       
       vector<double> Lx100(n, 0.0);
       for (int j = 0; j < n; ++j)
       {
       		double lxi = autovectorIesimoDe100[j] * autovaloresDe100[i]; 
       		if(lxi < 0){Lx100[j] = lxi*(-1);}else{Lx100[j] = lxi;} 
       }
       

       double diferencia = distanceBetweenVector(Ax100, Lx100);
       

       sal100 << i << "," << diferencia << endl;
    }
    sal100.close();

     //Escribo resultados niter 50
    fstream sal50("niter50", ios::out);
    sal50 << "Autovector,Error" << endl;
    for (int i = 0; i < 100; ++i)
    {
        vector<double> autovectorIesimoDe50(n, 0.0);

        for (int j = 0; j < n; ++j)
        {
        	autovectorIesimoDe50[j] = matrizAutovalores50[j][i];
        }

        vector<vector<double> > matrizImagenesPara50T = matrizImagenesOriginal;
        vector<double> Ax50(n, 0.0);
        multiplicarMatrizVectorDouble(matrizImagenesPara50T,autovectorIesimoDe50, Ax50);

       //Normalizo Ax
        for (int j = 0; j < n; ++j)
        {
        	if(Ax50[j] < 0){Ax50[j]= Ax50[j]*(-1);}
        }
       
       
       vector<double> Lx50(n, 0.0);
       for (int j = 0; j < n; ++j)
       {
       		double lxi = autovectorIesimoDe50[j] * autovaloresDe50[i]; 
       		if(lxi < 0){Lx50[j] = lxi*(-1);}else{Lx50[j] = lxi;} 
       }
       

       double diferencia = distanceBetweenVector(Ax50, Lx50);
       

       sal50 << i << "," << diferencia << endl;
    }
    sal50.close();

    //Escribo resultados niter 25
    fstream sal25("niter25", ios::out);
    sal25 << "Autovector,Error" << endl;
    for (int i = 0; i < 100; ++i)
    {
        vector<double> autovectorIesimoDe25(n, 0.0);

        for (int j = 0; j < n; ++j)
        {
        	autovectorIesimoDe25[j] = matrizAutovalores25[j][i];
        }

        vector<vector<double> > matrizImagenesPara25T = matrizImagenesOriginal;
        vector<double> Ax25(n, 0.0);
        multiplicarMatrizVectorDouble(matrizImagenesPara25T,autovectorIesimoDe25, Ax25);

       //Normalizo Ax
        for (int j = 0; j < n; ++j)
        {
        	if(Ax25[j] < 0){Ax25[j]= Ax25[j]*(-1);}
        }
       
       
       vector<double> Lx25(n, 0.0);
       for (int j = 0; j < n; ++j)
       {
       		double lxi = autovectorIesimoDe25[j] * autovaloresDe25[i]; 
       		if(lxi < 0){Lx25[j] = lxi*(-1);}else{Lx25[j] = lxi;} 
       }
       

       double diferencia = distanceBetweenVector(Ax25, Lx25);
       

       sal25 << i << "," << diferencia << endl;
    }
    sal25.close();

    //Escribo resultados niter 10
    fstream sal10("niter10", ios::out);
    sal10 << "Autovector,Error" << endl;
    for (int i = 0; i < 100; ++i)
    {
        vector<double> autovectorIesimoDe10(n, 0.0);

        for (int j = 0; j < n; ++j)
        {
        	autovectorIesimoDe10[j] = matrizAutovalores10[j][i];
        }

        vector<vector<double> > matrizImagenesPara10T = matrizImagenesOriginal;
        vector<double> Ax10(n, 0.0);
        multiplicarMatrizVectorDouble(matrizImagenesPara10T,autovectorIesimoDe10, Ax10);

       //Normalizo Ax
        for (int j = 0; j < n; ++j)
        {
        	if(Ax10[j] < 0){Ax10[j]= Ax10[j]*(-1);}
        }
       
       
       vector<double> Lx10(n, 0.0);
       for (int j = 0; j < n; ++j)
       {
       		double lxi = autovectorIesimoDe10[j] * autovaloresDe10[i]; 
       		if(lxi < 0){Lx10[j] = lxi*(-1);}else{Lx10[j] = lxi;} 
       }
       

       double diferencia = distanceBetweenVector(Ax10, Lx10);
       

       sal10 << i << "," << diferencia << endl;
    }
    sal10.close();

    
}


int main(){
	// testNiter("tests/entrenamientoFullBig.in");
	//kFoldTestAccurracy(5, "tests/testFullBig.in");
	//testCargarPredictorConImagenes("tests/testBig.in");
	//testPredecirImagen("tests/testRed.in", "ImagenesCaras/s41/5.pgm");
	//testAccurracyPromediadaBig("tests/entrenamientoFullBig.in","tests/testeoFullBig.in");
	//testAccurracyBig("tests/entrenamientoFullBig.in","tests/testeoFullBig.in");
	//kFoldEspecial(5, "tests/testFullRed.in");
}