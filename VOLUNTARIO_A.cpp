#include<stdio.h>
#include<math.h>
#include<cstdlib> //para la funcion rand(): decimal entre 0 y 1. RandBetween (entero entre min y max)

using namespce std;


#define constant nplt    //esto va a ser el numero inicial de planetesimales.


//Articulo interesante para clases en Marcadores/Fisica
class Planet {
    public:
        double x;
        double y;
        double vx;
        double vy;
        double mass;
        double radius;
        double U; //energia interna.
        bool real;
        bool rock;
    private:

    //y las funciones publicas o privadas que le añado a planet (el algoritmo de Verlet puedo añadirlo aqui pq solo van con el sol)
}
class System
{
    public:
         

    private:
    //y las funciones publicas o privadas caracteristicas de system
    
}

/*La disposición inicial de planetesimales debe involucrar una función random para las posiciones 
dentro de los limites del SS, pero no completamente random para las velocidades ya que si son completamente random
se cancelan y todo cae sobre el Sol. Habia pensado en un cono dentro del cual las velocidaes sean random, pero al 
menos que se respete el sentido. */


//la energia interna se obtiene de la diferencia de energias cineticas de los planetas
//que se han chocado inelasticamente

//El tiempo voy a medirlo en 59.1 dias igualmente pero para sacarlo lo saco como años terrestres. Ej: al cabo de 10000 años el sistema (40,1,5) ha terminado con 7 planetas.

//la aceleración va a ser solo con el sol pq no tomamos la interacción gravitatoria con otros planetas.
//entonces copio aqui el algoritmo de Verlet y lo adapto a las nuevas variables.

int main(){
    Planet.real()
    //supongo que definir run.system y ponerlo a correr sobre el espacio de parametros.

    //para run.universe no voy a correr todas las simulaciones en animacion_planetas.py o los demás ficheros .py.
    //entonces estaria bien sacar run.system para meterlo en un python.subprocess y que saque (o guarde) las animaciones/figuras del sistema con parametros definidos
    //Para run.multiverse más bien seria (o sacar solo algunas escogidas previamente) o sacar un txt con el resumen de lo que ha pasado (planetas que se han formado, lo que pida). 



}

//Quiero un run.system que corra parametros concretos y un run.universe (o un for para cada system) que me itere sobre conjuntos de parametros 