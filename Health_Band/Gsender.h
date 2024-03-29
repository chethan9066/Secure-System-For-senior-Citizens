/* Gsender class helps send e-mails from Gmail account
*  using Arduino core for ESP8266 WiFi chip
*/
#ifndef G_SENDER
#define G_SENDER
#define GS_SERIAL_LOG_1         // Print to Serial only server responce
//#define GS_SERIAL_LOG_2       //  Print to Serial client commands and server responce
#include <WiFiClientSecure.h>

class Gsender
{
    protected:
        Gsender();
    private:
        const int SMTP_PORT = 465;
        const char* SMTP_SERVER = "smtp.gmail.com";
        const char* EMAILBASE64_LOGIN = "c2VjdXJlc3lzdGVtNTdAZ21haWwuY29tCg==";
        const char* EMAILBASE64_PASSWORD = "c2VjdXJlc3lzdGVtMjAyMA==";
        const char* FROM = "securesystem57@gmail.com";
        const char* _error = nullptr;
        char* _subject = nullptr;
        String _serverResponce;
        static Gsender* _instance;
        bool AwaitSMTPResponse(WiFiClientSecure &client, const String &resp = "", uint16_t timeOut = 10000);

    public:
        static Gsender* Instance();
        Gsender* Subject(const char* subject);
        Gsender* Subject(const String &subject);
        bool Send(const String &to, const String &message , int number);
        String getLastResponce();
        const char* getError();
};
#endif // G_SENDER
