#include "mbed.h"
#include "function.h"
#include "stm32l475e_iot01_accelero.h"

using namespace std::chrono;

Thread t; 

int main() {

        t.start(wifi_mqtt);
    selecting();
    
 
    
}