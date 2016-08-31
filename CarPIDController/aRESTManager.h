
class aRESTManager
{
public:
  aRESTManager()
  : _server( 80 )
  {
  }

  void setup() {
    _rest.set_id("123456"); 
    _rest.set_name("esp8266");
  }

  void loop() {
    WiFiClient client = server.available();
    if ( client ) {
      while ( !client.available() ) {
        delay(1);
      }
    _rest.handle(client);
    }
  }

private:
  REST _rest;
  WiFiServer _server;
};


