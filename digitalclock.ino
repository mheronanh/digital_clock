#include <TimerOne.h>

//definisikan pin data
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7
#define G 8 

//definisikan pin selektor
#define C1 12
#define C2 11
#define C3 10
#define C4 9

const int segs[7] = {A,B,C,D,E,F,G}; //array dari pin data
int counter = 0; //variabel waktu
const byte numbers[10] = { 0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010,
0b0000010, 0b1111000, 0b0000000, 0b0010000 }; //array dari binary tiap representasi angka

void setup() 
{
    for (int i = 2; i <= 12; i++)
    {
        pinMode(i, OUTPUT); //inisialisasi pin output data dan selektor
    }
    Timer1.initialize(1000000); //inisialisasi timer interrupt 1 detik
    Timer1.attachInterrupt(main_time);
}

void loop() 
{
    seg(counter); //menampilkan angka var counter
}

void seg_clock(int number) //melakukan screening
{
    digit4(numbers[number%10]); delay(8);
    digit3(numbers[(number%60)/10]); delay(8); 
    digit2(numbers[number/60]); delay(8);
    digit1(numbers[number/600]); delay(8);
}

void main_time()
{
    counter++; //counting sebagai fungsi timer interrupt
}
void digit1(byte number)
{
    digitalWrite(C1, HIGH);
    digitalWrite(C2, LOW);
    digitalWrite(C3, LOW);
    digitalWrite(C4, LOW);
    light_number(number); //menampilkan angka digit 1 dengan mengaktifkan selektor digit 1
}

void digit2(byte number)
{
    digitalWrite(C1, LOW);
    digitalWrite(C2, HIGH);
    digitalWrite(C3, LOW);
    digitalWrite(C4, LOW);
    light_number(number); //menampilkan digit 2 dengan aktifasi selektor 2
}

void digit3(byte number)
{
    digitalWrite(C1, LOW);
    digitalWrite(C2, LOW);
    digitalWrite(C3, HIGH);
    digitalWrite(C4, LOW);
    light_number(number); //menampilkan digit 3 dengan aktifasi selektor 3
}

void digit4(byte number)
{
    digitalWrite(C1, LOW);
    digitalWrite(C2, LOW);
    digitalWrite(C3, LOW);
    digitalWrite(C4, HIGH);
    light_number(number); //menampilkan digit 4 dengan aktifasi selektor 4
}

void light_number(byte number)
{
    for (int i = 0; i < 7; i++)
    {
        int bits = bitRead(number, i);
        digitalWrite(segs[i], bits); //proses menampilkan angka dengan mengekstrak biner dari angka yang ingin ditampilkan
    }
}
