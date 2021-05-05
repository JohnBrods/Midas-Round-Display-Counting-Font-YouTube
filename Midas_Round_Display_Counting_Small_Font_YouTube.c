
                                                  /*Anyone is free to copy, modify, publish, use, compile or
                                                    distribute this software, either in source code form or as a compiled
                                                    binary, for non-commercial use only. (i.e. YOU MAY NOT SELL IT)
                                                    John B 05/05/2021

                                                    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
                                                    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
                                                    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
                                                    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
                                                    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
                                                    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
                                                    OTHER DEALINGS IN THE SOFTWARE.*/


// TFT display connections
unsigned char TFT_DataPort at PORTB;
sbit TFT_WR at LATA3_bit;
sbit TFT_RD at LATA4_bit;
sbit TFT_CS at LATA2_bit;         //..PIN2 RA0
sbit TFT_RS at LATA0_bit;        //This Is The #DC Pin  Needs Ra0
sbit TFT_RST at RA1_bit;

unsigned char TFT_DataPort_Direction at TRISB;
sbit TFT_WR_Direction at TRISA3_bit;
sbit TFT_RD_Direction at TRISA4_bit;
sbit TFT_CS_Direction at TRISA2_bit;
sbit TFT_RS_Direction at TRISA0_bit;    // D/C# BIT
sbit TFT_RST_Direction at TRISA1_bit;

void TFT_Set_Index_SSD1327(unsigned char Set_index_SSD1327) {
  TFT_RS = 0;
  TFT_DataPort = Set_index_SSD1327;
  TFT_WR = 0;
  TFT_WR = 1;
}

void TFT_Write_Command_SSD1327(unsigned char cmd) {
  TFT_RS = 0;
  TFT_DataPort = cmd;
  TFT_WR = 0;
  TFT_WR = 1;
}

void TFT_Write_Data_SSD1327(unsigned int _data) {
  TFT_RS = 1;
  TFT_DataPort = _data;
  TFT_WR = 0;
  TFT_WR = 1;
  TFT_RS = 0;
}

void TFT_Set_Reg_SSD1327(unsigned char index, unsigned char value) { //was short
  TFT_CS = 0;
  TFT_Set_Index_SSD1327(index);
  TFT_Write_Command_SSD1327(value);
}

sbit Enable_15_Volts at LATA5_bit;
sbit Enable_15_Volts_Direction at TRISA5_bit;  //Charge Pump Enable

void Initialize_Display(){

       TFT_RST = 0;
       Delay_ms(50);
       TFT_RST = 1;
       Delay_ms(50);
       Enable_15_Volts = 1;
       Delay_ms(300);
                                         // COMMAND == DATA SPACE
                                         // INDEX == REGISTER SPACE OF THE CONTROLLER
       TFT_CS = 0;

       TFT_Set_Index_SSD1327(0xAE);              // Display Off

       TFT_Set_Reg_SSD1327(0xAB, 0x01);          // 7.10 VDD Regulator PAGE 32  *********************IMPORTANT DO THIS FIRST*****************************
                                                 // Zero is hard wired supply, set it to 1 = capacitor 4.7uf to ground

       TFT_Set_Reg_SSD1327(0xA8,0x7F);           // Multiplex Ratio    Duty

       TFT_Set_Reg_SSD1327(0x81,200);           // Set Contrast Control   //Contrast Level

       TFT_Set_Reg_SSD1327(0xBC,0X07);           // Set pre-charge voltage level.  //  0000 00h 0.20 x VCC

       TFT_Set_Reg_SSD1327(0xBE,0X07);           // Set VCOMH Voltage

       TFT_Set_Index_SSD1327(0xB9);              // The default Linear Gray Scale

       TFT_Set_Reg_SSD1327(0xB1,0xF1);           // 9.1.12 Set Phase Length (B1h)

       TFT_Set_Reg_SSD1327(0xB6,0x07);           // Set Second Pre-charge period

       TFT_Set_Reg_SSD1327(0xAB,0x01);           // Set Function selection A

       TFT_Set_Reg_SSD1327(0xD5,0x62);           // Set Function selection B

       TFT_Set_Reg_SSD1327(0xB3,0x41);           // Define Divide Ratio (D) Of Display Clock      //Frequency & Divide By Bits (0 to 15)   PAGE 54

       TFT_Set_Index_SSD1327(0x15);              // Set Column Address
       TFT_Write_Command_SSD1327(0x00);          // Start Column Address
       TFT_Write_Command_SSD1327(0x7F);          // End Column Address

       TFT_Set_Index_SSD1327(0x75);              // Set Row Address (segment)
       TFT_Write_Command_SSD1327(0x00);          // Satrt Row Address
       TFT_Write_Command_SSD1327(0x7F);          // End Row Address

       TFT_Set_Reg_SSD1327(0xA1,0);           // Start Line

       TFT_Set_Reg_SSD1327(0xA2,0x00);           // Set Display Offset      Vertical offset by COM from 0 ~ 127    page 43

       TFT_Set_Index_SSD1327(0xAF);              // Display ON in normal mode

       TFT_Set_Index_SSD1327(0xA4);              // Resume to RAM content display Reset Output follows RAM content

    // TFT_Set_Index_SSD1327(0xA5);              // ALL PIXELS ON

    // TFT_Set_Index_SSD1327(0xA7);              // Invert Display

       TFT_Set_Reg_SSD1327(0xA0,0x5d);           // 1 Bit Per Pixel in one byte,  Scans From Start Row & Column, 8 Pixels Per Row Then Clocking Moves Down One Row. Yey :) This For My Font

        TFT_CS = 1;
}


  unsigned char x_pos;
  unsigned char y_pos;

void Set_Display(unsigned char x_pos){

   TFT_Set_Index_SSD1327(0x15);             // Set Column Address
   TFT_Write_Command_SSD1327(0x00|x_pos);   // Start Column Address
   TFT_Write_Command_SSD1327(127);          // End Column Address
   TFT_Set_Index_SSD1327(0x75);             // Set Row Address (segment)
   TFT_Write_Command_SSD1327(y_pos);        // Satrt Row Address
   TFT_Write_Command_SSD1327(y_pos);        // End Row Address
   }



void Write_Tahoma11x16_Zero(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00011110);        //line 1
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 2
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100001);        //line 3
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100001);        //line 4
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 5
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 6
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 7
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 8
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 9
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 10
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 11
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 12
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100001);        //line 13
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100001);        //line14
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line15
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011110);        //line 16
         TFT_Write_Data(0b00000000);

         TFT_CS = 1;
}


void Write_Tahoma11x16_One(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 1
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 2
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111110);        //line 3
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111110);        //line 4
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 5
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 6
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 7
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);       //line 8
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);       //line 9
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 10
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 11
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 12
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 13
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line14
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line15
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 16
         TFT_Write_Data(0b11000000);

         TFT_CS = 1;
}


void Write_Tahoma11x16_Two(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b01111110);        //line 1
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11111111);        //line 2
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b10000011);        //line 3
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 4
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 5
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 6
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 7
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000011);        //line 8
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 9
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00001110);        //line 10
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011100);        //line 11
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111000);        //line 12
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01110000);        //line 13
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line14
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11111111);        //line15
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11111111);        //line 16
         TFT_Write_Data(0b11000000);

         TFT_CS = 1;
}


void Write_Tahoma11x16_Three(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 1
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line 2
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01000001);        //line 3
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 4
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 5
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 6
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00001111);        //line 7
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00001111);        //line 8
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 9
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 10
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 11
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 12
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 13
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01000001);        //line14
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b011111111);        //line15
         TFT_Write_Data(0b100000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111110);        //line 16
         TFT_Write_Data(0b00000000);

         TFT_CS = 1;
}


void Write_Tahoma11x16_Four(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 1
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000011);        //line 2
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000111);        //line 3
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00001101);        //line 4
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011001);        //line 5
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00110001);        //line 6
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100001);        //line 7
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000001);        //line 8
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11111111);        //line 9
         TFT_Write_Data(0b11100000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11111111);        //line 10
         TFT_Write_Data(0b11100000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 11
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 12
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 13
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line14
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line15
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 16
         TFT_Write_Data(0b10000000);

         TFT_CS = 1;
}


void Write_Tahoma11x16_Five(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 1
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 2
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00110000);        //line 3
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00110000);        //line 4
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00110000);        //line 5
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00110000);        //line 6
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 7
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 8
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 9
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 10
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 11
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 12
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 13
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01000001);        //line14
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line15
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111110);        //line 16
         TFT_Write_Data(0b00000000);

         TFT_CS = 1;
}



void Write_Tahoma11x16_Six(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00001111);        //line 1
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011111);        //line 2
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111000);        //line 3
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100000);        //line 4
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100000);        //line 5
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 6
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11011111);        //line 7
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11111111);        //line 8
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11100001);        //line 9
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 10
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 11
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 12
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11100000);        //line 13
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100001);        //line14
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line15
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011110);        //line 16
         TFT_Write_Data(0b00000000);

         TFT_CS = 1;
}



void Write_Tahoma11x16_Seven(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line 1
         TFT_Write_Data(0b11100000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line 2
         TFT_Write_Data(0b11100000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 3
         TFT_Write_Data(0b01100000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 4
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 5
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 6
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 7
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000011);        //line 8
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000011);        //line 9
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 10
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000110);        //line 11
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00001100);        //line 12
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00001100);        //line 13
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011000);        //line14
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011000);        //line15
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00110000);        //line 16
         TFT_Write_Data(0b00000000);

         TFT_CS = 1;
}



void Write_Tahoma11x16_Eight(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00111111);        //line 1
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line 2
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11100001);        //line 3
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 4
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 5
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11100000);        //line 6
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111001);        //line 7
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111110);        //line 8
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100111);        //line 9
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000001);        //line 10
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 11
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 12
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 13
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11100001);        //line14
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line15
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00011110);        //line 16
         TFT_Write_Data(0b00000000);

         TFT_CS = 1;
}



void Write_Tahoma11x16_Nine(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b00011110);        //line 1
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line 2
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01100001);        //line 3
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000001);        //line 4
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 5
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 6
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11000000);        //line 7
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b11100001);        //line 8
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111111);        //line 9
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00111110);        //line 10
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000000);        //line 11
         TFT_Write_Data(0b11000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 12
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000001);        //line 13
         TFT_Write_Data(0b10000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b00000111);        //line14
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111110);        //line15
         TFT_Write_Data(0b00000000);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b01111100);        //line 16
         TFT_Write_Data(0b00000000);

         TFT_CS = 1;
}


void Write_Tahoma11x16_Space(unsigned char X_Position, unsigned char Y_position){

         y_pos = 0x00 | Y_Position;

         TFT_CS = 0;                             //  Tahoma11x16
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 1
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 2
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 3
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 4
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 5
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 6
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 7
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 8
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 9
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 10
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 11
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 12
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 13
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line14
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line15
         TFT_Write_Data(0b0);
         y_pos++;
         Set_Display(X_Position);
         TFT_Write_Data(0b0);        //line 16
         TFT_Write_Data(0b0);

         TFT_CS = 1;
}


void Write_char_Tahoma11x16(unsigned long num,unsigned char X_Position,unsigned char Y_Position) {

   unsigned char units_start;
   unsigned char tens_start;
   unsigned char hundreds_start;
   unsigned char thousands_start;
   unsigned char ten_thousands_start;
   unsigned char hundred_thousands_start;
   unsigned char million_start;
   unsigned char ten_million_start;
   unsigned char done1=0;
   unsigned char done2=0;
   unsigned char done3=0;
   unsigned char done4=0;
   unsigned char done5=0;
   unsigned char done6=0;
   unsigned char done7=0;

   unsigned char numberstring[14];

   TFT_CS =0;

    if (num <=9){
        units_start = X_Position;  }

    if (num >=10 && num<=99){
       done1 = 1;
       units_start= X_Position+2;
       tens_start= X_Position;
                            }

    if (num >=100 && num<=999){
       done2 = 1;
       units_start= X_Position+4;
       tens_start= X_Position+2;
       hundreds_start = X_Position;
      }

      if (num >=1000 && num<=9999){
       done3 = 1;
       units_start= X_Position+6;
       tens_start= X_Position+4;
       hundreds_start=X_Position+2;
       thousands_start = X_Position;
       }

       if (num >=10000 && num<=99999){
       done4 = 1;
       units_start = X_Position+8;
       tens_start = X_Position+6;
       hundreds_start = X_Position+4;
       thousands_start = X_Position+2;
       ten_thousands_start = X_Position;
       }


       if (num >=100000 && num<=999999){
       done5 = 1;
       units_start= X_Position+10;
       tens_start= X_Position+8;
       hundreds_start= X_Position+6;
       thousands_start = X_Position+4;
       ten_thousands_start = X_Position+2;
       hundred_thousands_start = X_Position;
       }

       if (num >=1000000){
       done6 = 1;
       units_start = X_Position+12;
       tens_start = X_Position+10;
       hundreds_start = X_Position+8;
       thousands_start = X_Position+6;
       ten_thousands_start = X_Position+4;
       hundred_thousands_start = X_Position+2;
       million_start = X_Position;
       }

        if (num >=10000000){
       done7 = 1;
       units_start = X_Position+14;
       tens_start = X_Position+12;
       hundreds_start = X_Position+10;
       thousands_start = X_Position+8;
       ten_thousands_start = X_Position+6;
       hundred_thousands_start = X_Position+4;
       million_start = X_Position+2;
       ten_million_start = X_Position;
       }


       if (num<10 && done1 !=1){               //Clears The Tens Position Once
       done1 = 1;
        Write_Tahoma11x16_Space(X_Position+2,Y_Position);
       }


       if (num<100 && done2 !=1){             //Clears The Hundreds Position Once
       done2 = 1;
       Write_Tahoma11x16_Space(X_Position+4,Y_Position);
       }


       if (num<1000 && done3 !=1){             //Clears The Thousands Position Once
       done3 = 1;
       Write_Tahoma11x16_Space(X_Position+6,Y_Position);
       }


       if (num<10000 && done4 !=1){           //Clears The ten Thousands Position Once
       done4 = 1;
        Write_Tahoma11x16_Space(X_Position+8,Y_Position);
       }

       if (num<100000 && done5 !=1){        //Clears The hundred Thousands Position Once
       done5 = 1;
       Write_Tahoma11x16_Space(X_Position+10,Y_Position);
       }

       if (num<1000000 && done6 !=1){     //Clears The million Position Once
       done6 = 1;
       Write_Tahoma11x16_Space(X_Position+12,Y_Position);
       }

        if (num<10000000 && done7 !=1){     //Clears The ten million Position Once
       done7 = 1;
        Write_Tahoma11x16_Space(X_Position+14,Y_Position);
       }


       LongWordToStr(num,numberstring);     // Converts input unsigned long integer number to a string


    switch (numberstring[9])  {  //units position


     case 48:    X_Position = units_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = units_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = units_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = units_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = units_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = units_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = units_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = units_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = units_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = units_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }


     switch (numberstring[8])  { //tens position

     case 48:    X_Position = tens_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = tens_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = tens_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = tens_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = tens_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = tens_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = tens_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = tens_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = tens_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = tens_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }


     switch (numberstring[7])  {   //hundreds position

     case 48:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = hundreds_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = hundreds_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }

     switch (numberstring[6])  {   //thousands position

     case 48:    X_Position = thousands_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = thousands_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = thousands_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = thousands_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = thousands_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = thousands_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = thousands_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = thousands_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = thousands_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = thousands_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }

     switch (numberstring[5])  {   //ten thousands position

     case 48:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = ten_thousands_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }


     switch (numberstring[4])  {   //hundred thousands position

     case 48:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = hundred_thousands_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }


     switch (numberstring[3])  {   //million position

     case 48:    X_Position = million_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = million_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = million_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = million_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = million_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = million_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = million_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = million_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = million_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = million_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }

     switch (numberstring[2])  {   //million position

     case 48:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Zero(X_Position,Y_Position);
                 break;
     case 49:    X_Position = ten_million_start;
                 Write_Tahoma11x16_One(X_Position,Y_Position);
                 break;
     case 50:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Two(X_Position,Y_Position);
                 break;
     case 51:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Three(X_Position,Y_Position);
                 break;
     case 52:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Four(X_Position,Y_Position);
                 break;
     case 53:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Five(X_Position,Y_Position);
                 break;
     case 54:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Six(X_Position,Y_Position);
                 break;
     case 55:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Seven(X_Position,Y_Position);
                 break;
     case 56:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Eight(X_Position,Y_Position);
                 break;
     case 57:    X_Position = ten_million_start;
                 Write_Tahoma11x16_Nine(X_Position,Y_Position);
                 break;
     default: break;
    }
}

//Tested with Pic18F27K42 @ 64MHz

void main() {

    unsigned long x;

    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;

    HFOEN_bit = 1;
                                //  Page 94
                                //  HFINTOSC  can produce 1, 2, 4, 8, 12, 16,3 2, 48 and 64 MHz clock. The frequency can be controlled through the OSCFRQ register (Register 7-5

                                //  110 HFINTOSC(2)  2: HFINTOSC frequency is set with the FRQ bits of the OSCFRQ register (Register 7-5).
    NOSC2_bit = 1;
    NOSC1_bit = 1;
    NOSC0_bit = 0;
                               //  NEW OSCILLATOR SOURCE
                               // (NOSC) AND NEW DIVIDER
                               // SELECTION REQUEST (NDIV) BITS

                                //   bit 3-0 FRQ[3:0]: HFINTOSC Frequency Selection bits(1)    OSCFRQ
                                //  1000 64      0101 16       0010 4
                                //  0111 48      0100 12       0001 2
                                //  0110 32      0011 8        0000 1
    FRQ3_bit = 1;
    FRQ2_bit = 0;
    FRQ1_bit = 0;
    FRQ0_bit = 0;


  //*****************************************************************************************************
     TFT_Init_Custom(128,128);
     TFT_Set_Active(TFT_Set_Index_Ptr,TFT_Write_Command_SSD1327,TFT_Write_Data);
     Initialize_Display();
      Delay_ms(100);

            TFT_Fill_Screen(CL_Black);
            Delay_ms(100);

     while(1){


                      for(x=0; x<99999999;   x++){

                     Write_char_Tahoma11x16(x,0,55);    }



     }



}

