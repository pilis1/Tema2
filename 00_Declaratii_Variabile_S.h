//#############################################################################
//@@@	Declaratie variabile GLOBALE
//############################################################################# 
static int panelHandle;
//Declaratii pentru Task-ul Automat / Manual
int regim;
int stare_led;
int valoare_led; 
//Declaratii generare semnale Treapta, Rampa, Sinusoida
float Refk, Refk_1, Refk_2, Refk0;   
int sel_ref;  
double timp_rampa_tk, timp_sinus_tk;
double T_sin, A_sin;
int comutare_Sin;

//Declaratie perioada de esantionare 
double Te;
//Momentele de esantionare
double tk_1, tk;
//Declaratii semnale F.D.T.
float y_matlabk_1, y_matlabk;
float uk, uk_1;
double ynouk, ynouk_1;
//Declaratii pm. discretizare F.D.T 
double C, T; 

double yHWk, yHWk_1, Te_HW;

// Trapezoid generator enable flag (1 = active, 0 = inactive)
int trap_enabled;

// Stari pentru modelul de ordin 4 (pentru H(s) din imagine)
double x_model[4];       // starea curenta x(k)
double x_model_next[4];  // starea urmatoare x(k+1)

