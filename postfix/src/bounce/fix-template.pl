print <<'EOF'
#
# Do not edit this file. This file shows the default delivery status
# notification (DSN) messages that are built into Postfix.
#
# To change Postfix DSN messages, perhaps to add non-English text,
# follow instructions in the BOUNCE_README document.
#
# QUICK INSTRUCTIONS:
#
# Copy this file to $config_directory/bounce.cf, edit that file,
# then specify in main.cf:
#
#	bounce_template_file = $config_directory/bounce.cf
#
#-The template file can specify bounce message templates for
# failed mail, delayed mail, successful delivery, and verbose delivery.
# You don't have to specify templates for all of these. If there is
# anything about a template that Postfix does not like it logs a
# warning and keeps using its built-in template.
#
#-Each template consists of a few headers and message text. The
# headers control what the recipient sees as From: and Subject:, and
# what MIME information Postfix will generate. The message text is
# not sent in Postmaster copies of delivery status notifications.
#
#-You can specify main.cf parameters in a template message text.
# Some parameters have additional features as described below.
#
#-You cannot specify main.cf parameters in template headers.
#
#-Each template starts with "template_name = <<EOF" and ends
# with a line that contains the word "EOF" only. You can change the
# word EOF if you like, but you can't do shell/perl/etc like things
# such as enclosing it in quotes (template_name = <<'EOF').
#
# IMPORTANT:
#
# If you add non-ASCII text then you MUST change the CHARSET: value,
# otherwise Postfix will not use your template. You must specify a
# character set that is a superset of US-ASCII, because Postfix
# appends ASCII text after the message template.
#
EOF
;

while (<>) {
    if (/^fail_template/) { print <<'EOF'

#
# The fail template is used when mail is returned to the sender;
# either the destination rejected the message, or the destination
# could not be reached before the message expired in the queue.
#

EOF
;
    } elsif (/^delay_template/) { print <<'EOF'

#
# The delay template is used when mail is delayed. Note a neat trick:
# the default template displays the delay_warning_time value as hours
# by appending the _hours suffix to the parameter name; it displays
# the maximal_queue_lifetime value as days by appending the _days
# suffix.
#
# Other suffixes are: _seconds, _minutes, _weeks. There are no other
# main.cf parameters that have this special behavior.
#
# You need to adjust these suffixes (and the surrounding text) if
# you have very different settings for these time parameters.
#

EOF
;
    } elsif (/^success_template/) { print <<'EOF'

#
# The success template is used when mail is delivered to mailbox,
# when an alias or list is expanded, or when mail is delivered to a
# system that does not announce DSN support. It is an error to specify
# a Postmaster-Subject: here.
#

EOF
;
    } elsif (/^verify_template/) { print <<'EOF'

#
# The verify template is used for address verification (sendmail -bv
# address...). or for verbose mail delivery (sendmail -v address...).
# It is an error to specify a Postmaster-Subject: here.
#

EOF
;
    }
    print $_;
}
