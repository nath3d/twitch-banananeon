#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <IRCClient.h>
#include <StreamlabsAPI.h>
#define IRC_SERVER   "irc.chat.twitch.tv"
#define IRC_PORT     6667

//BASTIUI
#define SSID "AGRID"
#define PASSWORD "internet3000"
#define TWITCH_BOT_NAME "agrid"
#define TWITCH_OAUTH_TOKEN "oauth:xaucjdsz612ho9yrd5g4ytiz262qcp"
#define SOCKET_TOKEN "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ0b2tlbiI6IkIwMDE4QkIwMDQwNjI4RTcyNEJFIiwicmVhZF9vbmx5Ijp0cnVlLCJwcmV2ZW50X21hc3RlciI6dHJ1ZSwieW91dHViZV9pZCI6IlVDbm5JUDM0Q3BUN25JQlpTNUt1dWhiQSIsInR3aXRjaF9pZCI6IjE0OTY0MzUxMCJ9.HRt1QwLFPnuBF2kmxXO53yWpZCvgiGXEZ7DSBYGYcn4"
const String twitchChannelName = "bastiui";

String ircChannel = "";
String bastiuiNickName = "BASTIUI";
String lunathNickName = "LUNATH_";
String stringToFindEmote = "bastiuiBanaNeon";
String stringToFindCheer1 = "Cheer1";
String stringToFindCheer5 = "Cheer5";

//brutal method
String streamlabsNickName = "STREAMLABS";
String stringToFindBits = "bits";
String stringToFindSub = "BastiPote";
String stringToFindRaid = "raid";
//brutal method

int blinkSpeed = 100;
int blinkCount = 6;
int relay1ID = 2;
int relay2ID = 0;
int relay3ID = 4;
int relay4ID = 5;
StreamlabsAPI streamlabsAPI;

WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

void setup()
{
  Serial.begin(9600);
  delay(1000);
  //RELAYS
  pinMode(relay1ID, OUTPUT);
  pinMode(relay2ID, OUTPUT);
  pinMode(relay3ID, OUTPUT);
  pinMode(relay4ID, OUTPUT);
  helloBanana();
  //WIFI
  WiFi.begin(SSID, PASSWORD);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println('\n');
  Serial.println("Wifi connection established.");
  Serial.print("IP address\t");
  Serial.println(WiFi.localIP());
  //IRC
  ircChannel = "#" + twitchChannelName;
  client.setCallback(callback);
  //API
  //streamlabsAPI.followTwitchEvent(followerEvent);
  //streamlabsAPI.subscriptionsTwitchEvent(subscriptionEvent);
  //streamlabsAPI.bitsTwitchEvent(bitsEvent);
  //streamlabsAPI.donationEvent(donationsEvent);
  //streamlabsAPI.connect(SOCKET_TOKEN);
  //END
  helloBanana();
}

void loop()
{
  //blinkBanana();
  //IRC
  if (!client.connected()) {
    Serial.println("Attempting to connect to IRC channel " + ircChannel );
    if (client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN))
    {
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("IRC connected.");
      sendTwitchMessage(stringToFindEmote);
    }
    else
    {
      Serial.println("IRC connection failed.");
      delay(5000);
    }
    return;
  }
  client.loop();
  //API
  streamlabsAPI.loop();
}


/////////////////////////////////////////////////////IRC

void sendTwitchMessage(String message)
{
  client.sendMessage(ircChannel, message);
}

void callback(IRCMessage ircMessage)
{
  Serial.println("ORIGINAL IRC MESSAGE " + ircMessage.original);
  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001')
  {
    ircMessage.nick.toUpperCase();
    String message("<" + ircMessage.nick + "> " + ircMessage.text);
    Serial.println(message);
    if (ircMessage.nick.indexOf(bastiuiNickName) >= 0)
    {
      if (ircMessage.text.indexOf(stringToFindEmote) >= 0)
        blinkBanana();
    }
    if (ircMessage.nick.indexOf(lunathNickName) >= 0)
    {
      if (ircMessage.text.indexOf(stringToFindEmote) >= 0)
        blinkBanana();
    }
    if (ircMessage.nick.indexOf(streamlabsNickName) >= 0)
    {
      if (ircMessage.text.indexOf(stringToFindBits) >= 0)
        bitsBanana(ircMessage.text);
      if (ircMessage.text.indexOf(stringToFindSub) >= 0)
        subBanana(ircMessage.text);
      if (ircMessage.text.indexOf(stringToFindRaid) >= 0)
        raidBanana(ircMessage.text);
    }
    //    else
    //    {
    //      if (ircMessage.text.indexOf(stringToFindCheer1) >= 0)
    //        blinkBanana();
    //      if (ircMessage.text.indexOf(stringToFindCheer5) >= 0)
    //        blinkBanana();
    //    }
    return;
  }
}

void subBanana(String message)
{
  String subMessage = message.substring(message.indexOf('(') + 1, message.length());
  int monthsCount =  subMessage.substring(0, subMessage.indexOf(' ')).toInt();
  if (monthsCount == 0)
    monthsCount = blinkCount;
  for (int i = 0; i < monthsCount; i++)
  {
    hardBanana();
    delay(blinkSpeed);
    noBanana();
    delay(blinkSpeed);
  }
  softBanana();
}

void bitsBanana(String message)
{
  int bitsCount = message.substring(0, message.indexOf(' ')).toInt();
  if(bitsCount > 10)
    bitsCount = bitsCount /10;
  if(bitsCount >= 10000)
    bitsCount = bitsCount /100;
    
  for (int i = 0; i < bitsCount; i++)
  {
    hardBanana();
    delay(blinkSpeed);
    noBanana();
    delay(blinkSpeed);
  }
  softBanana();
}

void raidBanana(String message)
{
  int raidersCount = message.substring(0, message.indexOf(' ')).toInt();
  for (int i = 0; i < raidersCount; i++)
  {
    hardBanana();
    delay(blinkSpeed);
    noBanana();
    delay(blinkSpeed);
  }
  softBanana();
}

void blinkBanana()
{
  for (int i = 0; i < blinkCount; i++)
  {
    hardBanana();
    delay(blinkSpeed);
    noBanana();
    delay(blinkSpeed);
  }
  softBanana();
}

void helloBanana()
{
  for (int i = 0; i < 2; i++)
  {
    hardBanana();
    delay(blinkSpeed);
    noBanana();
    delay(blinkSpeed);
  }
  softBanana();
}

void softBanana()
{
  digitalWrite(relay1ID, HIGH); //soft or blink
  digitalWrite(relay3ID, LOW); //gnd
  digitalWrite(relay2ID, HIGH); //hard or off
}

void hardBanana()
{
  digitalWrite(relay1ID, LOW); //soft or blink
  digitalWrite(relay3ID, HIGH); //gnd
  digitalWrite(relay2ID, HIGH); //hard or off
}

void noBanana()
{
  digitalWrite(relay1ID, LOW); //soft or blink
  digitalWrite(relay3ID, HIGH); //gnd
  digitalWrite(relay2ID, LOW); //hard or off
}

/////////////////////////////////////////////////////API
void followerEvent(const char * payload)
{
  Serial.print("got followerEvent message: ");
  Serial.println(payload);
  //blinkBanana();
}

void subscriptionEvent(const char * payload)
{
  Serial.print("got subscriptionEvent message: ");
  Serial.println(payload);
  blinkBanana();
}

void bitsEvent(const char * payload)
{
  Serial.print("got bitsEvent message: ");
  Serial.println(payload);
  blinkBanana();
}

void donationsEvent(const char * payload)
{
  Serial.print("got donationsEvent message: ");
  Serial.println(payload);
  blinkBanana();
}
