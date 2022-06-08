# Micro Processor
- This is the arduino-based system that I've developed.

---
## Arduino Spa 
### Description
- The proposed arduino spa is a bathtub that always maintains the customized temperature for the user.
- video
   * Total structure: https://youtube.com/shorts/BPJdXCAPliw?feature=share
   * Cutomized temperature setting using application: https://youtube.com/shorts/VFi2XZlYpUo?feature=share
   * Cutomized temperature setting using LCD: https://youtube.com/shorts/gmrIBQkYBGY?feature=share
   * Servo motor: https://youtube.com/shorts/DmHpi-TGe-w?feature=share
   * Vibration motor: https://youtube.com/shorts/RZT6GaXKF-g?feature=share
   * Fan: https://youtube.com/shorts/q7xfCArS2q0?feature=share

### Features
- The minimum and maximum values are input from the user, and when the temperature range is exceeded, a warning sound and the LED are turned on at the same time.
- If the temperature of the water tank is colder than the specified temperature, the motor is turned on the bucket of hot water, and if it is hot, the motor is turned on the bucket of the faucet filled with cold water.
- Since cold and hot water do not mix well, a vibrating motor is attached to each side of the water tank to vibrate the water tank so that the water can be mixed well.
- In order to circulate the air in the bathroom, it is possible to control the fan.
- The above functions are implemented so that they can be controlled with a smartphone application.

### Structure Diagram 
* There are two arduinos.
- Arduino 1 sends temperature value to Arduino 2 through serial communication.

### 1) Arduino 1 (Transmit to fan, temperature, and vibration information)
- LED, buzzer, servo motor are operated by measuring temperature value with thermistor.
- Fan, temperature, and viration information read from Arduino 1 is transmitted to Arduino 2 and output as a buzzer.

#### Components
- Smartphone app: User enters the customized min/max temperature values and send to Arduino 1 through bluetooth.
- Thermistor: A bathtub's temperature is measured and sends to Arduino 1.
- LCD: Arduino 1 sends the customized temperature and display current temperature which is measure via thermistor and customized min/max temperature values. 
- LED: If the temperature is appropriate, a green LED is output, if too hot, a red LED, and if too low, a yellow LED is output.
- Servo motor: If the temperature is higher or lower than the proper temperature, the faucet is controlled by the motor to mix the cold or hot water from the water tank.

### 2) Arduino 2 (Receive fan, temperature, and vibration information)
- Arduino receives fan, temperature, and vibration information.

#### Components
- Fan: Fan speed is controlled by the received fan speed value from Arduino 1 (*). 
- Buzzer: Buzzer is played by the temperature value when the measured tempeature is higher than the temperature that user is set.
- Vibration motor: Vibration can be on/off by the received viration information from Arduino 1 (*). 
    * (*) Because Arduino 1 knows all of information from smartphone app.


---
## Smart Stick for Blind 
#### Description
- The proposed smart stick consists of flame sensor, ultrasonic sensor, heart rate sensor whose information is provided to the guardian of the user who uses the smart stick through application.

#### Components 
- Flame sensor: When a flame is detected within a certain distance as required by the user to recognize an obstacle, a buzzer sounds to alert the visually impaired to the danger.
- Ultrasonic sensor: When an obstacle is detected within a certain distance as the user needs to recognize the obstacle, it vibrates and sends a message to the visually impaired that there is an obstacle.
- Heart rate sensor: The heart rate sensor attached to the handle receives the user's heart rate and periodically sends data to the guardian, and sends the data to the web (ThingSpeak) via WiFi.
    * ThingSpeak: It is possible to check heart rate statistics by sending a value to a site that provides a visualization platform. (url: https://thingspeak.com/channels/218510/private_show)
- Application: By creating an application through Bluetooth communication, it notifies the guardian of the disabled in danger (flame detection, heartbeat, etc.).

