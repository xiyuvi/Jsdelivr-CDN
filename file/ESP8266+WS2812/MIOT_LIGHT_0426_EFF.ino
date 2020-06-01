#include <FastLED.h>
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>

//blinker设备的授权码、设备所在环境的wifi、wifi密码
char auth[] = "26685703fba2";
char ssid[] = "Zoo";
char pswd[] = "sjq18338817007";

//五个开关按键 一个滑条 一个调色盘 
BlinkerButton Button1("ButtonKey1");
BlinkerButton Button2("ButtonKey2");
BlinkerButton Button3("ButtonKey3");
BlinkerButton Button4("ButtonKey4");
BlinkerButton Button5("ButtonKey5");
BlinkerSlider Slider1("SliderKey");
BlinkerRGB RGBWS2812("RGBKey");

//灯效模式，默认关灯
int openState = 0;
//灯效节奏
int freq_flash = 25;
uint8_t colorR, colorG, colorB, colorW;   
bool wsState;
uint8_t wsMode = BLINKER_CMD_MIOT_DAY;


#include <Adafruit_NeoPixel.h>

#define PIN            3                               //ws2812 DAT 接的引脚编号，注意开发板不同，请更改
#define NUMPIXELS      8                               //ws2812 灯数
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);//Adafruit_NeoPixel

//FASTLED
CRGB leds[NUMPIXELS];

//颜色写入
void pixelShow()
{
  pixels.setBrightness(colorW);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, colorR, colorG, colorB);
  }
  pixels.show();
}

//频率读取
void tip_led()
{
  digitalWrite(2, LOW);
  digitalWrite(2, HIGH);
  delay(50);
  digitalWrite(2, LOW);
  Blinker.vibrate();
}

void slider1_callback(int32_t value)
{
  tip_led();
  Blinker.vibrate();///震动
  BLINKER_LOG("get slider value: ", value);
  freq_flash = value;
}


//调色盘
void ws2812_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
  tip_led();
  button_clear();
  BLINKER_LOG("R value: ", r_value);
  BLINKER_LOG("G value: ", g_value);
  BLINKER_LOG("B value: ", b_value);
  BLINKER_LOG("Rrightness value: ", bright_value);
  /////颜色和亮度赋值，但不写入
  colorR = r_value;
  colorG = g_value;
  colorB = b_value;
  colorW = bright_value;
  openState = 6;
}


//按键样式
void button_clear()
{
  Button1.print("off");
  Button2.print("off");
  Button3.print("off");
  Button4.print("off");
  Button5.print("off");
  Button1.color("#585858");
  Button2.color("#585858");
  Button3.color("#585858");
  Button4.color("#585858");
  Button5.color("#585858");
}

//but1
void button1_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("日光 music模式开启");
    tip_led();
    button_clear();
    Button1.print("on");
  Button1.color("#DC143C");
    openState = 1;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("日光 music 模式关闭!");
    tip_led();
    button_clear();
    Button1.color("#008000");
    openState = 0;
  }

}

//btn2
void button2_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("夜光模式开启");
    tip_led();
    button_clear();
    Button2.print("on");
    Button2.color("#DC143C");
    openState = 2;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("夜光模式关闭!");
    tip_led();
    button_clear();
    openState = 0;
  }

}

//btn3
void button3_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("温馨模式开启");
    tip_led();
    button_clear();
    Button3.print("on");
    Button3.color("#DC143C");
    openState = 3;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("温馨模式关闭!");
    tip_led();
    button_clear();
    openState = 0;
  }

}

//btn4
void button4_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("电脑模式开启");
    tip_led();
    button_clear();
    Button4.print("on");
    Button4.color("#DC143C");
    openState = 4;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("电脑模式关闭!");
    tip_led();
    button_clear();
    openState = 0;
  }

}

//btn5
void button5_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("电视模式开启");
    tip_led();
    button_clear();
    Button5.print("on");
    Button5.color("#DC143C");
    openState = 5;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("电视模式关闭!");
    tip_led();
    button_clear();
    openState = 0;

  }

}



void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}


uint32_t getColor()
{

  uint32_t color = colorR << 16 | colorG << 8 | colorB;

  return color;
}


void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    tip_led();
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();

    wsState = true;

    if (colorW == 0) colorW = 255;
    openState = 6;
  }
  else if (state == BLINKER_CMD_OFF) {
    tip_led();

    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();

    wsState = false;
    openState = 0;
  }
}

void miotColor(int32_t color)
{
  BLINKER_LOG("need set color: ", color);

  colorR = color >> 16 & 0xFF;
  colorG = color >>  8 & 0xFF;
  colorB = color       & 0xFF;

  BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);
  openState = 6;
  //pixelShow();

  BlinkerMIOT.color(color);
  BlinkerMIOT.print();
}

void miotMode(uint8_t mode)
{
  BLINKER_LOG("need set mode: ", mode);

  if (mode == BLINKER_CMD_MIOT_DAY) {
    // Your mode function
    button_clear();
    Button1.print("on");
    Button1.color("#DC143C");
    openState = 1;

  }
  else if (mode == BLINKER_CMD_MIOT_NIGHT) {
    // Your mode function
    button_clear();
    Button2.print("on");
    Button2.color("#DC143C");
    openState = 2;
  }
  else if (mode == BLINKER_CMD_MIOT_COLOR) {
    // Your mode function
    button_clear();
    openState = 6;

  }
  else if (mode == BLINKER_CMD_MIOT_WARMTH) {
    // Your mode function
    button_clear();
    Button3.print("on");
    Button3.color("#DC143C");
    openState = 3;
  }
  else if (mode == BLINKER_CMD_MIOT_TV) {
    // Your mode function
    button_clear();
    Button5.print("on");
    Button5.color("#DC143C");
    openState = 5;
  }
  else if (mode == BLINKER_CMD_MIOT_READING) {
    // Your mode function
  }
  else if (mode == BLINKER_CMD_MIOT_COMPUTER) {
    // Your mode function
    button_clear();
    Button4.print("on");
    Button4.color("#DC143C");
    openState = 4;
  }

  wsMode = mode;

  BlinkerMIOT.mode(mode);
  BlinkerMIOT.print();
}

void miotBright(const String & bright)
{
  BLINKER_LOG("need set brightness: ", bright);

  colorW = bright.toInt();

  BLINKER_LOG("now set brightness: ", colorW);

  pixelShow();

  BlinkerMIOT.brightness(colorW);
  BlinkerMIOT.print();
  openState = 6;
}

void miotColoTemp(int32_t colorTemp)
{
  BLINKER_LOG("need set colorTemperature: ", colorTemp);

  BlinkerMIOT.colorTemp(colorTemp);
  BlinkerMIOT.print();
}

void miotQuery(int32_t queryCode)
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(wsState ? "on" : "off");
      BlinkerMIOT.color(0);
      BlinkerMIOT.mode(0);
      BlinkerMIOT.colorTemp(1000);
      BlinkerMIOT.brightness(1);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(wsState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_COLOR_NUMBER :
      BLINKER_LOG("MIOT Query Color");
      BlinkerMIOT.color(0);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_MODE_NUMBER :
      BLINKER_LOG("MIOT Query Mode");
      BlinkerMIOT.mode(0);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_COLORTEMP_NUMBER :
      BLINKER_LOG("MIOT Query ColorTemperature");
      BlinkerMIOT.colorTemp(1000);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
      BLINKER_LOG("MIOT Query Brightness");
      BlinkerMIOT.brightness(1);
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(wsState ? "on" : "off");
      BlinkerMIOT.color(0);
      BlinkerMIOT.mode(0);
      BlinkerMIOT.colorTemp(1000);
      BlinkerMIOT.brightness(1);
      BlinkerMIOT.print();
      break;
  }
}

/**************************灯光效果程序**************************/
//FASTLEDS的示例效果
//模式1
void fadeall() {
  for (int i = 0; i < NUMPIXELS; i++) {
    leds[i].nscale8(250);
  }
}

void cylon() {
  static uint8_t hue = 0;

  // First slide the led in one direction
  for (int i = 0; i < NUMPIXELS; i++) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(freq_flash);
  }

  // Now go in the other direction.
  for (int i = (NUMPIXELS) - 1; i >= 0; i--) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(freq_flash);
  }
}

void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  LEDS.addLeds<WS2812, PIN, RGB>(leds, NUMPIXELS);
  LEDS.setBrightness(84);
  Blinker.begin(auth, ssid, pswd);
  pixels.begin();
  Blinker.attachData(dataRead);
 
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachColor(miotColor);
  BlinkerMIOT.attachMode(miotMode);
  BlinkerMIOT.attachBrightness(miotBright);
  BlinkerMIOT.attachColorTemperature(miotColoTemp);
  BlinkerMIOT.attachQuery(miotQuery);

  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  Slider1.attach(slider1_callback);
  RGBWS2812.attach(ws2812_callback);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
  Button3.attach(button3_callback);
  Button4.attach(button4_callback);
  Button5.attach(button5_callback);
  pixels.setBrightness(0);
  button_clear();
}

//以下是灯光效果模式，请自行更改
void mode_1()
{
cylon();
}

void mode_2()
{
  colorR = 255;
  colorG = 206;
  colorB = 0;
  colorW = 150;
  wsState = true;
  pixelShow();
}

void mode_3()
{
  colorR = 225;
  colorG = 57;
  colorB = 0;
  colorW = 150;
  wsState = true;
  pixelShow();
}

void mode_4()
{
  colorR = 0;
  colorG = 255;
  colorB = 230;
  colorW = 150;
  wsState = true;
  pixelShow();
}

void mode_5()
{
  colorR = 255;
  colorG = 255;
  colorB = 255;
  colorW = 150;
  wsState = true;
  pixelShow();
}


void loop()
{
  Blinker.run();

  switch (openState)
  {
    case 0:
      wsState = false;
      openState = 0;
      colorR = 255;
      colorG = 255;
      colorB = 255;
      colorW = 0;
      pixelShow();
      break;
    case 1:
      mode_1();
      break;

    case 2:
      mode_2();
      break;
    case 3:
      mode_3();
      break;
    case 4:
      mode_4();
      break;
    case 5:
      mode_5();
      break;
    case 6:
      pixelShow();
      break;
    default:
      break;
  }
}
