#include <iostream>
#include <fstream>
#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor 

RTO IMPLEMENTADO
AMANHÃ PRECISO EXECUTAR MAIS UNS TESTES COM O TIMELY ENQUANTO LEIO E ENTENDO MELHOR

ESCREVER UMS HISTORINHA SOBRE O RTO, TIMELY+RTO E SOMENTE O TIMELY
CITAR O JACOBS E A MITTAL

*/
Controller::Controller( const bool debug ):debug_( debug ), ai(0.1), md(2), the_window_size(1), rtt(0), time_out_ms(0), boundary_window(12)
{
fstream file; 
file.open("/home/tp2/log.txt", ios::ate | ios::app); 
file << "Window: "<< the_window_size << " AI: " << ai << " MD: "<< md << " Bound: "<< boundary_window << " time out: "<< time_out_ms<<"\n";
 file.close();
}

/* Get current window size, in datagrams */
double Controller::window_size()
{
  /* Default: fixed window size of 100 outstanding datagrams */
/* double the_window_size = rate;*/
  cerr <<"\n"<<"TWS: "<< the_window_size ;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

void Controller::save_log(){
 fstream file;

 file.open("/home/tp2/logB.txt", ios::ate | ios::app);
 file << "time-stamp: "<< timestamp_ms() <<" Window: "<< the_window_size << " AI: " << ai << " MD: "<< md << " time out: "<< time_out_ms<<"\n";


 file.close();


}

void Controller::set_window_size( bool AIMD ){
  string AIMD_debug = "";
  string older_window = "";

  if (AIMD){
    older_window = string("Old window: " + to_string(the_window_size));
    the_window_size = max(the_window_size/md, 1.0);
    AIMD_debug = string("MD: "+ to_string(md))+string(older_window);

  }else{
    AIMD_debug = string("AI: "+ to_string(ai));
    the_window_size += ai;
  }

  if ( debug_ ){
    cerr << AIMD_debug<< "\n";
  }

  this->save_log();
}
void Controller::set_rtt(uint64_t timestamp_ack_received, uint64_t send_timestamp_acked){
  rtt = timestamp_ack_received - send_timestamp_acked;
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
  if (after_timeout){
   this->set_rtt(timestamp_ms(), send_timestamp);
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
  /* Default: take no action */
  this->set_rtt(timestamp_ack_received, send_timestamp_acked);
  if(rtt < time_out_ms && the_window_size < boundary_window){
    this->set_window_size(false);
   } else if ( the_window_size >= boundary_window ) {
    this->set_window_size(true);
  }
/*else if(the_window_size >= 12){
   this->set_window_size(true);
  }*/

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
  double alpha= 0.125;
  double beta = 0.25; 
if(first){
    srtt = rtt;
    rttvar = rtt/2;
    time_out_ms = srtt + max(100.0, 4*rttvar);
    first = 0;
  }else{
    double aux =rtt-srtt;if (aux < 0) aux *= -1;
    rttvar = ((1-beta)*rttvar) + (beta *  aux);	
    srtt = ((1-alpha)*srtt)+(alpha*rtt);
    time_out_ms = (srtt + max(10.0, 4*rttvar));
  }
/*   time_out_ms = (time_out_ms > rtt)? rtt+10:time_out_ms;*/
   cerr << "RTO: "<<time_out_ms <<"RTT: "<<rtt<<"\n";
  
  return time_out_ms; /* timeout of one second */
}
