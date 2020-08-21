#include "mbed.h"
#include "lv_examples-master/lv_examples.h"
#include "DHT.h" //https://os.mbed.com/users/Wimpie/code/DHT/
#include "TS_DISCO_F746NG.h" //  committed 2015-10-07 https://os.mbed.com/teams/ST/code/TS_DISCO_F746NG/
#include "LCD_DISCO_F746NG.h" // updated 2015-09-28	requires BSP_DISCO_F746NG https://os.mbed.com/teams/ST/code/LCD_DISCO_F746NG/
#include "lvgl/lvgl.h" //https://github.com/lvgl/lvgl


 
Ticker ticker;

    TS_StateTypeDef TS_State;  
    DHT sensor(A0,SEN51035P); // Use the SEN11301P sensor
    AnalogIn lightsensor (A2);
    LCD_DISCO_F746NG lcd;
    TS_DISCO_F746NG ts;
    DigitalOut light(D4);

    Thread temp;
    Thread lightsens;
    Thread water;

void daylight(){
    float daytime = 0.5, lys;
    while(true)
    {
    lys = lightsensor.read();
        if(lys > daytime or lys == daytime){
            printf("daylight %f\n",lys);
            wait(5);
            light = 0;
        }
        if(lys < daytime){
            printf("bad light %f\n",lys);
            wait(5);
            light = 1;
        }
    }

}


void sensorcheck(){
    int errtemp;
    float temperature, max_temperature,humidity, max_humidity, min_humidity;
    max_temperature = 28;
    min_humidity = 50; 
    max_humidity = 70;

    int xszie = lcd.GetXSize(); // gets the x axis for he lcd screen
    int ysize = lcd.GetYSize(); // gets the y axis for the lcd screen


    char showtemp[50];
    char showhumidity[10];
    char e [] = "its to hot"; // it varmer than should be allowed
    char f [] = "its to dry"; // humidity is very to
    char g [] = "its too humid"; // to dry
    char d [] = "its perfect conditions"; // its just right 


    while(true)
    {   
        humidity = sensor.ReadHumidity();
        temperature = sensor.ReadTemperature(CELCIUS);
        errtemp = sensor.readData();

        if(errtemp == 0)
        {
            if(temperature > max_temperature){
            lcd.DisplayStringAt(0, 25, (uint8_t *) & e , LEFT_MODE ); // Hot ones
            }
            if(humidity >min_humidity & humidity < max_humidity){
            lcd.DisplayStringAt(0, ysize/2, (uint8_t *) & d , LEFT_MODE ); // its perfect
            }
            if(humidity > max_humidity)
            {
            lcd.DisplayStringAt(0, ysize/2, (uint8_t *) & g , LEFT_MODE ); // to humid
            }
            if (humidity < min_humidity){
            lcd.DisplayStringAt(0, ysize/2, (uint8_t *) & f , LEFT_MODE ); // to dry
            }
        }

        sprintf(showtemp,"temp is %0.0f",temperature);
        BSP_LCD_DisplayStringAt(0, 0, (uint8_t * ) showtemp, LEFT_MODE);

        sprintf(showhumidity, "humidity is %0.0f", humidity);
        BSP_LCD_DisplayStringAt(0, 50, (uint8_t * ) showhumidity, LEFT_MODE);
        ThisThread::sleep_for(1000);


    }
}


int main(){
    
    temp.start(sensorcheck);
    lightsens.start(daylight);

}
