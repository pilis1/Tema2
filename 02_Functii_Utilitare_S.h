//#############################################################################
//@@@	Functii Utilitare pt. Aplicatie
//############################################################################# 
//Refresh Grafice
int CVICALLBACK stergere_grafice (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW); 
				tk_1=0;
				tk=0;
			break;
	}
	return 0;
}
//Stop aplicatie
int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

