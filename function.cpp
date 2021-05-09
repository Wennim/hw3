#include "function.h"
#include "uLCD_4DGL.h"
#include "stm32l475e_iot01_accelero.h"
#include "accelerometer_handler.h"
#include "config.h"
#include "magic_wand_model_data.h"
#include <math.h>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
//DigitalOut myled(LED1);
DigitalOut myled3(LED3);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread thread;
InterruptIn mypin_select(USER_BUTTON);

void ulcd_display(int i){

if(i==1){

// basic printf demo = 16 by 18 characters on screen
    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,0);
    uLCD.color(RED);
    uLCD.printf("    30"); //Default Green on black text
    

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,2);
    uLCD.color(GREEN);
    uLCD.printf("    45"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,4);
    uLCD.color(GREEN);
    uLCD.printf("    60"); //Default Green on black text
    
    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,6);
    uLCD.color(GREEN);
    uLCD.printf("    80"); //Default Green on black text

}

else if(i ==2){
// basic printf demo = 16 by 18 characters on screen
    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,0);
    uLCD.color(GREEN);
    uLCD.printf("    30"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,2);
    uLCD.color(RED);
    uLCD.printf("    45"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,4);
    uLCD.color(GREEN);
    uLCD.printf("    60"); //Default Green on black text

     uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,6);
    uLCD.color(GREEN);
    uLCD.printf("    80"); //Default Green on black text
}

else if(i==3){
// basic printf demo = 16 by 18 characters on screen
    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,0);
    uLCD.color(GREEN);
    uLCD.printf("    30"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,2);
    uLCD.color(GREEN);
    uLCD.printf("    45"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,4);
    uLCD.color(RED);
    uLCD.printf("    60"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,6);
    uLCD.color(GREEN);
    uLCD.printf("    80"); //Default Green on black text
}

else if(i==4){
// basic printf demo = 16 by 18 characters on screen
    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,0);
    uLCD.color(GREEN);
    uLCD.printf("    30"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,2);
    uLCD.color(GREEN);
    uLCD.printf("    45"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,4);
    uLCD.color(GREEN);
    uLCD.printf("    60"); //Default Green on black text

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,6);
    uLCD.color(RED);
    uLCD.printf("    80"); //Default Green on black text
}
}

void ulcd_display_selected(int i){
uLCD.reset();
if(i==1){

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,3);
    uLCD.color(RED);
    uLCD.printf("    30"); //Default Green on black text

}

else if(i ==2){

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,3);
    uLCD.color(RED);
    uLCD.printf("    45"); //Default Green on black text

}

else if(i==3){

    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,3);
    uLCD.color(RED);
    uLCD.printf("    60"); //Default Green on black text

    
}

else if(i==4){
    uLCD.text_width(2.5); //4X size text
    uLCD.text_height(2.5);
    uLCD.locate(0,3);
    uLCD.color(RED);
    uLCD.printf("    80"); //Default Green on black text
}
}

// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr int kTensorArenaSize = 60 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
int16_t DataXYZ[3] = {0};

// Return the result of the last prediction
int PredictGesture(float* output) {
  // How many times the most recent gesture has been matched in a row
  static int continuous_count = 0;
  // The result of the last prediction
  static int last_predict = -1;

  // Find whichever output has a probability > 0.8 (they sum to 1)
  int this_predict = -1;
  for (int i = 0; i < label_num; i++) {
    if (output[i] > 0.8) this_predict = i;
  }

  // No gesture was detected above the threshold
  if (this_predict == -1) {
    continuous_count = 0;
    last_predict = label_num;
    return label_num;
  }

  if (last_predict == this_predict) {
    continuous_count += 1;
  } else {
    continuous_count = 0;
  }
  last_predict = this_predict;

  // If we haven't yet had enough consecutive matches for this gesture,
  // report a negative result
  if (continuous_count < config.consecutiveInferenceThresholds[this_predict]) {
    return label_num;
  }
  // Otherwise, we've seen a positive result, so clear all our variables
  // and report it
  continuous_count = 0;
  last_predict = -1;

  return this_predict;
}

int gesture() {

  // Whether we should clear the buffer next time we fetch data
  bool should_clear_buffer = false;
  bool got_data = false;

  // The gesture index of the prediction
  int gesture_index;

  // Set up logging.
  static tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = tflite::GetModel(g_magic_wand_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return -1;
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.
  static tflite::MicroOpResolver<6> micro_op_resolver;
  micro_op_resolver.AddBuiltin(
      tflite::BuiltinOperator_DEPTHWISE_CONV_2D,
      tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_MAX_POOL_2D,
                               tflite::ops::micro::Register_MAX_POOL_2D());
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D,
                               tflite::ops::micro::Register_CONV_2D());
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_FULLY_CONNECTED,
                               tflite::ops::micro::Register_FULLY_CONNECTED());
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
                               tflite::ops::micro::Register_SOFTMAX());
  micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_RESHAPE,
                               tflite::ops::micro::Register_RESHAPE(), 1);

  // Build an interpreter to run the model with
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  tflite::MicroInterpreter* interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors
  interpreter->AllocateTensors();

  // Obtain pointer to the model's input tensor
  TfLiteTensor* model_input = interpreter->input(0);
  if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
      (model_input->dims->data[1] != config.seq_length) ||
      (model_input->dims->data[2] != kChannelNumber) ||
      (model_input->type != kTfLiteFloat32)) {
    error_reporter->Report("Bad input tensor parameters in model");
    return -1;
  }

  int input_length = model_input->bytes / sizeof(float);

  TfLiteStatus setup_status = SetupAccelerometer(error_reporter);
  if (setup_status != kTfLiteOk) {
    error_reporter->Report("Set up failed\n");
    return -1;
  }

  error_reporter->Report("Set up successful...\n");

  while (true) {

    // Attempt to read new data from the accelerometer
    got_data = ReadAccelerometer(error_reporter, model_input->data.f,
                                 input_length, should_clear_buffer);

    // If there was no new data,
    // don't try to clear the buffer again and wait until next time
    if (!got_data) {
      should_clear_buffer = false;
      continue;
    }

    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
      error_reporter->Report("Invoke failed on index: %d\n", begin_index);
      continue;
    }

    // Analyze the results to obtain a prediction
    gesture_index = PredictGesture(interpreter->output(0)->data.f);

    // Clear the buffer next time we read data
    should_clear_buffer = gesture_index < label_num;

    // Produce an output
    if (gesture_index < label_num) {
      //printf("%d\n",gesture_index);
      //error_reporter->Report(config.output_message[gesture_index]);
      
      return gesture_index;
    }
  }
}


int selected;
int option;

void detection(){

double ratio;
double angle;
double DataXYZ_float[3];

 ulcd_display_selected(option);
  while (1)
  {
    BSP_ACCELERO_AccGetXYZ(DataXYZ);
  //printf("%d, %d, %d\n", DataXYZ[0], DataXYZ[1], DataXYZ[2]);

  for(int i=0;i<3;i++)
  DataXYZ_float[i]=DataXYZ[i];

  ratio=DataXYZ_float[1]/DataXYZ_float[2];

  angle=atan(ratio)*180/3.14159265;

  //printf("The arc tangent of %lf is %lf degree\n",ratio,angle);
  
  if(angle>=selected)
    myled3=1;

  else 
  myled3=0;
  ThisThread::sleep_for(800ms);
  }
  
  //myled=0;
  
}


int stop;

void stop_condition(){

  stop=1;

}


void selecting(){
  
   BSP_ACCELERO_Init();
  thread.start(callback(&queue, &EventQueue::dispatch_forever));
  int select=1;
    int mypin;
   while (1)
    {    mypin=gesture();

        if (mypin==0)
            select++;
        if(mypin==1)
            select--;
        if(select>4)
            select=1;
        if(select<1)
            select=4;

        option=select;
        ulcd_display(select);

       if(select==2)
        selected=45;

        else if(select==1)
        selected=30;

        else if(select==3)
        selected=60;

        else if(select==4)
        selected=80;

        

        
      
       mypin_select.fall(queue.event(stop_condition));

      if(stop==1)
        break;
      
   }
   
  mypin_select.rise(queue.event(detection));
}