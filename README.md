# Chappie - Desk bot with Groq support

This project demonstrates how to create random emotions and movements for an OLED display using the FluxGarage Robo Eyes library. It also integrates AI-generated human-like behaviors using an API.

## Features
- Displays random emotions and movements on an I2C OLED display.
- Connects to WiFi and fetches AI-generated emotions/actions via an API.
- Supports automated blinking and idle modes for the Robo Eyes.

## Hardware Requirements
- **Microcontroller**: Arduino Nano v3, ESP32, or similar.
- **OLED Display**: I2C OLED display with SH1106 chip.
- **Other Components**: Breadboard, jumper wires.

### Pin Configuration (ESP32 Example)
- **SCL**: Pin 22
- **SDA**: Pin 21

## Software Requirements
- Arduino IDE or PlatformIO.
- Libraries:
  - `Adafruit_GFX`
  - `Adafruit_SH110X`
  - `FluxGarage_RoboEyes`
  - `WiFi`
  - `HTTPClient`
  - `ArduinoJson`

## Setup Instructions
1. **Install Required Libraries**: Use the Arduino Library Manager to install the necessary libraries.
2. **Connect Hardware**: Wire the OLED display to the microcontroller's I2C pins.
3. **Configure WiFi Credentials**:
   - Update the `ssid` and `password` variables with your WiFi credentials.
4. **Configure API Credentials**:
   - Update the `apiEndpoint` and `apiKey` variables with your API endpoint and key.
5. **Upload Code**: Compile and upload the code to your microcontroller.

## Code Overview
### Key Components
- **WiFi Connection**: Connects to a WiFi network.
- **API Integration**: Fetches AI-generated emotions/actions from an API.
- **Robo Eyes**: Displays emotions and animations on the OLED display.

### Functions
- `setup()`: Initializes the WiFi, OLED display, and Robo Eyes library.
- `getHumanLikeBehavior()`: Fetches a human-like emotion or action from the API.
- `loop()`: Updates the Robo Eyes display with random emotions or actions at intervals.

### Supported Emotions/Actions
- `HAPPY`
- `ANGRY`
- `TIRED`
- `CONFUSED`
- `LAUGHING`

## Example Output
- The OLED display will show random emotions or animations based on AI suggestions.
- Serial Monitor will log the current mood and other debug information.

## Notes
- Ensure the correct I2C address is uncommented in the code (`0x3C` or `0x3D`).
- If the API key is missing or invalid, the code will default to a blank response.

## License
This project is published under the MIT License.

## Author
Published in April 2025 by [kingsmen732].
