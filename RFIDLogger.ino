/*
  RFID Logger

  A Simple logger which reads RFID tag and sends the data into AWS IoT service through a separated gateway. 

  This scanner should be used with AWS gateway ( https://github.com/CheapskateProjects/ESP8266AWSIoTGateway ) to automatically send results to IoT cloud. 

  Dependency: https://github.com/miguelbalboa/rfid
  
  created   Mar 2017
  by CheapskateProjects

  ---------------------------
  The MIT License (MIT)

  Copyright (c) 2017 CheapskateProjects

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(10, 9); // SS / RST

void setup()
{
	Serial.begin(115200);		// Initialize IoT gateway serial communication
	SPI.begin();			// Initialize SPI
	mfrc522.PCD_Init();		// Initialize RFID reader
 pinMode(8, OUTPUT);
}

void loop()
{
  // Check that we have a card to read
	if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
	{
		return;
	}

  writeToGateway(mfrc522.uid.uidByte, mfrc522.uid.size);
}

void writeToGateway(byte *uid, byte size)
{
  Serial.print("RFID1;{\"state\":{\"reported\":{\"token\":\"");
  for (byte i = 0; i < size; i++)
  {
    // Space separator for key parts
    if(i > 0)
    {
      Serial.print(" ");
    }
    
    // If only one digit then prefix with 0
    if(uid[i] < 0x10)
    {
      Serial.print("0");  
    }
    
    // Write in hex
    Serial.print(uid[i], HEX);
  }
  Serial.print("\"}}}\n");

  // Light led for a while to prevent repeated read and give some indication on success
  digitalWrite(8, HIGH);
  delay(3000);
  digitalWrite(8, LOW);
}

