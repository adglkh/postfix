/*++
/* NAME
/*	lmtp 8
/* SUMMARY
/*	Postfix local delivery via LMTP
/* SYNOPSIS
/*	\fBlmtp\fR [generic Postfix daemon options] [server attributes...]
/* DESCRIPTION
/*	The LMTP client processes message delivery requests from
/*	the queue manager. Each request specifies a queue file, a sender
/*	address, a domain or host to deliver to, and recipient information.
/*	This program expects to be run from the \fBmaster\fR(8) process
/*	manager.
/*
/*	The LMTP client updates the queue file and marks recipients
/*	as finished, or it informs the queue manager that delivery should
/*	be tried again at a later time. Delivery problem reports are sent
/*	to the \fBbounce\fR(8) or \fBdefer\fR(8) daemon as appropriate.
/*
/*	If no server is given on the command line, the LMTP client connects
/*	to the destination specified in the message delivery request and to
/*	the TCP port defined as \fBlmtp\fR in \fBservices\fR(4).  If no such
/*	service is found, the \fBlmtp_tcp_port\fR configuration parameter
/*	(default value of 24) will be used. The LMTP client does not perform
/* 	MX (mail exchanger) lookups since those are defined only for SMTP.
/* SERVER ATTRIBUTE SYNTAX
/* .ad
/* .fi
/*	The server attributes are given in the \fBmaster.cf\fR file at
/*	the end of a service definition.  The syntax is as follows:
/* .IP "\fBserver=\fR\fIhost\fR"
/* .IP "\fBserver=\fR\fIhost\fR\fB:\fR\fIport\fR"
/* .IP "\fBserver=[\fR\fIipaddr\fR\fB]\fR"
/* .IP "\fBserver=[\fR\fIipaddr\fR\fB]:\fR\fIport\fR"
/*	Connect to the specified host or IP address and TCP port (default: the
/*	\fBlmtp\fR port as specified in the \fBservices\fR database).
/* .PP
/* SECURITY
/* .ad
/* .fi
/*	The LMTP client is moderately security-sensitive. It talks to LMTP
/*	servers and to DNS servers on the network. The LMTP client can be
/*	run chrooted at fixed low privilege.
/* STANDARDS
/*	RFC 821 (SMTP protocol)
/*	RFC 1651 (SMTP service extensions)
/*	RFC 1870 (Message Size Declaration)
/*	RFC 2033 (LMTP protocol)
/*	RFC 2197 (Pipelining)
/* DIAGNOSTICS
/*	Problems and transactions are logged to \fBsyslogd\fR(8).
/*	Corrupted message files are marked so that the queue manager can
/*	move them to the \fBcorrupt\fR queue for further inspection.
/*
/*	Depending on the setting of the \fBnotify_classes\fR parameter,
/*	the postmaster is notified of bounces, protocol problems, and of
/*	other trouble.
/* BUGS
/* CONFIGURATION PARAMETERS
/* .ad
/* .fi
/*	The following \fBmain.cf\fR parameters are especially relevant to
/*	this program. See the Postfix \fBmain.cf\fR file for syntax details
/*	and for default values. Use the \fBpostfix reload\fR command after
/*	a configuration change.
/* .SH Miscellaneous
/* .ad
/* .fi
/* .IP \fBdebug_peer_level\fR
/*	Verbose logging level increment for hosts that match a
/*	pattern in the \fBdebug_peer_list\fR parameter.
/* .IP \fBdebug_peer_list\fR
/*	List of domain or network patterns. When a remote host matches
/*	a pattern, increase the verbose logging level by the amount
/*	specified in the \fBdebug_peer_level\fR parameter.
/* .IP \fBerror_notice_recipient\fR
/*	Recipient of protocol/policy/resource/software error notices.
/* .IP \fBnotify_classes\fR
/*	When this parameter includes the \fBprotocol\fR class, send mail to the
/*	postmaster with transcripts of LMTP sessions with protocol errors.
/* .IP \fBlmtp_skip_quit_response\fR
/*	Do not wait for the server response after sending QUIT.
/* .IP \fBlmtp_tcp_port\fR
/*	The TCP port to be used when connecting to a LMTP server.  Used as
/*	backup if the \fBlmtp\fR service is not found in \fBservices\fR(4).
/* .SH "Resource controls"
/* .ad
/* .fi
/* .IP \fBlmtp_cache_connection\fR
/*	Should we cache the connection to the LMTP server? The effectiveness
/*	of cached connections will be determined by the number of LMTP servers
/*	in use, and the concurrency limit specified for the LMTP client.
/*	Cached connections are closed under any of the following conditions:
/* .RS
/* .IP \(bu
/*	The LMTP client idle time limit is reached. This limit is specified
/*	with the Postfix \fBmax_idle\fR configuration parameter.
/* .IP \(bu
/*	A delivery request specifies a different destination than the one
/*	currently cached.
/* .IP \(bu
/*	The per-process limit on the number of delivery requests is reached.
/*	This limit is specified with the Postfix \fBmax_use\fR configuration
/*	parameter.
/* .IP \(bu
/*	Upon the onset of another delivery request, the LMTP server associated
/*	with the current session does not respond to the \fBRSET\fR command.
/* .RE
/* .IP \fItransport_\fBdestination_concurrency_limit\fR
/*	Limit the number of parallel deliveries to the same destination
/*	via this mail delivery transport. \fItransport\fR is the name
/*	of the service as specified in the \fBmaster.cf\fR file.
/*	The default limit is taken from the
/*	\fBdefault_destination_concurrency_limit\fR parameter.
/* .IP \fItransport_\fBdestination_recipient_limit\fR
/*	Limit the number of recipients per message delivery via this mail
/*	delivery transport. \fItransport\fR is the name
/*	of the service as specified in the \fBmaster.cf\fR file.
/*	The default limit is taken from the
/*	\fBdefault_destination_recipient_limit\fR parameter.
/*
/*	This parameter becomes significant if the LMTP client is used
/*	for local delivery.  Some LMTP servers can optimize delivery of
/*	the same message to multiple recipients. The default limit for
/*	local mail delivery is 1.
/*
/*	Setting this parameter to 0 will lead to an unbounded number of
/*	recipients per delivery.  However, this could be risky since it may
/*	make the machine vulnerable to running out of resources if messages
/*	are encountered with an inordinate number of recipients.  Exercise
/*	care when setting this parameter.
/* .SH "Timeout controls"
/* .ad
/* .fi
/* .IP \fBlmtp_connect_timeout\fR
/*	Timeout in seconds for opening a connection to the LMTP server.
/*	If no connection can be made within the deadline, the message
/*	is deferred.
/* .IP \fBlmtp_lhlo_timeout\fR
/*	Timeout in seconds for sending the \fBLHLO\fR command, and for
/*	receiving the server response.
/* .IP \fBlmtp_mail_timeout\fR
/*	Timeout in seconds for sending the \fBMAIL FROM\fR command, and for
/*	receiving the server response.
/* .IP \fBlmtp_rcpt_timeout\fR
/*	Timeout in seconds for sending the \fBRCPT TO\fR command, and for
/*	receiving the server response.
/* .IP \fBlmtp_data_init_timeout\fR
/*	Timeout in seconds for sending the \fBDATA\fR command, and for
/*	receiving the server response.
/* .IP \fBlmtp_data_xfer_timeout\fR
/*	Timeout in seconds for sending the message content.
/* .IP \fBlmtp_data_done_timeout\fR
/*	Timeout in seconds for sending the "\fB.\fR" command, and for
/*	receiving the server response. When no response is received, a
/*	warning is logged that the mail may be delivered multiple times.
/* .IP \fBlmtp_rset_timeout\fR
/*	Timeout in seconds for sending the \fBRSET\fR command, and for
/*	receiving the server response.
/* .IP \fBlmtp_quit_timeout\fR
/*	Timeout in seconds for sending the \fBQUIT\fR command, and for
/*	receiving the server response.
/* SEE ALSO
/*	bounce(8) non-delivery status reports
/*	local(8) local mail delivery
/*	master(8) process manager
/*	qmgr(8) queue manager
/*	services(4) Internet services and aliases
/*	spawn(8) auxiliary command spawner
/*	syslogd(8) system logging
/* LICENSE
/* .ad
/* .fi
/*	The Secure Mailer license must be distributed with this software.
/* AUTHOR(S)
/*	Wietse Venema
/*	IBM T.J. Watson Research
/*	P.O. Box 704
/*	Yorktown Heights, NY 10598, USA
/*
/*	Alterations for LMTP by:
/*	Philip A. Prindeville
/*	Mirapoint, Inc.
/*	USA.
/*
/*	Additional work on LMTP by:
/*	Amos Gouaux
/*	University of Texas at Dallas
/*	P.O. Box 830688, MC34
/*	Richardson, TX 75083, USA
/*--*/

/* System library. */

#include <sys_defs.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dict.h>
#include <pwd.h>
#include <grp.h>

/* Utility library. */

#include <msg.h>
#include <argv.h>
#include <mymalloc.h>
#include <name_mask.h>
#include <split_at.h>

/* Global library. */

#include <deliver_request.h>
#include <mail_queue.h>
#include <mail_params.h>
#include <mail_conf.h>
#include <debug_peer.h>
#include <mail_error.h>

/* Single server skeleton. */

#include <mail_server.h>

/* Application-specific. */

#include "lmtp.h"

 /*
  * Tunable parameters. These have compiled-in defaults that can be overruled
  * by settings in the global Postfix configuration file.
  */
int     var_lmtp_tcp_port;
int     var_lmtp_conn_tmout;
int     var_lmtp_rset_tmout;
int     var_lmtp_lhlo_tmout;
int     var_lmtp_mail_tmout;
int     var_lmtp_rcpt_tmout;
int     var_lmtp_data0_tmout;
int     var_lmtp_data1_tmout;
int     var_lmtp_data2_tmout;
int     var_lmtp_quit_tmout;
char   *var_debug_peer_list;
int     var_debug_peer_level;
int     var_lmtp_cache_conn;
int     var_lmtp_skip_quit_resp;
char   *var_notify_classes;
char   *var_error_rcpt;

 /*
  * Global variables.
  * 
  * lmtp_errno is set by the address lookup routines and by the connection
  * management routines.
  * 
  * state is global for the connection caching to work.
  */
int     lmtp_errno;
static LMTP_STATE *state = 0;

/* get_service_attr - get command-line attributes */

static void get_service_attr(LMTP_STATE *state, char **argv)
{
    char   *myname = "get_service_attr";

    /*
     * Iterate over the command-line attribute list.
     */
    if (msg_verbose)
	msg_info("%s: checking argv for lmtp server", myname);

    for ( /* void */ ; *argv != 0; argv++) {

	/*
	 * Connect to a fixed LMTP server.
	 */
	if (strncasecmp("server=", *argv, sizeof("server=") - 1) == 0) {
	    state->fixed_dest = *argv + sizeof("server=") - 1;
	    if (state->fixed_dest[0] == 0)
		msg_fatal("invalid destination: %s", *argv);
	}

	/*
	 * Bad.
	 */
	else
	    msg_fatal("unknown attribute name: %s", *argv);
    }
}

/* deliver_message - deliver message with extreme prejudice */

static int deliver_message(DELIVER_REQUEST *request, char **unused_argv)
{
    char   *myname = "deliver_message";
    char   *destination;
    VSTRING *why;
    int     result;

    if (msg_verbose)
	msg_info("%s: from %s", myname, request->sender);

    /*
     * Sanity checks.
     */
    if (request->rcpt_list.len <= 0)
	msg_fatal("%s: recipient count: %d", myname, request->rcpt_list.len);

    /*
     * Initialize. Bundle all information about the delivery request, so that
     * we can produce understandable diagnostics when something goes wrong
     * many levels below. The alternative would be to make everything global.
     * 
     * Note: `state' was made global (to this file) so that we can cache
     * connections and so that we can close a cached connection via the
     * MAIL_SERVER_EXIT function (cleanup). The alloc for `state' is
     * performed in the MAIL_SERVER_PRE_INIT function (pre_init).
     * 
     */
    why = vstring_alloc(100);
    state->request = request;
    state->src = request->fp;
    destination = state->fixed_dest ? state->fixed_dest : request->nexthop;

    /*
     * See if we can reuse an existing connection.
     */
    if (state->session != 0) {

	/*
	 * Disconnect if we're going to a different destination. Discard
	 * transcript and status information from sending QUIT.
	 */
	if (strcasecmp(state->session->dest, destination) != 0) {
	    lmtp_quit(state);
	    lmtp_chat_reset(state);
	    state->session = lmtp_session_free(state->session);
	}

	/*
	 * Disconnect if RSET can't be sent over an existing connection.
	 * Discard transcript and status information from sending RSET.
	 */
	else if (lmtp_rset(state) != 0) {
	    lmtp_chat_reset(state);
	    state->session = lmtp_session_free(state->session);
	}

	/*
	 * Ready to go with another load. The reuse counter is logged for
	 * statistical analysis purposes. Given the Postfix architecture,
	 * connection cacheing makes sense only for dedicated transports.
	 * Logging the reuse count can help to convince people.
	 */
	else {
	    ++state->reuse;
	    if (msg_verbose)
		msg_info("%s: reusing (count %d) session with: %s",
			 myname, state->reuse, state->session->host);
	}
    }

    /*
     * See if we need to establish an LMTP connection.
     */
    if (state->session == 0) {

	/*
	 * Bounce or defer the recipients if no connection can be made.
	 */
	if ((state->session = lmtp_connect(destination, why)) == 0) {
	    lmtp_site_fail(state, lmtp_errno == LMTP_RETRY ? 450 : 550,
			   "%s", vstring_str(why));
	}

	/*
	 * Bounce or defer the recipients if the LMTP handshake fails.
	 */
	else if (lmtp_lhlo(state) != 0) {
	    state->session = lmtp_session_free(state->session);
	}

	/*
	 * Congratulations. We just established a new LMTP connection.
	 */
	else
	    state->reuse = 0;
    }

    /*
     * If a session exists, deliver this message to all requested recipients.
     */
    if (state->session != 0)
	lmtp_xfer(state);

    /*
     * Optionally, notify the postmaster of problems.
     */
    if (state->history != 0
    && (state->error_mask & name_mask(mail_error_masks, var_notify_classes)))
	lmtp_chat_notify(state);

    /*
     * Disconnect if we're not cacheing connections. The pipelined protocol
     * state machine knows to send QUIT as appropriate.
     */
    if (!var_lmtp_cache_conn && state->session != 0)
	state->session = lmtp_session_free(state->session);

    /*
     * Clean up.
     */
    vstring_free(why);
    result = state->status;
    lmtp_chat_reset(state);

    return (result);
}

/* lmtp_service - perform service for client */

static void lmtp_service(VSTREAM *client_stream, char *unused_service, char **argv)
{
    DELIVER_REQUEST *request;
    int     status;

    /*
     * This routine runs whenever a client connects to the UNIX-domain socket
     * dedicated to remote LMTP delivery service. What we see below is a
     * little protocol to (1) tell the queue manager that we are ready, (2)
     * read a request from the queue manager, and (3) report the completion
     * status of that request. All connection-management stuff is handled by
     * the common code in single_server.c.
     */
    if ((request = deliver_request_read(client_stream)) != 0) {
	status = deliver_message(request, argv);
	deliver_request_done(client_stream, request, status);
    }
}

/* post_init - post-jail initialization */

static void post_init(char *unused_name, char **argv)
{
    state = lmtp_state_alloc();
    get_service_attr(state, argv);
}

/* pre_init - pre-jail initialization */

static void pre_init(char *unused_name, char **unused_argv)
{
    debug_peer_init();
}

/* cleanup - close any open connections, etc. */

static void cleanup(void)
{
    if (state->session != 0) {
	lmtp_quit(state);
	lmtp_chat_reset(state);
	state->session = lmtp_session_free(state->session);
	if (msg_verbose)
	    msg_info("cleanup: just closed down session");
    }
    lmtp_state_free(state);
}

/* pre_accept - see if tables have changed */

static void pre_accept(char *unused_name, char **unused_argv)
{
    if (dict_changed()) {
	msg_info("table has changed -- exiting");
	cleanup();
	exit(0);
    }
}

/* main - pass control to the single-threaded skeleton */

int     main(int argc, char **argv)
{
    static CONFIG_STR_TABLE str_table[] = {
	VAR_DEBUG_PEER_LIST, DEF_DEBUG_PEER_LIST, &var_debug_peer_list, 0, 0,
	VAR_NOTIFY_CLASSES, DEF_NOTIFY_CLASSES, &var_notify_classes, 0, 0,
	VAR_ERROR_RCPT, DEF_ERROR_RCPT, &var_error_rcpt, 1, 0,
	0,
    };
    static CONFIG_INT_TABLE int_table[] = {
	VAR_LMTP_TCP_PORT, DEF_LMTP_TCP_PORT, &var_lmtp_tcp_port, 0, 0,
	VAR_LMTP_CONN_TMOUT, DEF_LMTP_CONN_TMOUT, &var_lmtp_conn_tmout, 0, 0,
	VAR_LMTP_RSET_TMOUT, DEF_LMTP_RSET_TMOUT, &var_lmtp_rset_tmout, 1, 0,
	VAR_LMTP_LHLO_TMOUT, DEF_LMTP_LHLO_TMOUT, &var_lmtp_lhlo_tmout, 1, 0,
	VAR_LMTP_MAIL_TMOUT, DEF_LMTP_MAIL_TMOUT, &var_lmtp_mail_tmout, 1, 0,
	VAR_LMTP_RCPT_TMOUT, DEF_LMTP_RCPT_TMOUT, &var_lmtp_rcpt_tmout, 1, 0,
	VAR_LMTP_DATA0_TMOUT, DEF_LMTP_DATA0_TMOUT, &var_lmtp_data0_tmout, 1, 0,
	VAR_LMTP_DATA1_TMOUT, DEF_LMTP_DATA1_TMOUT, &var_lmtp_data1_tmout, 1, 0,
	VAR_LMTP_DATA2_TMOUT, DEF_LMTP_DATA2_TMOUT, &var_lmtp_data2_tmout, 1, 0,
	VAR_LMTP_QUIT_TMOUT, DEF_LMTP_QUIT_TMOUT, &var_lmtp_quit_tmout, 1, 0,
	VAR_DEBUG_PEER_LEVEL, DEF_DEBUG_PEER_LEVEL, &var_debug_peer_level, 1, 0,
	0,
    };
    static CONFIG_BOOL_TABLE bool_table[] = {
	VAR_LMTP_CACHE_CONN, DEF_LMTP_CACHE_CONN, &var_lmtp_cache_conn,
	VAR_LMTP_SKIP_QUIT_RESP, DEF_LMTP_SKIP_QUIT_RESP, &var_lmtp_skip_quit_resp,
	0,
    };

    single_server_main(argc, argv, lmtp_service,
		       MAIL_SERVER_INT_TABLE, int_table,
		       MAIL_SERVER_STR_TABLE, str_table,
		       MAIL_SERVER_BOOL_TABLE, bool_table,
		       MAIL_SERVER_PRE_INIT, pre_init,
		       MAIL_SERVER_POST_INIT, post_init,
		       MAIL_SERVER_PRE_ACCEPT, pre_accept,
		       MAIL_SERVER_EXIT, cleanup,
		       0);
}
