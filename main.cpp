#include "mbed.h"
#include "function.h"
#include "stm32l475e_iot01_accelero.h"
#include "mbed_rpc.h"
BufferedSerial pc(USBTX, USBRX);

using namespace std::chrono;

Thread t; 
RPCFunction rpcGesture_UI(&gesture_UI, "gesture_UI");
RPCFunction rpcDetection(&detection, "detection");
RPCFunction rpcStop_condition(&stop_condition, "stop_condition");
int main() {

        t.start(wifi_mqtt);

//The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class
    // receive commands, and send back the responses
    char buf[256], outbuf[256];

    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");

    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = fgetc(devin);
            if (recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }
    
 
    
}