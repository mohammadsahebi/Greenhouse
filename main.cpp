#include "mbed.h"
#include "lv_examples-master/lv_examples.h"
#include "DHT.h" //https://os.mbed.com/users/Wimpie/code/DHT/
#include "TS_DISCO_F746NG.h" //  committed 2015-10-07 https://os.mbed.com/teams/ST/code/TS_DISCO_F746NG/
#include "LCD_DISCO_F746NG.h" // updated 2015-09-28	requires BSP_DISCO_F746NG https://os.mbed.com/teams/ST/code/LCD_DISCO_F746NG/
#include "lvgl/lvgl.h" //https://github.com/lvgl/lvgl
/* 
#define LVGL_TICK 5
#define TICKER_TIME 0.001 * LVGL_TICK

void lv_ticker_func();
void display_init(void);
void touchpad_init(void);
static void my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
static bool touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data);
 
Ticker ticker;
*/
    TS_StateTypeDef TS_State;  
    DHT sensor(A0,SEN51035P); // Use the SEN11301P sensor
    AnalogIn lightsensor (A2);
    LCD_DISCO_F746NG lcd;
    TS_DISCO_F746NG ts;
    DigitalOut light(D4);
    AnalogIn button(D3);

    Thread temp;
    Thread lightsens;
    Thread water;

    bool test;

void daylight(){
    float daytime = 0.5, lys;
    int artificiallight = 43200;
    while(true)
    {
    lys = lightsensor.read();
        if(lys > daytime or lys == daytime){
            printf("daylight %f\n",lys);
            wait(30);
        }
        if(lys < daytime){
            printf("bad light %f\n",lys);
            lys = 1;
            wait(43200);
        }
    }

}


void sensorcheck(){
    int errtemp;
    float temperature, max_temperature,humidity, max_humidity, min_humidity;
    max_temperature = 28;
    min_humidity = 50; 
    max_humidity = 70;

    while(true)
    {   
        humidity = sensor.ReadHumidity();
        temperature = sensor.ReadTemperature(CELCIUS);
        errtemp = sensor.readData();
        if(errtemp == 0)
        {
            printf("temp %f\n",temperature);
            printf("current humidity %f\n", humidity);
            if(temperature > max_temperature){
                printf("opening widows \n");
                wait(300);
            }
            if(humidity > max_humidity)
            {
                printf("its to humid \n");
            }
            if (humidity < min_humidity){
                printf("its to dry \n");
            }
        }

    }
}

void waterplant(){

    int interval = 2;
    int watertime = 600;
}

int main(){
    
    temp.start(sensorcheck);
    lightsens.start(daylight);
    //water.start(waterplant);
    test = button.read();
}





 
/* 
void lv_ticker_func(){
    lv_tick_inc(LVGL_TICK); 
    //Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10). 
    //It is required for the internal timing of LittlevGL.
    lv_task_handler(); 
    //Call lv_task_handler() periodically every few milliseconds. 
    //It will redraw the screen if required, handle input devices etc.
}
void display_init(void){
    //Init the touch screen display via the BSP driver. Based on ST's example.
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT );
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
    
    lv_init();                                  //Initialize the LittlevGL
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf[LV_HOR_RES_MAX * 10]; //Declare a buffer for 10 lines                                                              
    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10); //Initialize the display buffer
    
    //Implement and register a function which can copy a pixel array to an area of your display
    lv_disp_drv_t disp_drv;                     //Descriptor of a display driver
    lv_disp_drv_init(&disp_drv);                //Basic initialization
    disp_drv.flush_cb = my_disp_flush_cb;       //Set your driver function
    disp_drv.buffer = &disp_buf;                //Assign the buffer to the display
    lv_disp_drv_register(&disp_drv);            //Finally register the driver
}
void my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    //The most simple case (but also the slowest) to put all pixels to the screen one-by-one
    uint16_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            //put_px(x, y, *color_p)
            BSP_LCD_DrawPixel( x, y, color_p->full);
            color_p++;
        }
    }
    //IMPORTANT!!!* Inform the graphics library that you are ready with the flushing
    lv_disp_flush_ready(disp_drv);
}
void touchpad_init(void){
    BSP_TS_Init(480, 272);
    lv_indev_drv_t indev_drv;                       //Descriptor of an input device driver
    lv_indev_drv_init(&indev_drv);                  //Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER;         //The touchpad is pointer type device
    indev_drv.read_cb = touchpad_read;              //Set the touchpad_read function
    lv_indev_drv_register(&indev_drv);              //Register touch driver in LvGL
}

static bool touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data){
    // Read your touchpad
    BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected) {
        data->point.x = TS_State.touchX[0];
        data->point.y = TS_State.touchY[0];
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->point.x = 0;
        data->point.y = 0;
        data->state = LV_INDEV_STATE_REL;
    }
    return false;   //false: no more data to read because we are no buffering
}
*/