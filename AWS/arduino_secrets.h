// Fill in  your WiFi networks SSID and password
#define SECRET_SSID "Galaxy"
#define SECRET_PASS "12345678"

// Fill in the hostname of your AWS IoT broker
#define SECRET_BROKER "a37a9entcjl8r-ats.iot.us-east-2.amazonaws.com"

// Fill in the boards public certificate
const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE-----
MIHSMHoCAQAwGDEWMBQGA1UEAxMNTXlNS1JXaUZpMTAxMDBZMBMGByqGSM49AgEGCCqGSM49AwEH
A0IABHeXCyXPQZwXP3Y0+qOWQquO+I+ykhukNjqDFVrZ69pvlItl8Glw6R+gIiSMTAAENmWhQyey
96ecxkoFK+zfIyygADAKBggqhkjOPQQDAgNIADBFAiBrS21H8cNy7c2XR2IAVAiY2dFZdl88TxZX
WAcRa4SUNQIhAJbGwrgGsTagovMYkb1BA3zkm+mWnB7EVfxtA/Jg1gez
-----END CERTIFICATE-----
)";
