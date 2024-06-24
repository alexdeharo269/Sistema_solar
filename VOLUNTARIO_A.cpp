#include<stdio.h>
#include<math.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include<random>
#include<cmath>
#include<vector>
#include "System.h"



using namespace std;




//si quisiera limitar el alcance de la constante a este archivo usaria static const int nplt=1000
//Articulo interesante para clases en Marcadores/Fisica

//la energia interna se obtiene de la diferencia de energias cineticas de los planetas
//que se han chocado inelasticamente

//El tiempo voy a medirlo en 59.1 dias igualmente pero para sacarlo lo saco como años terrestres. Ej: al cabo de 10000 años el sistema (40,1,5) ha terminado con 7 planetas.

//la aceleración va a ser solo con el sol pq no tomamos la interacción gravitatoria con otros planetas.
//entonces copio aqui el algoritmo de Verlet y lo adapto a las nuevas variables.

int main(){
    //supongo que definir run.system y ponerlo a correr sobre el espacio de parametros.

    //para run.universe no voy a correr todas las simulaciones en animacion_planetas.py o los demás ficheros .py.
    //entonces estaria bien sacar run.system para meterlo en un python.subprocess y que saque (o guarde) las animaciones/figuras del sistema con parametros definidos
    
    //Para run.multiverse más bien seria (o sacar solo algunas escogidas previamente) o sacar un txt con el resumen de lo que ha pasado (planetas que se han formado, lo que pida).
    /*La disposición inicial de planetesimales debe involucrar una función random para las posiciones
dentro de los limites del SS, pero no completamente random para las velocidades ya que si son completamente random
se cancelan y todo cae sobre el Sol. Habia pensado en un cono dentro del cual las velocidaes sean random, pero al
menos que se respete el sentido. */
    double t;
    const int nplt = 100;      // esto va a ser el numero inicial de planetesimales.
    unsigned seed = 1946402; // semilla generador de números aleatorios
    const int iter_max=1000;
    const double h=1.0;
    int iter,count;

    System system(nplt,seed,30.0f,0.1f,0.05f,10000.0f,1.0f);
    

    // Generar posiciones iniciales aleatorias para los planetas
    system.Parametrosiniciales();
    //generarPosiciones sobre los parametros de densidad 
    //Sistema.generarVelocidades(); sobre los parametros de velocidad

    FILE *data;
    data=fopen("planets_data.dat","w");
    count=0;
    for(t=0;t<iter_max;t+=h){
        system.Mover(h,data);
        iter=system.Interacciones();
        count += iter;
    }
    

    fprintf(stdout, "%i interacciones\n\n", count);
    //Aqui defino el tiempo 
    //Sistema.mover(); //sobre los parametros de radio y sobre el parametro de la linea del cinturon de anteroides. 10x10x10x10 ya tines 1e4 simulaciones
    //mover podria ser en polares? 

    // Guardar las posiciones en un archivo
    //system.guardarPosiciones();

    return 0;
}

//Quiero un run.system que corra parametros concretos y un run.universe (o un for para cada system) que me itere sobre conjuntos de parametros 