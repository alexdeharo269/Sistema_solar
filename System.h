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
        double x(){
            double x;
            x=dtS*cos(phi);
            return x;
        }
        double y()
        {
            double y;
            y = dtS * sin(phi);
            return y;
        }
};

class System {
    private:
        int numPlanetas;
        float D_max, rock_density, vrmod;
        mt19937_64 generator;
        uniform_real_distribution<double> real_distribution;
        uniform_real_distribution<double> rad_distribution;
        uniform_real_distribution<double> vr_distribution;
        const double pi = M_PI;

    public:
        vector<Planet> planetas;

        // Constructor
        System(int nplt, unsigned seed, float Dmax, float rock_den, float vr_mod)
            : numPlanetas(nplt), D_max(Dmax), rock_density(rock_den),vrmod(vr_mod), generator(seed),
              real_distribution(0.0, 1.0), rad_distribution(1.27 / D_max, 1.0), // pongo otra distribución para el radio ya que no pueden tener radio 0.
              // el menor radio será la órbita de Mercurio.
              vr_distribution(-1.0,1.0),
              planetas(static_cast<index_t>(numPlanetas))
        {
        }

        // Método para generar posiciones aleatorias para los planetas
        void Parametrosiniciales()
        {
            // Generar posiciones aleatorias dentro del rango especificado
            for (index_t i = 0; i < static_cast<index_t>(numPlanetas); ++i)
            {
                planetas[i].dtS = rad_distribution(generator) * D_max;
                planetas[i].phi = real_distribution(generator) * 2 * pi;

                planetas[i].vr = vrmod*vr_distribution(generator); // PASO 2
                planetas[i].radius=0.1;
                //Para la velocidad angular vamos a calcular una velocidad angular (radianes/59.1días) en función de la distancia
                //Usamos la velocidad de una trayectoria circular:
                
                planetas[i].mass=0.0014/numPlanetas;
                planetas[i].vphi = sqrt(1 / planetas[i].dtS);
                if(real_distribution(generator)<=rock_density){planetas[i].rock=1;}
            }
        }

        void Mover(double h, FILE* data){
            

            //Algoritmo de Verlet
            index_t nplt = static_cast<index_t>(numPlanetas);
            vector<double> ax(nplt,0.0), ay(nplt,0.0),wx(nplt,0.0),wy(nplt,0.0);

            vector<double> energy(nplt, 0.0);
            vector<double> y_tminus1(nplt, 0.0);
            vector<double> aux(nplt, 0.0);
            vector<double> period(nplt,0.0);

            // Initialize positions and velocities
            vector<vector<double>> pic(nplt, vector<double>(4, 0.0));
            for (index_t i = 0; i < static_cast<index_t>(nplt); ++i)
            {
                pic[i][0] = planetas[i].x();
                pic[i][1] = planetas[i].y();
                pic[i][2] = (-planetas[i].vphi * planetas[i].y()+planetas[i].vr*planetas[i].x())/planetas[i].dtS; // vx = -vphi * y
                pic[i][3] = (planetas[i].vphi * planetas[i].x() + planetas[i].vr * planetas[i].y()) / planetas[i].dtS; // vy = vphi * x
            }
            /*
            // Files for output
            ofstream data("planets_data.dat");
            ofstream energy_data("e_vals.dat");
            ofstream t_data("t_data.dat");*/

            // Save previous y for period calculation
            for (index_t i = 0; i < static_cast<index_t>(nplt); ++i)
            {
                y_tminus1[i] = pic[i][1];
            }

            // Print current state
            //print(data, energy_data, t_data, pic, energy, y_tminus1, aux, t);

            // Calculate acceleration
            for (index_t i = 0; i < static_cast<index_t>(nplt); ++i)
            {
                double dx = pic[i][0];
                double dy = pic[i][1];
                double den = pow(pow(dx, 2) + pow(dy, 2), 1.5);

                ax[i] = -planetas[i].mass * dx / den;
                ay[i] = -planetas[i].mass * dy / den;
            }

            // Update positions and velocities
            for (index_t i = 0; i < static_cast<index_t>(nplt); ++i)
            {
                pic[i][0] += h * pic[i][2] + 0.5 * h * h * ax[i];
                pic[i][1] += h * pic[i][3] + 0.5 * h * h * ay[i];

                wx[i] = pic[i][2] + 0.5 * h * ax[i];
                wy[i] = pic[i][3] + 0.5 * h * ay[i];
            }

            for (index_t i = 0; i < static_cast<index_t>(nplt); ++i)
            {
                double dx = pic[i][0];
                double dy = pic[i][1];
                double den = pow(pow(dx, 2) + pow(dy, 2), 1.5);

                ax[i] =-planetas[i].mass * dx / den;
                ay[i] =-planetas[i].mass * dy / den;
            }

            for (index_t i = 0; i < static_cast<index_t>(nplt); ++i)
            {
                pic[i][2] = wx[i] + 0.5 * h * ax[i];
                pic[i][3] = wy[i] + 0.5 * h * ay[i];
            }


            for (index_t i = 0; i < static_cast<index_t>(nplt); ++i)
            {
                planetas[i].dtS=sqrt(pow(pic[i][0],2)+pow(pic[i][1],2));
                planetas[i].phi=atan2(pic[i][1],pic[i][0]);
                fprintf(data, "%lf, %lf, %i\n", planetas[i].x(), planetas[i].y(), planetas[i].rock);
            }
            fprintf(data, "\n");

            /*
            // Calculate energy
            evalue(energy, pic, nplt);

            // Update periods
            tfunc(y_tminus1, pic, period, aux, t);

            data.close();
            energy_data.close();
            t_data.close();*/
        }
        
        void Interacciones(){

        }
         
        // Método para guardar las posiciones en un archivo
        /*void guardarPosiciones()
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
        }*/

        /*
        void evalue(vector<double> &E, double pic[][4], int nplt)
        {
            for (index_t i = 0; i < nplt; i++)
            {
                E[i] = 0;
                E[i] += 0.5 * planetas[i].mass * (pow(pic[i][2], 2) + pow(pic[i][3], 2));
                for (int j = 0; j < nplt; j++)
                {
                    if (i != j)
                    {
                        E[i] -= planetas[j].mass * planetas[i].mass / sqrt(pow(pic[i][0] - pic[j][0], 2) + pow(pic[i][1] - pic[j][1], 2));
                    }
                }
            }
        }



        void tfunc(vector<double> &y_tminus1, double pic[][4], vector<double> &period, vector<double> &aux, double t)
        {
            for (index_t i = 0; i < numPlanetas; i++)
            {
                if (signbit(y_tminus1[i]) != signbit(pic[i][1]))
                {
                    period[i] = (t - aux[i]) * 58.1 * 2;
                    aux[i] = t;
                }
            }
        }*/
};