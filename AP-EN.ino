// Imports the library tools...
#include <WiFi.h>

// Library with the native ESP-IDF functions...
#include "esp_wifi.h"

// Pointers to the network name and password...
const char *network_name = "ESP32-WIFI";
const char *network_password = "45928866";
// Identifies the device on the network (host)...
const char *host_name = "esp32s3";

// Defines the local IP address...
const IPAddress local_ip(192,168,10,1);
// Defines the gateway IP address...
const IPAddress gateway_ip(192,168,10,1);
// Defines the subnet mask...
const IPAddress subnet_mask(255,255,255,0);

// Checks whether the network is active or not...
bool network_active = false;
// Limits the number of users connected simultaneously...
int connection_limit = 4;

// Structure to return the number of connected devices...
wifi_sta_list_t connected_devices;

void setup() {
  Serial.begin(115200);
  
  // Function to set the access point (AP) hostname...
  WiFi.softAPsetHostname(host_name);
  // Function to configure the IP as static (fixed), the gateway, and the subnet mask...
  WiFi.softAPConfig(local_ip, gateway_ip, subnet_mask);
  
  delay(100);
  // Starts Wi-Fi as an access point...
  network_active = WiFi.softAP(network_name, network_password, 1, 0, connection_limit);
  delay(100);
  
  if (network_active) {
    // The %s specifier indicates where the 'host_name' variable string will be inserted...
    Serial.printf("Network activated successfully: %s\n\n", network_name);
 
    Serial.printf("Host Name: %s\n\n", host_name);
  }
  // Wait time to connect a device to Wi-Fi before the single reading...
  delay(10000);
}

// Stores the number from the last reading for comparison...
int previous_count = -1;

void loop() {
  // The official function from the documentation that fills the structure with the devices...
  if (esp_wifi_ap_get_sta_list(&connected_devices) == ESP_OK) {
    
    // Only prints to Serial if the number of connections has changed...
    // The "!=" reads: "If the number of devices is DIFFERENT from the previous number"...
    if (connected_devices.num != previous_count) {
      previous_count = connected_devices.num;

      Serial.printf("Number of devices: %d\n\n", connected_devices.num);

      // Scans the list of devices present in the structure...
      for (int index = 0; index < connected_devices.num; index++) {
        wifi_sta_info_t station_info = connected_devices.sta[index];

        // Prints the MAC address formatted in hexadecimal...
        Serial.printf("Connections: %d - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n\n", 
          index + 1,
          station_info.mac[0],
          station_info.mac[1],
          station_info.mac[2],
          station_info.mac[3],
          station_info.mac[4],
          station_info.mac[5]
        );
      }
      // Credits...
      Serial.printf("===...Developed by: Bruno Maker - 2026...===\n\n");
    }
  } else {
    // Error feedback...
    Serial.println("Error retrieving the access point list!");
  }

  // Necessary pause to feed the Watchdog and avoid freezing the CPU...
  delay(1000);
}
