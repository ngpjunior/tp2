#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <cstdint>

/* Congestion controller interface */

class Controller
{
private:
  bool debug_; /* Enables debugging output */
  const double ALPHA;
  const double BETA;
  const double DELTA;

  const unsigned int T_LOW;
  const unsigned int T_HIGH;
  const unsigned int MIN_RTT;
  const double MIN_RATE;
  const unsigned int N;

  unsigned int prev_rtt;
  double rtt_diff;
  double  the_window_size;
  double new_rtt;
  double time_out_ms;
  bool first = 1;
  double srtt = 0;
  double rttvar = 0;

/* Add member variables here */

public:
  /* Public interface for the congestion controller */
  /* You can change these if you prefer, but will need to change
     the call site as well (in sender.cc) */

  /* Default constructor */
  Controller( const bool debug );

  /* Get current window size, in datagrams */
  double window_size();
  void set_window_size(uint64_t timestamp_ack_received, uint64_t send_timestamp_acked);
  void save_log();
  /* A datagram was sent */
  void datagram_was_sent( const uint64_t sequence_number,
			  const uint64_t send_timestamp,
			  const bool after_timeout );

  /* An ack was received */
  void ack_received( const uint64_t sequence_number_acked,
		     const uint64_t send_timestamp_acked,
		     const uint64_t recv_timestamp_acked,
		     const uint64_t timestamp_ack_received );

  /* How long to wait (in milliseconds) if there are no acks
     before sending one more datagram */
  unsigned int timeout_ms();
};

#endif
