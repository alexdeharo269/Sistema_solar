#include<stdio.h>
#include<math.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include<random>
#include<cmath>
#include<vector>

using namespace std;
using index_t = vector<int>::size_type; // Define index_t as the size type of vector<int>

class Planet {
    public:
        double dtS;
        double phi;
        double vr;
        double vphi;
        double mass;
        double radius;
        double U; //energia interna.
        bool real;
        int rock=0; // y las funciones publicas o privadas que le añado a planet (el algoritmo de Verlet puedo añadirlo aqui pq solo van con el sol)

        // Constructor
        Planet()=default;

        //Paso a cartesianas 
        double x(double r, double theta){
            double x;
            x=r*cos(theta);
            return x;
        }
        double y(double r, double theta)
        {
            double y;
            y = r * sin(theta);
            return y;
        }
};

class System {
    private:
        int numPlanetas;
        float D_max, rock_density;
        mt19937_64 generator;
        uniform_real_distribution<double> r_distribution;
        const double pi = M_PI;

    public:
        vector<Planet> planetas;

        // Constructor
        System(int nplt, unsigned seed, float Dmax, float rock_den)
            : numPlanetas(nplt), D_max(Dmax), rock_density(rock_den), generator(seed),
            r_distribution(0.0, 1.0), planetas(static_cast<index_t>(numPlanetas)) {}

        // Método para generar posiciones aleatorias para los planetas
        void Parametrosiniciales()
        {
            // Generar posiciones aleatorias dentro del rango especificado

            for (index_t i = 0; i < static_cast<index_t>(numPlanetas); ++i)
            {
                planetas[i].dtS = r_distribution(generator) * D_max;
                planetas[i].phi = r_distribution(generator) * 2 * pi;

                planetas[i].vr = r_distribution(generator); // PASO 2
                if(r_distribution(generator)<=rock_density){planetas[i].rock=1;}
            }
        }

        // Método para guardar las posiciones en un archivo
        void guardarPosiciones()
        {
            //Pasamos las variables angulares a las de cuadricula
            ofstream archivo("posiciones_iniciales.txt");
            if (!archivo.is_open())
            {
                cout << "Error al abrir el archivo." << endl;
                return;
            }

            for (index_t i = 0; i < static_cast<index_t>(numPlanetas); ++i)
            {
                archivo << planetas[i].dtS * cos(planetas[i].phi) << "," << planetas[i].dtS * sin(planetas[i].phi)<<","<< planetas[i].rock << endl; ////////////////
            }

            archivo.close();
        }
};