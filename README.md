# suvi
Suvi is an IoT device that bridges data from a server to the embedded OLED and a local network.

Suvi designed as an ELE417 - Embedded System Design Project. Purpose of this device is basically receiving data from an data provider and broadcast it into embedded LCD screen and to a web based platform. Data received from **IoT** service data provider over **TCP connection** via **ESP8266** module over Wi-Fi. Then data processed on the embedded **MSP430** platform. The outcomes are shared with to the user by both via **OLED screen** display and a website on a private network.

## LCD Display Function
System is able to show meteorological data such as weather status, temperature, air pressure etc. on a embedded OLED screen. Additionally battery charge percent measured and showed on the screen.

<p align="center">
<img src="/docs/a.jpg" width="250" height="250">
<img src="/docs/b.jpg" width="250" height="250">
<img src="/docs/c.jpg" width="250" height="250">
</p>

## suvi.com 
Processed data broadcasted into a website on a private network provided from the onboard ESP8266. Users can access this webpage by connecting into this network and then visiting the webpage suvi.com from their mobile or desktop browsers.

<p align="center">
<img src="/docs/web.jpg" width="400">
</p>

## System prototype
Prototype designed as a handheld device. A custom PCB and housing is prepared for these purposes. Since this is an initial prototype, various port connections leaved open to ease development.

<p align="center">
<img src="/docs/case1.jpg" width="400" height="300">
<img src="/docs/pcb2.jpg" width="400" height="300">
</p>
<p align="center">
<img src="/docs/pcb1.jpg" width="400">
<img src="/docs/case2_t.png" width="400">
</p>

## Demo
<p align="center">
<img src="/docs/demo1.gif">
</p>

## Group Members 
This project is a result of a collabrated effort. <br>
<ul>
<li><strong>Ahmet Vehbi GENÇ </strong> - Custom OLED interface design and embedded OLED driver software development</li>
<li><strong>İbrahim KAYMAK </strong>   - Embedded electronic and PCB design/production, prototype housing design</li>
<li><strong>Sümeyra DURAK </strong>   - Custom ESP8266 firmware development and web based interface design</li>
<li><strong>Umut Utku KOÇAK </strong> - Core embedded software development on MSP430</li>
</ul>




