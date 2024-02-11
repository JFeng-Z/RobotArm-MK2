#include "webUI.h"
#include "application.h"
#include "ArmControl.h"
#include "ArmRobotConfig.h"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

const char* ssid = "JFeng";
const char* password = "51348888";

const char* hostname = "espui";

extern ArmRobot_Angle ArmRobotAngle;

void base_slider(Control* sender, int type)
{
    ArmRobotAngle.Base_Angle = sender->value.toInt();
    SetBaseAngle(sender->value.toInt());
}

void bigarm_slider(Control* sender, int type)
{
    ArmRobotAngle.BigArm_Angle = sender->value.toInt();
    SetBigArmAngle(sender->value.toInt());
}

void forearm_slider(Control* sender, int type)
{
    ArmRobotAngle.Forearm_Angle = sender->value.toInt();
    SetForearmAngle(sender->value.toInt());
}

extern xpItem temp_item;

void save_button(Control* sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        ArmMotionSave(temp_item);
        break;

    case B_UP:
        Serial.println("Save success!");
        break;
    }
}

void espUI_Init(void)
{
    ESPUI.setVerbosity(Verbosity::Quiet);
    WiFi.hostname(hostname);

    WiFi.begin(ssid, password);
    Serial.print("\n\nTry to connect to existing network");

    {
        uint8_t timeout = 10;

        // Wait for connection, 5s timeout
        do
        {
            delay(500);
            Serial.print(".");
            timeout--;
        } while (timeout && WiFi.status() != WL_CONNECTED);

        // not connected -> create hotspot
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.print("\n\nCreating hotspot");

            WiFi.mode(WIFI_AP);
            delay(100);
            WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
#if defined(ESP32)
            uint32_t chipid = 0;
            for (int i = 0; i < 17; i = i + 8)
            {
                chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
            }
#else
            uint32_t chipid = ESP.getChipId();
#endif

            char ap_ssid[25];
            snprintf(ap_ssid, 26, "ESPUI-%08X", chipid);
            WiFi.softAP(ap_ssid);

            timeout = 5;

            do
            {
                delay(500);
                Serial.print(".");
                timeout--;
            } while (timeout);
        }
    }
    dnsServer.start(DNS_PORT, "*", apIP);

    Serial.println("\n\nWiFi parameters:");
    Serial.print("Mode: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
    Serial.print("IP address: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

    ESPUI.slider("Base angle", &base_slider, ControlColor::Alizarin, 0, 0, 180);
    ESPUI.slider("BigArm angle", &bigarm_slider, ControlColor::Alizarin, 0, 0, 180);
    ESPUI.slider("Forearm angle", &forearm_slider, ControlColor::Alizarin, 0, 0, 90);
    ESPUI.button("Save motion", &save_button, ControlColor::Alizarin, "Save");

    // ESPUI.sliderContinuous = true;
    ESPUI.begin("ESPUI Control");
}
