# ACS_ESP32
### Current State of ACS Development (April 2022)

The Altitude Control System has been updated to work with the ESP32 Microcontroller which has enabled wireless file sharing via WiFi. The exact module is the **DOIT ESP32 DEVKIT V1**. 

**Firmware**

The [ACS_ESP32 Code](https://github.com/AdlerFarHorizons/ACS_ESP32) has been organized for clarity, usage, and further development. The code now lives publicly within the [Adler Far Horizons GitHub](https://github.com/AdlerFarHorizons). Additionally, the code and the libraries are packaged by the [PlatformIO](https://platformio.org/platformio-ide) package, which runs within [VS Code](https://code.visualstudio.com/), and should be used to maintain and update the code. Cloning the repository from within GitHub will ensure PlatformIO compiles the code properly. 

**![](https://lh4.googleusercontent.com/VOr_KgZr00MRzIDMwjUCEOm4a1e9Y-rEuFOX8q_m2Qh6UUP_i6r4cdt7RppA9evksXh6Mhy3-KavLnoiUJTBSeL9S8vGmpLnPLSDPtDoZ_utGRDkurdfP1ipgVKnsE-Vo4wNQ98Q)**

<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image1.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image1.png "image_tooltip")

The following are updates to the code:

- Code has been organized into separate header files (_name_.h) so as to keep all related code for specific peripherals in separate files. This allows the programmer to quickly find specific functions.
- Wireless file retrieval has been added. The microcontroller will broadcast a hotspot with SSID: “ACS SD-CARD”. The SD Card content can be displayed by entering the IP-ADDRESS: “192.168.4.1” into a web browser after establishing wifi connection. 

To access wifi settings, 

1. Turn on the ACS by using the Toggle ON/OFF switch or by plugging using a USB cable.

2. Using your computer (or mobile device), look for the “ACS SD-Card” Hotspot under the WIFI settings. You do not need a password to access the hotspot.

3. Identify the 3-position switch on the ACS Faceplate. Place this switch to the Closed Position (or the center position), and Press the Button as indicated on the Display. You should now see the IP Address displayed, which should be “192.168.4.1”.

   1. Sometimes the connection is poor, and you may need to bring the ACS closer to you, or you may need to reestablish the WIFI connection by turning off/on your wireless antenna on your computer/mobile device.

4. If your connection is established, you should now see a window as shown below: 

![](https://lh3.googleusercontent.com/1RIIhETCsgd1uu1DkK87giPBo_XgOyol-EdeXCVPd9iRJGjhDsVPD6hFuSCJugC-odw2SgYhFw1vIyTiesrnZenyNT-ew4V4eF0WKlwmX6uPVY4TfuS3P8eMGee0YzicEkgNWy98)

Within this window you may browse through the content within the SD Card by selecting Directory. If you wish to download a file, you can copy/paste the full name of the file and paste it into the Downloads prompt window. 

The use of WIFI is to be able to download the param.txt file, edit it and upload it back into the ACS SD Card. Additionally, one can download the datalog.csv, which contains all the flight ACS data. Ideally, one should not have to remove the SD Card from the board. 

_Note: I’ve noticed that deleting a file via WIFI sends it to the .Trash Folder. It doesn’t fully delete the file. So when the script runs and looks for a file within the list of files, if the file exists within the .Trash folder, then the code will send a True that the file exists. The code either needs a exception to not search within .Trash or a “Permanently Delete” function that deletes the content within .Trash. _


### Design Files, Notes, and Code

[GitHub - ACS_ESP32](https://github.com/AdlerFarHorizons/ACS_ESP32)

[Google Drive: System Redesign Concept 2021/22](https://drive.google.com/drive/folders/1W37AFGbH8UgUThIRDDYVn1NRzvqxMLDn)

- [PCB Design Files](https://drive.google.com/drive/folders/1NPGxpG3kCgMrHuZ5ZHsxEYcvnIpvRIrO?usp=sharing)
- [PCB Schematic PDF](https://drive.google.com/file/d/19PMDI1tQQiT3bByivIbot_-rFZreG9Ll/view?usp=sharing)

[Purchase Materials****](https://docs.google.com/spreadsheets/d/1xs60UxvHJ-fiQykCOm7pP0R7RBD7VAjFjShzD04MD6w/edit#gid=0)

**Compiling using Arduino**

To compile and upload the script file using Arduino, you must install the ESP32 Arduino Boards using instructions found here.

**Instructions**: [Installing ESP32 in Arduino IDE](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) 

Upon Install, select the **DOIT ESP32 DEVKIT V1 **Board from the drop down menu within **Tools/Board/ESP32 Arduino/. **The this dropdown menu is shown below:

**![](https://lh6.googleusercontent.com/_kFKrzMAFXds2ba18bY5IaDNvxRggebDt9xj-rtJn0WOh6LvTkpCSqUdV0bCA5D-mXpEnjwVt6NLFoG2gF00hyCTdZaY4fOtIBXLDIl06hqou-I4Gh8wCQLdviymnrGOm575QcbS)**

****

**Printed Circuit Board (PCB)**

The PCB for the ACS has had several modifications. Primarily, the Teensy 3.6 Microcontroller has been exchanged for the WIFI capable microcontroller [**ESP-WROOM-32 ESP-32S Development Board**](https://www.amazon.com/dp/B08D5ZD528?psc=1&ref=ppx_yo2ov_dt_b_product_details)**. **This microcontroller does not include an SD Card Reader, which is why an [SD Card Reader Breakout Board](https://www.sparkfun.com/products/13743) has been added to the new design. 

In addition to the microcontroller modifications, the PCB now utilizes a few surface-mounted devices. 

- **Power**: The previous design used the [LM7805 Voltage Regulator](https://www.sparkfun.com/datasheets/Components/LM7805.pdf) which dropped the 7.2V to 5V using a conventional heat dissipation voltage regulator. This approach worked, but consumed much higher current draw. Therefore, the DC-DC voltage regulator was exchanged for a low-dropout voltage regulator. The selected voltage regulator is now the [MC33269](https://www.onsemi.com/pdf/datasheet/mc33269-d.pdf). 
- **Logic Level Shifter: The original [4-channel I2C-safe Bi-directional Logic Level Converter - BSS138](https://www.adafruit.com/product/757) has been stripped down to only use 1 of the [BSS138](https://www.onsemi.com/pdf/datasheet/bss138-d.pdf) configured to convert signals from low voltage 3V3 to 5.0 Volts.**
- **Actuator**: The current actuator is the L12-50-210-6-R, which runs a servo motor to extend the actuator. However, this actuator does not provide feedback positional data, therefore the recommendation is to upgrade the actuator to the [**L12-50-210-6-I**](https://s3.amazonaws.com/actuonix/Actuonix+L12+Datasheet.pdf)**. **The L12-I version contains an integrated microcontroller which allows the user to obtain positional feedback data. 

  


**Connection Diagram**

**AA Battery Set for Actuator + Primary Board**

The current AA battery set used in the ACS is the [ Xeno XL-060F AA 2400mAh 3.6V Lithium Thionyl Chloride (LiSOCI2)](https://www.batteryjunction.com/xl-060f-xeno-aa.html) \[[datasheet](https://files.batteryjunction.com/frontend/files/xeno/datasheet/Xeno-XL-060F-Datasheet.pdf)], which is placed in Series to increase the voltage to 7.2. The ACS uses four (4) AA Batteries - 2 sets in Series, and those two sets are merged in parallel to increase the capacity (mAh).

  


**External Temperature Battery Board**

This board has been prepared to connect to the primary board, but is placed separately from it in order to be close enough to the heating elements for it to monitor temperatures and hold the two 9V batteries to power the heating element.

- **Temperature:** The temperature is monitored using a [DS18B20 Digital Thermometer](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf).
- **Batteries:** The 9V batteries are held in place by both the structure of the compartment it will reside in and power the heating through two [5331TR (Leaf-Spring Battery Contacts)](https://www.digikey.com/en/products/detail/keystone-electronics/5331TR/8347168?utm_adgroup=General&utm_source=google&utm_medium=cpc&utm_campaign=Smart%20Shopping_Product_Zombie%20SKUS&utm_term=&utm_content=General&gclid=CjwKCAiAprGRBhBgEiwANJEY7DhVwsQ1aTq7gBAcb5RRHrVmZTxuCmMAlXrXpHnIgzOPE_fwFTYqjRoCqlUQAvD_BwE) each. These battery contacts serve a dual purpose as the screws that will go through them are the same ones being used to mount the board securely.
- **Connection: The board is connected to the primary one using a 5 Pin [Screw Terminal Block](https://www.pololu.com/product/2493) affixed to the bottom of the board which will allow wires to be led through a hole beneath it on the way to the main board.**

**![](https://lh6.googleusercontent.com/i0KB2U1GQi786iQtHSaZy-ddk3Zv3pPtQQ_pFyo2oJTYZWIr0ot2n-d4BkqI7z3PDKpxytboJKdZvbsOLsbjTsA4rEDEALrGlNwFOGAW2bFgv2ZOo5hGOq7ev7apg0GzySilAuDs)**




### NEXT TASKS

**Actuator**

- After providing 6.5V to the L-12-I actuator via the DC-Power Supply, **ensure the actuator responds correctly to the ACS_ESP32 Code**. If it does not respond correctly, change the RC_INPUT_SIG Pin to another pin on the ESP32. I’ve identified that some of the pins on the ESP32 seem to short out the microcontroller and/or do not behave as intended. On the code, navigate to actuator.h and find line “**int actuator_pin = 17;”.** Test out a different GPIO pio, upload code, and test for actuator response. 

  - If the actuator ‘jerks’ back and forth when attempting to reach a certain position, the datasheet recommends to “place a 1–4Ω resistor in series with the actuator’s red V+ lead wire”.

- **Actuator Feedback** via the POS_FEEDBACK_SIG pin, which has been connected to GPIO13, must be checked for incoming data. After declaring the POS_FEEDBACK_PIN as an int, use pinMode(POS_FEEDBACK_PIN, INPUT); to initialize the pin on the microcontroller, one should be able to read the voltage using the analogRead() function. One should expect a voltage reading from 0-255, which then can be mapped using map() to correspond to a percentage. 

- Actuator Pins CURR_INPUT_SIG and VOLT_INPUT_SIG have purposefully been disconnected from the microcontroller. These features for controlling the actuator are unnecessary as the actuator will be controlled via the RC_INPUT_SIG.

**The Heating Element**

- The Heating Element needs to be tested the new downsize of voltage output. Originally the heating element uses 3 batteries in series of 9V totalling 27V. Now, the downsize should yield 18V. This downsize has been considered given the usage of an aluminum pipe for heat distribution. 
- Code: Create Algorithm to measure heat transfer between the heating element and the internal electronics, by measuring the temperature outside the heating element (battery board) and the internal Pressure Sensor thermometer. Using these temperatures, create a function that activates the heating element below a specific temperature, and conversely deactivates the heating element when temperatures reach a specific maximum. 
- Heating Element Transistor 2N2222 schematic needs to be tested prior to finalizing the schematic design of the primary board. 

![](https://lh4.googleusercontent.com/cy-zCJkUdToRx2x5MYrzngHi63EEDRt6C_1e_I6U-U68gQK20TWlEMT-Kvwc1UCDBazY9g2Exo4PO3QFt3Nisfm_ZxCTdyNH_Fgaa0cesZSyLa0i2u9uzPNjcH-xj-gi6Ek2dk_m)

Above: Circuit concept idea for the resistor being the heating element connected to one end of the transistor 2N2222.

![](https://lh6.googleusercontent.com/d84EPyA9o9O5S8bfoZSG486fP3kDaZE4rPvCy8hbTmJAi8RXO4Nz4tsgF7DOlYikVSxNmH4JD22n-L7T1mKx6lcx0h40YCCuueMFQL_KuoA2AwHLrMVWxZ88jR3TRUUjpZLXT24s)

Above: Schematic capture of the heating element connector. Here the heating element is connected to one side of the NPN transistor. The Base is connected to the microController, and the Collector is connected to the negative end of the 9V battery set, which is connected in series. 

**Code**

Within the void setup() function, ensure that when the sd_setup() does not create a new file each time the boot process occurs. The ACS should conserve and append the currently existing logfile if it exists. If the file does not exist, the code is instructed to write the parameters below:


else
{ unsigned long size = logFile.size();
  Serial.print("Filesize: "); Serial.println(size);
  delay(5000);
  if (size < 500){
    display.clearDisplay();
    display.println("Creating new File");
    display.display();
    delay(5000);

    String log = 
                "Far Horizons | Adler Planetarium" + String("\n") + 
                "Flight Number: " + "," +            String(flightNum) + "\n" + 
                "Firmware: " + "," +                  String(the_sketchname) + "\n" + 
                "Compiled on: " +  "," +               String(__DATE__) +  " at " +  String(__TIME__) + "\n" +
                "Venting Altitude (m): " + "," +      String(VENT_ALT) + "\n" + 
                "Float Time (min): " + "," +          String(FLOAT_TIME) + "\n" + 
                "Max Float Altitude (m): " + "," +   String(MAXFLOATALT) + "\n" + 
                "Release Position %: " + "," +        String(RELEASE_POS) + "\n" + 
                "Venting Position %: " + "," +        String(OPEN_POS) + "," + 
                "Closed Position" + "," + String(CLOSED_POS) + "\n";
                
                

Currently this section of the code does not work as intended. Instead, the code creates a new file and does not append the parameters found above.

  


**ACS Primary PCB**

- Label Pins on Connectors using layer tNames and bNames.
- Ensure Connector are placed in the correct location to help diminish excess wire routing and unnecessary wire-curvatures.
- Rework routes after device placement and use ratsnest to create the correct ground plane

**Currently Wired Diagram on Breadboard: **

  
![](https://lh5.googleusercontent.com/cx3chtunG8r0tcLXQ2pn3sirq7Yzx_SdSa8WVnv58y4vLIpiBuIkMUIVn3K186V_daPH47Eb-MjRIMm78APOeJQojA3zaHeKiBJ9uM8mHvCPeBoiB3ROGuA0RTfAOV5SxwXxGZts)  


**Full Schematic Diagram with External Temp Board**![](https://lh4.googleusercontent.com/1EcRGJUC4HCBjgi28EfTGdwfkmC6R9JiOnQl4y07BvHmfR6103X_zS45_lIdTIq2vvUHQToiE5R9rN8Nt3rsxm3D0LHVsP7kZ7ojaMMSgOMwmLRmV-EVQIoSm4Wxo9XOCy3lMLhw)
