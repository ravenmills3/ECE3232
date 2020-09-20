/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "stdio.h"
#include "stdbool.h"
/* DACOutputWave Written By Farhan Farooq(farhanfarooq93@gmail.com)
 * Cite:
 * 10kHz Sine Wave Generator on DAC / PIT Using FR...
 * NXP Community, Farhan Farooq, 27 Sept. 2019, community.nxp.com/message/1211888.
 */
static const uint16_t DACOutputWave[256] = {
        2047U,  2097U,  2147U,  2198U,  2248U,  2298U,  2347U,  2397U,  2446U,  2496U,  2544U,  2593U,  2641U,  2689U,
        2737U,  2784U,  2830U,  2877U,  2922U,  2967U,  3012U,  3056U,  3099U,  3142U,  3184U,  3226U,  3266U,  3306U,
        3346U,  3384U,  3422U,  3458U,  3494U,  3530U,  3564U,  3597U,  3629U,  3661U,  3691U,  3721U,  3749U,  3776U,
        3803U,  3828U,  3852U,  3875U,  3897U,  3918U,  3938U,  3957U,  3974U,  3991U,  4006U,  4020U,  4033U,  4044U,
        4055U,  4064U,  4072U,  4079U,  4084U,  4088U,  4092U,  4093U,  4094U,  4093U,  4092U,  4088U,  4084U,  4079U,
        4072U,  4064U,  4055U,  4044U,  4033U,  4020U,  4006U,  3991U,  3974U,  3957U,  3938U,  3918U,  3897U,  3875U,
        3852U,  3828U,  3803U,  3776U,  3749U,  3721U,  3691U,  3661U,  3629U,  3597U,  3564U,  3530U,  3494U,  3458U,
        3422U,  3384U,  3346U,  3306U,  3266U,  3226U,  3184U,  3142U,  3099U,  3056U,  3012U,  2967U,  2922U,  2877U,
        2830U,  2784U,  2737U,  2689U,  2641U,  2593U,  2544U,  2496U,  2446U,  2397U,  2347U,  2298U,  2248U,  2198U,
        2147U,  2097U,  2047U,  1997U,  1947U,  1896U,  1846U,  1796U,  1747U,  1697U,  1648U,  1598U,  1550U,  1501U,
        1453U,  1405U,  1357U,  1310U,  1264U,  1217U,  1172U,  1127U,  1082U,  1038U,  995U,   952U,   910U,   868U,
        828U,   788U,   748U,   710U,   672U,   636U,   600U,   564U,   530U,   497U,   465U,   433U,   403U,   373U,
        345U,   318U,   291U,   266U,   242U,   219U,   197U,   176U,   156U,   137U,   120U,   103U,   88U,    74U,
        61U,    50U,    39U,    30U,    22U,    15U,    10U,    6U,     2U,     1U,     0U,     1U,     2U,     6U,
        10U,    15U,    22U,    30U,    39U,    50U,    61U,    74U,    88U,    103U,   120U,   137U,   156U,   176U,
        197U,   219U,   242U,   266U,   291U,   318U,   345U,   373U,   403U,   433U,   465U,   497U,   530U,   564U,
        600U,   636U,   672U,   710U,   748U,   788U,   828U,   868U,   910U,   952U,   995U,   1038U,  1082U,  1127U,
        1172U,  1217U,  1264U,  1310U,  1357U,  1405U,  1453U,  1501U,  1550U,  1598U,  1648U,  1697U,  1747U,  1796U,
        1846U,  1896U,  1947U,  1997U
};

void InitSystem(){
    InitPutty();
	InitADC();
	InitDAC();
	InitPump();
	InitPIT();
    InitMessage();
}

void InitADC(){
//    Enable ADC0 module
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
//    Configure ADC0 for 16 bit conversion
    ADC0_CFG1 |= 0b11 << 2;
//    Enable PORTE module
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
//    Configure PTE24 for ADC
    PORTE_PCR24 |= PORT_PCR_MUX(0);
}

void InitPutty(){
//    Enable UART0 module
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
//    Enable clock for PTB
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
//    Configure UART0TX and UART0RX for PTB16 for PTB17
    PORTB_PCR16 |= PORT_PCR_MUX(3);
    PORTB_PCR17 |= PORT_PCR_MUX(3);
//    Disable Transmit Enable ad Receive Enable using UART0_C2
    UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
//    Set baud rate to 9600, no parity bit and 8 bit transmission and no interrupts
    UART0_BDL = 0x88;
    UART0_BDH = 0;
//    Enable Transmit Enable ad Receive Enable using UART0_C2
    UART0_C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
}

void InitDAC(){
//    Enable DAC0 module
    SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK;
//    Enable DAC0 system
    DAC0_C0 |= DAC_C0_DACEN_MASK;
//    Enable software trigger for DAC0
    DAC0_C0 |= DAC_C0_DACTRGSEL_MASK;
//    Set reference voltage to DACREF_2 = VDDA = 3.6V
    DAC0_C0 |= DAC_C0_DACRFS_MASK;
}

void InitPump(){
//    Enable PORT B module
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
//    Configure MUX for PCR
    PORTB_PCR10 |= PORT_PCR_MUX(1);
//  Configure PTB10 to output pin
    GPIOB_PDDR |= 0b1 << 23;
}

void InitPIT(){
// 	Enable PIT module
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
//	Enable PIT system clock
	PIT_MCR &= 0b0 << 1;
}

void InitMessage(){
    char line1[] = "Welcome to the Thirsty Pot System\n";
    char line2[] = "Please choose a type of plant:\n";
    char line3[] = "---------------------------------\n";
    char plantA[] = "A: Basil\n";
    char plantB[] = "B: Mint\n";
    char plantC[] = "C: Parsley\n";
    UART0_PutString(line1);
    UART0_PutString(line2);
    UART0_PutString(line3);
    UART0_PutString(plantA);
    UART0_PutString(plantB);
    UART0_PutString(plantC);
}

void OutputPump(int time) {
//    Output logic high on pump pin
    GPIOB_PSOR |= 0x01 << 10;
//    Delay high output on pin
    for (int i=0; i < time; i++);
}

void OutputDAC(uint32_t frequency, int time) {
//	Using PIT to change frequency of DAC output

// setup timer1 for frequency cycles
	PIT_LDVAL0 = frequency;
//	Start PIT timer
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;

//	DAC OUTPUT FOR OUTPUT TIME
	for (int j = 0; j < time; j++) {
	//	Output frequency on DAC
		int k=0;
		while(k < 256) {
			if((PIT_TFLG0 & 0x00000001) == 1){
				DAC0_DAT0L = (DACOutputWave[k] & 0xFF); 		 // set low values in DAC
				DAC0_DAT0H = ((DACOutputWave[k] & 0xF00) >> 8);	// set high values in DAC
				PIT_TFLG0 |= 0b1 << 0; // clear PIT_TFLG
			}
			k++;
		}

	}

//	STOP DAC OUTPUT
	DAC0_DAT0L = (0 & 0xFF); 		 // set low values in DAC
	DAC0_DAT0H = ((0 & 0xF00) >> 8); 	// set high values in DAC
}

uint16_t SampleADC() {
//    Select Channel for ADC
    ADC0_SC1A &= 0b10001;
    uint16_t sample;
//    wait until conversion is complete
    while ((ADC0_SC1A & 0b10000000) == 0);
//    store value from RA in sample
    sample = ADC0_RA;
    return (sample);
}

void UpdateSerialDisplay(int sample){
	char buf[100];
	snprintf(buf, 100, "Voltage from Plant Moisture Level: %d\n", sample);
	UART0_PutString(buf);
}

void UART0_PutChar(char character){
//    Wait until previous transmission is finished
    while ((UART0_S1 & 0b10000000) == 0);
//    Send character
    UART0_D = character;
//    Wait until transmission is finished before leaving PutChar
    while ((UART0_S1 & 0b01000000) == 0);
}

char UART0_GetChar(){
    char character;
//    Wait until previous character is written
    while ((UART0_S1 & 0b00100000) == 1);
    //    Write from UART0_D
		character = UART0_D;
    return (character);
}

void UART0_PutString(char* string){
//    Loop Through characters and call PutChar
    while (*string != '\0'){
        UART0_PutChar(*string);
        string++;
    }
}

bool CompareMoisture(uint16_t currentMoisture, uint16_t idealMoisture){
	if (currentMoisture < idealMoisture){
		return true;
	} else{
		return false;
	}
}

int main(void) {

    char plantChoice;
    char invalid[] = "The choice you entered is invalid\n";
    char plantA[] = "A: Basil\n";
    char plantB[] = "B: Mint\n";
    char plantC[] = "C: Parsley\n";
    char pumpOn[] = "Plant is thirsty. Turning water pump on now!\n";

    uint16_t currMoistureLevel;
    uint16_t moistureLevel;
    int outputTime;
    uint32_t soundFreq;

    //	For demonstration purposes output time will be set to 60 minutes, assuming clock period is 50MHz
	outputTime = 0x112A87F;

    InitSystem();

    choice:
        while (plantChoice == '\0') {
            plantChoice = UART0_GetChar();

        }
        plantChoice = toupper(plantChoice);

        if (plantChoice != 'A' && plantChoice != 'B' && plantChoice !='C') {
            UART0_PutString(invalid);
            UART0_PutString(plantA);
            UART0_PutString(plantB);
            UART0_PutString(plantC);
            plantChoice = '\0';

            goto choice;
        }

    char plant[100];
    switch (plantChoice) {
        case 'A' :
//            Basil is medium moisture level
        	snprintf(plant, 100, "The plant selected is Basil\n");
			moistureLevel = (uint16_t)0xD178;
//			Output frequency of 5000Hz
			soundFreq = 0x00000009;
             break;
        case 'B':
//            Mint is high moisture level
        	snprintf(plant, 100, "The plant selected is Mint\n");
            moistureLevel = (uint16_t)0xD253;
//          Output frequency of 6000Hz
            soundFreq = 0x00000007;
            break;
        case 'C':
//            Parsley is low moisture level
        	snprintf(plant, 100, "The plant selected is Parsley\n");
            moistureLevel = (uint16_t)0xC1F4;
//          Output frequency of 4000Hz
            soundFreq = 0x0000000C;
            break;
    }

    UART0_PutString(plant);

    while (1) {
        currMoistureLevel = SampleADC();
        bool needsWater = CompareMoisture(currMoistureLevel, moistureLevel);
        UpdateSerialDisplay(currMoistureLevel);

        if (needsWater){
        	UART0_PutString(pumpOn);
        	OutputPump(outputTime);
        	OutputDAC(soundFreq, outputTime);
        }

    }

    /* Leave main if user selects exit */

    return 0;

}
