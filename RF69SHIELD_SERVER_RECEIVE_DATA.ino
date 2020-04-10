// Example sketch showing how to create a simple messageing server
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_client
// Demonstrates the use of AES encryption, setting the frequency and modem
// configuration.
// Tested on Moteino with RFM69 http://lowpowerlab.com/moteino/
// Tested on miniWireless with RFM69 www.anarduino.com/miniwireless
// Tested on Teensy 3.1 with RF69 on PJRC breakout board
// code modified by Velleman (nl)

#include <SPI.h>
#include <RH_RF69.h>

// Singleton instance of the radio driver
RH_RF69 rf69;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  if (!rf69.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(433.0))
    Serial.println("setFrequency failed");

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:

  rf69.setTxPower(20, true);

  // The encryption key has to be the same as the one in the client
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                  };
  rf69.setEncryptionKey(key);

#if 0
  // For compat with RFM69 Struct_send
  rf69.setModemConfig(RH_RF69::GFSK_Rb250Fd250);
  rf69.setPreambleLength(3);
  uint8_t syncwords[] = { 0x2d, 0x64 };
  rf69.setSyncWords(syncwords, sizeof(syncwords));
  rf69.setEncryptionKey((uint8_t*)"thisIsEncryptKey");
#endif
}

void loop()
{
  if (rf69.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf69.recv(buf, &len))
    {
      //      RH_RF69::printBuffer("request: ", buf, len);
      Serial.print("Temperature: ");
      Serial.print("\t C");
      Serial.println((char*)buf);
      
      
      //      Serial.print("RSSI: ");
      //      Serial.println(rf69.lastRssi(), DEC);

      // Send a reply
      uint8_t data[] = "Temperature received, thanks!";
      rf69.send(data, sizeof(data));
      rf69.waitPacketSent();
      //Serial.println("Sent a reply");
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
