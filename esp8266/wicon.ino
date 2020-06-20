/*
 *  wicon.ino
 *  Author: S�meyra Durak
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "index.h"
#include <DNSServer.h>

ESP8266WebServer server(80);
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
String recData = "";
String hello = "";
String AirTemp="Air Temperature",AirPres="Air Pressure",Humid="%Humidity",WindSpeed="Wind Speed",Cloudiness="Cloudiness",WeatSta="Weather Status",CurTime="Current Time",CurDate="Current Date",SerUptime="Server Uptime",SysTemp="System Temperature",ClientNum="Client Number";
String temp = "";
String data_ser = "";
bool server_flag = false;
bool check = false;
bool received = false;
bool week_flag=true;

WiFiClient client;

void WebsiteContent(){
 String s = MAIN_page;
  server.send(200,"text/html",s);
 }

void XMLcontent(){
  String XML ="<?xml version='1.0'?>";
  XML+="<data>";
  XML+="{\"temperature\":\"";
  XML+=AirTemp;
  XML+=", ";
  XML+=WeatSta;
  XML+="\", \"pressure\":\"";
  XML+=AirPres;
  XML+="\", \"humidity\":\"";
  XML+=Humid;
  XML+="\", \"windspeed\":\"";
  XML+=WindSpeed;
  XML+="\", \"cloudiness\":\"";
  XML+=Cloudiness;
  XML+="\", \"currenttime\":\"";
  XML+=CurTime;
  XML+="\", \"currentdate\":\"";
  XML+=CurDate;
  XML+="\", \"serverup\":\"";
  XML+=SerUptime;
  XML+="\", \"systemtemp\":\"";
  XML+=SysTemp;
  XML+="\", \"numberofclients\":\"";
  XML+=ClientNum;
  XML+="\"}";
  XML+="</data>";
  server.send(200,"text/xml",XML);
  }

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(300);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin("ELE417", "EmbeddeD");
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500); }
  
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("SUVI","ELE417SUVI",1,false,8);
  dnsServer.setTTL(1000);
  dnsServer.start(53, "www.suvi.com",apIP);
  
  server.on("/",WebsiteContent);
  server.on("/xml",XMLcontent);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
   server.handleClient(); //softAP client
   
    if (Serial.available() > 0) {
      temp = Serial.readString(); 
      if ((temp[0]=='S') && (temp[1]=='E') && (temp[2]=='R')) {
      server_flag = true;
      data_ser = temp.substring(3, temp.length()); 
      }
      else if((temp[0]=='W') && (temp[1]=='E') && (temp[2]=='B')) {
        if (temp[3]=='1') { 
          AirTemp    = temp.substring(4, temp.length()); 
          AirTemp += "°C";
          }   
        if (temp[3]=='2') { 
          AirPres    = temp.substring(4, temp.length()); 
          AirPres += " hpa";
          }
        if (temp[3]=='3') { 
          Humid = "%";
          Humid += temp.substring(4, temp.length()); 
          }
        if (temp[3]=='4') { 
          WindSpeed  = temp.substring(4, temp.length()); 
          WindSpeed += " m/s";
          }
        if (temp[3]=='5') { 
          Cloudiness = "%";
          Cloudiness += temp.substring(4, temp.length()); 
          }
        if (temp[3]=='6') { 
          WeatSta    = temp.substring(4, temp.length()); 
          }
        if (temp[3]=='7') { 
          CurTime    = temp.substring(4, temp.length());  
          }
        if (temp[3]=='8') { 
          CurDate    = temp.substring(4, temp.length());  
          }
        if (temp[3]=='9') { 
          //SerUptime  = temp.substring(4, temp.length());  
          SerUptime ="";
          int i=4;
          while (!(temp[i]=='w')) {
          SerUptime += temp[i];
          i++;
          if (i>7) {
            SerUptime ="";
            i=4;
            week_flag=false;
            break;  }
          }
          if (week_flag){
            SerUptime += " weeks,";
            bool week_flag=true;
            i++;
          }
          while (!(temp[i]=='d')) {
          SerUptime += temp[i];
          if (i>200) {
            break;  }
          i++; }
          i++;
          SerUptime += " days,";
          while (!(temp[i]=='h')) {
          SerUptime += temp[i];
            if (i>200) {
            break;
          }
          i++; }
          i++;
          SerUptime += " hours,";
          while (temp[i]!='m') {
          SerUptime += temp[i];
          if (i>200) {
            break;
          }
          i++; }
          SerUptime += " mins";
          }
        if (temp[3]==':') { 
          SysTemp    = temp.substring(4, temp.length());
          SysTemp += "°C";
          }
        if (temp[3]==';') { 
          ClientNum  = temp.substring(4, temp.length());  
          }
      } 
    }
    temp="";
    
    if(server_flag) {
     if (!client.connected()) {
          while(!client.connect("10.50.106.10", 10000)) {
           delay(500);
          }
          client.print("hi");
          delay(500);
          while (client.available()) {
          hello += static_cast<char>(client.read());
          }
          hello = "";
      }
            
      if (client.connected()) {
          client.print(data_ser);
          server_flag = false;
          data_ser="";
          check = true;
      }
          
     if (check) { 
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 1500) {
          Serial.print("ª@"); //no response handshake with msp
          check = false;
          break;
        }
      }
  
      while (client.available()) {
        recData += static_cast<char>(client.read());
        received = true;
      }

      if (received) {
        String dummy="ª";
        dummy+=recData;
        recData=dummy;
        Serial.print(recData);
        received = false;
        }
      
    check = false;
    recData = "";
   }
}
}
