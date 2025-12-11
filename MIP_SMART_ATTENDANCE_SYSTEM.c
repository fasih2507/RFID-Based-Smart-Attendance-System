#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 20000000
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
#define BUZZER RD1

void LCD_Init();
void LCD_Command(unsigned char);
void LCD_Char(unsigned char);
void LCD_String(const char *);
void LCD_Clear();
void UART_Init();
char UART_Read();
void checkCard(char *);

const char card1[] = "2D0058F1A024";
const char card2[] = "2C0054C60EB0";

void main() {
    TRISD = 0x00;
    TRISC7 = 1;
    TRISC6 = 0;

    UART_Init();
    LCD_Init();
    LCD_Clear();
    LCD_String("Scan your card");

    while (1) {
        while (RCIF) { char dump = RCREG; }

        char cardID[13];
        int index = 0;

        while (index < 12) {
            char ch = UART_Read();
            if ((ch >= '0' && ch <= '9') ||
                (ch >= 'A' && ch <= 'Z') ||
                (ch >= 'a' && ch <= 'z')) {
                cardID[index++] = ch;
            }
        }
        cardID[12] = '\0';

        LCD_Clear();
        LCD_String(cardID);
        __delay_ms(1500);

        checkCard(cardID);

        __delay_ms(2000);
        LCD_Clear();
        LCD_String("Scan your card");
    }
}

void checkCard(char *id) {
    if (strcmp(id, card1) == 0) {
        LCD_Clear();
        LCD_String("Attendance Marked");

        BUZZER = 1;
        __delay_ms(200);
        BUZZER = 0;
    }
    else if (strcmp(id, card2) == 0) {
        LCD_Clear();
        LCD_String("Attendance Marked");

        BUZZER = 1;
        __delay_ms(200);
        BUZZER = 0;
    }
    else {
        LCD_Clear();
        LCD_String("Invalid Card");

        BUZZER = 1;
        __delay_ms(800);
        BUZZER = 0;
    }
}

void UART_Init() {
    SPBRG = 129;
    TXEN = 1;
    SYNC = 0;
    BRGH = 1;
    SPEN = 1;
    CREN = 1;
}

char UART_Read() {
    while (!RCIF);
    return RCREG;
}

void LCD_Command(unsigned char cmd) {
    RS = 0;

    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    EN = 1; __delay_us(1); EN = 0;
    __delay_us(200);

    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    EN = 1; __delay_us(1); EN = 0;
    __delay_ms(2);
}

void LCD_Char(unsigned char data) {
    RS = 1;

    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    EN = 1; __delay_us(1); EN = 0;
    __delay_us(200);

    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    EN = 1; __delay_us(1); EN = 0;
    __delay_ms(2);
}

void LCD_String(const char *str) {
    while (*str) LCD_Char(*str++);
}

void LCD_Clear() {
    LCD_Command(0x01);
    __delay_ms(2);
}

void LCD_Init() {
    LCD_Command(0x02);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
}
