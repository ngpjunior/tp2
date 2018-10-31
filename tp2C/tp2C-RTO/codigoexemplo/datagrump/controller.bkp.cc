#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

 debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* Default constructor * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
Controller::Controller( const bool debug )
  : debug_( debug ), the_window_size(1), ai(0.5), md(15),boundary_window(12),time_out_ms(80)
{

 fstream file;

 file.open("log.txt", ios::ate | ios::app);
 file << "Window: "<< the_window_size << " AI: " << ai << " MD: "<< md << " Bound: "<< boundary_window << " time out: "<< time_out_ms<<"\n";
 file.close();
}

/ *
LOGS EM TEMPO REAL PARA O GRÁFICO DENTE DE SERRA
void Controller::save_log(){
 fstream file;

 file.open("logB.txt", ios::ate | ios::app);
 file << "time-stamp: "<< timestamp_ms() <<" Window: "<< the_window_size << " AI: " << ai << " MD: "<< md << " Bound: "<< boundary_window << " time out: "<< time_out_ms<<"\n";


 file.close();


}*/


 debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* Get current window size, in datagrams * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
double Controller::window_size()
{
   debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* Default: fixed window size of 100 outstanding datagrams * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
  debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* unsigned int the_window_size = 50;* debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)

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
    the_window_size = max(the_window_size debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)md, 1.0);
    AIMD_debug = string("\nMD: "+ to_string(md))+string(older_window);

  }else{
    AIMD_debug = string("\nAI: "+ to_string(ai));
    the_window_size += ai;
  }
 
  cerr << "TWS: "<<the_window_size<<"\n";
 debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* this->save_log();  * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
if ( debug_ ){
    cerr << AIMD_debug<< "\n";
  }
  

}



 debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* A datagram was sent * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
void Controller::datagram_was_sent( const uint64_t sequence_number,
				     debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* of the sent datagram * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
				    const uint64_t send_timestamp,
                                     debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* in milliseconds * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
				    const bool after_timeout
				     debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* datagram was sent because of a timeout * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80) )
{
   debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* Default: take no action * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)

  if ( after_timeout ){
    this->set_window_size(true);
  }

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << " (timeout = " << after_timeout << ")\n";
  }
}

 debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* An ack was received * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
void Controller::ack_received( const uint64_t sequence_number_acked,
			        debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* what sequence number was acknowledged * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
			       const uint64_t send_timestamp_acked,
			        debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* when the acknowledged datagram was sent (sender's clock) * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
			       const uint64_t recv_timestamp_acked,
			        debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* when the acknowledged datagram was received (receiver's clock)* debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
			       const uint64_t timestamp_ack_received )
                                debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* when the ack was received (by sender) * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
{
   debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* Default: take no action 
     boundary window limited if != 0
    * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80) 

  if( the_window_size < boundary_window ){
    
    this->set_window_size( false );

  }else if ( boundary_window == 0 ){
  
    this->set_window_size( false );

  }else{
    this->set_window_size(true);
  }

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
   
  }
}

 debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
unsigned int Controller::timeout_ms()
{
  return time_out_ms;  debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)* timeout of one second * debug_( debug ), the_window_size(1), ai(0.1), md(19),boundary_window(12),time_out_ms(80)
}
