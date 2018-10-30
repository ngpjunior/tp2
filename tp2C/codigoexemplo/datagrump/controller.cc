#include <iostream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug ), ALPHA(0.8), BETA(0.3), DELTA(0.25), T_LOW(60), T_HIGH(150), MIN_RTT(50), MIN_RATE(1.0), N(1), prev_rtt(MIN_RTT), rtt_diff(0), the_window_size(50), new_rtt(0), time_out_ms(0)
{
  
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

void Controller::set_window_size(uint64_t timestamp_ack_received, uint64_t send_timestamp_acked){


  new_rtt = timestamp_ack_received - send_timestamp_acked;

  double new_rtt_diff = new_rtt - prev_rtt;
  prev_rtt = new_rtt;
  rtt_diff = (1 - ALPHA) * rtt_diff + ALPHA * new_rtt_diff;

  double normalized_gradient = rtt_diff / MIN_RTT;

  if (new_rtt < T_LOW) {
    the_window_size += DELTA;
  }else if (new_rtt > T_HIGH) {
    the_window_size *= (1 - BETA * (1- T_HIGH / new_rtt));

    if (the_window_size < MIN_RATE) {
      the_window_size = MIN_RATE;
    }
    return;
  }
  if (normalized_gradient <= 0) {
    the_window_size += N * DELTA;
  } else {
    the_window_size *= (1 - BETA * normalized_gradient);
  }

  if (the_window_size < MIN_RATE) {
    the_window_size = MIN_RATE;
  }
  
  if (first){
  cerr<<"ERRADO";
  srtt = new_rtt;
  rttvar = new_rtt/2;
  time_out_ms = srtt + max(100.0, 4*rttvar);
  first = 0;
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
  this->set_window_size(timestamp_ack_received, send_timestamp_acked);

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

    double aux =srtt-new_rtt;if (aux < 0) aux *= -1;  
  if (not first){

    rttvar = 0.75*(rttvar) + 0.25 *  aux;	
    srtt = 0.875*srtt+0.125*new_rtt;
    time_out_ms = (srtt + max(100.0, 4*rttvar));
  }  
   cerr << "RTO: "<<time_out_ms <<"RTTVAR: "<<rttvar<<"SRTT: "<< srtt<<"RTT: "<< new_rtt<<"AUX: "<<aux<<"\n";

  return (time_out_ms>80) ? 80 : time_out_ms; /* timeout of one second */
}
