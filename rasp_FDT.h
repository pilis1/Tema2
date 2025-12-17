/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_OKBUTTON_2                 2       /* control type: command, callback function: stergere_grafice */
#define  PANEL_OKBUTTON                   3       /* control type: command, callback function: start_aplicatie */
#define  PANEL_QUITBUTTON                 4       /* control type: command, callback function: QuitCallback */
#define  PANEL_TEXTBOX                    5       /* control type: textBox, callback function: (none) */
#define  PANEL_LED                        6       /* control type: LED, callback function: (none) */
#define  PANEL_BINARYSWITCH               7       /* control type: binary, callback function: (none) */
#define  PANEL_BINARYSWITCH_2             8       /* control type: binary, callback function: (none) */
#define  PANEL_GRAPH                      9       /* control type: graph, callback function: (none) */
#define  PANEL_SEL_REF                    10      /* control type: slide, callback function: selectie_semnal */
#define  PANEL_REFERINTA                  11      /* control type: scale, callback function: (none) */
#define  PANEL_T_SINUS                    12      /* control type: numeric, callback function: detectare_comutare_SIN */
#define  PANEL_A_SINUS                    13      /* control type: numeric, callback function: detectare_comutare_SIN */
#define  PANEL_M_PANTA                    14      /* control type: numeric, callback function: modificare_Panta */
#define  PANEL_TIMER                      15      /* control type: timer, callback function: esantionare */
#define  PANEL_DECORATION                 16      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_3               17      /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG_2                  18      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    19      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK detectare_comutare_SIN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK esantionare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK modificare_Panta(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK selectie_semnal(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK start_aplicatie(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK stergere_grafice(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif