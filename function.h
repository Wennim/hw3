#include "mbed.h"
#include "mbed_rpc.h"


void ulcd_display(int i);

int gesture(); 

void detection(Arguments *in, Reply *out);

void selecting();

void stop_condition(Arguments *in, Reply *out);

void gesture_UI(Arguments *in, Reply *out);

void ulcd_display_selected(int i);

int wifi_mqtt();