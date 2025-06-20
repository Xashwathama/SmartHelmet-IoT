# IoT-Based Smart Helmet for Enhanced Vehicle Safety:
An Arduino-based smart helmet system that improves motorcycle safety by ensuring the rider is wearing a helmet and is not under the influence of alcohol. The system restricts ignition access under unsafe conditions.
---

## Features:
- Helmet detection using IR sensor  
- Alcohol detection using MQ3 sensor  
- Ignition lock for unsafe conditions  
- LED and buzzer alerts  
- Real-time monitoring with Arduino UNO  
---

## Components Used:
- Arduino UNO  
- IR Sensor  
- MQ3 Alcohol Sensor  
- Red & Green LEDs  
- Buzzer  
- Breadboard, Jumper Wires  
- Relay (for ignition simulation)
---

## How It Works:
1. IR sensor checks if helmet is worn  
2. MQ3 sensor detects alcohol  
3. If either check fails:  
   - Red LED and buzzer activate  
   - Ignition remains off  
4. If both checks pass:  
   - Green LED turns on  
   - Ignition is allowed
---

## Code:
- `SmartHelmet.ino`  
Handles sensor input, logic, and alerts using non-blocking delay for smooth operation.
---

## Future Scope:
- GPS tracking  
- Accident detection with ultrasonic sensor  
- Mobile app integration
---

## Note:
For educational and safety awareness purposes only.
