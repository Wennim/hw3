#include "mbed.h"



void ulcd_display(int i);

int gesture(); 

void detection();

void selecting(Arguments *in, Reply *out);

void stop_condition();

void ulcd_display_selected(int i);

int wifi_mqtt();