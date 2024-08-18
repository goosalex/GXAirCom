#include "LCD.h"
#include "tools.h"
#include "icons.h"
#include "icons2.h"
#include <FanetLora.h>

LCD_class::LCD_class(){
}


bool LCD_class::begin(SPIClass *SPI_LCD_SD, uint8_t SHARP_SS){
  oldScreenNumber = 0;
  if (displayLCD == NULL){
    //displayLCD = new Adafruit_SharpMem(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);
    //displayLCD = new Adafruit_SharpMem(&SPI_LCD_SD, SHARP_SS, 144, 168);
    displayLCD = new Adafruit_SharpMem(SPI_LCD_SD, SHARP_SS, 144, 168);
  }
  displayLCD->begin();

  displayLCD->setTextColor(BLACK);
  displayLCD->clearDisplay();
  displayLCD->refresh();
  displayLCD->drawXBitmap(0,2,G_Logo_bits,G_Logo_width,G_Logo_height,BLACK);
  displayLCD->refresh();
  delay(1000);
  displayLCD->drawXBitmap(30,2,X_Logo_bits,X_Logo_width,X_Logo_height,BLACK);
  displayLCD->refresh();
  delay(1000);
  displayLCD->drawXBitmap(69,2,AirCom_Logo_bits,AirCom_Logo_width,AirCom_Logo_height,BLACK);
  displayLCD->setTextSize(1);
  displayLCD->setCursor(85,55);
  displayLCD->print(VERSION);
  /*
  displayLCD->refresh();
  while (setting.myDevId.length() == 0){ //waiting for dev-ID
    delay(100);
  }
  */
  displayLCD->setTextSize(1);
  displayLCD->setCursor(0,55);
  displayLCD->print(setting.myDevId.c_str());
  displayLCD->refresh();
  delay(1000);
  if (setting.gs.SreenOption == eScreenOption::ALWAYS_OFF){
    PowerOff();
  }else{
    displayLCD->clearDisplay();
    displayLCD->refresh();
  }  
  return true;
}


void LCD_class::PowerOn(void){
  if (bDisplayOn){
    return; //already on
  }
  bDisplayOn = true;
}

void LCD_class::PowerOff(void){
  if (!bDisplayOn){ 
    return; //already off
  }
  displayLCD->clearDisplay();
  displayLCD->refresh();
  log_i("set display to off");
  bDisplayOn = false;
}

void LCD_class::drawWifiStat(eConnectionState wifiStat)
{
  if (wifiStat!=IDLE) 
  {
    
    WIFI_bits[2]=0xC4;
    WIFI_bits[6]=0xC9;
    displayLCD->drawXBitmap(85,0,WIFI_bits,WIFI_width,WIFI_height,BLACK);
  }
  if (wifiStat==FULL_CONNECTED) 
  { 
    WIFI_bits[2]=0x4;
    WIFI_bits[6]=0x9;
    displayLCD->drawXBitmap(85,0,WIFI_bits,WIFI_width,WIFI_height,BLACK);
  }
}

void LCD_class::webUpdate(void){
  displayLCD->clearDisplay();
  displayLCD->setTextSize(2);
  displayLCD->setCursor(10,5);
  displayLCD->print("FW-UPDATE");
  displayLCD->setCursor(10,30);
  displayLCD->print("wait...");
  displayLCD->refresh();
}

void LCD_class::drawAircraftType(int16_t x, int16_t y, uint8_t AircraftType){
  switch (AircraftType)
  {
  case FanetLora::paraglider :
  case FanetLora::leg_para_glider :
      displayLCD->drawXBitmap(x,y, Paraglider16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  case FanetLora::hangglider :
  case FanetLora::leg_hang_glider :
      displayLCD->drawXBitmap(x,y, Hangglider16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  case FanetLora::balloon :
  case FanetLora::leg_balloon :
      displayLCD->drawXBitmap(x,y, Ballon16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  case FanetLora::glider :
  case FanetLora::leg_glider_motor_glider :
      displayLCD->drawXBitmap(x,y, Sailplane16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  case FanetLora::poweredAircraft :
  case FanetLora::leg_aircraft_reciprocating_engine :
      displayLCD->drawXBitmap(x,y, Airplane16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  case FanetLora::helicopter :
  case FanetLora::leg_helicopter_rotorcraft :
      displayLCD->drawXBitmap(x,y, Helicopter16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  case FanetLora::uav :
  case FanetLora::leg_uav :
      displayLCD->drawXBitmap(x,y, UAV16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  
  default:
      displayLCD->drawXBitmap(x,y, UFO16_bits, 16, 16, BLACK);   //GxEPD_BLACK);
      break;
  }
}

void LCD_class::drawSatCount(int16_t x, int16_t y,uint8_t value){
    displayLCD->setTextSize(1);
    if (value == 0){
        displayLCD->drawXBitmap(x, y,gpsoff_bits,  16, 16, BLACK);
    }else{
        displayLCD->drawXBitmap(x, y,gpsOn_bits,  16, 16, BLACK);
        displayLCD->setCursor(x+18,y+4);
        displayLCD->print(String(value));
    }

}


void LCD_class::drawspeaker(int16_t x, int16_t y){
    uint8_t volume = 0;
    if (status.bMuting){
        volume = 0;
    }else{
        if (setting.vario.volume == LOWVOLUME){
            volume = 1;
        }else if (setting.vario.volume == MIDVOLUME){
            volume = 2;
        }else{
            volume = 3;
        }
    }
    switch (volume)
    {
    case 1:
        displayLCD->drawXBitmap(x, y,speakerlow_bits,  16, 16, BLACK);
        break;
    case 2:
        displayLCD->drawXBitmap(x, y,speakermid_bits,  16, 16, BLACK);
        break;
    case 3:
        displayLCD->drawXBitmap(x, y,speakerhigh_bits,  16, 16, BLACK);
        break;        
    default:
        displayLCD->drawXBitmap(x, y,speakeroff_bits,  16, 16, BLACK);
        break;
    }
}

void LCD_class::drawflying(int16_t x, int16_t y, bool flying){
    if (flying){
        displayLCD->drawXBitmap(x, y,flying_bits,  16, 16, BLACK);
    }else{
        displayLCD->drawXBitmap(x, y,not_flying_bits,  16, 16, BLACK);
    }
}

void LCD_class::drawBluetoothstat(int16_t x, int16_t y){
    if (status.bluetoothStat == 1){
     displayLCD->drawXBitmap(x,y,BT_bits,8,10,BLACK);
    }else if (status.bluetoothStat == 2){
      displayLCD->fillRect(x,y,8,10,BLACK);
      displayLCD->drawXBitmap(x,y,BT_bits,8,10,WHITE);
    }
}

void LCD_class::drawBatt(int16_t x, int16_t y,uint8_t value){
  static uint8_t DrawValue = 0;
  if (value == 255){
      DrawValue = (DrawValue + 1) %5; 
  }else{
      DrawValue = value / 25;
  }
  //log_i("%d",DrawValue);
  displayLCD->fillRect(x,y,17,8,BLACK);
  switch (DrawValue)
  {
  case 1:
      displayLCD->drawBitmap(x, y, bat1icons, 17, 8, WHITE);   //GxEPD_BLACK);    
      break;
  case 2:
      displayLCD->drawBitmap(x, y, bat2icons, 17, 8, WHITE);   //GxEPD_BLACK);    
      break;
  case 3:
      displayLCD->drawBitmap(x, y, bat3icons, 17, 8, WHITE);   //GxEPD_BLACK);    
      break;
  case 4:
      displayLCD->drawBitmap(x, y, bat4icons, 17, 8, WHITE);   //GxEPD_BLACK);    
      break;
  default:
      displayLCD->drawBitmap(x, y, bat0icons, 17, 8, WHITE);   //GxEPD_BLACK);    
      break;
  }
}

void LCD_class::printGPSData(uint32_t tAct){
  String s = "";
  displayLCD->clearDisplay();
  displayLCD->setTextSize(2);
  displayLCD->setCursor(0,0);
  drawAircraftType(0,0,setting.AircraftType);
  
  drawSatCount(18,0,(status.gps.NumSat > 9) ? 9 : status.gps.NumSat);
  drawspeaker(47,0);
  drawflying(67,0,status.flying);
  drawWifiStat(status.wifiSTA.state);
  drawBluetoothstat(101,0);
  drawBatt(111, 0,(status.battery.charging) ? 255 : status.battery.percent);

  displayLCD->setTextSize(3);

  displayLCD->setCursor(0,20);
  displayLCD->print(setStringSize(String(status.vario.ClimbRate,1) + "ms",7));

  displayLCD->setTextSize(2);

  displayLCD->setCursor(0,46);
  displayLCD->print(setStringSize(String(status.gps.alt,0) + "m",4));

  displayLCD->setCursor(65,46);
  displayLCD->print(setStringSize(String(status.gps.speed,0) + "kh",5));

  displayLCD->refresh();

}

String LCD_class::setStringSize(String s,uint8_t sLen){
  uint8_t actLen = (uint8_t)s.length();
  String sRet = "";
  for (uint8_t i = actLen;i < sLen;i++){
    sRet += " ";
  }
  sRet += s;
  return sRet;
}

void LCD_class::DrawAngleLine(int16_t x,int16_t y,int16_t length,float deg){
  int16_t xStart;
  int16_t yStart;
  int16_t xEnd;
  int16_t yEnd;
  float rads;
  rads = deg2rad(deg);
  xStart=(int)roundf(((sin(rads) * length/2) * 1) + x);
  yStart=(int)roundf(((cos(rads) * length/2) * -1) + y);
  xEnd=(int)roundf(((sin(rads) * length/2) * -1) + x);
  yEnd=(int)roundf(((cos(rads) * length/2) * 1) + y);  
  displayLCD->drawLine(xStart,yStart,xEnd,yEnd,BLACK);
  //log_i("x=%i,y=%i,deg=%0.1f,X-Start=%i,Y-Start=%i,X-End=%i,Y-End=%i",x,y,deg,xStart,yStart,xEnd,yEnd);
}
void LCD_class::DrawRadarPilot(uint8_t neighborIndex){
  float pilotDistance = 0.0;
  int bearing = 0;
  float rads;
  int relNorth;
  int relEast;
  displayLCD->setCursor(68,16);
  if (fanet.neighbours[neighborIndex].name.length() > 0){
    displayLCD->print(fanet.neighbours[neighborIndex].name.substring(0,10)); //max. 10 signs
  }else{
    displayLCD->print(fanet.getDevId(fanet.neighbours[neighborIndex].devId));
  }
  pilotDistance = distance(fanet._myData.lat, fanet._myData.lon,fanet.neighbours[neighborIndex].lat,fanet.neighbours[neighborIndex].lon, 'K') * 1000 ;
  bearing = CalcBearingA( fanet._myData.lat, fanet._myData.lon,fanet.neighbours[neighborIndex].lat,fanet.neighbours[neighborIndex].lon);
  rads = deg2rad(bearing + (fanet._myData.heading * -1));
  relEast=(int)((sin(rads) * 16) + RADAR_SCREEN_CENTER_X-8);
  relNorth=(int)(((cos(rads) * 16) * -1) + RADAR_SCREEN_CENTER_Y-8);
  //log_i("bearing=%i",bearing);
  //log_i("relNorth=%i",relNorth);
  //log_i("relEast=%i",relEast);
  switch (fanet.neighbours[neighborIndex].aircraftType)
  {
  case FanetLora::aircraft_t ::paraglider :
  case FanetLora::leg_para_glider :
    displayLCD->drawXBitmap(relEast, relNorth, Paraglider16_bits,16, 16,BLACK);      
    break;
  case FanetLora::aircraft_t::hangglider :
  case FanetLora::leg_hang_glider :
    displayLCD->drawXBitmap(relEast, relNorth, Hangglider16_bits,16, 16,BLACK);      
    break;
  case FanetLora::aircraft_t::balloon :
  case FanetLora::leg_balloon :
    displayLCD->drawXBitmap(relEast, relNorth, Ballon16_bits,16, 16,BLACK);      
    break;
  case FanetLora::aircraft_t::glider :
  case FanetLora::leg_glider_motor_glider :
    displayLCD->drawXBitmap(relEast, relNorth, Sailplane16_bits,16, 16,BLACK);      
    break;
  case FanetLora::aircraft_t::poweredAircraft :
  case FanetLora::leg_aircraft_reciprocating_engine :
    displayLCD->drawXBitmap(relEast, relNorth, Airplane16_bits,16, 16,BLACK);      
    break;
  case FanetLora::aircraft_t::helicopter :
  case FanetLora::leg_helicopter_rotorcraft :
    displayLCD->drawXBitmap(relEast, relNorth, Helicopter16_bits,16, 16,BLACK);      
    break;
  case FanetLora::aircraft_t::uav:
  case FanetLora::leg_uav :
    displayLCD->drawXBitmap(relEast, relNorth, UAV16_bits,16, 16,BLACK);      
    break;
  
  default:
    displayLCD->drawXBitmap(relEast, relNorth, UFO16_bits,16, 16,BLACK);      
    break;
  }
  if (pilotDistance > 1000){
    pilotDistance /= 1000;
    displayLCD->setCursor(68,28);
    displayLCD->printf("%skm",setStringSize(String(pilotDistance,1),5).c_str());
  }else{
    displayLCD->setCursor(75,28);
    displayLCD->printf("%sm",setStringSize(String(pilotDistance,0),5).c_str());
  }
  displayLCD->setCursor(75,40); //display relative alt
  displayLCD->printf("%sm",setStringSize(String(fanet.neighbours[neighborIndex].altitude - fanet._myData.altitude,0),5).c_str());
  displayLCD->setCursor(75,52); //display climbing
  displayLCD->printf("%sms",setStringSize(String(fanet.neighbours[neighborIndex].climb,1),5).c_str());
}



void LCD_class::DrawRadarScreen(uint32_t tAct,eRadarDispMode mode){
  static uint8_t neighborIndex = 0;
  int index;
  int16_t xStart;
  int16_t yStart;
  float rads;
  
  String s = "";
  displayLCD->clearDisplay();
  drawAircraftType(0,0,setting.AircraftType);
  drawSatCount(18,0,(status.gps.NumSat > 9) ? 9 : status.gps.NumSat);
  drawWifiStat(status.wifiSTA.state);
  drawBluetoothstat(101,0);
  drawBatt(111, 0,(status.battery.charging) ? 255 : status.battery.percent);

  
  displayLCD->setTextSize(1);
  displayLCD->drawCircle(RADAR_SCREEN_CENTER_X,RADAR_SCREEN_CENTER_Y,24,BLACK);

  DrawAngleLine(RADAR_SCREEN_CENTER_X,RADAR_SCREEN_CENTER_Y,30,fanet._myData.heading * -1);
  DrawAngleLine(RADAR_SCREEN_CENTER_X,RADAR_SCREEN_CENTER_Y,6,(fanet._myData.heading + 90) * -1);
  rads = deg2rad(fanet._myData.heading * -1);
  xStart=(int)(((sin(rads) * 19) * 1) + RADAR_SCREEN_CENTER_X);
  yStart=(int)(((cos(rads) * 19) * -1) + RADAR_SCREEN_CENTER_Y);
  displayLCD->setCursor(xStart-2,yStart-3);
  displayLCD->print("N");
  displayLCD->setTextSize(1);
  displayLCD->setCursor(42,0);
  switch (mode)
  {
  case eRadarDispMode::CLOSEST:
    displayLCD->print("CLOSEST");
    if (status.gps.Fix == 0){
      displayLCD->setCursor(60,16);
      displayLCD->print("NO GPS-FIX");
      break;
    } 
    index = fanet.getNearestNeighborIndex();
    //log_i("index %i",index);
    if (index < 0) break;
    neighborIndex = index;
    DrawRadarPilot(neighborIndex);
    break;
  case eRadarDispMode::LIST:
    displayLCD->print("LIST");
    if (status.gps.Fix == 0){
      displayLCD->setCursor(60,16);
      displayLCD->print("NO GPS-FIX");
      break;
    } 
    index = fanet.getNextNeighbor(neighborIndex);
    if (index < 0) break;
    neighborIndex = index;
    DrawRadarPilot(neighborIndex);
    break;
  case eRadarDispMode::FRIENDS:
    displayLCD->print("FRIENDS");
    if (status.gps.Fix == 0){
      displayLCD->setCursor(50,16);
      displayLCD->print("NO GPS-FIX");
      break;
    } 
    break;    
  default:
    break;
  }
  displayLCD->refresh();
}

void LCD_class::drawSignal(int16_t x, int16_t y,uint8_t strength) {
  if ((strength <= 9) && (strength >= 3)){
      displayLCD->drawBitmap(x,y,signal_1, SIGNALWIDTH, SIGNALHEIGHT, BLACK,WHITE);
  }else if ((strength >= 10) && (strength <= 14)){
      displayLCD->drawBitmap(x,y,signal_2, SIGNALWIDTH, SIGNALHEIGHT, BLACK,WHITE);
  }else if ((strength >= 15) && (strength <= 19)){
      displayLCD->drawBitmap(x,y,signal_3, SIGNALWIDTH, SIGNALHEIGHT, BLACK,WHITE);
  }else if ((strength >= 19) && (strength <= 30)){
      displayLCD->drawBitmap(x,y,signal_4, SIGNALWIDTH, SIGNALHEIGHT, BLACK,WHITE);
  }else{
      displayLCD->drawBitmap(x,y,signal_0, SIGNALWIDTH, SIGNALHEIGHT, BLACK,WHITE);
  }
}

void LCD_class::printWeather(uint32_t tAct){
  static uint32_t tRefresh = millis();
  static uint8_t screen = 0;
  if (timeOver(tAct,tRefresh,3000)){
    tRefresh = tAct;
    String s = "";
    displayLCD->clearDisplay();
    displayLCD->setTextSize(1);
    displayLCD->setCursor(0,0);
    displayLCD->print(setting.PilotName);
    drawWifiStat(status.wifiSTA.state);
    drawBluetoothstat(101,0);
    drawBatt(111, 0,(status.battery.charging) ? 255 : status.battery.percent);
    if (status.gsm.bHasGSM){
      drawSignal(60,0,status.gsm.SignalQuality);
    }    
    displayLCD->setTextSize(3); //set Textsize
    displayLCD->setCursor(0,21);
    switch(screen)
    {
      case 0: //battery-voltage
        s = String(status.battery.voltage / 1000.,2) + "V";
        break;
      case 1: //wind dir
        s = "  " + getWDir(status.weather.WindDir);
        break;
      case 2: //speed and gust
        s = String(round(status.weather.WindSpeed),0) + "|" + String(round(status.weather.WindGust),0); // + "kh";
        break;
      case 3: //temp
        s = " " + String(status.weather.temp,1) + "C";
        break;
      case 4: //pressure
        displayLCD->setCursor(0,26);
        displayLCD->setTextSize(2); //set Textsize
        s = " " + String(status.weather.Pressure,1) + "hPa";
        break;
      case 5: //humidity
        s = " " + String(status.weather.Humidity,0) + "%";
        break;
    }
    screen++;
    if (screen > 5) screen = 0;
    displayLCD->print(s);
    displayLCD->setTextSize(1);
    displayLCD->setCursor(0,55);
    displayLCD->print(setting.myDevId);
    displayLCD->setCursor(85,55);
    displayLCD->print(VERSION);
    displayLCD->refresh();
  }
}

void LCD_class::printScanning(uint32_t tAct){
  static uint8_t icon = 0;
  if (setting.gs.SreenOption == eScreenOption::ON_WHEN_TRAFFIC){
    PowerOff();
    return;
  }
  PowerOn();
  displayLCD->clearDisplay();
  drawWifiStat(status.wifiSTA.state);
  drawBluetoothstat(101,0);
  drawBatt(111, 0,(status.battery.charging) ? 255 : status.battery.percent);
  switch (icon)
  {
  case 0: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 1: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    break;
  case 2: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 3: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, PGRX_bits,PGRX_width, PGRX_height,BLACK);      
    break;
  case 4: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 5: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, HGRX_bits,HGRX_width, HGRX_height,BLACK);      
    break;
  case 6: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 7: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, BLRX_bits,BLRX_width, BLRX_height,BLACK);      
    break;
  case 8: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 9: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, SPRX_bits,SPRX_width, SPRX_height,BLACK);      
    break;
  case 10: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 11: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, Airplane40_bits,Airplane40_width, Airplane40_height,BLACK);      
    break;
  case 12: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 13: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, Helicopter40_bits,Helicopter40_width, Helicopter40_height,BLACK);      
    break;
  case 14: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 15: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, UAVRX_bits,UAVRX_width, UAVRX_height,BLACK);      
    break;
  case 16: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    break;
  case 17: 
    displayLCD->drawXBitmap(1,10,Antenna_bits,Antenna_width,Antenna_height,BLACK);
    displayLCD->drawXBitmap(42,10,WFTX_bits,WFTX_width,WFTX_height,BLACK);
    displayLCD->drawXBitmap(66, 34, WFRX_bits,WFRX_width, WFRX_height,BLACK );
    displayLCD->drawXBitmap(88, 10, UFORX_bits,UFORX_width, UFORX_height,BLACK);      
    break;



  
  default:
    break;
  }
  icon++;
  if (icon > 17) icon = 2;

  displayLCD->setTextSize(1);
  displayLCD->setCursor(0,54);
  displayLCD->print("Scanning the skyes...");

  displayLCD->refresh();
}

void LCD_class::printGSData(uint32_t tAct){
  static uint8_t index = 0;
  static uint16_t fanetRx = 0;
  static uint16_t legacyRx = 0;
  static uint8_t received = 0;
  char buf[10];
  PowerOn();
  displayLCD->clearDisplay();
  drawWifiStat(status.wifiSTA.state);
  drawBluetoothstat(101,0);
  drawBatt(111, 0,(status.battery.charging) ? 255 : status.battery.percent);


  //show rx-count
  displayLCD->setTextSize(1);

  displayLCD->setCursor(78,0);
  if (fanetRx != status.fanetRx){
    fanetRx = status.fanetRx;
    received++;
  }
  if (legacyRx != status.legRx){
    legacyRx = status.legRx;
    received++;
  }

  sprintf(buf, "%d", received % 10);
  displayLCD->print(buf);


  //get next index
  for (int i = 0;i < MAXNEIGHBOURS;i++){
    if (fanet.neighbours[index].devId) break;
    index ++;
    if (index >= MAXNEIGHBOURS) index = 0;
  }
  if (!fanet.neighbours[index].devId){
    //no more neighbours --> return false
    displayLCD->refresh();
    return;
  } 



  displayLCD->setCursor(0,0);
  displayLCD->print(fanet.getDevId(fanet.neighbours[index].devId));
  displayLCD->setCursor(35,0);
  sprintf(buf, "%4ddb", fanet.neighbours[index].rssi);
  displayLCD->print(buf);

  displayLCD->setCursor(0,10);
  displayLCD->print(fanet.getNeighbourName(fanet.neighbours[index].devId));

  switch (fanet.neighbours[index].aircraftType)
  {
  case FanetLora::aircraft_t ::paraglider :
  case FanetLora::leg_para_glider :
    displayLCD->drawXBitmap(88, 12, PGRX_bits,PGRX_width, PGRX_height,BLACK);      
    break;
  case FanetLora::aircraft_t::hangglider :
  case FanetLora::leg_hang_glider :
    displayLCD->drawXBitmap(88, 12, HGRX_bits,HGRX_width, HGRX_height,BLACK);      
    break;
  case FanetLora::aircraft_t::balloon :
  case FanetLora::leg_balloon :
    displayLCD->drawXBitmap(88, 12, BLRX_bits,BLRX_width, BLRX_height,BLACK);      
    break;
  case FanetLora::aircraft_t::glider :
  case FanetLora::leg_glider_motor_glider :
    displayLCD->drawXBitmap(88, 12, SPRX_bits,SPRX_width, SPRX_height,BLACK);      
    break;
  case FanetLora::aircraft_t::poweredAircraft :
  case FanetLora::leg_aircraft_reciprocating_engine :
    displayLCD->drawXBitmap(88, 12, Airplane40_bits,Airplane40_width, Airplane40_height,BLACK);      
    break;
  case FanetLora::aircraft_t::helicopter :
  case FanetLora::leg_helicopter_rotorcraft :
    displayLCD->drawXBitmap(88, 10, Helicopter40_bits,Helicopter40_width, Helicopter40_height,BLACK);      
    break;
  case FanetLora::aircraft_t::uav:
  case FanetLora::leg_uav :
    displayLCD->drawXBitmap(88, 12, UAVRX_bits,UAVRX_width, UAVRX_height,BLACK);      
    break;
  
  default:
    displayLCD->drawXBitmap(88, 12, UFORX_bits,UFORX_width, UFORX_height,BLACK);      
    break;
  }
  
  displayLCD->setCursor(0,20);
  displayLCD->print("alt:");
  displayLCD->print(fanet.neighbours[index].altitude,0);
  displayLCD->print("m");

  displayLCD->setCursor(0,32);
  displayLCD->print("speed:");
  displayLCD->print(fanet.neighbours[index].speed,0);
  displayLCD->print("kmh");

  displayLCD->setCursor(0,44);
  displayLCD->print(fanet.neighbours[index].climb,1);
  displayLCD->print("m/s");

  displayLCD->setCursor(50,44);
  displayLCD->print(fanet.neighbours[index].heading,0);
  displayLCD->print("deg");

  displayLCD->setCursor(0,56);
  displayLCD->print(fanet.neighbours[index].lat,6);
  displayLCD->setCursor(64,56);
  displayLCD->print(fanet.neighbours[index].lon,6);


  displayLCD->refresh();
  index ++;
  if (index >= MAXNEIGHBOURS) index = 0;
}


void LCD_class::run(void){
  uint32_t tAct = millis();
  static uint32_t tDisplay = millis();  
  #ifdef GSMODULE
  if (setting.Mode == eMode::GROUND_STATION){
    if (setting.gs.SreenOption == eScreenOption::WEATHER_DATA){
      printWeather(tAct);
    }else if (setting.gs.SreenOption != eScreenOption::ALWAYS_OFF){
      if (fanet.getNeighboursCount() == 0){
        if (timeOver(tAct,tDisplay,500)){
          tDisplay = tAct;
          printScanning(tAct);
        }
      }else{
        if (timeOver(tAct,tDisplay,DISPLAY_UPDATE_RATE_GS)){
          tDisplay = tAct;
          printGSData(tAct);
        }
      }
    }
  }
  #endif
  #ifdef AIRMODULE
  if (setting.Mode == eMode::AIR_MODULE){
    switch (setting.screenNumber)
    {
    case 0: //main-Display
      if ((timeOver(tAct,tDisplay,DISPLAY_UPDATE_RATE)) || (oldScreenNumber != setting.screenNumber)){
        tDisplay = tAct;
        printGPSData(tAct);          
      }
      break;
    case 1: //radar-screen with list
      if ((timeOver(tAct,tDisplay,DISPLAY_UPDATE_RATE2)) || (oldScreenNumber != setting.screenNumber)){
        tDisplay = tAct;              
        DrawRadarScreen(tAct,eRadarDispMode::LIST);
      }
      break;
    case 2: //radar-screen with closest
      if ((timeOver(tAct,tDisplay,DISPLAY_UPDATE_RATE)) || (oldScreenNumber != setting.screenNumber)){
        tDisplay = tAct;
        DrawRadarScreen(tAct,eRadarDispMode::CLOSEST);
      }
      break;
    case 3: //list aircrafts
      if ((timeOver(tAct,tDisplay,DISPLAY_UPDATE_RATE)) || (oldScreenNumber != setting.screenNumber)){
        tDisplay = tAct;
      }
      break;
    default:
      break;
    }
    oldScreenNumber = setting.screenNumber;
  }
  #endif
}

void LCD_class::end(void){
  displayLCD->setTextColor(BLACK);
  displayLCD->clearDisplay();
  displayLCD->drawXBitmap(0,2,G_Logo_bits,G_Logo_width,G_Logo_height,BLACK);
  displayLCD->drawXBitmap(30,2,X_Logo_bits,X_Logo_width,X_Logo_height,BLACK);
  displayLCD->drawXBitmap(69,2,AirCom_Logo_bits,AirCom_Logo_width,AirCom_Logo_height,BLACK);
  displayLCD->setTextSize(1);
  displayLCD->setCursor(85,55);
  displayLCD->print(VERSION);
  displayLCD->setCursor(0,55);
  displayLCD->print(setting.myDevId);
  displayLCD->refresh();
  delay(1000);
  displayLCD->clearDisplay();
  displayLCD->drawXBitmap(0,2,G_Logo_bits,G_Logo_width,G_Logo_height,BLACK);
  displayLCD->drawXBitmap(30,2,X_Logo_bits,X_Logo_width,X_Logo_height,BLACK);
  displayLCD->refresh();
  delay(1000);
  displayLCD->clearDisplay();
  displayLCD->drawXBitmap(0,2,G_Logo_bits,G_Logo_width,G_Logo_height,BLACK);
  displayLCD->refresh();
  delay(1000);
  PowerOff();
}