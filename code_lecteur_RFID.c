// ******************************************************************
//
//    File: App_HUS100_Heig-vd_UART_Standard.c
//    Date: 2020-04-02
//    Version: 1.00
//    Case: #25543
//    Mod: EP
//
//    V1.00
//    -----
//    - initial realise
//		
//	Modifications: 
//
//  Auteur: Quentin Fornerod || Date: 07.05.2020 || Changement de la communication UART vers i2C
//  Auteur: Quentin Fornerod || Date: 12.05.2020 || Affectation des GPIOs en copiant l'etat des LEDs
// ******************************************************************

#include "twn4.sys.h"
#include "apptools.h"

#define MAXIDBYTES  10
#define MAXIDBITS   (MAXIDBYTES*8)

byte ID[MAXIDBYTES];
int IDBitCnt;
int TagType;

byte LastID[MAXIDBYTES];
int LastIDBitCnt;
int LastTagType;

// ******************************************************************
// ****** Section containing configurable behaviour *****************
// ******************************************************************

// If necessary adjust default parameters
#ifndef __APP_CONFIG_H__
    #define LFTAGTYPES        		0
    #define HFTAGTYPES        		0
#endif

const unsigned char AppManifest[] = { EXECUTE_APP, 1, EXECUTE_APP_ALWAYS, TLV_END };

// Setup tag types as configured in AppBlaster
void ConfigSetTagTypes(void)
{
	if (LFTAGTYPES || HFTAGTYPES)
    	SetTagTypes(LFTAGTYPES,HFTAGTYPES);
}

int main(void)
{
	/* La communication étant de type I2C, ces éléments ne sont plus nécessaires
	// UART Communication
	SetHostChannel(CHANNEL_COM1);
	// Show the startup message
    if (GetHostChannel() == CHANNEL_COM1)
		{
        // A V24 device is writing the version at startup
        HostWriteVersion();
        HostWriteChar('\r');
		}*/
	SetHostChannel(CHANNEL_I2C);
	I2CInit(I2CMODE_SLAVE | 0x30 | I2CMODE_CHANNEL);
	
	// Set tag types
	ConfigSetTagTypes();
    //Init des GPIO 2 et 3
	GPIOConfigureOutputs(GPIO2,GPIO_PUPD_PULLDOWN,GPIO_OTYPE_PUSHPULL);
	GPIOConfigureOutputs(GPIO3,GPIO_PUPD_PULLDOWN,GPIO_OTYPE_PUSHPULL);
    // Init LEDs
    LEDInit(REDLED | GREENLED);
    // Turn on green LED
    LEDOn(GREENLED);
	GPIOSetBits(GPIO2);//vert
    // Turn off red LED
    LEDOff(REDLED);
	GPIOClearBits(GPIO3);
    // Make some noise at startup at low volume
    SetVolume(30);
    BeepLow();
    BeepHigh();
    // Continue with maximum  volume
    SetVolume(80);
    
    // No transponder found up to now
    LastTagType = NOTAG;


	while (true)
    {
        // Search a transponder
        if (SearchTag(&TagType,&IDBitCnt,ID,sizeof(ID)))
        {
			// Is this transponder new to us?
			if (TagType != LastTagType || IDBitCnt != LastIDBitCnt || !CompBits(ID,0,LastID,0,MAXIDBITS))
			{
				// Save this as known ID, before modifying the ID for proper output format
				CopyBits(LastID,0,ID,0,MAXIDBITS);
				LastIDBitCnt = IDBitCnt;
				LastTagType = TagType;
				// Yes! Sound a beep
				BeepHigh();
				// Turn off the green LED
				LEDOff(GREENLED);
				GPIOClearBits(GPIO2);
				// Let the red one blink, start with on-state
				LEDOn(REDLED);
				GPIOSetBits(GPIO3);
				LEDBlink(REDLED,500,500);
				// Send UID
				HostWriteHex(ID,IDBitCnt,(IDBitCnt+7)/8*2);
				// Send CR
				//HostWriteChar('\r');
			}
			// Start a timeout of two seconds
			StartTimer(2000);
        }
        if (TestTimer())
        {
            LEDOn(GREENLED);
			GPIOSetBits(GPIO2);
            LEDOff(REDLED);
			GPIOClearBits(GPIO3);
            LastTagType = NOTAG;
        }
    }
}
