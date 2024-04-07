#include<stdio.h>
#include<math.h>
#include<cmath>


#define nplt 10 //number of planets
const double tprime = sqrt(6.67e-11*1.99e30/pow(1.496e11,3.0));
const double h = 0.2; //el salto temporal empieza siendo 1-->59.8 dias.
const double tmax = 500;
double t;
double pic[nplt][4]={0};  
double y_tminus1[nplt]={0};
double mass[nplt];
double energy[nplt];
double period[nplt];
double aux[nplt] = {0}; // Variable auxiliar para el tiempo (ultima vez que pasó el planeta por y=0)

using namespace std;




//Funciones:
void rescale(FILE*);
void accel(double v1[nplt], double v2[nplt]);
void evalue(double eval[nplt]);
void tfunc(); 
int print(FILE*,FILE*,FILE*);


//El fichero init.txt contiene las x y v_y iniciales de todos los cuerpos
//El primero es el Sol y luego van en orden
//Cogemos directamente los datos del enlace, luego reescalamos.
//Inicialmente todos los cuerpos se sitúan en el eje X (distance from Sun)
//con velocidad en Y (orbital velocity)
int main()
{
    double ax[nplt], ay[nplt], wx[nplt], wy[nplt];
    // lo meto todo en una picture que toma una foto del sistema a tiempo t (x, y, vx, vy)
    FILE *initial_dat, *data, *energy_data,*t_data;
    initial_dat = fopen("init.txt", "r"); // Contiene las x y v_y iniciales y masa de todos los cuerpos
    rescale(initial_dat);                 // Ya están los datos iniciales metidos en mass y pic.
    data = fopen("planets_data.dat", "w"); //(se puede usar modo append "a" en vez de write "w".
    energy_data=fopen("e_vals.dat","w");
    t_data=fopen("t_data.dat","w");
    

    //ALGORITMO DE VERLET.
    for (t=0;t<tmax;t+=h){
        // Initialize ax and ay arrays
        for (int i = 0; i < nplt; i++)
        {
            ax[i] = 0;
            ay[i] = 0;
            wx[i]=0;
            wy[i]=0;
            y_tminus1[i]=pic[i][1];    //Aqui guardo la y del instante previo para calcular el periodo
        }
        
        print(data,energy_data,t_data); 
        //print
        accel(ax, ay); // obtengo la aceleración en función de la matriz actualizada 
        for (int i = 0; i < nplt; i++)
        {
            // meter los datos en pic para que se guarden los del instate 0.
            
            pic[i][0] = pic[i][0] + h * pic[i][2] + pow(h, 2) / 2 * (ax[i]); // r(t+h)=r(t)+hv(t)+h^2/2a(t)
            pic[i][1] = pic[i][1] + h * pic[i][3] + pow(h, 2) / 2 * (ay[i]); //
            
            
            wx[i]=pic[i][2]+h/2*ax[i]; //variables auxialiares para poder guardar ax[t]
            wy[i]=pic[i][3]+h/2*ay[i];
            
            accel(ax, ay); // actualizo las aceleraciones para las nuevas posiciones

            pic[i][2]=wx[i]+h/2*ax[i]; //v(t+h)=w(t)+h/2a(t+h)
            pic[i][3]=wy[i]+h/2*ay[i];

            //guardar
            evalue(energy);  //Calcula la energia y la mete en el cuadro 

        }

        tfunc();
    }
    fclose(initial_dat);
    fclose(data);
    fclose(energy_data);
    fclose(t_data);
    return 0;
}



void tfunc(){  
    // Voy a plantearlo de forma que cada vez que y cambie de signo cuente medio periodo.
    // No hay que tener en cuenta el estado inicial ya que y empieza en 0, solo tener cuidado de no poner el contador en la iteración 0.
    // Necesito una variable auxiliar para poder comparar los signos --> y_tminus1.  
    for (int i = 0; i < nplt; i++)
    {
        if (signbit(y_tminus1[i]) != signbit(pic[i][1])) //signbit(): bit del signo de un float (de cmath)
        {
            period[i]=(t-aux[i])*59.8*2;
            aux[i] = t;
        }
    }
}

int print(FILE* out,FILE*out2,FILE* out3 ){
    for (int i=0;i<nplt;i++){
        fprintf(out, "%lf, %lf \n", pic[i][0], pic[i][1]);
        fprintf(out2, "%lf\n",energy[i]);
        fprintf(out3,"%lf\n",period[i]);                   // lo primero esta el periodo del sol pero no tiene sentido 
    }
    fprintf(out, "\n");
    fprintf(out2,"\n");
    fprintf(out3,"\n");
    return 0;
}

void rescale(FILE* in){
    double x0;
    double v0;
    int i;
    i=0;
    while(fscanf(in,"%lf %lf %lf",&x0,&v0, &mass[i])!=EOF)
    {
        x0=x0/1.496e2; //(1e6 km a UA)
        v0=v0/1.496e8/tprime; //paso km/s a UA/59.8 dias  
        mass[i]=mass[i]/1.99e30; //paso kg a masas solares
        
        pic[i][0]=x0;
        pic[i][3]=v0;
        i++;
    }
}

void evalue(double E[nplt]){
    for(int i=0;i<nplt;i++){
        E[i]=0;
        E[i]+=0.5*mass[i]*(pow(pic[i][2],2)+pow(+pic[i][3],2));  //Energia cinética
        for(int j=0;j<nplt;j++){         
            if(i!=j){
                E[i]-=mass[j]*mass[i]/sqrt(pow(pic[i][0]-pic[j][0],2)+pow(pic[i][1]-pic[j][1],2)); // Energía potencial de i con el resto de planetas
            }                  
        }
    }
}

void accel(double v1[nplt], double v2[nplt])
{
    double sumx, sumy, den;
    // Initialize acceleration components to zero for each planet
    for (int i = 0; i < nplt; i++)
    {
        v1[i] = 0;
        v2[i] = 0;
    }

    // Calculate acceleration components for each planet
    for (int i = 0; i < nplt; i++)
    {
        sumx = 0;
        sumy = 0;
        for (int j = 0; j < nplt; j++)
        {
            if (j != i)
            {
                // Calculate distance between planets
                double dx = pic[j][0] - pic[i][0];
                double dy = pic[j][1] - pic[i][1];
                den = sqrt(pow(pow(dx, 2) + pow(dy, 2), 3)); // Corrected calculation of den

                // Calculate acceleration components
                sumx += mass[j] * dx / den;
                sumy += mass[j] * dy / den;
            }
        }
        // Assign acceleration components to respective planet
        v1[i] = sumx;  
        v2[i] = sumy;
    }
}


