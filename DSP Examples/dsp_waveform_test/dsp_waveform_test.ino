// https://m0agx.eu/2018/05/23/practical-fft-on-microcontrollers-using-cmsis-dsp/

#include <ILI9341_t3.h>           // fast display driver lib
#include <ILI9341_t3_Controls.h>
#include <font_Arial.h>           // custom fonts that ships with ILI9341_t3.h

// you must create and pass fonts to the function
#define FONT_TITLE Arial_16
#define FONT_DATA Arial_10

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

// defines for graph location and scales
#define X_ORIGIN    50      //GraphXLoc
#define Y_ORIGIN    200     //GraphYLoc
#define X_WIDE       250    //GraphHeight
#define Y_HIGH      150     //GraphWidth
#define X_LOSCALE   0       //XAxisLow
#define X_HISCALE   300
#define X_INC       50       //XAxisInc
#define Y_LOSCALE   0
#define Y_HISCALE   10000
#define Y_INC       2000

#define TEXTCOLOR C_WHITE
#define GRIDCOLOR C_GREY
#define AXISCOLOR C_GREEN
#define BACKCOLOR C_BLACK
#define PLOTCOLOR C_DKGREY
#define VOLTSCOLOR C_RED
#define SINCOLOR C_YELLOW
#define COSCOLOR C_BLUE

// used to monitor elaspsed time
unsigned long oldTime;

// create a variable for each data data point
//float x, volts;

// create an ID for each data to be plotted
int fftID, fftID1, fftID2;

// create the display object
ILI9341_t3 Display(TFT_CS, TFT_DC);

// create the cartesian coordinate graph object
CGraph MyGraph(&Display, X_ORIGIN, Y_ORIGIN, X_WIDE, Y_HIGH, X_LOSCALE, X_HISCALE, X_INC, Y_LOSCALE, Y_HISCALE, Y_INC);

#define X_LOSCALE1   0       //XAxisLow
#define X_HISCALE1   11
#define X_INC1        1       //XAxisInc
#define Y_LOSCALE1   -1.5
#define Y_HISCALE1   +1.5
#define Y_INC1       0.5
CGraph MyGraph1(&Display, X_ORIGIN, Y_ORIGIN, X_WIDE, Y_HIGH, X_LOSCALE1, X_HISCALE1, X_INC1, Y_LOSCALE1, Y_HISCALE1, Y_INC1);

#define X_LOSCALE2   0       //XAxisLow
#define X_HISCALE2   25
#define X_INC2       5       //XAxisInc
#define Y_LOSCALE2   0
#define Y_HISCALE2   240
#define Y_INC2       40
CGraph MyGraph2(&Display, X_ORIGIN, Y_ORIGIN, X_WIDE, Y_HIGH, X_LOSCALE2, X_HISCALE2, X_INC2, Y_LOSCALE2, Y_HISCALE2, Y_INC2);


//=== End Graph setup

#include "arm_math.h"
#include "arm_const_structs.h"
#include "testData.h"

#define printf Serial.printf


//===========================================================
#define FFT_SIZE 256
 
typedef struct {
  const char *desc;
  unsigned char *data;
} test_wave_t;
 
static const test_wave_t WAVES[] = {
    { "697Hz", __697hz_raw },
};
 
void fft_test(void){
  static arm_rfft_instance_q15 fft_instance;
  static q15_t output[FFT_SIZE*2]; //has to be twice FFT size
  static q15_t pResult;
  uint32_t index;

  // initialize the graph object
  MyGraph.init("", "bin", "mag", TEXTCOLOR, GRIDCOLOR, AXISCOLOR, BACKCOLOR, PLOTCOLOR, FONT_TITLE, FONT_DATA);
  fftID = MyGraph.add("mag", SINCOLOR);
  MyGraph.drawGraph();  //draws empty graph
  
  arm_status status;
 
  status = arm_rfft_init_q15(&fft_instance, 256/*bin count*/, 0/*forward FFT*/, 1/*output bit order is normal*/);
  printf("FFT init %d\n", status);
 
  for (uint32_t i = 0; i < sizeof(WAVES)/sizeof(WAVES[0]); i++){
 
    uint32_t c_start = micros();
 
    arm_rfft_q15(&fft_instance, (q15_t*)WAVES[i].data, output);
 
    arm_abs_q15(output, output, FFT_SIZE);
 
    uint32_t c_stop = micros();
 
    //printf("%s %ld \n", WAVES[i].desc, c_stop-c_start);

    for (uint32_t j = 0; j < FFT_SIZE; j++){
      //printf("%d, %d\n ", j, output[j]);
        MyGraph.setX(j);
        MyGraph.plot(fftID, output[j]);
    }
    printf("\n");

    arm_max_q15(output, FFT_SIZE, &pResult, &index);
    //printf("Max Val: %d, Bin: %d\n", pResult, index);
    Display.setCursor(50,20);
    Display.setTextColor(C_YELLOW);
    Display.print("Peak at Bin "); Display.print(index);
  }

  Serial.println("NEXT Example");
  nextPage();
}

//=====================================================
union
{
    float32_t f[512];
    uint8_t b[4*512];
}data_out;

union
{
    float32_t f[512];
    uint8_t b[4*512];
}data_mag;

void AN4841_fft_test()
{
  float Fs = 48000.;
  float T = 1/Fs;
  int Length = 512;
  float t[512], input_signal[512];

  Serial.println("Plotting Signal from AN4841");
  // initialize the graph object
  MyGraph1.init("", "bin", "mag", TEXTCOLOR, GRIDCOLOR, AXISCOLOR, BACKCOLOR, PLOTCOLOR, FONT_TITLE, FONT_DATA);
  fftID1 = MyGraph1.add("mag", SINCOLOR);
  MyGraph1.drawGraph();  //draws empty graph

  for(int i = 0; i < Length; i++) {
    t[i] = i*T;
    input_signal[i] = sin(2*PI*1000*t[i]) + 0.5*sin(2*PI*15000*t[i]);
    //printf("%f, %f\n", t[i]*1000, input_signal[i]);
    MyGraph1.setX(t[i]*1000);
    MyGraph1.plot(fftID, input_signal[i]);
  }

  nextPage();
  
  Serial.println("FFT in Frequency Domain next");
  //Lets experiment with using Real FFT, 
  // https://www.keil.com/pack/doc/CMSIS/DSP/html/group__RealFFT.html#details
  // https://stackoverflow.com/questions/42299932/dsp-libraries-rfft-strange-results

  static arm_rfft_fast_instance_f32 refft_instance;
  static float32_t pResult;
  float32_t freq;
  //float32_t * pDst  = (float32_t *)malloc(sizeof(float32_t) * Length);

  // initialize the graph object
  MyGraph2.init("", "bin", "mag", TEXTCOLOR, GRIDCOLOR, AXISCOLOR, BACKCOLOR, PLOTCOLOR, FONT_TITLE, FONT_DATA);
  fftID2 = MyGraph2.add("mag", SINCOLOR);
  MyGraph2.drawGraph();  //draws empty graph
  
  arm_status status;
 
  status = arm_rfft_fast_init_f32(&refft_instance, Length);
  printf("FFT init %d\n", status);

  arm_rfft_fast_f32(&refft_instance, input_signal, data_out.f, 0);
  // General case for the magnitudes
  arm_cmplx_mag_f32(data_out.f+2, data_mag.f+1, Length/2 - 1);
  data_mag.f[0]           = data_out.f[0];
  data_mag.f[Length/2]    = data_out.f[1];

  for(int j = 0; j < Length/2; j++) {
    //printf("%f, %f\n", data_mag.f[j], (j*Fs/Length)/10000);
    MyGraph2.setX((j*Fs/Length)/1000);
    MyGraph2.plot(fftID2, data_mag.f[j]);
  }
  nextPage();
}

void setup() {
  Serial.begin(9600);
  while(!Serial){};

  // fire up the display
  Display.begin();
  Display.setRotation(1);
  Display.fillScreen(C_BLACK);

  fft_test();
  AN4841_fft_test();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void nextPage()
{
  Serial.println("Press anykey to continue");
  while (Serial.read() == -1) ;
  while (Serial.read() != -1) ;

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
}
