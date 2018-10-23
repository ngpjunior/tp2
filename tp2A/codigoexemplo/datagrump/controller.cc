#include <iostream>
#include <string>
#include <fstream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug ), the_window_size(1), ai(0.5), md(1.5),boundary_window(50),time_out_ms(80)
{
cout << the_window_size;
 fstream file;

 file.open("/home/tp2/log.txt", ios::ate | ios::app);
 file << "Window: "<< the_window_size << " AI: " << ai << " MD: "<< md << " Bound: "<< boundary_window << " time out: "<< time_out_ms<<"\n";
 file.close();
}

/* Get current window size, in datagrams */
double Controller::window_size()
{
  /* Default: fixed window size of 100 outstanding datagrams */
 /* unsigned int the_window_size = 50;*/

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}


void Controller::set_window_size(bool AIMD){

  string AIMD_debug = "";
  string older_window = "";
  


  if (AIMD){
    older_window = string("Old window: " );
    the_window_size = max(the_window_size/md, 1.0);
    AIMD_debug = string("MD: "+ to_string(md))+string(older_window);

  }else{
    AIMD_debug = string("AI: "+ to_string(ai));
    the_window_size += ai;
  }

  if ( debug_ ){
    cerr << AIMD_debug<< "\n";
  }


}



/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp,
                                    /* in milliseconds */
				    const bool after_timeout
				    /* datagram was sent because of a timeout */ )
{
  /* Default: take no action */

  if ( after_timeout ){
    this->set_window_size(true);
  }

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << " (timeout = " << after_timeout << ")\n";
  }
}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action 
     boundary window limited if != 0
    */ 

  if( the_window_size < boundary_window ){
    
    this->set_window_size( false );

  }else if ( boundary_window == 0 ){
  
    this->set_window_size( false );

  }

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms()
{
  return time_out_ms; /* timeout of one second */
}
