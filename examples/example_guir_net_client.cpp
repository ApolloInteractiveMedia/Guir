#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <GuirApi.h>

int main() {

  printf("Guir Net Client Test.\n");

  GuirClient* guir_client = NULL;
  if (0 != guir_client_alloc(&guir_client)) {
    printf("Oops failed to allocate the GuirClient. (exiting). \n");
    exit(EXIT_FAILURE);
  }

  if (0 != guir_client_init(guir_client, "127.0.0.1", 1234)) {
    printf("Oops failed to initialize the GuirClient. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  int circle_radius = 0;
  float speed = 0.0f;
  float circle_color[4] = { 0.0f };
  bool log_info_button_down = false;

  if (0 != guir_client_add_value_listener_for_sliderint(guir_client, "circle-radius", circle_radius)) {
    printf("Failed to add listener for sliderfloat (circle-radius). (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 != guir_client_add_value_listener_for_button(guir_client, "log-info", log_info_button_down)) {
    printf("Failed to add a listener for log_info_button_down (button). (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 != guir_client_add_value_listener_for_colorpicker(guir_client, "circle-color", circle_color)) {
    printf("Failed to add a listener for circle_color (color picker). (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 != guir_client_add_value_listener_for_sliderfloat(guir_client, "speed", speed)) {
    printf("Failed to add a listener for float slider (speed). (exiting).\n");
    exit(EXIT_FAILURE);
  }

  while (true) {

    if (0 != guir_client_update(guir_client)) {
      printf("Failed to update guir. (exiting).");
      exit(EXIT_FAILURE);
    }

    if (true == log_info_button_down) {
      printf("circle_radius: %d\n", circle_radius);
      printf("circle_color: %2.2f, %2.2f, %2.2f, %2.2f\n", circle_color[0], circle_color[1], circle_color[2], circle_color[3]);
      printf("speed: %3.3f\n", speed);
      printf("-\n");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  if (0 != guir_client_dealloc(guir_client)) {
    printf("Failed to cleanly deallocate the Guirclent. (exiting).\n");
    exit(EXIT_FAILURE);
  }
  
  return 0;
}

