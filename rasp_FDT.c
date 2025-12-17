#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "rasp_FDT.h"

//#includere Biblioteci de functii
#include "00_Declaratii_Variabile_S.h" 
#include "01_Functionare_A_M_S.h" 
#include "02_Functii_Utilitare_S.h" 
#include "03_Generare_Semnale_S.h"
//double generare_semnal()  ;

// --- Trapezoid signal generator state (simple internal implementation)
static double trap_t = 0.0;            // current time inside trapezoid period
// Make default trapezoid period much longer so changes persist visibly
static double trap_period = 20.0;      // seconds (default period)
static double trap_A = 1.0;            // amplitude (default)
static double trap_tr = 0.2;          // rise time (s)
static double trap_tf = 1.6;          // flat time (s)
static double trap_td = 0.2;          // fall time (s)

// Generate one sample of a periodic trapezoid waveform. Caller must update Te.
static double generare_trapez(double dt)
{
	if (dt <= 0.0) return 0.0;
	trap_t += dt;
	if (trap_t >= trap_period) trap_t -= trap_period * floor(trap_t / trap_period);
	double t = trap_t;
	double y = 0.0;
	if (t < trap_tr) {
		// rising edge
		y = (trap_A / trap_tr) * t;
	} else if (t < (trap_tr + trap_tf)) {
		// flat top
		y = trap_A;
	} else if (t < (trap_tr + trap_tf + trap_td)) {
		// falling edge
		y = trap_A - (trap_A / trap_td) * (t - (trap_tr + trap_tf));
	} else {
		// low period
		y = 0.0;
	}
	return y;
}

int __stdcall WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					   LPSTR lpszCmdLine, int nCmdShow)
{
	if (InitCVIRTE (hInstance, 0, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "rasp_FDT.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}
//#############################################################################
//@@@	Functia prin care se porneste aplicatia software:
//	=>se initializeaza toate variabilele necesare pentru a putea porni aplicatia
//############################################################################# 
int CVICALLBACK start_aplicatie (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		
		//Task#### Initializare variabile
			//Afisare text ...Hello real-time World
				InsertTextBoxLine (panelHandle, PANEL_TEXTBOX, -1, "Hello real-time World ! SUCCES !!!!");
			//Setare perioada de esantionare 
				// Perioada de esantionare redusa pentru grafica fluida
				Te = 0.02;    // 20 ms (0.02 s) - ajusteaza dupa nevoie (ex: 0.01 = 10 ms)
				Te_HW = Te;   // foloseste acelasi interval pentru timer-ul UI
				tk_1=0; tk=Te;
			//initializare folosita pentru a detecta trecerea prin "0"
					Refk=0; Refk_1=0;  Refk_2=0; 
					sel_ref=0;    
			//Comutare pe sinus fara socuri>>>>doar dupa trecerea fct. Sin prin 0
			comutare_Sin=0;
				GetCtrlVal (panelHandle, PANEL_A_SINUS, &A_sin);
				GetCtrlVal (panelHandle, PANEL_T_SINUS, &T_sin);
			//Parametrii f.d.t.
			T=5; //T=10;
			C=Te/(Te+T);
		//Ascunde BUTONUL START daca a pornit aplicatia
			SetCtrlAttribute (panelHandle,PANEL_OKBUTTON, ATTR_DIMMED,1); 
		//Task#### Initializare/Pornire TIMER		
			SetCtrlAttribute (panelHandle,PANEL_TIMER, ATTR_INTERVAL, Te_HW);
 					SetCtrlAttribute (panelHandle,PANEL_TIMER, ATTR_ENABLED,1); 
 			// Initialize model states
 			x_model[0]=x_model[1]=x_model[2]=x_model[3]=0.0;
			// Initialize trapezoid generator time
			trap_t = 0.0;
 			break;
	}
	return 0;
}
//#############################################################################
//@@@	Functia principala a unei aplicatii de Timp-Real 
//	=>asociata intreruperilor de la ceasul de timp real; 
//	=>aici se fac toate prelucrarile necesare unei aplicatii de automatizare
//############################################################################# 
int CVICALLBACK esantionare (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
		//Functionare aplicatie Atomat / Manual
		 	functiune_Automat_Manual();
		// Read current selection each tick and generate accordingly
		GetCtrlVal(panelHandle, PANEL_SEL_REF, &sel_ref);
		if (sel_ref == 3) {
			Refk = generare_trapez(Te);
		} else {
			Refk = generare_semnal();
		}
		//Prelucrare Procesul tehnologic
			//marimea de intrare in proces este semnalul generat Refk
				uk=Refk;
			//Discretizare cu ajutorul Matlab
				//y_matlabk = 0.999*y_matlabk_1+uk*0.0004998  + 0.0004998*uk_1;
				//zY - 0.9967Y= 0.001664 zU + 0.001664U
			//	y_matlabk = 0.9967*y_matlabk_1+ 0.001664*uk + 0.001664*uk_1;
								 
			//Discretizare cu aproximarea derivatei pe o perioada de esantionare 	
				ynouk=ynouk_1+(Te/(Te+T))*(uk-ynouk_1);	

			// -------------------------
			// Model continuu H(s) -> aproximare Euler explicit (stare de ordin 4)
			// H(s) = (392 s + 196) / (2.4 s^4 + 59.24 s^3 + 710.52 s^2 + 2410.8 s + 196)
			// rescalat pe forma monica: s^4 + a1 s^3 + a2 s^2 + a3 s + a4
			// a1..a4 si b0..b3 calculate inainte
			// -------------------------
			{
				// Denominator monic coefficients a1..a4
				const double a1 = 24.6833333333333;
				const double a2 = 296.05;
				const double a3 = 1004.5;
				const double a4 = 81.6666666666667;
				// Numerator coefficients for forma de grad n-1 (s^3 .. s^0): b0,b1,b2,b3
				const double b0 = 0.0;
				const double b1 = 0.0;
				const double b2 = 163.3333333333333; // 392/2.4
				const double b3 = 81.6666666666667;  // 196/2.4

				// Companion (controllable canonical) A, B, C matrices (continuous)
				// A = [ -a1 -a2 -a3 -a4
				//        1    0   0   0
				//        0    1   0   0
				//        0    0   1   0 ]
				// B = [1 0 0 0]^T
				// C = [b0 b1 b2 b3]
				double A0[4][4] = {
					{-a1, -a2, -a3, -a4},
					{1.0,  0.0,  0.0,  0.0},
					{0.0,  1.0,  0.0,  0.0},
					{0.0,  0.0,  1.0,  0.0}
				};
				const double B0[4] = {1.0, 0.0, 0.0, 0.0};
				const double C0[4] = {b0, b1, b2, b3};

				// compute xdot = A * x + B * uk
				double xdot[4] = {0,0,0,0};
				int i,j;
				for (i=0;i<4;i++) {
					double s=0.0;
					for (j=0;j<4;j++) s += A0[i][j] * x_model[j];
					s += B0[i] * uk;
					xdot[i] = s;
				}
				// Euler explicit: x_next = x + Te * xdot
				for (i=0;i<4;i++) x_model_next[i] = x_model[i] + Te * xdot[i];

				// Output y = C * x_next (D=0)
				double y_model = 0.0;
				for (i=0;i<4;i++) y_model += C0[i] * x_model_next[i];

				// update model states (for next tick)
				for (i=0;i<4;i++) x_model[i] = x_model_next[i];

				// assign to yHWk (folosit ca raspunsul modelului)
				yHWk = y_model;
			}
			 
		// Afisare Referinta	
			PlotLine (panelHandle, PANEL_GRAPH, tk_1, Refk_1, tk, Refk, VAL_GREEN); 
		//Afisare raspunsul procesului in cele 2 variante
			PlotLine (panelHandle, PANEL_GRAPH, tk_1, ynouk_1, tk, ynouk, VAL_RED); 
				PlotLine (panelHandle, PANEL_GRAPH, tk_1, yHWk_1, tk, yHWk, VAL_BLUE); 
			//PlotLine (panelHandle, PANEL_GRAPH, tk_1, y_matlabk_1, tk, y_matlabk, VAL_BLUE); 
		//Reactualizare un nou ciclu pentru viitoarea intrerupere de la Timer
		tk_1=tk;
			//tk=tk+0.020;//Te;
			tk=tk+Te; 
		Refk_2=Refk_1;//operatie de reactualizare folosita si pentru detectarea trecerii prin "Ref0" a sinusului 
			Refk_1=Refk;
		uk_1=uk;
			ynouk_1=ynouk; 
				y_matlabk_1=y_matlabk;
				yHWk_1=yHWk;

			break;
	}
	return 0;
}








