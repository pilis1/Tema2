//citire de pe interfata grafica valoarea comutatorului de selectie RegimAutomat/Manual
int functiune_Automat_Manual()
{
			   	GetCtrlVal (panelHandle, PANEL_BINARYSWITCH, &regim);
			//Prelucrare Automat/Manual
			if (regim)
			{ //Manual
				//citire comutator stare led
				SetCtrlAttribute (panelHandle,PANEL_BINARYSWITCH_2, ATTR_DIMMED,0); 
					GetCtrlVal (panelHandle, PANEL_BINARYSWITCH_2, &stare_led);
				//setare valoarea obiectului LED
				SetCtrlVal (panelHandle, PANEL_LED, stare_led);
			
			}
			else
			{ //Automat
				//citire valoarea obiectului LED
				SetCtrlAttribute (panelHandle,PANEL_BINARYSWITCH_2, ATTR_DIMMED,1); 
				
				GetCtrlVal (panelHandle, PANEL_LED, &valoare_led);
				if (valoare_led==0)
				{
					//setare valoarea obiectului LED
					SetCtrlVal (panelHandle, PANEL_LED, 1);
				}
				else
				{
				 	//setare valoarea obiectului LED
					SetCtrlVal (panelHandle, PANEL_LED, 0);
				}
			}
return 0;
}
