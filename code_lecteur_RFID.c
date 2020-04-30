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


// ******************************************************************

int main(void)
{
	// UART Communication
	SetHostChannel(CHANNEL_COM1);
	
	// Show the startup message
    if (GetHostChannel() == CHANNEL_COM1)
    {
        // A V24 device is writing the version at startup
        HostWriteVersion();
        HostWriteChar('\r');
    }
	// Set tag types
	ConfigSetTagTypes();
    	
    // Init LEDs
    LEDInit(REDLED | GREENLED);
    // Turn on green LED
    LEDOn(GREENLED);
    // Turn off red LED
    LEDOff(REDLED);
    // Make some noise at startup at low volume
    SetVolume(30);
    BeepLow();
    BeepHigh();
    // Continue with maximum  volume
    SetVolume(100);
    
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
				// Let the red one blink, start with on-state
				LEDOn(REDLED);
				LEDBlink(REDLED,500,500);
				// Send UID
				HostWriteHex(ID,IDBitCnt,(IDBitCnt+7)/8*2);
				// Send CR
				HostWriteChar('\r');
			}
			// Start a timeout of two seconds
			StartTimer(2000);
        }
        if (TestTimer())
        {
            LEDOn(GREENLED);
            LEDOff(REDLED);
            LastTagType = NOTAG;
        }
    }
}
