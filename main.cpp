#include "mbed.h"
#include "lv_examples-master/lv_examples.h"
#include "DHT.h" //https://os.mbed.com/users/Wimpie/code/DHT/
#include "TS_DISCO_F746NG.h" //  committed 2015-10-07 https://os.mbed.com/teams/ST/code/TS_DISCO_F746NG/
#include "LCD_DISCO_F746NG.h" // updated 2015-09-28	requires BSP_DISCO_F746NG https://os.mbed.com/teams/ST/code/LCD_DISCO_F746NG/
#include "lvgl/lvgl.h" //https://github.com/lvgl/lvgl


 
Ticker ticker;

    TS_StateTypeDef TS_State;  
    DHT sensor(A0,SEN51035P); // Use the SEN11301P sensor, insert it into A0 pin. the tempeture sensor 
    AnalogIn lightsensor (A2); // Put it into the A2 pin. lightsensor
    LCD_DISCO_F746NG lcd; 
    TS_DISCO_F746NG ts;
    DigitalOut light(D4); // light diode/artificial

    Thread temp; // my 1 thread so i can take the tempeture and show it 
    Thread lightsens; // my 2 thread it show the light and will turn on the artifical light source if it gets below 

void daylight(){ 
    float daytime = 0.5, lys; // two floats, daytime is set to 0.5, and my lys is my lightsensor 
    while(true)
    {
    lys = lightsensor.read(); // reads the lightsensor
        if(lys > daytime or lys == daytime){ // if statement if the light is adequate it wont turn on the artifical light 
            printf("daylight %f\n",lys); 
            wait(5);
            light = 0;
        }
        if(lys < daytime){ // if statement, if its too dark it will turn on the light 
            printf("bad light %f\n",lys);
            wait(5);
            light = 1; // the light gets turned on
        }
    }

}


void sensorcheck(){
    int errtemp;
    float temperature, max_temperature,humidity, max_humidity, min_humidity; // float variable, to take the temperature, humidity and to set a min/max temperature and humidity
    max_temperature = 28; // my minimum temperature 
    min_humidity = 50;  // minimum humidity
    max_humidity = 70; // max humidity

    int xszie = lcd.GetXSize(); // gets the x axis for he lcd screen 
    int ysize = lcd.GetYSize(); // gets the y axis for the lcd screen


    char showtemp[50]; // so i can display my temperature on the lcd screen
    char showhumidity[10]; // so i can display my humidity on the lcd screen
    char e [] = "its to hot"; // it varmer than should be allowed
    char f [] = "its to dry"; // humidity is above my max
    char g [] = "its too humid"; // humidity is below my max
    char d [] = "its perfect conditions"; // my humidity is between the minimum and maximum


    while(true) // while loop runs all the time 
    {   
        humidity = sensor.ReadHumidity(); // reads the humidity in the room
        temperature = sensor.ReadTemperature(CELCIUS); // read the temperature in the room in celcius
        errtemp = sensor.readData(); 

        if(errtemp == 0) 
        {
            if(temperature > max_temperature){ // if statement
            lcd.DisplayStringAt(0, 25, (uint8_t *) & e , LEFT_MODE ); // its above my maximum temperature
            }
            if(humidity >min_humidity & humidity < max_humidity){
            lcd.DisplayStringAt(0, ysize/2, (uint8_t *) & d , LEFT_MODE ); // my humidity is between min and max, 
            }
            if(humidity > max_humidity){
            lcd.DisplayStringAt(0, ysize/2, (uint8_t *) & g , LEFT_MODE ); // my humidity is above maximum
            }
            if (humidity < min_humidity){
            lcd.DisplayStringAt(0, ysize/2, (uint8_t *) & f , LEFT_MODE ); // my humidity is below my minimum 
            }
        }

        sprintf(showtemp,"temp is %0.0f",temperature); // displays the tempereture 
        BSP_LCD_DisplayStringAt(0, 0, (uint8_t * ) showtemp, LEFT_MODE);

        sprintf(showhumidity, "humidity is %0.0f", humidity); // displays the humidity
        BSP_LCD_DisplayStringAt(0, 50, (uint8_t * ) showhumidity, LEFT_MODE);
        ThisThread::sleep_for(1000); // waits a second before updating the display


    }
}

int main(){ 
    
    temp.start(sensorcheck); 
    lightsens.start(daylight);

}
