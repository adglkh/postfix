/*++
/* NAME
/*	bounce_service 3h
/* SUMMARY
/*	bounce message service
/* SYNOPSIS
/*	#include <bounce_service.h>
/* DESCRIPTION
/* .nf

 /*
  * Utility library.
  */
#include <vstring.h>

 /*
  * Global library.
  */
#include <bounce_log.h>

 /*
  * bounce_append_service.c
  */
extern int bounce_append_service(int, char *, char *, RECIPIENT_VAR *, DSN_VAR *);

 /*
  * bounce_notify_service.c
  */
extern int bounce_notify_service(int, char *, char *, char *, char *, char *, char *, int);

 /*
  * bounce_warn_service.c
  */
extern int bounce_warn_service(int, char *, char *, char *, char *, char *, char *, int);

 /*
  * bounce_trace_service.c
  */
extern int bounce_trace_service(int, char *, char *, char *, char *, char *, char *, int);

 /*
  * bounce_notify_verp.c
  */
extern int bounce_notify_verp(int, char *, char *, char *, char *, char *, char *, int, char *);

 /*
  * bounce_one_service.c
  */
extern int bounce_one_service(int, char *, char *, char *, char *, char *, int, RECIPIENT *, DSN *);

 /*
  * bounce_cleanup.c
  */
extern VSTRING *bounce_cleanup_path;
extern void bounce_cleanup_register(char *, char *);
extern void bounce_cleanup_log(void);
extern void bounce_cleanup_unregister(void);

#define bounce_cleanup_registered() (bounce_cleanup_path != 0)

 /*
  * bounce_template.c
  */
typedef struct {
    const char *class;			/* template type */
    const char *charset;		/* character set */
    const char *encoding;		/* 7bit or 8bit */
    const char *from;			/* originator */
    const char *subject;		/* general subject line */
    const char *postmaster_subject;	/* postmaster subject line */
    const char **message_text;		/* message text */
} BOUNCE_TEMPLATE;

extern void bounce_template_load(const char *);
extern void bounce_template_expand(VSTREAM *, const BOUNCE_TEMPLATE *);
extern const BOUNCE_TEMPLATE *bounce_template_find(const char *, const BOUNCE_TEMPLATE *);
extern void bounce_template_dump_default(VSTREAM *);
extern void bounce_template_dump_actual(VSTREAM *);

#define BOUNCE_TEMPLATE_DICT	"bounce_templates"
#define BOUNCE_TMPL_DICT_FAIL	"fail_template"
#define BOUNCE_TMPL_DICT_DELAY	"delay_template"
#define BOUNCE_TMPL_DICT_SUCCESS "success_template"
#define BOUNCE_TMPL_DICT_VERIFY	"verify_template"

#define FAIL_TEMPLATE() \
    (bounce_fail_template ? bounce_fail_template : \
	(bounce_fail_template = \
	    bounce_template_find(BOUNCE_TMPL_DICT_FAIL, \
		&def_bounce_fail_template)))

#define DELAY_TEMPLATE() \
    (bounce_delay_template ? bounce_delay_template : \
	(bounce_delay_template = \
	    bounce_template_find(BOUNCE_TMPL_DICT_DELAY, \
		&def_bounce_delay_template)))

#define SUCCESS_TEMPLATE() \
    (bounce_success_template ? bounce_success_template : \
	(bounce_success_template = \
	    bounce_template_find(BOUNCE_TMPL_DICT_SUCCESS, \
		&def_bounce_success_template)))

#define VERIFY_TEMPLATE() \
    (bounce_verify_template ? bounce_verify_template : \
	(bounce_verify_template = \
	    bounce_template_find(BOUNCE_TMPL_DICT_VERIFY, \
		&def_bounce_verify_template)))

extern const BOUNCE_TEMPLATE *bounce_fail_template;
extern const BOUNCE_TEMPLATE *bounce_delay_template;
extern const BOUNCE_TEMPLATE *bounce_success_template;
extern const BOUNCE_TEMPLATE *bounce_verify_template;

extern const BOUNCE_TEMPLATE def_bounce_fail_template;
extern const BOUNCE_TEMPLATE def_bounce_delay_template;
extern const BOUNCE_TEMPLATE def_bounce_success_template;
extern const BOUNCE_TEMPLATE def_bounce_verify_template;

 /*
  * bounce_notify_util.c
  */
typedef struct {
    const char *service;		/* bounce or defer */
    const char *queue_name;		/* incoming, etc. */
    const char *queue_id;		/* base name */
    const char *mime_encoding;		/* null or encoding */
    const char *dsn_envid;		/* DSN envelope ID */
    const char *mime_boundary;		/* for MIME */
    const BOUNCE_TEMPLATE *template;	/* see above */
    VSTRING *buf;			/* scratch pad */
    VSTRING *sender;			/* envelope sender */
    VSTREAM *orig_fp;			/* open queue file */
    long    orig_offs;			/* start of content */
    time_t  arrival_time;		/* time of arrival */
    long    message_size;		/* size of content */
    BOUNCE_LOG *log_handle;		/* open logfile */
    char   *mail_name;			/* $mail_name, cooked */
} BOUNCE_INFO;

 /* */

extern BOUNCE_INFO *bounce_mail_init(const char *, const char *, const char *, const char *, const char *, const BOUNCE_TEMPLATE *);
extern BOUNCE_INFO *bounce_mail_one_init(const char *, const char *, const char *, const char *, RECIPIENT *, DSN *);
extern void bounce_mail_free(BOUNCE_INFO *);
extern int bounce_header(VSTREAM *, BOUNCE_INFO *, const char *);
extern int bounce_boilerplate(VSTREAM *, BOUNCE_INFO *);
extern int bounce_recipient_log(VSTREAM *, BOUNCE_INFO *);
extern int bounce_diagnostic_log(VSTREAM *, BOUNCE_INFO *, int);
extern int bounce_header_dsn(VSTREAM *, BOUNCE_INFO *);
extern int bounce_recipient_dsn(VSTREAM *, BOUNCE_INFO *);
extern int bounce_diagnostic_dsn(VSTREAM *, BOUNCE_INFO *, int);
extern int bounce_original(VSTREAM *, BOUNCE_INFO *, int);
extern void bounce_delrcpt(BOUNCE_INFO *);
extern void bounce_delrcpt_one(BOUNCE_INFO *);

/* LICENSE
/* .ad
/* .fi
/*	The Secure Mailer license must be distributed with this software.
/* AUTHOR(S)
/*	Wietse Venema
/*	IBM T.J. Watson Research
/*	P.O. Box 704
/*	Yorktown Heights, NY 10598, USA
/*--*/
