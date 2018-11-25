#include <p24fxxxx.h>
#include <stdio.h>
#include <string.h>

#define RTS _RF13

// Setup configuration bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C2SEL_SEC)
#else
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif


/*UART*/

void InitUART() {

	U2BRG = 25;         //Set Baudrate
	U2STA = 0;           //0x0400 ?
	U2MODE = 0x8000; //Enable Uart for 8-bit data //no parity, 1 STOP bit
	U2STAbits.UTXEN = 1; //Enable Transmit

}

void writeChar(char c) {
	int i;
	U2TXREG = c;
	for (i = 0; i < 20000; i++);
}

void writeString(char *s) {
	//	return;
	int i;
	while (*s != '\0') {
		while (U2STAbits.UTXBF == 1);
		U2TXREG = *s;
		*s++;
	}
}

void getString(char* s, int size) {

}

void writeNewLine() {
	//return;
	while (U2STAbits.UTXBF == 1);
	U2TXREG = '\r';
	while (U2STAbits.UTXBF == 1);
	U2TXREG = '\n';
}

char getChar() {
	RTS = 0;
	while (!U2STAbits.URXDA);
	RTS = 1;
	return U2RXREG;
}

/*End UART*/

/* Init ADC*/

void InitADC() {
	AD1PCFG = 0xFFFB;
	AD1CON1 = 0x0000; // SSRC bit = 010 implies GP TMR3
					  // compare ends sampling and starts
					  // converting.
	AD1CSSL = 0;

	AD1CON3 = 0x0002; // Manual Sample, Tad = 2 Tcy
	AD1CON2 = 0;
	AD1CON1bits.ADON = 1; // turn ADC ON

}

int read_temperature() {

	int i;

	AD1CHS = 0x0002;
	AD1CON1bits.SAMP = 1; // start sampling
	for (i = 0; i < 100; i++);
	AD1CON1bits.SAMP = 0; // stop sampling and start conv
	while (!AD1CON1bits.DONE);    // conversion done?
	return ADC1BUF0;            // yes then get ADC value
}

void temperature(int t_init, int M1, int M2, int M3) {

	int i;
	int temp = read_temperature();
	char str [35];
	
	while(	temp > t_init + 25){
		
			sendByte_I2C(0x0C, 0x31); //31 M_3 off			
				for (i = 0; i < 2000; i++);
			sendByte_I2C(0x0B, 0x32); //32 M_2 off
				for (i = 0; i < 2000; i++);
			sendByte_I2C(0x0B, 0x31); //11 M_1 off
				for (i = 0; i < 2000; i++);
			sendByte_I2C(0x0E, 'R');  //led
				for (i = 0; i < 2000; i++);
			sendByte_I2C(0x0F, 'O');  //buzz on
				for (i = 0; i < 2000; i++);
			temp = read_temperature();
		}
	sendByte_I2C(0x0F, 'S');  //buzz off
				for (i = 0; i < 2000; i++);
	if(M1){
		sendByte_I2C(0x0B, 0x11); //11 M_1 on
		for (i = 0; i < 2000; i++);
			}
	if(M2){
		sendByte_I2C(0x0B, 0x12); //12 M_2 on
		for (i = 0; i < 2000; i++);
		}
	if(M3){
		sendByte_I2C(0x0C, 0x11); //31 M_3 on
		for (i = 0; i < 2000; i++);
	}
}

/*End ADC*/


void sendByte_I2C(int addr, int var) {

	//start I2C
	while (I2C2CONbits.SEN == 1 || I2C2CONbits.RSEN == 1 || I2C2CONbits.PEN == 1 || I2C2CONbits.RCEN == 1 || I2C2CONbits.ACKEN == 1); 
	//Wait till Start sequence is completed
	I2C2CONbits.SEN = 1;
	while (I2C2CONbits.SEN);

	//address
	I2C2TRN = (addr << 1); //to write
	while (I2C2STATbits.TRSTAT);
	while(I2C2STATbits.TBF);

	//data
	I2C2TRN = var;
	while (I2C2STATbits.TRSTAT);
	while(I2C2STATbits.TBF);

	//stop I2C
	I2C2CONbits.PEN = 1;
	while (I2C2CONbits.PEN);
}

int receiveByte_I2C(int addr) {

	//start I2C
	I2C2CONbits.SEN =1;
	while(I2C2CONbits.SEN);

	//address
	I2C2TRN = ((addr << 1) | 1);
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	
	//read byte
	I2C2CONbits.RCEN = 1;
	while(I2C2CONbits.RCEN );
	while(I2C2STATbits.RBF==0 );
	int var = I2C2RCV;

	//stop I2C
	I2C2CONbits.PEN = 1;
	while (I2C2CONbits.PEN);
	return var;
}

int receive_2Byte_I2C(int addr) {
	
	//start I2C
	I2C2CONbits.SEN = 1;
	while (I2C2CONbits.SEN);

	//address
	I2C2TRN = ((addr << 1) | 1);
	while (I2C2STATbits.TBF);
	while (I2C2STATbits.TRSTAT);

	//read lower byte
	I2C2CONbits.RCEN = 1;
	while (I2C2CONbits.RCEN);
	while (I2C2STATbits.RBF == 0);
	int var1 = I2C2RCV;

	//figure 24-10
	I2C2CONbits.ACKDT = 0;
	I2C2CONbits.ACKEN = 1;
	while (I2C2CONbits.ACKEN);

	//read upper byte
	I2C2CONbits.RCEN = 1;
	while (I2C2CONbits.RCEN);
	while (I2C2STATbits.RBF == 0);
	int var2 = I2C2RCV;

	//stop I2C
	I2C2CONbits.PEN = 1;
	while (I2C2CONbits.PEN);

	var2 = var2 << 8;
	var2 = var2 + var1;
	return var2;
}

void wait_idle() {
	while (I2C2CONbits.SEN || I2C2CONbits.RSEN || I2C2CONbits.RCEN || I2C2CONbits.PEN || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT) {};
}

void atraso(){
int j, k;
for(j=0;j<1000;j++)
	for(k=0;k<400;k++);
}

void __attribute__((interrupt, auto_psv)) _MI2C2Interrupt(void)
{
	IFS3bits.MI2C2IF = 0;
}

void panicB(int M1, int M2 , int M3, int init_temp){
int i;
	temperature(init_temp, M1, M2, M3);
	sendByte_I2C(0x0E, 'G');
	if (!PORTDbits.RD7) {
	//	writeString("000000 \n\r");
		while (!PORTDbits.RD7);		
		//while (!PORTDbits.RD7);		
	//	writeString("------ \n\r");
		sendByte_I2C(0x0C, 0x31); //31 M_3 off			
			for (i = 0; i < 2000; i++);
		sendByte_I2C(0x0B, 0x32); //32 M_2 off
			for (i = 0; i < 2000; i++);
		sendByte_I2C(0x0B, 0x31); //11 M_1 off
			for (i = 0; i < 2000; i++);
		sendByte_I2C(0x0E, 'Y');  //led
			for (i = 0; i < 2000; i++);

	//	writeString("111111 \n\r");
		//while(!PORTDbits.RD7);//não será normal?
		while (PORTDbits.RD7){temperature(init_temp, 0, 0, 0);sendByte_I2C(0x0E, 'Y');}
		while (!PORTDbits.RD7){temperature(init_temp, 0, 0, 0);sendByte_I2C(0x0E, 'Y');
	}
	//	writeString("222222 \n\r");
		sendByte_I2C(0x0E, 'G');  //led
			for (i = 0; i < 2000; i++);	

		if(M1){
			sendByte_I2C(0x0B, 0x11); //11 M_1 on
			for (i = 0; i < 2000; i++);
				}
		if(M2){
			sendByte_I2C(0x0B, 0x12); //12 M_2 on
			for (i = 0; i < 2000; i++);
			}
		if(M3){
			sendByte_I2C(0x0C, 0x11); //31 M_3 on
			for (i = 0; i < 2000; i++);
		}
	}
}
/*Initialize I2C*/
void initialize_I2C() {
	I2C2CON = 0x8000; //-enable bit
	I2C2BRG = 39;
}

int main(void)
{

	InitADC();
	InitUART();
	writeString("\n\riniciar1\n\r");
	initialize_I2C();
	int var = 1;
	char str[50];
	int count_piece = 0;
	int count_piece_a = 0;
	int count_piece_b = 0;
	unsigned long int k;
	int i = 0, j=0, hs=0;
	int M1 = 0, M2 = 0, M3 = 0;
	int hs_first;
	//init temp
	sendByte_I2C(0x0D, 0x33);
	for (i = 0; i < 2000; i++);
	hs_first = receive_2Byte_I2C(0x0D);
	int init_temp = read_temperature();

	while (1) {
//botao
	/*	sendByte_I2C(0x0A, 0x66);
				for (i = 0; i < 2000; i++);

			var = receiveByte_I2C(0x0A);*/
//led
		sendByte_I2C(0x0E, 'G');  //led
		for (i = 0; i < 10000; i++);
		sendByte_I2C(0x0A, 0x33);
		for (i = 0; i < 2000; i++);
		var = receiveByte_I2C(0x0A);
		sprintf(str,"var2: %d\n\r",var);
		writeString(str);
//continue;

		if (var == 0) {
			writeString("Erro sensor ultra som");
			writeNewLine();
		}
		if (var == 1) {
			writeString("Peca recebida");
			writeNewLine();

			count_piece++;

			sendByte_I2C(0x0A, 0x66);
				for (i = 0; i < 2000; i++);

			var = receiveByte_I2C(0x0A);
			for(i = 0; i < 2000; i++);		
			//atraso();
			
			if (var == 3) {
				writeString("Peca Recebida - A");
				writeNewLine();

				sendByte_I2C(0x0C, 0x31); //31 M_3 off			
				for (i = 0; i < 2000; i++);
				M3 = 0; panicB(M1,M2,M3, init_temp);
				sendByte_I2C(0x0B, 0x11); //11 M_1 on
				for (i = 0; i < 2000; i++);
				M1 = 1; panicB(M1,M2,M3, init_temp);
				sendByte_I2C(0x0B, 0x12); //12 M_2 on
				M2 = 1; panicB(M1,M2,M3, init_temp);

				int aux = 1;

				while(aux){
					panicB(M1,M2,M3, init_temp);
					//sendByte_I2C(0x0A, 0x99);
					sendByte_I2C(0x0D, 0x33);
					for (i = 0; i < 2000; i++);
					hs = receive_2Byte_I2C(0x0D);
					if(hs > 500) aux = 0;
				}

				count_piece_a++;
				//ajustar
				for(j=0;j<20;j++){
					for(k=0;k<40;k++);
					panicB(M1,M2,M3,init_temp);
				}
				sendByte_I2C(0x0B, 0x32); //32 M_2 off
				for (i = 0; i < 2000; i++);
				M2 = 0; panicB(M1,M2,M3, init_temp);
				sendByte_I2C(0x0B, 0x31); //11 M_1 off
				M1 = 0; panicB(M1,M2,M3,init_temp);
				
				
										  //ir buscar HS? dellay
			}
			if (var == 4) {
				writeString("Peca Recebida - B");
				writeNewLine();

				sendByte_I2C(0x0B, 0x32); //32 M_2 off
				for (i = 0; i < 2000; i++);
				M2 = 0; panicB(M1,M2,M3,init_temp);
				sendByte_I2C(0x0B, 0x11); //11 M_1 on
				for (i = 0; i < 2000; i++);
				M1 = 1; panicB(M1,M2,M3,init_temp);

				//ajustar
				for(j=0;j<80;j++){
					for(k=0;k<40;k++);
					panicB(M1,M2,M3,init_temp);
				}

				sendByte_I2C(0x0B, 0x31); //11 M_1 off
				for (i = 0; i < 2000; i++);
				M1 = 0; panicB(M1,M2,M3,init_temp);
				sendByte_I2C(0x0C, 0x11); //31 M_3 on	
				for (i = 0; i < 2000; i++);
				M3 = 1; panicB(M1,M2,M3,init_temp);		
				
				//ajustar
				for(j=0;j<90;j++){
					for(k=0;k<40;k++);
					panicB(M1,M2,M3,init_temp);			
				}			
				sendByte_I2C(0x0C, 0x31); //31 M_3 off			
				for (i = 0; i < 2000; i++);
				M3 = 0; panicB(M1,M2,M3,init_temp);		
			}
		}

		if (var == 2) {
			writeString("No piece");
			writeNewLine();
		}
		panicB(M1,M2,M3,init_temp);
		count_piece_b = count_piece - count_piece_a;
		sprintf(str, "Total Pieces: %d\n\rPieces A: %d\n\rPieces B: %d\n\r", count_piece, count_piece_a, count_piece_b);
		writeString(str);
		

	}
}





