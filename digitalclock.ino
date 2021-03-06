#include <TimerOne.h>
#include <ezButton.h>

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

//define pin button
#define B1 A0
#define B2 A1
#define B3 A2

//define pin LED
#define LED 13 //alarm
#define LED1 A3
#define LED2 A4
#define LED3 A5
#define LED4 0
#define LED5 1

//variabel global
const int segs[7] = {A,B,C,D,E,F,G}; //array dari pin data
//button param
ezButton button1(B1);
ezButton button2(B2);
ezButton button3(B3);
long counter = 0; //variabel waktu
int option = 0; //variabel opsi menu
int alarm_flag = 0; //flag untuk alarm (default = 0 untuk tidak menyala)
int alarm_led_on = 0;//flag led alarm
long counter_alarm = 0; //variabel waktu alarm
long counter_stopwatch = 0;
int stopwatch_flag = 0;
const byte numbers[10] = { 0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010,
0b0000010, 0b1111000, 0b0000000, 0b0010000 }; //array dari binary tiap representasi angka

void setup() 
{
    for (int i = 2; i <= 12; i++)
    {
        pinMode(i, OUTPUT); //inisialisasi pin output data dan selektor
    }
    pinMode(B1, INPUT); pinMode(B2, INPUT); pinMode(B3, INPUT);
    pinMode(LED, OUTPUT); pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT); pinMode(LED3, OUTPUT); pinMode(LED4, OUTPUT); pinMode(LED5, OUTPUT);//pin output untuk LED
    Timer1.initialize(1000000); //inisialisasi timer interrupt 1 detik
    Timer1.attachInterrupt(main_time);
}

void loop() 
{
    init_button();
    time_overflow();
    mode();
    led_mode();
    change_menu();
    alarm_mode();
}

void change_menu()
{
    if ((button1.isPressed() == HIGH) && (alarm_led_on == 0))
    {
        option++;
    }
    if(option > 4) option = 0;
}

void stopwatch()
{
    if (option == 4)
    {
        time_minute_second(counter_stopwatch);
        if (button3.isPressed() == HIGH)
        {
            if (stopwatch_flag == 0)
            {
                stopwatch_flag = 1;
            } 
            else
            {
                stopwatch_flag = 0;
            }
        }
        if (button2.isPressed() == HIGH) counter_stopwatch = 0;
        if (counter_stopwatch >= 3600) counter_stopwatch = 0;
    }
}

void alarm_mode()
{
    if ((alarm_flag == 1) && (counter == counter_alarm)) 
    {
        alarm_led_on = 1; digitalWrite(13, HIGH);
    }
    if ((alarm_led_on == 1) && (button1.isPressed()==HIGH)) 
    {
        digitalWrite(13, LOW); alarm_led_on = 0;
    }
}

void alarm_setting()
{
    static int hour = 0, min = 0;
    static int alarm_option = 0;
    if (option == 3)
    {
        display_setting(hour, min);
        if ((button3.isPressed() == HIGH) && (alarm_option == 0)) hour++;
        if ((button3.isPressed() == HIGH) && (alarm_option == 1)) min++;
        if (hour > 23) hour = 0;
        if (min > 59) min = 0;
        if (button2.isPressed() == HIGH) alarm_option++;
        if (alarm_option == 2)
        {
            counter_alarm = (long(hour)*3600) + (min*60); //atasi bug overflow pada operasi int dengan menjadikan salah satu var long sementara (hemat memory)
            alarm_option = 0;
            alarm_flag = 1; //enabling alarm
            option++;
        }
    }
}

void clock_setting()
{
    static int hour = 0, min = 0;
    static int set_option = 0;
    if (option == 2)
    {
        display_setting(hour, min);
        if ((button3.isPressed() == HIGH) && (set_option == 0)) hour++;
        if ((button3.isPressed() == HIGH) && (set_option == 1)) min++;
        if (hour > 23) hour = 0;
        if (min > 59) min = 0;
        if (button2.isPressed() == HIGH) set_option++;
        if (set_option == 2)
        {
            counter = (long(hour)*3600) + (min*60); //atasi bug overflow pada operasi int dengan menjadikan salah satu var long sementara (hemat memory)
            set_option = 0;
            option++;
        }
    }
}

void display_setting(int hour, int min)
{
    digit1(numbers[hour/10]); delay(3);
    digit2(numbers[hour%10]); delay(3);
    digit3(numbers[min/10]); delay(3);
    digit4(numbers[min%10]); delay(3);
}

void mode()
{
    switch (option)
    {
        case 0:
            time_minute_second(counter);
            break;
        case 1:
            time_hour(counter);
            break;
        case 2:
            clock_setting();
            break;
        case 3:
            alarm_setting();
            break;
        case 4:
            stopwatch();
            break;
    }
}

void led_mode()
{
    switch (option)
    {
        case 0:
            digitalWrite(LED1, HIGH); digitalWrite(LED2, LOW); digitalWrite(LED3, LOW); digitalWrite(LED4, LOW); digitalWrite(LED5, LOW);
            break;
        case 1:
            digitalWrite(LED1, LOW); digitalWrite(LED2, HIGH); digitalWrite(LED3, LOW); digitalWrite(LED4, LOW); digitalWrite(LED5, LOW);
            break;
        case 2:
            digitalWrite(LED1, LOW); digitalWrite(LED2, LOW); digitalWrite(LED3, HIGH); digitalWrite(LED4, LOW); digitalWrite(LED5, LOW);
            break;
        case 3:
            digitalWrite(LED1, LOW); digitalWrite(LED2, LOW); digitalWrite(LED3, LOW); digitalWrite(LED4, HIGH); digitalWrite(LED5, LOW);
            break;
        case 4:
            digitalWrite(LED1, LOW); digitalWrite(LED2, LOW); digitalWrite(LED3, LOW); digitalWrite(LED4, LOW); digitalWrite(LED5, HIGH);
    }
}

void init_button()
{
    button1.loop();
    button2.loop();
    button3.loop();
}

void time_overflow()
{
    if (counter >= 86400) counter = 0;
}

void time_minute_second(long number) //melakukan screening
{
    digit1(numbers[(number/600)%6]); delay(3);
    digit2(numbers[(number/60)%10]); delay(3);
    digit3(numbers[(number%60)/10]); delay(3); 
    digit4(numbers[number%10]); delay(3);
}

void time_hour(long number)
{
    digit3(numbers[number/36000]); delay(3);
    digit4(numbers[(number/3600)%10]); delay(3);
}

void main_time()
{
    counter++; //counting sebagai fungsi timer interrupt
    if (stopwatch_flag == 1) counter_stopwatch++;
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
