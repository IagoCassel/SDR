#define F_CPU 8000000UL			
#include <avr/io.h>			
#include <util/delay.h>			
#include <math.h>
#include <stdio.h>
#include <string.h>

#define LCD_Dir  DDRB			
#define LCD_Port PORTB			
#define RS PB0				
#define EN PB1 				

#define M PC0 				
#define P PC1 				
#define R PC2				
#define LED PC3				
#define ANA PC4				
#define SEL PC5			

//Valor correspondente ao resultado da corversão Analógica Digital
int resultado_adc = 0;

//Valor correspondente ao canal usado na conversão Analógica Digital do potenciômetro
char canal_potenciometro = 0b0101;

//Valor da frequência da portadora em Hz, de 5000Hz até 50000Hz, fórmula direta com o valor do conversor
unsigned int freq_portadora;

//Auxiliar da frequência para achar o valor em kHz. Evita divisão e existe para ser mostrada no Display
unsigned int freq_portadora_div;

//Valor da frequência da portadora em kHz, de 5kHz até 50kHz
int freq_portadora_kHz;

//Valor da frequência da portadora em kHz, de 5kHz até 50kHz, em forma de texto, para ser mostrada no Display
char freq_portadora_kHz_str[12];

void LCD_comando(unsigned char comando){
	LCD_Port = (LCD_Port & 0x0F) | (comando & 0xF0); 
	LCD_Port &= ~ (1<<RS);		                 
	LCD_Port |= (1<<EN);		                 
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (comando << 4);  
	LCD_Port |= (1<<EN);
	LCD_Port &= ~ (1<<EN);
}


void LCD_escrever_caractere(unsigned char caractere){
	LCD_Port = (LCD_Port & 0x0F) | (caractere & 0xF0); 
	LCD_Port |= (1<<RS);		              
	LCD_Port|= (1<<EN);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (caractere << 4); 
	LCD_Port |= (1<<EN);
	LCD_Port &= ~ (1<<EN);
}

void LCD_inicializador(void){
	LCD_Dir = 0xFF;
	_delay_ms(20);			
	LCD_comando(0x02);		/* Posição inicial  */
	LCD_comando(0x28);              /* Dá preset no modo de 4 bits de interferencia */
	LCD_comando(0x0c);              /* Restaura do Display*/
	LCD_comando(0x06);              /* Desloca o cursor para direita*/
	LCD_comando(0x01);              /* Limpa o display*/
}


void LCD_mensagem(char *str){
	int i;
	for(i=0;str[i]!=0;i++){		/* Escreve caractere até a mensagem esvaziar*/
		LCD_escrever_caractere (str[i]);
	}
}

/* BOTõES */

void entradas_inicializador(){
	DDRC=0xFFu; 			//Coloca todas as PORTC como saídas
	DDRC &= ~(1<<M);		//PC0 entrada (Botão M)
	DDRC &= ~(1<<P);		//PC1 entrada (Botão P)
	DDRC &= ~(1<<R);		//PC2 entrada (Botão R)
        DDRC &= ~(1<<ANA);		//PC4 entrada (Sinal Analógico)
	DDRC &= ~(1<<SEL);		//PC5 entrada (Sinal do Potenciometro)

}
/* FUNCOES ADC */

void configuracao_adc(char canal_escolhido){
/* 
  00 = REFS1 REFS0 =  Vref é igual ao do AREF
   0 = ADLAR = Conversão é organizada em xx:xxxxxxxx em ADCH:ADCL
000 = MUX3 MUX2 MUX1 MUX0 = ADC0 como canal
*/

   ADMUX = (0 << REFS1) 
	 | (0 << REFS0)
	 | (0 << ADLAR)
	 | (canal_escolhido << MUX0);

/* 
  0 = ADEN = Habilita o ADC
010 = ADPS2 ADPS1 ADPS0 = Frequência de conversão é igual ao CLK/4
*/

  ADCSRA = (1<<ADEN)
         | (0b010<<ADPS0);
}

void configuracao_adc_potenciometro(char canal_escolhido){
/* 
  00 = REFS1 REFS0 =  Vref é igual ao do AREF
   0 = ADLAR = Conversão é organizada em xx:xxxxxxxx em ADCH:ADCL
0000 = MUX3 MUX2 MUX1 MUX0 = ADC0 como canal
*/

   ADMUX = (0 << REFS1) 
	 | (0 << REFS0)
	 | (0 << ADLAR)
	 | (canal_escolhido << MUX0);

/* 
  0 = ADEN = Habilita o ADC
101 = ADPS2 ADPS1 ADPS0 = Frequência de conversão é igual ao CLK/32
*/

  ADCSRA = (1<<ADEN)
         | (0b101<<ADPS0);
}

int leitura_adc(){
  ADCSRA |= (1<<ADSC);
  while(!(ADCSRA & (1<<ADIF)));
  return (ADC);
   
}

void mensagem_LCD_AM(){
   LCD_comando(0x80);
   LCD_mensagem("Mod: AM F: ---Hz");
   LCD_comando(0xC0);
   LCD_mensagem("Msg: ---        ");
}

void mensagem_LCD_ASK(){
   LCD_comando(0x80);
   LCD_mensagem("Mod: ASK T:---bs");
   LCD_comando(0xC0);
   LCD_mensagem("Msg: --.--.--.--");
}

void mensagem_LCD_portadora_AM(){
   resultado_adc = leitura_adc();
   freq_portadora = (45*resultado_adc) + 5000 - resultado_adc;
   freq_portadora_div = freq_portadora;
   freq_portadora_kHz = 1;
   while(freq_portadora_div > 1000){
     freq_portadora_kHz = freq_portadora_kHz + 1;
     freq_portadora_div = freq_portadora_div - 1000;
   }
   if(freq_portadora_kHz > 50){
     freq_portadora_kHz = 50;
   }
   sprintf(freq_portadora_kHz_str, "%d", freq_portadora_kHz);
   char linha_LCD_1[16] = "Mod: AM P: ";
   if(freq_portadora_kHz<10){
     strcat(linha_LCD_1, "0");
   }
   strcat(linha_LCD_1, freq_portadora_kHz_str);
   strcat(linha_LCD_1, "kHz");
   LCD_comando(0x80);
   LCD_mensagem(linha_LCD_1);
   LCD_comando(0xC0);
   LCD_mensagem("Msg: ---        ");
}

void mensagem_LCD_portadora_ASK(){
   resultado_adc = leitura_adc();
   freq_portadora = (45*resultado_adc) + 5000 - resultado_adc;
   freq_portadora_div = freq_portadora;
   freq_portadora_kHz = 1;
   while(freq_portadora_div > 1000){
     freq_portadora_kHz = freq_portadora_kHz + 1;
     freq_portadora_div = freq_portadora_div - 1000;
   }
   if(freq_portadora_kHz > 50){
     freq_portadora_kHz = 50;
   }
   sprintf(freq_portadora_kHz_str, "%d", freq_portadora_kHz);
   char linha_LCD_1[16] = "Mod: ASK P:";
   if(freq_portadora_kHz<10){
     strcat(linha_LCD_1, "0");
   }
   strcat(linha_LCD_1, freq_portadora_kHz_str);
   strcat(linha_LCD_1, "kHz");
   LCD_comando(0x80);
   LCD_mensagem(linha_LCD_1);
   LCD_comando(0xC0);
   LCD_mensagem("Msg: --.--.--.--");
}

int main(){                                            //ESTADO START - Realiza todos as inicializações
   configuracao_adc_potenciometro(canal_potenciometro);
   entradas_inicializador();		
   LCD_inicializador();			                 
   mensagem_LCD_AM();
   unsigned char ciclo_completo = 0;
   unsigned char AM_1_ASK_0 = 1;
   while(1){                                          //ESTADO WAIT - Espera o pressionamento do botão M para escolher a demodulação
      ciclo_completo = 0;
      if ((PINC & (1<<M))){
	 while(ciclo_completo == 0){                  //ESTADO DEMODULAÇÃO - Escolhe a demodulação que será feita com o potenciômetro
	    resultado_adc = leitura_adc();
	    if(resultado_adc <= 511){
	       mensagem_LCD_AM();
	       AM_1_ASK_0 = 1;
	    }
	    else if(resultado_adc >= 512){
	       mensagem_LCD_ASK();
	       AM_1_ASK_0 = 0;
	    }
	    if ((PINC & (1<<P))){                      //ESTADO PORTADORA - Escolhe a frequência da portadora da demodulação AM ou ASK
	       while(ciclo_completo == 0){
		  if(AM_1_ASK_0 == 1){
		     mensagem_LCD_portadora_AM();
		  }
		  else if(AM_1_ASK_0 == 0){
		     mensagem_LCD_portadora_ASK();
		  }
		  if ((PINC & (1<<R))){                 //ESTADO RUN - Realiza e mostra a demodulação
		     while(ciclo_completo == 0){
			//FILTRO AQUI
			//LED Vermelho OU Azul
			if((PINC & (1<<M))){
			   ciclo_completo = 1;          //Volta para o primeiro while (ESTADO WAIT)
			}
		     }
		  }
	       }
	    }
	 } 
      }
   }
   return (0); 
}