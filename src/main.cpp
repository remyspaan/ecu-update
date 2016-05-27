#include <stdio.h>
#include <stdlib.h>
#include "avrnacl.h"
#include "MPC5566.h"
#include "randombytes.h"
#include "Exceptions.h"
#include "IntcInterrupts.h"
#include "clock.h"
#include "fail.h"

#define FUNC_TYPE_LENGTH 2

const unsigned char FUNC_KEY_EXCHANGE[]           = {0x01, 0x01};
const unsigned char FUNC_KEY_EXCHANGE_CONT[]      = {0x01, 0x02};
const unsigned char FUNC_ASK_UPDATE[]             = {0x02, 0x01};
const unsigned char FUNC_ASK_VERSION[]            = {0x03, 0x01};
const unsigned char FUNC_RESPOND_VERSION[]        = {0x04, 0x01};
const unsigned char FUNC_RESPOND_UPDATE[]         = {0x05, 0x01};
const unsigned char FUNC_ASK_FIRMWARE[]           = {0x06, 0x01};
const unsigned char FUNC_SEND_FIRMWARE[]          = {0x07, 0x01};
const unsigned char FUNC_CONFIRM_FIRMWARE[]       = {0x08, 0x01};

const unsigned char ECU_UPDATE_ONE[1079] = {0x2b, 0x77, 0xc4, 0x58, 0x72, 0x87, 0x2c, 0xff, 0x91, 0x5b, 0xee, 0x00, 0xe0, 0x38, 0x8e, 0xfc, 0xa6, 0x55, 0x4b, 0x70, 0x9b, 0x1f, 0xfd, 0x18, 0x29, 0xd0, 0x34, 0x9a, 0xb6, 0x22, 0x0b, 0xc5, 0x93, 0xf7, 0x1f, 0x1f, 0x45, 0x47, 0xb3, 0xbd, 0x67, 0x3b, 0x6a, 0xdd, 0x51, 0x41, 0x6f, 0x47, 0xe7, 0x97, 0x2a, 0x20, 0x54, 0x7d, 0xee, 0xc3, 0x86, 0xac, 0x04, 0x07, 0x3a, 0x81, 0xc8, 0x9f, 0xb4, 0x60, 0x73, 0xb0, 0x18, 0x12, 0xfb, 0xba, 0xd0, 0x1b, 0x2d, 0xa9, 0x65, 0x94, 0x03, 0x85, 0xfc, 0x5c, 0x3e, 0x76, 0xc8, 0x18, 0xf3, 0x52, 0x62, 0x3d, 0x72, 0x4a, 0x2e, 0x99, 0x44, 0x9d, 0x04, 0x32, 0x78, 0xd9, 0xab, 0xfa, 0x09, 0x0b, 0x0b, 0xd6, 0x7d, 0xb2, 0xd3, 0x05, 0x16, 0x4c, 0x6d, 0x93, 0xd6, 0xf5, 0x10, 0x9f, 0x7c, 0x9b, 0xe5, 0x5e, 0x46, 0xa6, 0xb8, 0x0f, 0x43, 0x5e, 0xa9, 0x7d, 0x65, 0xba, 0x32, 0x0c, 0x1a, 0x81, 0x8a, 0xc7, 0x68, 0x01, 0x23, 0xbf, 0x93, 0x9a, 0xc7, 0x79, 0x33, 0x6c, 0x67, 0x9a, 0x78, 0x5b, 0x34, 0xc1, 0xe2, 0x21, 0xb3, 0xbf, 0xbd, 0x48, 0x86, 0xe3, 0xbc, 0x80, 0x89, 0x13, 0x44, 0xf0, 0xcf, 0xa0, 0x33, 0x94, 0x6d, 0xaa, 0xd1, 0xf0, 0xec, 0xbf, 0x2b, 0x14, 0xe5, 0x8b, 0x4a, 0x32, 0x08, 0x78, 0x16, 0xff, 0x6c, 0x9d, 0x25, 0xa3, 0x6f, 0x81, 0xea, 0x56, 0x95, 0xc1, 0x78, 0x7c, 0x7b, 0x9f, 0x1b, 0x11, 0x94, 0x73, 0xef, 0x66, 0x48, 0x84, 0xbb, 0xcc, 0x35, 0xbf, 0x53, 0x8a, 0x40, 0x31, 0xa9, 0x41, 0x88, 0xd6, 0x95, 0xf4, 0x02, 0xd3, 0x69, 0x85, 0x61, 0xa8, 0xe7, 0xab, 0x80, 0xd9, 0x01, 0xf3, 0x6b, 0x34, 0xf7, 0x27, 0xa3, 0xef, 0x09, 0xb2, 0x52, 0x34, 0xa2, 0x5d, 0xfa, 0x46, 0x77, 0xa7, 0x0d, 0x3a, 0x74, 0x10, 0x3b, 0x83, 0x31, 0x1d, 0xa6, 0xaa, 0xa9, 0x5e, 0x34, 0xad, 0x40, 0xc3, 0x3d, 0x16, 0xeb, 0xc3, 0xe2, 0xea, 0x70, 0xd6, 0xae, 0x9a, 0xcf, 0xc3, 0x67, 0x76, 0xda, 0x71, 0x7e, 0x67, 0xb7, 0xba, 0x8f, 0xca, 0xee, 0xcd, 0x71, 0xf5, 0xd7, 0x10, 0x93, 0x8c, 0xf8, 0xb8, 0xdf, 0xc4, 0x63, 0xa4, 0x77, 0x0b, 0x23, 0x9c, 0x72, 0xd9, 0x04, 0xa9, 0x8e, 0x76, 0x60, 0x9e, 0x23, 0xc1, 0xde, 0x58, 0xf9, 0x07, 0x3d, 0x1b, 0xb7, 0x95, 0x7c, 0x2e, 0x30, 0x82, 0x33, 0x74, 0xbc, 0x05, 0x2e, 0x32, 0x79, 0x79, 0x39, 0x68, 0x83, 0xb1, 0x2d, 0x7e, 0xde, 0x29, 0x91, 0x5a, 0xe6, 0x66, 0x0a, 0x2c, 0x11, 0x03, 0xee, 0xb9, 0xc2, 0x28, 0xd0, 0xf5, 0xd5, 0x16, 0xf2, 0x90, 0xb4, 0x0d, 0x97, 0x49, 0x0f, 0x76, 0xe6, 0xfb, 0xf6, 0x3c, 0xd1, 0xf2, 0x98, 0xbf, 0x32, 0x53, 0xec, 0xd4, 0x8f, 0x65, 0xa2, 0x89, 0xf3, 0x4f, 0x1f, 0x91, 0x3f, 0x35, 0xc6, 0x6b, 0x7c, 0xb6, 0x70, 0x4c, 0x86, 0x09, 0x72, 0x3b, 0xef, 0xc5, 0xfe, 0xf8, 0xbe, 0xa6, 0x37, 0xb4, 0x1c, 0xc6, 0x71, 0xa7, 0xe4, 0x07, 0xa9, 0xbb, 0xf8, 0xb8, 0xa2, 0xe6, 0x49, 0xf9, 0xf3, 0xba, 0xe1, 0x37, 0x31, 0xb0, 0xeb, 0x61, 0xc9, 0xb7, 0x41, 0xb1, 0xf5, 0x67, 0xd0, 0x2d, 0x5d, 0x47, 0xcb, 0xa1, 0x34, 0x81, 0xe8, 0xd3, 0x7e, 0x0f, 0xe5, 0xcf, 0x92, 0x97, 0xab, 0x70, 0xd2, 0x45, 0x98, 0xd9, 0x71, 0x2c, 0x97, 0x42, 0xc1, 0x85, 0x73, 0x4d, 0xcb, 0x40, 0xc2, 0xe3, 0x81, 0x58, 0x77, 0xce, 0x7a, 0xfc, 0xb4, 0xc7, 0xae, 0x67, 0x88, 0xd6, 0xef, 0xe7, 0x44, 0xe8, 0xb1, 0xe1, 0x5a, 0x0a, 0xbb, 0xf1, 0x0c, 0x80, 0xb0, 0x3f, 0x64, 0x57, 0xbd, 0x7c, 0xc2, 0x38, 0xab, 0xf1, 0xdb, 0x93, 0x55, 0xf9, 0x8e, 0xfb, 0x52, 0x22, 0xc1, 0xf0, 0x68, 0xf9, 0x88, 0xad, 0xef, 0xfa, 0x8b, 0x40, 0x98, 0x3a, 0x52, 0xa6, 0x59, 0xf5, 0x84, 0x74, 0x7e, 0x4e, 0x56, 0x3d, 0xa3, 0x3e, 0x5d, 0x61, 0x29, 0xe8, 0xae, 0x2f, 0xc7, 0x0e, 0xa5, 0x50, 0xf1, 0x34, 0xcc, 0xe4, 0x37, 0xfa, 0x22, 0x4e, 0x18, 0xa4, 0x5e, 0xa7, 0x23, 0xcb, 0xf6, 0xf3, 0x06, 0x44, 0x2a, 0x63, 0x96, 0x52, 0xbd, 0x66, 0x6d, 0x07, 0xde, 0x3a, 0x22, 0x75, 0x07, 0x51, 0x03, 0xfb, 0xd3, 0x4b, 0x3e, 0x2a, 0x0c, 0x34, 0x3a, 0x50, 0xcc, 0x32, 0xb1, 0x9a, 0xfe, 0x3f, 0xe6, 0x7d, 0x97, 0xd5, 0xf6, 0xd9, 0x4c, 0x6c, 0xea, 0xb3, 0xc6, 0x5e, 0x7e, 0xb0, 0xae, 0xdf, 0x69, 0x65, 0x72, 0x30, 0x6f, 0xf8, 0x90, 0x79, 0xaf, 0xad, 0xef, 0xea, 0x83, 0x73, 0x28, 0x5b, 0xf8, 0xbb, 0xf9, 0x5f, 0xa6, 0xe5, 0x32, 0x9d, 0xfd, 0xc8, 0xf6, 0x97, 0xab, 0xb7, 0x15, 0xf5, 0x11, 0xeb, 0x05, 0xea, 0x0d, 0x28, 0x6f, 0x0c, 0x12, 0x69, 0xa5, 0xdd, 0xbd, 0x43, 0x7b, 0x27, 0x33, 0xc6, 0xb9, 0xa2, 0xf2, 0x80, 0xf6, 0x8f, 0x91, 0x7b, 0xc3, 0xcd, 0x1f, 0x47, 0xc0, 0x3c, 0x07, 0x47, 0x4b, 0x79, 0x1c, 0x44, 0x10, 0x01, 0x76, 0xb3, 0x14, 0x5b, 0x09, 0x5d, 0x19, 0xba, 0x49, 0xc3, 0x43, 0x8a, 0x1c, 0x6b, 0xb9, 0x0c, 0x41, 0x90, 0x3f, 0xd2, 0x9a, 0x40, 0x65, 0x79, 0xde, 0x93, 0xe3, 0xab, 0x94, 0xd7, 0x6e, 0xcc, 0x6c, 0x9d, 0xc0, 0x27, 0xae, 0x81, 0xb1, 0xf2, 0xd1, 0xa4, 0x78, 0x12, 0xe6, 0x58, 0x7e, 0x35, 0x2d, 0xc5, 0x3c, 0xd3, 0x43, 0xbd, 0xfe, 0xe7, 0x90, 0xea, 0xbf, 0x5f, 0xfc, 0x33, 0xa5, 0x1c, 0x41, 0x90, 0xe3, 0xaf, 0x3e, 0x4e, 0x0f, 0xbd, 0x71, 0x3b, 0x07, 0xc1, 0xa1, 0x2b, 0x40, 0xca, 0x50, 0x8e, 0x80, 0xe8, 0x1f, 0xf9, 0xf0, 0x88, 0xa5, 0x35, 0xd4, 0x3e, 0x04, 0x3a, 0x9a, 0xa5, 0xc2, 0x32, 0x0c, 0x28, 0x16, 0xd0, 0xe8, 0x04, 0x5c, 0xb1, 0x6d, 0xa2, 0x3b, 0xf4, 0x31, 0xa4, 0x54, 0xb4, 0x3b, 0x74, 0x8c, 0xf7, 0x9b, 0x84, 0xe7, 0xca, 0xfe, 0x19, 0x0e, 0x10, 0x9b, 0x8e, 0x06, 0x22, 0x2c, 0x69, 0x9c, 0xf6, 0xc7, 0xf7, 0xbf, 0xb5, 0x73, 0xf2, 0xf3, 0x9c, 0x3f, 0xae, 0x08, 0xb9, 0x52, 0x1f, 0xcd, 0x74, 0x68, 0xed, 0xab, 0x10, 0x74, 0x04, 0x68, 0xd0, 0xf9, 0xcb, 0xeb, 0x84, 0x70, 0x99, 0x0c, 0x89, 0xe8, 0xe0, 0xe8, 0xac, 0x76, 0x9f, 0x0a, 0x5d, 0x2a, 0xe5, 0x93, 0x98, 0xcc, 0x56, 0x15, 0xcf, 0x31, 0x20, 0x17, 0x61, 0x57, 0x93, 0xf6, 0x03, 0x68, 0x4d, 0x82, 0x0c, 0xc0, 0xed, 0xa4, 0xf9, 0xcf, 0x3c, 0xc3, 0x03, 0xc3, 0xc7, 0xb2, 0xc8, 0xf9, 0x71, 0x95, 0xae, 0xd1, 0x2a, 0x3b, 0x45, 0xaf, 0x2c, 0x29, 0xd8, 0x12, 0x8b, 0xa5, 0xd4, 0x7b, 0x2e, 0xb6, 0xc6, 0x17, 0x02, 0xd5, 0x66, 0xf0, 0x64, 0x4f, 0x53, 0x64, 0xe3, 0xc0, 0xe8, 0xc2, 0x1c, 0x17, 0x31, 0xab, 0x44, 0x1f, 0x53, 0x53, 0xc3, 0xaa, 0x13, 0x73, 0xa6, 0x44, 0x21, 0x2e, 0x86, 0x5f, 0x59, 0x44, 0x5d, 0xe9, 0x7d, 0xba, 0x27, 0x0a, 0x3a, 0x61, 0xdb, 0x75, 0x3d, 0x67, 0x2c, 0xe6, 0x25, 0x17, 0x54, 0x5b, 0x5f, 0x40, 0x52, 0xbe, 0xb8, 0x4f, 0x24, 0x29, 0xf1, 0x68, 0xec, 0x70, 0x6c, 0xfd, 0x0e, 0xb5, 0x33, 0x44, 0xba, 0x0d, 0x97, 0x73, 0x07, 0xe9, 0xad, 0x3a, 0x6e, 0x3b, 0xb3, 0x06, 0xdb, 0x85, 0xed, 0x00, 0x0f, 0xf3, 0x74, 0x27, 0x81, 0x0a, 0xa1, 0x80, 0xcd, 0xff, 0xe8, 0xec, 0x93, 0xd3, 0xa8, 0xc7, 0xdb, 0x7e, 0x25, 0xc5, 0xd0, 0x81, 0xb9, 0x6e, 0x43, 0xa4, 0xc3, 0x42, 0x6e, 0xc8, 0xfc, 0x06, 0x36, 0x9c, 0x14, 0x19, 0x86, 0x16, 0x29, 0xa2, 0x97, 0x44, 0x9e, 0x0d, 0x6a, 0x39, 0x36, 0x73, 0x82, 0x29, 0xea, 0x90, 0xde, 0x8c, 0x5d, 0x52, 0x33, 0xf9, 0x41, 0x33, 0x69, 0x41, 0x62, 0x99, 0x02, 0xfa, 0xa8, 0x45, 0x90, 0xa0, 0xbd, 0xce, 0x8b, 0xb5, 0xa4, 0xd2, 0x5a, 0xfc, 0xe4, 0x00, 0x50, 0xdf, 0xdc, 0x66};
const int ECU_UPDATE_ONE_SIZE = 1079;

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

static void HW_init(void);
static void FMPLL_init(void);
static void DisableWatchdog(void);

int keyExchange(unsigned char *pkx, unsigned char *pky, unsigned char *skx, unsigned char *sky, unsigned char *ssxy, unsigned char *ssyx);
int askUpdate(unsigned char *message, crypto_uint16 messagelen,unsigned char *nonce, unsigned char *encryptedmessage, unsigned char *sharedsecretxy, unsigned char *sharedsecretyx);
int askVersion(unsigned char *message, crypto_uint16 messagelen,unsigned char *nonce, unsigned char *encryptedmessage, unsigned char *sharedsecretxy, unsigned char *sharedsecretyx);
int respondVersion(unsigned char *message, crypto_uint16 messagelen,unsigned char *nonce, unsigned char *encryptedmessage, unsigned char *sharedsecretxy, unsigned char *sharedsecretyx);
int respondUpdate(unsigned char *message, crypto_uint16 messagelen,unsigned char *nonce, unsigned char *encryptedmessage, unsigned char *sharedsecretxy, unsigned char *sharedsecretyx);
int askFirmware(unsigned char *message, crypto_uint16 messagelen,unsigned char *nonce, unsigned char *encryptedmessage, unsigned char *sharedsecretxy, unsigned char *sharedsecretyx);
int sendFirmware(unsigned char *message, crypto_uint16 messagelen,unsigned char *nonce, unsigned char *encryptedmessage, unsigned char *sharedsecretxy, unsigned char *sharedsecretyx);
int confirmFirmware(unsigned char *message, crypto_uint16 messagelen,unsigned char *nonce, unsigned char *encryptedmessage, unsigned char *sharedsecretxy, unsigned char *sharedsecretyx);

/*******************************************************************************
* Local functions
*******************************************************************************/

/*******************************************************************************
Function Name : HW_init 
Engineer      : b05111
Date          : May-11-2015
Parameters    : 
Modifies      : 
Returns       : 
Notes         :                             
Issues        : 
*******************************************************************************/
static void HW_init(void)
{
    DisableWatchdog();
    FMPLL_init();
}


/*******************************************************************************
Function Name : DisableWatchdog
Engineer      : b08110
Date          : Mar-14-2011
Parameters    : 
Modifies      : 
Returns       : 
Notes         : disable e200 Core Watchdog Timer
Issues        : 
*******************************************************************************/
static void DisableWatchdog(void)
{    

    asm
    {
        mfhid0  r11		  	  /* Move from spr HID0 to r11 (copies HID0) */
        li      r12, 0xBFFF  /* Load immed. data of ~0x4000 to r12 */
        oris    r12, r12, 0xFFFF
        and     r11, r12, r11  /* OR r12 (~0x00004000) with r11 (HID0 value) */
        mthid0  r11          /* Move result to HID0 */

        lis     r12, 0x00
        mtspr   tcr, r12  /* clear SPR TCR register */
    }
}


/*******************************************************************************
Function Name : FMPLL_init
Engineer      : b05111
Date          : May-11-2015
Parameters    : 
Modifies      : 
Returns       : 
Notes         : - Initializes PLL for 144/132/112/80/12MHz system clock 
                - used crystal is 8 MHz                        
Issues        : - macros are defined in clock.h
                - if none exist, 12MHz used (configured by BAM by default)
*******************************************************************************/
static void FMPLL_init(void)
{								
    #if defined(CORE_CLOCK_144MHz)
    FMPLL.SYNCR.R = 0x07200000;                  /* Fsys =9Mhz */								
    while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */								
    FMPLL.SYNCR.R = 0x07180000;                  /* Fsys =18Mhz */								
    FMPLL.SYNCR.R = 0x07100000;                  /* Fsys =36Mhz */								
    FMPLL.SYNCR.R = 0x07080000;                  /* Fsys =72Mhz */								
    FMPLL.SYNCR.R = 0x07000000;                  /* Fsys =144Mhz */														
    printf("fsys = 144MHz\n\r");

    #elif defined(CORE_CLOCK_132MHz)
    FMPLL.SYNCR.R = 0x01200000;                  /* Fsys =3Mhz */								
    FMPLL.SYNCR.R = 0x1EA00000;                  /* Fsys =8,25Mhz */								
    while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */								
    FMPLL.SYNCR.R = 0x1E980000;                  /* Fsys =16,5Mhz */								
    FMPLL.SYNCR.R = 0x1E900000;                  /* Fsys =33Mhz */								
    FMPLL.SYNCR.R = 0x1E880000;                  /* Fsys =66Mhz */								
    FMPLL.SYNCR.R = 0x1E800000;                  /* Fsys =132Mhz */
    
    #elif defined(CORE_CLOCK_112MHz)
    FMPLL.SYNCR.R = 0x05200000;                  /* Fsys =7Mhz */								
    while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */								
    FMPLL.SYNCR.R = 0x05180000;                  /* Fsys =14Mhz */								
    FMPLL.SYNCR.R = 0x05100000;                  /* Fsys =28Mhz */								
    FMPLL.SYNCR.R = 0x05080000;                  /* Fsys =56Mhz */								
    FMPLL.SYNCR.R = 0x05000000;                  /* Fsys =112Mhz */						
    
    #elif defined(CORE_CLOCK_80MHz)
    FMPLL.SYNCR.R = 0x03200000;                  /* Fsys =5Mhz */								
    while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */								
    FMPLL.SYNCR.R = 0x03180000;                  /* Fsys =10Mhz */								
    FMPLL.SYNCR.R = 0x03100000;                  /* Fsys =20Mhz */								
    FMPLL.SYNCR.R = 0x03080000;                  /* Fsys =40Mhz */								
    FMPLL.SYNCR.R = 0x03000000;                  /* Fsys =80Mhz */					
	  
    #else
    #endif	
      								
}

/*******************************************************************************
* Main function
*******************************************************************************/
int main(void)
{
    HW_init();

    
    crypto_uint16 mlen = 0;
    unsigned char *sm; 
    unsigned char *m;
    crypto_uint16 smlen = 0;
    static unsigned char *pkx;
    static unsigned char *skx;
    static unsigned char *pky;
    static unsigned char *sky;
    unsigned char *nonce;
    unsigned char *em;
  
    pkx = (unsigned char*)calloc(crypto_sign_PUBLICKEYBYTES+32,1);
    if (!pkx) fail("allocation of pkx failed");
    skx = (unsigned char*)calloc(crypto_sign_SECRETKEYBYTES+32,1);
    if (!skx) fail("allocation of skx failed");
    pky = (unsigned char*)calloc(crypto_sign_PUBLICKEYBYTES+32,1);
    if (!pky) fail("allocation of pky failed");
    sky = (unsigned char*)calloc(crypto_sign_SECRETKEYBYTES+32,1);
    if (!sky) fail("allocation of sky failed");
  
    pkx += 16;
    skx += 16;
    pky += 16;
    sky += 16;  
  
    unsigned char portalsk[32] = {
      0x08,0xa2,0x21,0x3e,0xe7,0x3a,0xc0,0xad,0x05,0x78,0xc6,0x1d,0xc5,0x4b,0xca,0x9f
      ,0x8a,0x05,0x4f,0xdb,0xd9,0x8e,0xe5,0xc2,0x83,0xeb,0xa0,0x60,0xd8,0x62,0x93,0x75
    };
  
    unsigned char portalpk[32] = {
   	  0x0a,0xde,0xcc,0x66,0x6e,0x84,0xb4,0x22,0xc8,0x1c,0x90,0x20,0x04,0x06,0x10,0x18
      ,0x76,0x1c,0x88,0xca,0x62,0x26,0xd4,0xf4,0x4a,0x6e,0x72,0xaa,0xd0,0x8a,0x4a,0xb4
    };
  
    unsigned char ecusk[32] = {
      0x08,0xa2,0x21,0x3e,0xe7,0x3a,0xc0,0xad,0x05,0x78,0xc6,0x1d,0xc5,0x4b,0xca,0x9f
      ,0x8a,0x05,0x4f,0xdb,0xd9,0x8e,0xe5,0xc2,0x83,0xeb,0xa0,0x60,0xd8,0x62,0x93,0x75
    };
  
    unsigned char ecupk[32] = {
      0x0a,0xde,0xcc,0x66,0x6e,0x84,0xb4,0x22,0xc8,0x1c,0x90,0x20,0x04,0x06,0x10,0x18
      ,0x76,0x1c,0x88,0xca,0x62,0x26,0xd4,0xf4,0x4a,0x6e,0x72,0xaa,0xd0,0x8a,0x4a,0xb4
    };

    unsigned char tcusk[32] = {
      0x48,0x9f,0xbe,0xd8,0x5b,0x92,0xde,0x2c,0xfe,0xa4,0x25,0x1d,0xe2,0x0e,0x5b,0x18
      ,0xc6,0x01,0x6e,0x45,0x2b,0x27,0x51,0x65,0xbb,0xc7,0x25,0xc2,0xe9,0x3a,0x2c,0x5c
    };
  
    unsigned char tcupk[32] = {
      0xd0,0x40,0x90,0xb0,0x80,0xd0,0x50,0x30,0x10,0xa0,0x00,0x00,0x20,0xf0,0x50,0x10
      ,0x30,0x80,0x80,0xe0,0x80,0xc0,0x90,0x80,0x00,0xc0,0x60,0x40,0x90,0x20,0x60,0x00
    };
  
    /* load corresponding keys for key exchange */
    /* in this case, the portal and tcu */
  
    for (int i = 0; i < 32; i++) 
    {
      skx[i] = portalsk[i];
  	  pkx[i] = portalpk[i];
  	  sky[i] = tcusk[i];
  	  pky[i] = tcupk[i];
    }

    static unsigned char *ssxy;
    static unsigned char *ssyx;
    static unsigned char *ssyz;
    static unsigned char *sszy;
    
    ssxy = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!ssxy) fail("allocation of ecskx failed");
    ssyx = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!ssyx) fail("allocation of ecskx failed");
    ssyz = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!ssyz) fail("allocation of ecskx failed");
    sszy = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!sszy) fail("allocation of ecskx failed");
    
    ssxy += 16;
    ssyz += 16;
    ssyz += 16;
    sszy += 16;
    
    if (keyExchange(pkx, pky, skx, sky, ssxy, ssyx) != 0)
    {
    	fail("keyExchange command failed");
    }
	
    if (askUpdate(m, mlen, nonce, em, ssyx, ssyx) != 0)
    {
        fail("askUdate command failed");
    }
    if (askVersion(m, mlen, nonce, em, ssxy, ssyx) != 0)
    {
        fail("askVersion command failed");
    }
    if (respondVersion(m, mlen, nonce, em, ssxy, ssyx) != 0)
    {
        fail("respondVersion command failed");
    }
    if (respondUpdate(m, mlen, nonce, em, ssxy, ssyx) != 0)
    {
        fail("respondUpdate command failed");
    }
    if (askFirmware(m, mlen, nonce, em, ssxy, ssyx) != 0)
    {
        fail("askFirmware command failed");
    }
    
    /* load corresponding keys for key exchange */
    /* in this case, the tcu and ecu */
    
    for (int i = 0; i < 32; i++) 
    {
      skx[i] = tcusk[i];
  	  pkx[i] = tcupk[i];
  	  sky[i] = ecusk[i];
  	  pky[i] = ecupk[i];
    }
    
    if (keyExchange(pkx, pky, skx, sky, ssyz, sszy) != 0)
    {
    	fail("keyExchange command failed");
    }
    
    if (sendFirmware(m, mlen, nonce, em, ssyz, sszy) != 0)
    {
        fail("sendFirmware command failed");
    }
    if (confirmFirmware(m, mlen, nonce, em, sszy, sszy) != 0)
    {
        fail("confirmFirmware command failed");
    }
    return 0;
}

/*
  Performs the key exchange between initiator x and responder y
  The shared keys are the only variables that need to be saved, so initialized pointers are used.
  returns 0 on success.
*/
int keyExchange(unsigned char *pkx, unsigned char *pky, unsigned char *skx, unsigned char *sky, unsigned char *ssxy, unsigned char *ssyx)
{
    unsigned char *m;
    unsigned char *sm;
    crypto_uint16 mlen = 0;
    crypto_uint16 smlen = 0;
    
   	static unsigned char *ecskx;
    static unsigned char *ecpkx;
    static unsigned char *ecsky;
    static unsigned char *ecpky;
    
    
    ecskx = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!ecskx) fail("allocation of ecskx failed");
    ecpkx = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!ecpkx) fail("allocation of ecpkx failed");
    ecsky = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!ecsky) fail("allocation of ecsky failed");
    ecpky = (unsigned char*)calloc(crypto_scalarmult_BYTES+32,1);
    if (!ecpky) fail("allocation of ecpky failed");
    
    ecskx += 16;
    ecpkx += 16;
    ecsky += 16;
    ecpky += 16;
    
	randombytes(ecskx, crypto_scalarmult_BYTES);
	
    if (crypto_scalarmult_base(ecpkx, ecskx) != 0)
	{
	    fail("generation of curve25519 keypair failed");
	    return -1;
	}
	
	mlen = FUNC_TYPE_LENGTH + crypto_scalarmult_BYTES;
	
	sm = (unsigned char*)calloc(mlen+crypto_sign_BYTES+32,1);
    if (!sm) fail("allocation of sm failed");
    
    sm += 16;
    
    m = (unsigned char*)calloc(mlen+32,1);
    if (!m) fail("allocation of m failed");
    
    m += 16;
    
    for (int i = 0;i < 2;++i) 
    {
        m[i] = FUNC_KEY_EXCHANGE[i];
    }
    
    for (int i = FUNC_TYPE_LENGTH;i < mlen;++i) 
    {
        m[i] = ecpkx[i-FUNC_TYPE_LENGTH];
    }
        
    if (crypto_sign(sm,&smlen,m,mlen,skx) != 0)
	{
		fail("signing of the message failed");
		return -1;
	}
	
	m -= 16;
	
	free(m);

	/* send */
	
	/* Responder part */
	
	/* receive */
	
    m = (unsigned char*)calloc(smlen+32,1);
    if (!m) fail("allocation of m failed");
    
    m += 16;
	

	if (crypto_sign_open(m,&mlen,sm,smlen,pkx) != 0)
	{
		fail("verification of m failed");
		return -1;
	}
	
	sm -= 16;
	free(sm);
	
	if (m[0] == FUNC_KEY_EXCHANGE[0] && m[1] == FUNC_KEY_EXCHANGE[1])
	{
		randombytes(ecsky, crypto_scalarmult_BYTES);
	    
	    if (crypto_scalarmult_base(ecpky, ecsky) != 0)
		{
		    fail("generation of curve25519 keypair failed");
		    return -1;
		}
		
		m += 2;
		
		if (crypto_scalarmult(ssyx, ecsky, m) != 0)
		{
			fail("generation of shared secret ssyx failed");
			return -1;
		}
		
		m -= 2;
		m -= 16;
		free(m);
		
		mlen = FUNC_TYPE_LENGTH + crypto_scalarmult_BYTES;
		
		m = (unsigned char*)calloc(mlen+32,1);
    	if (!m) fail("allocation of m failed");
    
    	m += 16;
    
	    for (int i = 0;i < 2;++i) 
	    {
	        m[i] = FUNC_KEY_EXCHANGE_CONT[i];
	    }
	    
	    for (int i = FUNC_TYPE_LENGTH;i < mlen;++i) 
	    {
	        m[i] = ecpky[i-FUNC_TYPE_LENGTH];
	    }
		
		sm = (unsigned char*)calloc(mlen+crypto_sign_BYTES+32,1);
	    if (!sm) fail("allocation of sm failed");
	    
	    sm += 16;
	    
	    if (crypto_sign(sm,&smlen,m,mlen,sky) != 0)
		{
			fail("signing of the message failed");
			return -1;
		}
		
		m -= 16;
		
		free(m);
	
		/* send */
	}
	
	/* Initiator cont */
	
	/* receive */
	
	m = (unsigned char*)calloc(smlen+32,1);
    if (!m) fail("allocation of m failed");
    
    m += 16;
	
	if (crypto_sign_open(m,&mlen,sm,smlen,pky) != 0)
	{
		fail("verification of m failed");
		return -1;
	}
	
	sm -= 16;
	free(sm);
	
	if (m[0] == FUNC_KEY_EXCHANGE_CONT[0] && m[1] == FUNC_KEY_EXCHANGE_CONT[1])
	{
		m += 2;
		if (crypto_scalarmult(ssxy, ecskx, m) != 0)
		{
			fail("generation of shared secret ssxy failed");
			return -1;
		}
		m -= 2;
		m -= 16;
		free(m);
	}
	ecskx -= 16;
    ecpkx -= 16;
    ecsky -= 16;
    ecpky -= 16;
    
    free(ecskx);
    free(ecpkx);
    free(ecsky);
    free(ecpky);
    
	return 0;
}

int pack(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecret)
{	
	
    randombytes(nonce, crypto_secretbox_NONCEBYTES);
    
    /* Check if message is 0-padded */
    for (int i = 0; i < crypto_secretbox_ZEROBYTES; ++i)
    {
    	if (message[i] != 0x00)
    	{
    		return -1;
    	}
    }
	if (crypto_secretbox(encryptedmessage,message,messagelen,nonce,sharedsecret) != 0) 
	{
		return -1;
	}
	
	return 0;
}

int unpack(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecret)
{
    /* Check if encrypted message is 0-padded */
    for (int i = 0; i < crypto_secretbox_BOXZEROBYTES; ++i)
    {
    	if (encryptedmessage[i] != 0x00)
    	{
    		return -1;
    	}
    }
	if (crypto_secretbox_open(message,encryptedmessage,messagelen,nonce,sharedsecret) != 0) 
	{
		return -1;
	}
	return 0;
}

int askUpdate(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecretxy,
          unsigned char *sharedsecretyx)
{
    messagelen = FUNC_TYPE_LENGTH + crypto_secretbox_ZEROBYTES;
    
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of m failed");
    	return -1;
    }
    
    message += 16;
    message += crypto_secretbox_ZEROBYTES;
    
	for (int i = 0;i < 2;++i) 
	{
	    message[i] = FUNC_ASK_UPDATE[i];
	}
	message -= crypto_secretbox_ZEROBYTES;
	
	encryptedmessage = (unsigned char*)calloc(messagelen+32,1);
	if (!encryptedmessage) fail("allocation of encryptedmessage failed");
	
	encryptedmessage += 16;
	
	nonce = (unsigned char*)calloc(crypto_secretbox_NONCEBYTES+32,1);
	if (!nonce) fail("allocation of nonce failed");
	nonce += 16;
	
	if (pack(message, messagelen, nonce, encryptedmessage, sharedsecretyx) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message -= 16;
	free(message);
	
	/* send */
	
	/* receive */
	
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of m failed");
    	return -1;
    }
    
    message += 16;
    
	if (unpack(message, messagelen, nonce, encryptedmessage, sharedsecretxy) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message += crypto_secretbox_ZEROBYTES;
	
	if (message[0] == FUNC_ASK_UPDATE[0] && message[1] == FUNC_ASK_UPDATE[1])
	{
	    /* process */
		return 0;
	}
	return -1;
}

int askVersion(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecretxy,
          unsigned char *sharedsecretyx)
{
    messagelen = FUNC_TYPE_LENGTH + crypto_secretbox_ZEROBYTES;
    
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of m failed");
    	return -1;
    }
    
    message += 16;
    message += crypto_secretbox_ZEROBYTES;
    
	for (int i = 0;i < 2;++i) 
	{
	    message[i] = FUNC_ASK_VERSION[i];
	}
	message -= crypto_secretbox_ZEROBYTES;
	
	encryptedmessage = (unsigned char*)calloc(messagelen+32,1);
	if (!encryptedmessage) fail("allocation of encryptedmessage failed");
	
	encryptedmessage += 16;
	
	nonce = (unsigned char*)calloc(crypto_secretbox_NONCEBYTES+32,1);
	if (!nonce) fail("allocation of nonce failed");
	nonce += 16;
	
	if (pack(message, messagelen, nonce, encryptedmessage, sharedsecretxy) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message -= 16;
	free(message);
	
	/* send */
	
	/* receive */
	
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    
	if (unpack(message, messagelen, nonce, encryptedmessage, sharedsecretyx) != 0)
	{
		fail("unpacking of encryptedmessage to message failed");
		return -1;
	}
	
	message += crypto_secretbox_ZEROBYTES;
	
	if (message[0] == FUNC_ASK_VERSION[0] && message[1] == FUNC_ASK_VERSION[1])
	{
	    /* process */
		return 0;
	}
	return -1;
}

int respondVersion(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecretxy,
          unsigned char *sharedsecretyx)
{
    unsigned char ecuversions[12] = {0x44, 0x2A, 0x00, 0xC8, 
                                     0x29, 0x01, 0x00, 0x6A,
                                     0x61, 0x85, 0x00, 0x1E};
    int ecuversionssize = 12;
    
	messagelen = FUNC_TYPE_LENGTH + crypto_secretbox_ZEROBYTES + ecuversionssize;
    
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    message += crypto_secretbox_ZEROBYTES;
    
	for (int i = 0;i < 2;++i) 
	{
	    message[i] = FUNC_RESPOND_VERSION[i];
	}
	
	message += FUNC_TYPE_LENGTH;
	for (int i = 0;i < ecuversionssize;++i) 
	{
	    message[i] = ecuversions[i];
	}
	message -= FUNC_TYPE_LENGTH;
	message -= crypto_secretbox_ZEROBYTES;	
	
	encryptedmessage = (unsigned char*)calloc(messagelen+32,1);
	if (!encryptedmessage) fail("allocation of encryptedmessage failed");
	
	encryptedmessage += 16;
	
	nonce = (unsigned char*)calloc(crypto_secretbox_NONCEBYTES+32,1);
	if (!nonce) fail("allocation of nonce failed");
	nonce += 16;
	
	if (pack(message, messagelen, nonce, encryptedmessage, sharedsecretyx) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message -= 16;
	free(message);
	
	/* send */
	
	/* receive */
	
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    
	if (unpack(message, messagelen, nonce, encryptedmessage, sharedsecretxy) != 0)
	{
		fail("unpacking of encryptedmessage to message failed");
		return -1;
	}
	
	message += crypto_secretbox_ZEROBYTES;
	
	if (message[0] == FUNC_RESPOND_VERSION[0] && message[1] == FUNC_RESPOND_VERSION[1])
	{
	    /* process */
		return 0;
	}
	return -1;
}

int respondUpdate(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecretxy,
          unsigned char *sharedsecretyx)
{
    unsigned char ecus[4] = {0x44, 0x2A, 0x29, 0x01};
    
    int ecusize = 4;
    
	messagelen = FUNC_TYPE_LENGTH + crypto_secretbox_ZEROBYTES + ecusize;
    
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    message += crypto_secretbox_ZEROBYTES;
    
	for (int i = 0;i < 2;++i) 
	{
	    message[i] = FUNC_RESPOND_UPDATE[i];
	}
	
	message += FUNC_TYPE_LENGTH;
	for (int i = 0;i < ecusize;++i) 
	{
	    message[i] = ecus[i];
	}
	message -= FUNC_TYPE_LENGTH;
	message -= crypto_secretbox_ZEROBYTES;	
	
	encryptedmessage = (unsigned char*)calloc(messagelen+32,1);
	if (!encryptedmessage) fail("allocation of encryptedmessage failed");
	
	encryptedmessage += 16;
	
	nonce = (unsigned char*)calloc(crypto_secretbox_NONCEBYTES+32,1);
	if (!nonce) fail("allocation of nonce failed");
	nonce += 16;
	
	if (pack(message, messagelen, nonce, encryptedmessage, sharedsecretxy) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message -= 16;
	free(message);
	
	/* send */
	
	/*  receive */
	
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    
	if (unpack(message, messagelen, nonce, encryptedmessage, sharedsecretyx) != 0)
	{
		fail("unpacking of encryptedmessage to message failed");
		return -1;
	}
	
	message += crypto_secretbox_ZEROBYTES;
	
	if (message[0] == FUNC_RESPOND_UPDATE[0] && message[1] == FUNC_RESPOND_UPDATE[1])
	{
	    /* process */
		return 0;
	}
	return -1;
}

int askFirmware(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecretxy,
          unsigned char *sharedsecretyx)
{
    unsigned char ecuversions[8] = {0x44, 0x2A, 0x00, 0xC8, 
                                     0x29, 0x01, 0x00, 0x6A};
    int ecuversionssize = 8;
    
	messagelen = FUNC_TYPE_LENGTH + crypto_secretbox_ZEROBYTES + ecuversionssize;
    
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    message += crypto_secretbox_ZEROBYTES;
    
	for (int i = 0;i < 2;++i) 
	{
	    message[i] = FUNC_ASK_UPDATE[i];
	}
	
	message += FUNC_TYPE_LENGTH;
	for (int i = 0;i < ecuversionssize;++i) 
	{
	    message[i] = ecuversions[i];
	}
	message -= FUNC_TYPE_LENGTH;
	message -= crypto_secretbox_ZEROBYTES;	
	
	encryptedmessage = (unsigned char*)calloc(messagelen+32,1);
	if (!encryptedmessage) fail("allocation of encryptedmessage failed");
	
	encryptedmessage += 16;
	
	nonce = (unsigned char*)calloc(crypto_secretbox_NONCEBYTES+32,1);
	if (!nonce) fail("allocation of nonce failed");
	nonce += 16;
	
	if (pack(message, messagelen, nonce, encryptedmessage, sharedsecretyx) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message -= 16;
	free(message);
	
	/* send */
	
	/* receive */
	
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    
	if (unpack(message, messagelen, nonce, encryptedmessage, sharedsecretxy) != 0)
	{
		fail("unpacking of encryptedmessage to message failed");
		return -1;
	}
	
	message += crypto_secretbox_ZEROBYTES;
	
	if (message[0] == FUNC_ASK_UPDATE[0] && message[1] == FUNC_ASK_UPDATE[1])
	{
	    /* process */
		return 0;
	}
	return -1;
}

int sendFirmware(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecretxy,
          unsigned char *sharedsecretyx)
{ 
	messagelen = FUNC_TYPE_LENGTH + crypto_secretbox_ZEROBYTES + ECU_UPDATE_ONE_SIZE;
    
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    message += crypto_secretbox_ZEROBYTES;
    
	for (int i = 0;i < 2;++i) 
	{
	    message[i] = FUNC_SEND_FIRMWARE[i];
	}
	
	message += FUNC_TYPE_LENGTH;
	for (int i = 0;i < ECU_UPDATE_ONE_SIZE;++i) 
	{
	    message[i] = ECU_UPDATE_ONE[i];
	}
	message -= FUNC_TYPE_LENGTH;
	message -= crypto_secretbox_ZEROBYTES;	
	
	encryptedmessage = (unsigned char*)calloc(messagelen+32,1);
	if (!encryptedmessage) fail("allocation of encryptedmessage failed");
	
	encryptedmessage += 16;
	
	nonce = (unsigned char*)calloc(crypto_secretbox_NONCEBYTES+32,1);
	if (!nonce) fail("allocation of nonce failed");
	nonce += 16;
	
	if (pack(message, messagelen, nonce, encryptedmessage, sharedsecretxy) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message -= 16;
	free(message);
	
	/* send */
	
	/* receive */
	
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    
	if (unpack(message, messagelen, nonce, encryptedmessage, sharedsecretyx) != 0)
	{
		fail("unpacking of encryptedmessage to message failed");
		return -1;
	}
	
	message += crypto_secretbox_ZEROBYTES;
	
	if (message[0] == FUNC_SEND_FIRMWARE[0] && message[1] == FUNC_SEND_FIRMWARE[1])
	{
	    /* process */
		return 0;
	}
	return -1;
}

int confirmFirmware(unsigned char *message, 
          crypto_uint16 messagelen,
          unsigned char *nonce, 
          unsigned char *encryptedmessage, 
          unsigned char *sharedsecretxy,
          unsigned char *sharedsecretyx)
{
    unsigned char ecuversions[8] = {0x44, 0x2A, 0x00, 0xC8, 
                                     0x29, 0x01, 0x00, 0x6A};
    int ecuversionssize = 8;
    
	messagelen = FUNC_TYPE_LENGTH + crypto_secretbox_ZEROBYTES + ecuversionssize;
    
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    message += crypto_secretbox_ZEROBYTES;
    
	for (int i = 0;i < 2;++i) 
	{
	    message[i] = FUNC_CONFIRM_FIRMWARE[i];
	}
	
	message += FUNC_TYPE_LENGTH;
	for (int i = 0;i < ecuversionssize;++i) 
	{
	    message[i] = ecuversions[i];
	}
	message -= FUNC_TYPE_LENGTH;
	message -= crypto_secretbox_ZEROBYTES;	
	
	encryptedmessage = (unsigned char*)calloc(messagelen+32,1);
	if (!encryptedmessage) fail("allocation of encryptedmessage failed");
	
	encryptedmessage += 16;
	
	nonce = (unsigned char*)calloc(crypto_secretbox_NONCEBYTES+32,1);
	if (!nonce) fail("allocation of nonce failed");
	nonce += 16;
	
	if (pack(message, messagelen, nonce, encryptedmessage, sharedsecretxy) != 0)
	{
		fail("packing of message to encryptedmessage failed");
		return -1;
	}
	
	message -= 16;
	free(message);
	
	/* send */
	
	/* receive */
	
    message = (unsigned char*)calloc(messagelen+32,1);
    if (!message) 
    {
    	fail("allocation of message failed");
    	return -1;
    }
    
    message += 16;
    
	if (unpack(message, messagelen, nonce, encryptedmessage, sharedsecretyx) != 0)
	{
		fail("unpacking of encryptedmessage to message failed");
		return -1;
	}
	
	message += crypto_secretbox_ZEROBYTES;
	
	if (message[0] == FUNC_CONFIRM_FIRMWARE[0] && message[1] == FUNC_CONFIRM_FIRMWARE[1])
	{
	    /* process */
		return 0;
	}
	return -1;
}