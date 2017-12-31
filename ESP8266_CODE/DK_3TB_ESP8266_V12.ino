
#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>



#define SSID "Tien"        // TEN WIFI
#define PASS "0873008888"                    // PASSWORD WIFI
 int16_t i;
boolean status1 = false;
boolean status2 = false;

ESP8266WebServer server(80);                             // chon cong giao tiep la 80,
long period;
int offset=1,refresh=0;
// ******************* String form to sent to the client-browser ************************************
String form;
void homepage()        ////TAO GIAO DIEN WEB
{
  form=" ";
form+="<html>" ; 
form+="<head>";
form+="<meta http-equiv='refresh' content='5;  url=/' >";
form+="<title> DIEU KHIEN THIET BI QUA MANG LAN</title>";
form+="<script>";
       
form+="            function startTime() ";
form+="            {";
form+="                var today = new Date();";

form+="                var h = today.getHours();";
form+="                var m = today.getMinutes();";
form+="                var s = today.getSeconds();";

form+="                m = checkTime(m);";
form+="                s = checkTime(s);";

form+="                document.getElementById('timer').innerHTML = h + ':' + m + ':' + s;";

form+="                var t = setTimeout(function() {";
form+="                    startTime();";
form+="                }, 500);";
form+="            }";

form+="            function checkTime(i) ";
form+="            {";
form+="                if (i < 10) {";
form+="                    i = '0' + i;";
form+="                }";
form+="                return i;";
form+="            }";
form+="        </script>";
 

form+="</head>";

form+="<style type = 'text/css'>";
form+="body {background-color:black;";
form+="          background-size:cover;";
         
form+="}";
form+="a { ";
form+="display: inline-block;";
form+=" width: 32px;";
form+="   height: 32px;";
form+="}";

form+="h1 {";
form+="	font: 3em normal Arial, Helvetica, sans-serif;";
form+="	padding: 15px;	margin: 0;";
form+="	text-align:center;";
form+="	color:yellow;";
form+="}";
form+="h2 {";
form+="	font: 2em normal Arial, Helvetica, sans-serif;";
form+="	padding: 15px;	margin: 0;";
form+="	text-align:center;";
form+="	color:cyan;";
form+="}";
form+="h3 {";
form+="	font: 2em normal Arial, Helvetica, sans-serif;";
form+="	padding: 5px;	margin: 0;";
form+="	text-align:center;";
form+="	color:white;";
form+="}";
form+=" a1 {";
form+=" 	font: 1em normal Arial, Helvetica, sans-serif;";
form+=" 	padding: 5px;	margin: 0;";
form+=" 	text-align:center;";
form+=" 	color:red;";
form+=" }";
form+="</style>";


form+="<body background='http://st2.depositphotos.com/1184748/7055/v/450/depositphotos_70556373-stock-video-nature-background-beautiful-blurred-abstract.jpg' onload='startTime()' >";
form+="<img src= 'https://www.tuyensinh.in/images/truong/71/logo-spk.png' width='150' height='100'>";
form+="  <p>";
form+="  <center>";
form+="  <h1>DIEU KHIEN THIET BI QUA MANG LAN</h1>";
form+="  <h3>";

form+="  <br>";

form+="  RELAY 1:     &nbsp  &nbsp  &nbsp"; if(status1==false){form+="   TINH TRANG: OFF &nbsp  &nbsp  &nbsp "; 
                                                  form+="<img src= 'http://www.designer-download.com/wp-content/uploads/2013/09/White-light-bulb-icon-psd.jpg' width='40' height='40'> &nbsp  &nbsp  &nbsp ";}
                                                       else {form+= "  TINH TRANG: ON &nbsp  &nbsp  &nbsp ";
                                                  form += "<img src= 'http://hinhanh.gamechocon.com/Thu_Vien_Anh/Anh_Lon/Actions-help-hint-icon.png' width='40' height='40'>&nbsp  &nbsp  &nbsp ";}
form+="  <a href='./on1'><input type='submit' value='ON' ></a> &nbsp";
form+="  <a href='./off1'><input type='submit' value='OFF'></a>";

form+="  <br />";
form+="   <br>";
form+="  RELAY 2:    &nbsp   &nbsp  &nbsp  &nbsp";if(status2==false){form+="   TINH TRANG: OFF &nbsp  &nbsp  &nbsp "; 
                                                  form+="<img src= 'http://www.designer-download.com/wp-content/uploads/2013/09/White-light-bulb-icon-psd.jpg' width='40' height='40'> &nbsp  &nbsp  &nbsp ";}
                                                        else {form+= "  TINH TRANG: ON &nbsp  &nbsp  &nbsp ";
                                                  form += "<img src= 'http://hinhanh.gamechocon.com/Thu_Vien_Anh/Anh_Lon/Actions-help-hint-icon.png' width='40' height='40'>&nbsp  &nbsp  &nbsp ";}
form+="  <a href='./on2'><input type='submit' value='ON' ></a> &nbsp";
form+="  <a href='./off2'><input type='submit' value='OFF'></a>";
form+="  <br />";



form+="<h2>";

form+="<br>";
form+="        <div id='timer'></div>";
form+="<br/>";
form+="</h2>";
form+="  </center>";
form+="</h3>";
form+="</body>";
form += "</html>";
}

void setup(void) {

//ESP.wdtDisable();                               // used to debug, disable wachdog timer, 
  Serial.begin(9600);                           // full speed to monitor                         
  WiFi.begin(SSID, PASS);                         // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }
  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
 
  server.on("/", []() {
    homepage();
    server.send(200, "text/html", form);
  });
 
     
  server.on("/on1",[](){                          //HAM THUC HIEN KHI NHAN DUOC GIA TRI on1 tu web
  Serial.println("b");
  status1 = true;
  homepage();
  server.send(200, "text/html", form);  });
  
   
  server.on("/off1",[](){
  Serial.println("a");
  status1 = false;
  homepage();
  server.send(200, "text/html", form);});
  
  server.on("/on2",[](){
  Serial.println("d");
  status2 = true;
  homepage();
  server.send(200, "text/html", form);});
  
  server.on("/off2",[](){
  Serial.println("c");
  status2 = false;
  homepage();
  server.send(200, "text/html", form);});
  
  server.begin();                                 // Start the server 


  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(SSID);                           // to monitor             
 
  char result[16];
  sprintf(result, "%3d.%3d.%1d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
//  Serial.println();
//  Serial.println(result);
//  Serial.println("WebServer ready!   ");

  Serial.println(WiFi.localIP());                 // Serial monitor prints localIP
  Serial.println("s");
}


void loop(void) {
 
 server.handleClient();
 
}

