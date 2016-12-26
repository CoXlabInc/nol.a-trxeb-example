#include <cox.h>

IPv6Interface *ppp;
Timer ledTimer;
Timer reportTimer;

SerialPort &Serial2 = enableSerialUCA3();

static void ledOffTask(void *args);

static void ledOnTask(void *args) {
  ledTimer.onFired(ledOffTask, NULL);
  ledTimer.startOneShot(10);
  ledOn(0);
}

static void ledOffTask(void *args) {
  ledTimer.onFired(ledOnTask, NULL);
  ledTimer.startOneShot(990);
  ledOff(0);
}

static void periodicReport(void *) {
  printf("Report!\n");
  void *rep = Noliter.send("NODE005",
                            "\"ppm1\":\"0\","
                            "\"conf1\":[\"2000\",\"1000\",\"500\"],"
                            "\"status1\":\"0\","
                            "\"ppm2\":\"0\","
                            "\"conf2\":[\"2000\",\"1000\",\"500\"],"
                            "\"status2\":\"0\","
                            "\"ppm3\":\"0\","
                            "\"conf3\":[\"2000\",\"1000\",\"500\"],"
                            "\"status3\":\"0\","
                            "\"ppm4\":\"0\","
                            "\"conf4\":[\"2000\",\"1000\",\"500\"],"
                            "\"status4\":\"0\"");
}

static void ip6_state_changed(IPv6Interface &interface, IPv6Interface::State_t state) {
  printf("IPv6 iface 'ppp0': State changed to %s\n",
         ip6_state_string(state));

  if (state == IPv6Interface::STATE_HOST && !reportTimer.isRunning()) {
    printf("Nol.iter start!\n");
    Noliter.setGateway(interface);
    reportTimer.onFired(periodicReport, NULL);
    reportTimer.startPeriodic(10000);
  }
}

void setup(void) {
  Serial.begin(115200);
  printf("\n\n*** PPP Host ***\n");

  /* Single interface, no routing entry */
  ip6_init(1, 0);

  // Initialize the PPP interface.
  Serial2.begin(19200);
  Serial2.listen();

  ppp = enableIPv6PPPoS(Serial2);
  if (ppp) {
    ppp->begin();
    ppp->setStateNotifier(ip6_state_changed);
    ip6_start();
  } else {
    printf("* Error on enable PPPoS.\n");
  }

  ledTimer.onFired(ledOnTask, NULL);
  ledTimer.startOneShot(1000);
}
