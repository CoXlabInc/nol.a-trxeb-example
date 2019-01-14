#include <cox.h>

Timer ledTimer;
Timer printTimer;

static void ledOffTask(void *args);

static void ledOnTask(void *args) {
  ledTimer.onFired(ledOffTask, NULL);
  ledTimer.startOneShot(10);
  digitalWrite(LED_1, LOW);
}

static void ledOffTask(void *args) {
  ledTimer.onFired(ledOnTask, NULL);
  ledTimer.startOneShot(990);
  digitalWrite(LED_1, HIGH);
}

static void printTask(void *args) {
  printf("[%lu usec] Timer works!\n", micros());
}

void setup() {
  Serial.begin(115200);

  printf("\n*** [TrxEB] Basic Functions ***\n");

  ledTimer.onFired(ledOnTask, NULL);
  ledTimer.startOneShot(1000);

  printTimer.onFired(printTask, NULL);
  printTimer.startPeriodic(1000);
}
