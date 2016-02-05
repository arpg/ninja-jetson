#include <stdio.h>
#include <string>
#include <node/Node.h>

#include "NinjaMsgs.pb.h"
#include "FtdiDriver.h"
#include "GetPot"
#include <cstring>
#include "JoystickHandler.h"

bool DEBUG = true;

FtdiDriver  g_FtdiDriver;
NinjaCommandMsg cmd;
NinjaStateMsg state_msg;


NinjaStateMsg BuildNinjaStateMsg( const SensorPacket& p )
{
  NinjaStateMsg msg;
  msg.set_acc_x( p.Acc_x );
  msg.set_acc_y( p.Acc_y );
  msg.set_acc_z( p.Acc_z );
  msg.set_gyro_x( p.Gyro_x );
  msg.set_gyro_y( p.Gyro_y );
  msg.set_gyro_z( p.Gyro_z );
  msg.set_mag_x( p.Mag_x );
  msg.set_mag_y( p.Mag_y );
  msg.set_mag_z( p.Mag_z );
  msg.set_enc_lb( p.Enc_LB );
  msg.set_enc_lf( p.Enc_LF );
  msg.set_enc_rb( p.Enc_RB );
  msg.set_enc_rf( p.Enc_RF );
  msg.set_adc_steer( p.ADC_Steer );
  msg.set_adc_lb( p.ADC_LB );
  msg.set_adc_lf( p.ADC_LF );
  msg.set_adc_rb( p.ADC_RB );
  msg.set_adc_rf( p.ADC_RF );

  return msg;
}

int main( int argc, char** argv )
{
  GetPot cl(argc,argv);
  std::string dev = cl.follow("/dev/ttyUSB0","--dev");

  printf("Connecting to FTDI com port '%s'...\n", dev.c_str() );
  g_FtdiDriver.Connect(dev);


  JoystickHandler joystick;
  joystick.InitializeJoystick();
  joystick.UpdateJoystick();
  for( size_t ii = 0; ; ++ii ) {



    // read from the car's microcontroller
    SensorPacket state,new_state;
/*
    //Check package contents received over usb
    if( g_FtdiDriver.ReadSensorPacket(new_state) == 0 ){
      //printf("Ftdi read returned 0\n");
    }else
    {
      memmove(&state,&new_state,sizeof(SensorPacket));
      if(DEBUG){
        //printf("*********************************************************\n");
        //printf("acc_x: %d  acc_y: %d  acc_z: %d \n",state.Acc_x,state.Acc_y,state.Acc_z);
        //printf("Gyro_x: %d  Gyro_y: %d  Gyro_z: %d \n",state.Gyro_x,state.Gyro_y,state.Gyro_z);
        //printf("Mag_x: %d  Mag_y: %d  Mag_z: %d \n",state.Mag_x,state.Mag_y,state.Mag_z);
        printf("Enc_lb: %d  Enc_lf: %d  Enc_rb: %d  Enc_rf: %d \n",state.Enc_LB,state.Enc_LF,state.Enc_RB,state.Enc_RF);
        //printf("adc_steer: %d  adc_lb: %d  adc_lf: %d  adc_rb: %d  adc_rf: %d \n", state.ADC_Steer,state.ADC_LB,state.ADC_LF,state.ADC_RB,state.ADC_RF);
        //printf("time: %d",state.timestamp);
      }
    }

*/
    Gamepad_processEvents();
    int a;
    //printf("%d----\n",a++);
    float speed = -(double)joystick.GetAxisValue(1);
    float turnrate = -(double)joystick.GetAxisValue(2);

    if(  speed || turnrate){
      printf("speed is %f and turnrate is %f\n",speed,turnrate);
      // relay command to the car
      g_FtdiDriver.SendCommandPacket( speed , turnrate );
    }

    // Sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  joystick.JoinThread();
  return 0;
}

