#include <stdio.h>
#include <string>
#include <Node/Node.h>

#include "NinjaMsgs.pb.h"
#include "FtdiDriver.h"
#include "GetPot"

FtdiDriver  g_FtdiDriver;
//node::node  nc_node;

NinjaStateMsg BuildNinjaStateMsg( const SensorPacket& p )
{
  NinjaStateMsg msg;
  msg.set_m_cdelimiter1( p.m_cDelimiter1 );
  msg.set_m_cdelimiter2( p.m_cDelimiter2 );
  msg.set_m_csize( p.m_cSize );
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
};

int main( int argc, char** argv )
{
  GetPot cl(argc,argv);
  std::string dev = cl.follow("/dev/ttyUSB0","--dev");

  //nc_node.init("nc_node");

  printf("Connecting to FTDI com port '%s'...\n", dev.c_str() );
  g_FtdiDriver.Connect(dev);
  // subscribe to the ninja_command topic
  //nc_node.subscribe("ninja_commander/command");

  // advertise that we will transmit state data
  //if( nc_node.advertise("state") == false ) {
  //  printf("Error setting publisher.\n");
  //}


  for( size_t ii = 0; ; ++ii ) {
//    if( ii % 1000 == 0 ) {
//      printf(".");
//      fflush(stdout);
//    }

    // read from the car's microcontroller
    SensorPacket state;
    if( g_FtdiDriver.ReadSensorPacket(state) == 0 ) {
      printf("Ftdi read returned 0\n");
    }else
    {
      printf("*********************************************************");
      printf("acc_x: %d  acc_y: %d  acc_z: %d \n",state.Acc_x,state.Acc_y,state.Acc_z);
      printf("Gyro_x: %d  Gyro_y: %d  Gyro_z: %d \n",state.Gyro_x,state.Gyro_y,state.Gyro_z);
      printf("Mag_x: %d  Mag_y: %d  Mag_z: %d \n",state.Mag_x,state.Mag_y,state.Mag_z);
      printf("Enc_lb: %d  Enc_lf: %d  Enc_rb: %d  Enc_rf: %d \n",state.Enc_LB,state.Enc_LF,state.Enc_RB,state.Enc_RF);
      printf("adc_steer: %d  adc_lb: %d  adc_lf: %d  adc_rb: %d  adc_rf: %d \n", state.ADC_Steer,state.ADC_LB,state.ADC_LF,state.ADC_RB,state.ADC_RF);
    }

    // package and send our state protobuf
    NinjaStateMsg state_msg = BuildNinjaStateMsg( state );
//    if ( nc_node.publish( "state", state_msg ) == false ) {
//      printf("Error sending message.\n");
//    }

    // let's see what the controller is telling us to do
    NinjaCommandMsg cmd;
//    if( nc_node.receive("ninja_commander/command", cmd) ){
//      printf("Received a=%.2f and p=%.2f\n", cmd.speed(), cmd.turnrate() );
      // relay command to the car 
      //g_FtdiDriver.SendCommandPacket( cmd.speed(), cmd.turnrate() );
      g_FtdiDriver.SendCommandPacket( 123, 45 );
//    }
    usleep(100000);
  }

  return 0;
}

