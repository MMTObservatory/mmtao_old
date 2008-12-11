# AOserver
#
# began as the Camelserver from hacksaw:/mmt/dataserver/mini.pm
#
# This is a socket server, based upon Lincoln Stein's
#    "Network Programming with Perl"
# (but wandering inexorably away from that code base)
#
# This server architecture is handles by 3 files, with 3 packages
#  AOserver (this file) - one instance per application.
#  AOsession - the "session_set" - one per server.
#  AObuffer - one per "session", where a session is either
#  	a gizmo/client connection or a network connection.
#

package AOserver;

use strict;
use warnings;
use DBI;
use RRDs;
use POSIX;

use IO::Socket;
#use IO::File;
use IO::Handle;
use IO::AOsession;

use JSON;

# This loads up the MINISERVER hash, which later gets copied into $self.
# It is expected to contain:
#    $self->{LOG_DIRECTORY} = $self->{MINISERVER}->{log_directory};
#    $self->{MYSQL_INTERVAL} = $self->{MINISERVER}->{mysql_interval};
#    $self->{MYSQL_HI_RES_INTERVAL} = $self->{MINISERVER}->{mysql_hi_res_interval};
#    $self->{UPDATE_INTERVAL} = $self->{MINISERVER}->{update_interval};
#    $self->{gizmo} = $self->{MINISERVER}->{gizmo};
#    $self->{MINISERVER}->{name};
#    $self->{MINISERVER}->{host} = $self->{PORTS}->{$name . "_HOST"};
#    $self->{MINISERVER}->{port} = $self->{PORTS}->{$name . "_PORT"};

sub hork_database {
    my ( $info, $name ) = @_;
    my $sql;
    my $statement;
    my $key;

    # we can only access databases on hacksaw
    my $whoami = (uname())[1];
    return undef unless $whoami =~ /hacksaw/;

    my $DBI =  DBI->connect('DBI:mysql:miniservers', 'mmtstaff', 'multiple');
    
    # --------------------------------
    # Get stuff from miniservers table.
    # --------------------------------

    $sql = "SELECT * FROM miniservers WHERE (name = '" . $name . "') LIMIT 1";
    $statement = $DBI->prepare($sql)
      or die "Couldn't prepare query '$sql': $DBI::errstr\n";
    $statement->execute()
      or die "Couldn't execute query '$sql': $DBI::errstr\n";
    
    # only one row possible (see LIMIT 1 above)
    # but still need this silly loop or disconnect() will complain.
    my $row_ref;
    while (my $row = $statement->fetchrow_hashref() )  {      
	$row_ref = $row;
    	#print "MINISERVER row " . $row->{name} . "\n";
    }
    return undef unless defined ( $row_ref );

    $info->{MINISERVER} = $row_ref;

    # Note: $name and $info->{MINISERVER}->{name} are (and MUST be)
    # the same, since that is how we fetched to row from the database.

    #print "MINIserver DB name = $info->{MINISERVER}->{name}\n";

    # --------------------------------
    # Get stuff from dataports table.
    # (and only what we actually need)
    # --------------------------------

    # As an example, for Temptrax, we get TEMPTRAX and GIZMO_TEMPTRAX
    $sql = "SELECT * FROM dataports WHERE ((name = '"  . $info->{MINISERVER}->{name}  . "') OR (name = '" . $info->{MINISERVER}->{gizmo}  . "'))";
    # $sql = "SELECT * FROM dataports WHERE (name = '"  . $info->{MINISERVER}->{name}  . "')";
    $statement = $DBI->prepare($sql)
      or die "Couldn't prepare query '$sql': $DBI::errstr\n";
    $statement->execute()
      or die "Couldn't execute query '$sql': $DBI::errstr\n";
    
    while (my $row_ref = $statement->fetchrow_hashref() )  {      
        $key = $row_ref->{'name'};
	#print "Dataports line for $key\n";

        # Checking the the gizmo for this miniserver is active.
	if ( $info->{MINISERVER}->{gizmo} eq $key ) {
            $info->{IS_ACTIVE} = $row_ref->{'is_active'};
            $info->{IS_LANTRONIX} = $row_ref->{'is_lantronix'};
        }

        $info->{PORTS}->{$key . "_HOST"} = $row_ref->{'host'};
        $info->{PORTS}->{$key . "_PORT"} = $row_ref->{'port'};
        $info->{PORTS}->{$key . "_IS_ACTIVE"} = $row_ref->{'is_active'};
        $info->{PORTS}->{$key . "_IS_LANTRONIX"} = $row_ref->{'is_lantronix'};
        $info->{PORTS}->{$key . "_NOTES"} = $row_ref->{'notes'};
        $info->{PORTS}->{$key . "_ID"} = $row_ref->{'id'};
    }

    # --------------------------------
    # Get stuff from parameters table.
    # --------------------------------

    # Get the most recent parameters entries for this miniserver
    # The table may have more than one entry for a parameter,
    # by specifying order by timestamp, we ensure that the most
    # recent entry will be fetched.

    $sql = "SELECT * FROM parameters WHERE (miniserver = '" . $name . "') ORDER BY `parameters`.`timestamp` ASC";
    $statement = $DBI->prepare($sql)
      or die "Couldn't prepare query '$sql': $DBI::errstr\n";
    $statement->execute()
      or die "Couldn't execute query '$sql': $DBI::errstr\n";
    
    while (my $row_ref = $statement->fetchrow_hashref() )  {     
        my $name = $row_ref->{name};
        $info->{PARAMETERS}->{$name} = $row_ref;
        $info->{$name} = $info->{PARAMETERS}->{$name}->{initial_value};
        
        if ($info->{PARAMETERS}->{$name}->{mysql_logging} eq '1') {
            push (@{ $info->{MYSQL_KEYS} },  $name); 
        }
        
        if ($info->{PARAMETERS}->{$name}->{mysql_hi_res_logging} eq '1') {
            push (@{ $info->{MYSQL_HI_RES_KEYS} },  $name); 
        }
        
        if ($info->{PARAMETERS}->{$name}->{rrd_logging} eq '1') {
            push (@{ $info->{RRD_KEYS} },  $name); 
            #  RRDTool variables typically have shortened names.
            push (@{ $info->{RRD_VARIABLES} },  $info->{PARAMETERS}->{$name}->{rrd_variable}); 
        }
    }

    # --------------------------------
    # Move some variables around.
    # --------------------------------

    $info->{LOG_DIRECTORY} = $info->{MINISERVER}->{log_directory};
    $info->{MYSQL_INTERVAL} = $info->{MINISERVER}->{mysql_interval};
    $info->{MYSQL_HI_RES_INTERVAL} = $info->{MINISERVER}->{mysql_hi_res_interval};
    $info->{UPDATE_INTERVAL} = $info->{MINISERVER}->{update_interval};

    $info->{PORT} = $info->{PORTS}->{$name . "_PORT"};
    $info->{MINISERVER}->{host} = $info->{PORTS}->{$name . "_HOST"};
    $info->{MINISERVER}->{port} = $info->{PORTS}->{$name . "_PORT"};

    $info->{gizmo_host} = $info->{PORTS}->{$info->{MINISERVER}->{gizmo} . "_HOST"};
    $info->{gizmo_port} = $info->{PORTS}->{$info->{MINISERVER}->{gizmo} . "_PORT"};

    $info->{MINISERVER}->{GIZMO_HOST} = $info->{gizmo_host};
    $info->{MINISERVER}->{GIZMO_PORT} = $info->{gizmo_port};

    if ( $info->{SERVER_DEBUG} ) {
	print "PORT: " . $info->{PORT} . "\n";
	print "HOST: " . $info->{MINISERVER}->{host} . "\n";
	print "PORT: " . $info->{MINISERVER}->{port} . "\n";
    
	print "G HOST: " . $info->{MINISERVER}->{GIZMO_HOST} . "\n";
	print "G PORT: " . $info->{MINISERVER}->{GIZMO_PORT} . "\n";
    }
    
    $DBI->disconnect();

    return 1;
}

sub new {
    my $class = shift;
    my $name = shift;
    
    my $self = {};
    bless $self, $class;
    
    # The new API demands a name argument here!
    die "You gotta give me a miniserver name!" unless defined ( $name );
    
    $self->{NAME} = $name;
    $self->{PREFIX} = lc ( $name ) . "_";
    
    # DEBUG is debug of the gizmo client connection.
    # SERVER_DEBUG will include all the connections to us as a server.
    $self->{DEBUG} = 0;
    $self->{SERVER_DEBUG} = 0;
    
    # Set this to accept special <STX><ETX> wrapped strings for the AO-TSS
    $self->{tssmode} = 0;
    
    $self->{session_set} = 0;

    $self->{client_session} = 0;
    $self->{client_func} = 0;
    
    # This is the hash of current values stored in each miniserver.
    # Anything placed here goes to the outside world via the all command.
    $self->{'data'} = {};
    
    $self->{server_count} = 0;
    
    $self->{HOOKS} = {};
    $self->{update_func} = 0;
    $self->{UPDATE_INTERVAL} = 1.0;
    $self->{UPDATE_TIME} = time();
    
    $self->{MYSQL_INTERVAL} = 0;
    $self->{MYSQL_TIME} = time();
    
    $self->{MYSQL_HI_RES_INTERVAL} = 0;
    $self->{MYSQL_HI_RES_TIME} = time();
    
    $self->{WAXED} = 0;
    
    $self->{always_reply} = 0;
    
    # Details for all of the dataports, including host:port.
    # Obtained from the MySQL miniservers:dataports table.
    $self->{PORTS} = {};
    
    # Details for all of the miniservers..
    # Obtained from the MySQL miniservers:miniservers table.
    $self->{MINISERVER} = {};
    
    # Details for all of the parameters for this server.
    # Obtained from the MySQL miniservers:parameters table.
    $self->{PARAMETERS} = {};
    
    # keys for all the parameters, used for MySQL logging.
    $self->{MYSQL_KEYS} = ();
    $self->{MYSQL_HI_RES_KEYS} = ();
    
    # miniserver parameters keys and RRDTool variable names for RRDTool logging.
    $self->{RRD_KEYS} = ();
    $self->{RRD_VARIABLES}  = ();
    
    # Boolean on whether the gizmo is active for this miniserver.
    # Default is true (== 1);
    $self->{IS_ACTIVE} = 1;
    
    # Boolean on whether the gizmo is connected via a Lantronxi unit.
    $self->{IS_LANTRONIX} = 0;

    # --------------------------------------
    # Get miniserver parameters from database.

    $self->hork_database ( $name );
    
    #print "NAME: " . $self->{NAME} . "\n";
    
    $self->{gizmo_sock} = 0;
    $self->{gizmo_session} = 0;
    $self->{gizmo_func} = 0;
    $self->{gizmo_status} = "RUN";
    $self->{gizmo_count} = 0;

    $self->{tss_func} = 0;
    
    # initial values for some stuff we always publish

    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
    $self->publish ( "restarted", sprintf ( "%4d-%02d-%02d %02d:%02d:%02d", 1900+$year, $mon+1, $mday,  $hour, $min, $sec ) );

    $self->publish ( "interval", $self->{UPDATE_INTERVAL} );
    $self->publish ( "unit_status", "OK" );
    $self->publish ( "server_status", "INIT" );
    
    ##return bless $self, $class;
    return $self;
}

sub recover_lantronix {
    my ( $self ) = @_;
    
    # Try to do a remote reboot of the dusttrak Lantronix.  This can work even if port 10001 is locked up.
    # However, port 9999, which is used here, can also be locked up so the reboot may not work.
    # You would have to manually reset the unit in that case.
    
    system('/mmt/bin/reset_lantronix', $self->{gizmo_host} );
}

sub gizmo_connect {
    my ($self,$type) = @_;
    my $session;
    
    return 1 if $self->{gizmo_sock};
    
    my $host = $self->{gizmo_host};
    my $port = $self->{gizmo_port};
    
    # Adding a parameter for the type of socket connection:  tcp or udp.
    my $proto = 'tcp';
    if (defined($type) && $type == 'udp' ) {
        $proto = 'udp';
    }
    
    my $sock = IO::Socket::INET->new (	PeerAddr => $host,
        PeerPort => $port,
        Proto => $proto,
        Timeout => 1.0 );
    
    unless ( $sock ) {
        if ( $self->{IS_LANTRONIX} ) {
            $self->recover_lantronix ();
            $self->{gizmo_status} ='REBOOT';
            $self->{gizmo_count} = 30 / $self->{UPDATE_INTERVAL};
            $self->publish ( "unit_status", "Rebooting" );
        } else {
            $self->publish ( "unit_status", "Dead" );
        }
        return;
    }
    
    print "Connection to $host on port $port\n" if $self->{DEBUG};
    
    die "No gizmo function specified" unless $self->{gizmo_func};
    die "No session yet to add gizmo to" unless $self->{'session_set'};
    
    $self->{gizmo_sock} = $sock;
    
    $session = $self->{'session_set'}->add ( $sock, "client (device)" );
    $session->debug () if $self->{DEBUG};
    $self->{gizmo_session} = $session;
    
    return 1;
}

sub gizmo_print {
    my ( $self, $msg ) = @_;
    my $sock = $self->{gizmo_sock};
    
    print $sock $msg;
}

sub add_gizmo_hook {
    my ( $self, $hook_sub ) = @_;
    
    $self->{gizmo_func} = $hook_sub;
}

# switch into the special binary mode the TSS server needs.
# This is now pretty much obsolete, given the next
# function (add_tss_hook), which sets the mode AND
# arranges a callback.

sub tssmode {
    my ($self, $mode) = @_;
    
    $self->{tssmode} = $mode;
    ##print "AOserver setting tssmode to $mode\n";
}

sub add_tss_hook {
    my ( $self, $hook_sub ) = @_;
    
    $self->{tssmode} = 1;
    $self->{tss_func} = $hook_sub;
}

# Keep this, the tss server needs it.
sub add_client {
    my ($self, $handle, $func) = @_;
    my $client_session;

    $self->{client_func} = $func;

    unless ( $self->{session_set} ) {
        die "client_add: no session yet"
    }

    $client_session = $self->{session_set}->add ( $handle, "client (device)" );

    $self->{client_session} = $client_session;

    return $client_session;
}

sub remove_client {
    my ($self, $session ) = @_;
    
    $self->{session_set}->delete ( $session );
}

#sub mysql_handler {
#  my ($self) = @_;
#}
#
#sub mysql_hi_res_handler {
#  my ($self) = @_;
#}

sub get_data_ref {
    my ($self) = @_;
    
    # this IS a hash reference.
    return $self->{data};
}

sub publish {
    my ($self, $var, $val) = @_;
    
    $self->{data}->{$self->{PREFIX} . $var} = $val;
}

# set this, and "get" will act like "get_eof", and we
# always get a reply, even if we ask for a variable that
# does not exist.
sub always_reply {
    my ( $self ) = @_;
    
    $self->{always_reply} = 1;
}

# Call this and you get the original behaviour for get,
# where get will be silent if you ask for an undefined variable.
# (Since this is now the default behavior, calling this makes
#  no sense, but someday the situation may invert).
sub deprecated_reply {
    my ( $self ) = @_;
    
    $self->{always_reply} = 0;
}

# Set the port (override any database info).
sub set_port {
    my ($self, $port) = @_;
    
    $self->{PORT} = $port;
}

# It is very unlikely this will be useful in the new scheme of things.
sub set_name {
    my ($self, $name) = @_;
    
    $self->{NAME} = $name;
    $self->{PREFIX} = lc ( $name ) . "_";
    
    # look up port number in the PORTS hash
    # now that we have a name for this thing.
    $self->{PORT} = $self->get_port ( $name );
}

sub server_debug {
    my ($self, $debug) = @_;
    
    $self->{SERVER_DEBUG} = $debug;
}

sub debug {
    my ($self, $debug) = @_;
    
    $self->{DEBUG} = $debug;
}

# Returns a Perl hash of the host and port values.
sub get_ports {
    my $self = shift;
    
    return $self->{PORTS};
}

# Returns a listing of the host and port values, terminated by ".EOF".
# Mainly for testing.
sub print_ports {
    my $self = shift;
    my $str = "";
    
    foreach my $port ( sort keys %{ $self->{PORTS} } ) {
        $str .= $port . " " . $self->{PORTS}->{$port} . "\n";
    }
    $str .= ".EOF\n";
    return $str;
}

# Returns the host or port for a miniserver.
sub lookup {
    my $self = shift;
    my $key = undef;
    
    $key = shift if @_;
    if ( defined( $key ) && defined ( $self->{PORTS}->{$key} ) ) {
        return $self->{PORTS}->{$key};
    } else {
        return undef;
    }
}

# Returns the miniserver host name
sub get_host {
    my $self = shift;
    my $name = undef;
    
    $name = shift if @_;
    if ( defined ($name)  ) {
        return $self->lookup( $name . "_HOST" );
    } else {
        return undef
    }
}

# Returns the miniserver port
sub get_port {
    my $self = shift;
    my $name = undef;
    
    $name = shift if @_;
    
    if ( defined ($name)  ) {
        return $self->lookup( $name . "_PORT" );
    } else {
        return undef;
    }
}

# Perl trim function to remove whitespace from the start and end of the string
sub trim {
    my $self = shift;
    my $string = "";
    
    $string = shift if @_;
    $string =~ s/^\s+//;
    $string =~ s/\s+$//;
    return $string;
}

# Left trim function to remove leading whitespace
# XXX - never used
sub ltrim {
    my $self = shift;
    my $string = "";
    
    $string = shift if @_;
    $string =~ s/^\s+//;
    return $string;
}

# Right trim function to remove trailing whitespace
# XXX - never used
sub rtrim {
    my $self = shift;
    my $string = "";
    
    $string = shift if @_;
    $string =~ s/\s+$//;
    return $string;
}

# Returns a datetime in MySQL format.

#sub mysql_datetime {
#  my $self = shift @_;
#  my $dt = DateTime->now;
#  $dt->set_time_zone( 'America/Phoenix' );
#  return DateTime::Format::MySQL->format_datetime( $dt );
#}

# XXX - the use POSIX, pulls in a megabyte or so of stuff!
#use POSIX qw(strftime);
#
#sub mysql_datetime {
#    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime();
#    my $fmt = "%Y-%m-%d %H:%M:%S";
#
#    return strftime($fmt, $sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst);
#}

# Returns a datetime in MySQL format.
# namely:  yyyy-mm-dd  hh:mm:ss

sub mysql_datetime {
    my $self = shift;
    my $time = undef;
    $time = shift if @_;
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst);
    
    if (defined($time)) {
        ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime($time);
    } else {
        ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
    }
    
    return sprintf ( "%4d-%02d-%02d %02d:%02d:%02d", 1900+$year, $mon+1, $mday,  $hour, $min, $sec );
}

# Not used at present, but we have ideas for it.
sub data2table {
    my $self = shift;
    my $data;
    
    $data = shift if @_;
    
    $data->{'webserver_time'} = Miniserver->mysql_datetime();
    $data->{'webserver_timestamp'} = time();
    
    my $html = "";
    $html .= "Content-type: text/html\n";
    # Required to prevent IE caching.
    $html .= "Cache-control: no-cache\n";
    $html .= "Pragma: no-cache\n\n";
    
    $html .= "<table border=\"1\" width=\"100%\" style=\"table-layout: fixed\"><col width=\"50%\"><col width=\"50%\">\n";
    $html .= "<tbody>\n";
    my $row = 1;
    
    foreach my $key ( sort keys %{ $data } ) {
        my $val = $data->{$key};
        unless ( defined( $val ) && length( $val ) > 0) {
            $val = "&nbsp;"
        }
        # Alternating the background color for rows in the HTML table.
        if ($row%2 == 0) {
            $html .= "<tr align=\"center\"><td>" . $key . "</td><td>" . $val . "</td></tr>\n";
        } else {
            $html .= "<tr align=\"center\" bgcolor=\"lightsteelblue\"><td>" . $key . "</td><td>" . $val . "</td></tr>\n";
        }
        $row++;
    }
    $html .= "</tbody>\n";
    $html .= "</table>\n";
    
    return $html;
}

# The main socket server subroutine!
# This is a multiplexing server, using Lincoln Stein's
# IO::LineBufferedSet module.
sub server_init {
    my ( $self ) = @_;
    my $port;
    my $rv;
    my $handle;
    
    # Adding MySQL hooks.
    my $mysql_interval = $self->{MINISERVER}->{mysql_interval};  # In seconds
    my $mysql_hi_res_interval = $self->{MINISERVER}->{mysql_hi_res_interval};  # In seconds
    
    ## BUG:  5:25PM  13Mar2007 JDG.
    #
    ## Undefined subroutine &main::
    ## called at /mmt/miniserver/IO/AOserver.pm line 526.
    #
    #
    ## Commenting out add_mysql_hook() for now...
    #if ( $mysql_interval > 0 ) {
        ## print "Adding mysql_handler with $mysql_interval interval\n";
        #$self->add_mysql_hook ( \&{ $self->mysql_handler }, $mysql_interval );
        #}
    #if ( $mysql_hi_res_interval > 0 ) {
        ## print "Adding mysql_hi_res_handler with $mysql_interval interval\n";
        #$self->add_mysql_hi_res_hook ( \&{ $self->mysql_hi_res_handler }, $mysql_hi_res_interval );
        #}
    
    $port = $self->{PORT};
    
    unless ( defined ( $port ) ) {
        die "No port specified for server to run on\n";
    }
    
    print "Starting a $self->{NAME} miniserver on port $port\n" if $self->{DEBUG};
    
    # Create a socket to listen to a port
    $handle = IO::Socket::INET->new(Proto => 'tcp',
        LocalPort => $port,
        Listen => 20,
        Timeout => 1,
        Reuse => 1) or die $!;
    
    unless ( $handle ) {
        die "Cannot start server on port $port\n";
    }
    
    $rv = IO::AOsession->new($handle);
    $rv->tssmode ( $self->{tssmode} );
    $self->{'session_set'} = $rv;
    
    print "Creating " . $self->{NAME} . " server.\n" if $self->{DEBUG};
    
    return $rv;
}

sub do_update {
    my ( $self ) = @_;
    
    $self->{server_count}++;
    $self->publish ( "count", $self->{server_count} );
    
    # print ("Update!!\n");
    
    if ( $self->{WAXED} ) {
        $self->{gizmo_status} = "WAXED";
        $self->publish ( "server_status", "WAXED" );
        $self->{gizmo_count} = 0;
        if ( $self->{gizmo_sock} ) {
            #print "OK, waxing ...\n";
            $self->{session_set}->delete ( $self->{gizmo_session} );
            $self->{gizmo_sock}->close ();
            undef $self->{gizmo_sock};
        }
        return;
    }
    
    if ( $self->{gizmo_status} eq 'WAXED' ) {
        #print "UNwaxing ...\n";
        $self->{gizmo_status} = 'IDLE';
        $self->publish ( "server_status", "IDLE" );
    }
    
    # We don't hammer it if it is rebooting.
    if ( $self->{gizmo_status} eq 'REBOOT' ) {
        print "Waiting on reboot\n" if $self->{DEBUG};
        $self->{gizmo_count}--;
        if ( $self->{gizmo_count} <= 0 ) {
            $self->{gizmo_status} = 'IDLE';
        }
        return;
    }
    
    if ( $self->{update_func} ) {
        &{$self->{update_func}}();
    }
}

sub main_loop {
    my ( $self ) = @_;
    
    my $timeout = 1.0;
    my $first = 1;
    my $now;
    my $delta;
    
    # Maybe this will free up some memory.
    # (it does, but not much, maybe 60kb)
    # undef $self->{PORTS};
    
    for ( ;; ) {
        
        # Blocking here for new connections.
        # Argument to wait is timeout
        # 0 argument means immediate timeout
        #  (essentially IO polling via select)
        # no argument means block till IO
        
        print "Call wait\n" if $self->{DEBUG};
        
        # wait is our wrapper on select.
        # here are some notes on timeout:
        #	undef = block forever (till some IO is possible).
        #	0 = polling, returns immediately no matter what.
        #	x = block with timeout, returns when IO is possible,
        #       or when timeout expires.  Note that if there is
        #	    a lot of IO, you may never see timeouts.
        
        # *****************
        # *****************
        
        ##    print "Calling wait with timeout: $timeout\n";
        
        my @ready = $self->{'session_set'}->wait ( $timeout );
        
        #    if ( $! ) {
        #    	warn ( "select error (not timeout): $!\n" );
        #    }
        
        # We don't care what or why, but as long as we are
        # here (for IO, or timeout, or an error), we will do
        # an update if it is time for one.
        
        # Check that the gizmo for this miniserver is active 
        # as per the dataports configuration table.
        if ( $self->{IS_ACTIVE} ) {
            $now = time ();
            $delta = $now - $self->{UPDATE_TIME};
            
            if ( $first or $delta > $self->{UPDATE_INTERVAL} ) {
                $self->{UPDATE_TIME} = $now;
                $self->do_update ();
            }
        } # end of is_active loop.
        
        undef $first;
        
        my $nr = @ready;
        print "wait has $nr handles ready\n" if $self->{DEBUG};
        ##    $self->{session_set}->debug ( "readable: $nr" );
        
        # Handle each socket that's ready
        for my $session (@ready) {
            my $buf;
            my $nio;
            my $debug;
            my $rv;
	    my $tss_flag;
            
            $debug = $self->{SERVER_DEBUG};
            if ( $self->{'gizmo_session'} ) {
                if ( $session eq $self->{'gizmo_session'} ) {
                    $debug = $self->{DEBUG};
                }
            }
            
            # read any available data.
            $session->do_read ();

	    for ( ;; ) {
		# See if a complete line has accumulated.
		$nio = $session->check_line ( $buf, $tss_flag );

		# This is the typical case, we have not assembled
		# a complete line yet.
		unless ( defined $nio ) {
		    # yields continual chatter in debug stream
		    # print "check_line ... not yet\n" if $debug;
		    last;
		}

		# This handles closed connections.
		if ( $nio == 0 ) {
		    print "connection or client gone, close\n" if $debug;
		    $session->close ();
		    undef $buf;
		    if ( $session eq $self->{gizmo_session} ) {
			undef $self->{gizmo_sock};
		    }
		    if ( $session eq $self->{client_session} ) {
			&{$self->{client_func}}( $buf );
		    }
		    last;
		}
            
		# This handles IO errors (unusual!)
		if ( $nio < 0 ) {
		    print "IO error: closing\n" if $debug;
		    $session->close ();
		    undef $buf;
		    if ( $session eq $self->{gizmo_session} ) {
			undef $self->{gizmo_sock};
		    }
		    if ( $session eq $self->{client_session} ) {
			&{$self->{client_func}}( $buf );
		    }
		    last;
		}

		# from here on, we are dealing with a fully assembled line.

		print "check_line returns count: $nio\n" if $debug;

		#print "check_line returns: $nio, tss_flag = $tss_flag: $buf\n";
            
		# If we are handling client data, and this is some
		# of that data, hand it to the client handler.
		#
		# NOTE: we really only should have one or the other, i.e.
		# a client session or a gizmo session,
		# they are flavors of the same thing!
		# (may get unified someday).
		# right now the client thing handles the TSS only
		# all other miniservers have "gizmo" clients.

		if ( $self->{client_session} ) {
		    if ( $session eq $self->{client_session} ) {
			#print "HANDING to client func: $buf\n";
			&{$self->{client_func}}( $buf );
			next;
		    }
		}
            
		if ( $self->{gizmo_session} ) {
		    if ( $session eq $self->{gizmo_session} ) {
			unless ( defined $buf ) {
			    print "Lost gizmo connection!\n" if $self->{DEBUG};
			    # $self->client_close ( $self->{gizmo_session} );
			    undef $self->{gizmo_sock};
			    next;
			}
			if ( $self->{gizmo_func} ) {
			    #print "HANDING to gizmo func: $buf\n";
			    $rv = &{$self->{gizmo_func}}( $buf );
			    $self->update_db() if $rv;
			}
			next;
		    }
		}
		
		# Any other IO going on must be coming from one of
		# the connections to the network socket.

		# If this is a binary thingie and we have a tss_hook
		# set, we pass the thingie to the hook and go our way.
		if ( $tss_flag && $self->{tss_func} ) {
		    #print "HANDING to tss func: $buf\n";
		    &{$self->{'tss_func'}}( $session, $buf );
		    next;
		}
		
		# run of the mill network commands end up here (most things).
		if ( $buf ) {
		    #print "HANDING to parse request: $buf\n";
		    # remove control-M's that we get in telnet sessions.
		    $buf =~ s/\cM//;
		    
		    print "process line: $buf \n" if $debug;
		    $self->parse_request ( $session, $buf );
		}
		
	    }	# end of check_line loop
        }	# end of session loop
        
    }	# end of infinite wait loop
    
}	# end for "sub main_loop"

sub update_db {
    my $self = shift @_;
    my $delta;
    my $now = time ();
    
    # print localtime() . ": update_db called\n";
    
    # Update the RRDTOOL everytime we get new data.
    $self->update_rrdtool();
    
    # Add MySQL and HI RES MySQL logging
    if ( $self->{MYSQL_INTERVAL} > 0 ) {
        $delta = $now - $self->{MYSQL_TIME};
        # if ( $delta > $self->{MYSQL_INTERVAL} - $self->{UPDATE_INTERVAL} ) {
        if ( $delta > $self->{MYSQL_INTERVAL} ) {
            # print  localtime() . ": update MySQL\n";
            $self->{MYSQL_TIME} = $now;
            $self->update_mysql($self->{NAME},
                $self->{MINISERVER}->{mysql_table}, 
			    $self->{MYSQL_KEYS}); 
        }
    }
    
    if ( $self->{MYSQL_HI_RES_INTERVAL} > 0 ) {
        $delta = $now - $self->{MYSQL_HI_RES_TIME};    
        # if ( $delta > $self->{MYSQL_HI_RES_INTERVAL} - $self->{UPDATE_INTERVAL} ) {
        if ( $delta > $self->{MYSQL_HI_RES_INTERVAL} ) {
            $self->{MYSQL_HI_RES_TIME} = $now;
            $self->update_mysql($self->{NAME}, 
                $self->{MINISERVER}->{mysql_hi_res_table}, 
                $self->{MYSQL_HI_RES_KEYS}); 
        }
    }
}

# for compatibility with the original miniserver API
sub server {
    my ($self, $name, $debug) = @_;
    
    $self->{DEBUG} = $debug;
    $self->{NAME} = $name;
    $self->{PORT} = $self->get_port ( $name );
    
    $self->server_init ();
    
    # Don't do this, it will go into a furious polling loop.
    # $self->main_loop ( 0 );
    
    $self->main_loop ();
}

sub get_table {
    my $self = shift @_;
    my $html;
    my $bg_color = "red";
    my $name = $self->{NAME};
    
    my $now = time();
    my $staleness = $self->{MINISERVER}->{staleness};
    my $interval = $self->{MINISERVER}->{update_interval};
    my $title = "Miniserver Name: " .  $name  . ", Interval: " . $interval . ", Staleness: " . $staleness;
    
    my $update = $self->{UPDATE_TIME}; 
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst);
    my $datetime = "NA";
    if ( defined($staleness) && defined($update) ) {
        ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime($update);
        $datetime = sprintf ( "%4d-%02d-%02d %02d:%02d:%02d", 1900+$year, $mon+1, $mday,  $hour, $min, $sec );
        if ( $now - $update  < $staleness ) {
            $bg_color = "limegreen";	
        }
    }
    
    my $header = "<big>Current " . $name . " Parameters</big><br />Displaying Data from " . $datetime;
    
    $html = "<table border=\"1\" width=\"100%\">\n";
    $html .= "<tbody><tr bgcolor=\"" . $bg_color . "\"><th colspan=\"3\" id=\"header\" title=\"" . $title . "\">" . $header . "</th></tr>\n";
    $html .= "<tr bgcolor=\"beige\"><th>Description</th><th>Parameter</th><th>Value</th></tr>\n";
    
    my $row = 1;
    # Alternate the background color for rows in the HTML table.
    foreach my $key (sort keys %{ $self->{'data'} } ) {
        if ($row%2 == 0) {
            $bg_color= "white";
        } else {
            $bg_color= "lightsteelblue";
        }
        $html .= "<tr align=\"center\" bgcolor=\"" . $bg_color . "\">"; 
        $title = "Notes: ";
        if ( defined $self->{PARAMETERS}->{$key}->{notes}  ) {
            $title .= $self->{PARAMETERS}->{$key}->{notes};
        } else {
            $title .= "NA";
        }
        if ( defined $self->{PARAMETERS}->{$key}->{descriptor}  ) {
            if (length($self->{PARAMETERS}->{$key}->{units}) > 0) {
                $html .= "<td class=\"descriptor\" title=\"" . $title . "\">" . $self->{PARAMETERS}->{$key}->{descriptor} . " (" . $self->{PARAMETERS}->{$key}->{units}  . ")</td>";
            } else {
                $html .= "<td class=\"descriptor\" title=\"" . $title . "\">" . $self->{PARAMETERS}->{$key}->{descriptor}  . "</td>";         
            }
        } else {
            if ( $key eq "webserver_timestamp" ) {
                $html .= "<td class=\"descriptor\" title=\"" . $title . "\">Webserver Timestamp</td>";	
            } elsif ( $key eq "webserver_time" ) {
                $html .= "<td class=\"descriptor\" title=\"" . $title . "\">Webserver Date/Time</td>";	
            } else {
                $html .= "<td class=\"descriptor\" title=\"" . $title . "\">NA</td>";
            }	
        }
        $title = "Initial Value: ";
        if ( defined $self->{PARAMETERS}->{$key}->{initial_value}  ) {
            $title .= $self->{PARAMETERS}->{$key}->{initial_value};
        } else {
            $title .= "NA";
        }
        $title .= ", Max: ";
        if ( defined $self->{PARAMETERS}->{$key}->{max}  ) {
            $title .= $self->{PARAMETERS}->{$key}->{max};
        } else {
            $title .= "NA";
        }
        $title .= ", Min: ";
        if ( defined $self->{PARAMETERS}->{$key}->{min}  ) {
            $title .= $self->{PARAMETERS}->{$key}->{min};
        } else {
            $title .= "NA";
        }
        $title .= ", Is Active: ";
        if ( defined $self->{PARAMETERS}->{$key}->{is_active}  ) {
            $title .= $self->{PARAMETERS}->{$key}->{is_active};
        } else {
            $title .= "NA";
        }
        $html .= "<td  class=\"name\" title=\"" . $title . "\">" . $key . "</td>";
        $title = "Units: ";
        if ( defined $self->{PARAMETERS}->{$key}->{units}  ) {
            $title .= $self->{PARAMETERS}->{$key}->{units};
        } else {
            $title .= "NA";
        }
        $html .= "<td class=\"value\" title=\"" . $title . "\" id=\"" . $key  . "\">" . $self->{'data'}->{$key} . "</td>";
        $html .= "</tr>\n";
        $row++;
    }
    
    $html .= "</tbody>\n";
    $html .= "</table>\n";
    
    return $html;
}

# This subroutine returns the current data values as HTML text.
sub get_html {
    my $self = shift @_;
    my $name = $self->{NAME};
    # my $html= "<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">" . "\n";
    my $html .= "<html>\n";
    $html .= "<head>\n";
    $html .= "<title>Current " . $name . " Parameters</title>\n";
    $html .= "<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\">\n";
    $html .= "<meta http-equiv=\"refresh\" content=\"10\">\n";
    $html .= "</head>\n";
    $html .= "<body>\n";
    $html .= $self->get_table();
    $html .= "</body>\n";
    $html .= "</html>\n";
    
    return $html
}

# This subroutine returns the current data values as HTML text.
sub get_ajax {
    my $self = shift @_;
    my $name = $self->{NAME};
    my $html= '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">';
    # my $html .= "\n<html>\n";
    $html .= "\n<head>\n";
    $html .= "<title>Current " . $name . " Parameters</title>\n";
    $html .= "<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\"></meta>\n";
    $html .= "<script type=\"text/javascript\" src=\"/src/js/ajaxObject.js\"></script>\n";
    $html .= "<script type=\"text/javascript\" src=\"/src/js/DOMNode.js\"></script>\n";
    $html .= "<script type=\"text/javascript\" src=\"/src/js/AOserver_ajax.js\"></script>\n";
    $html .= "</head>\n";
    $html .= "<body onload=\"start()\" onunload=\"stop()\">\n";
    $html .= $self->get_table();
    $html .= "</body>\n";
    $html .= "</html>\n";
    
    return $html
}

sub return_http {
	my ( $self, $session, $payload ) = @_;
	my $headers;
	my $length;
    
	$length = length ( $payload );
    
	$headers = "HTTP/1.1 200 OK\r\n";
	$headers .= "Server: MMT miniserver\r\n";
	$headers .= "Content-Length: $length\r\n";
	$headers .= "Connection: close\r\n";
	$headers .= "Content-Type: text/html; charset=UTF-8\r\n";
	$headers .= "\r\n";
    
    $session->write($headers);
    $session->write($payload);
}

sub update_times {
    my ($self) = @_;
    
    $self->{'data'}->{'webserver_timestamp'} = time();
    $self->{'data'}->{'webserver_time'} = $self->mysql_datetime();
}

sub parse_request {
    my ($self, $session, $line) = @_;
    my $payload;
    
    # Just do this once right here.
    $self->update_times();
    
    # Looks like an http GET request
    # This typically is something like:
    # GET /json HTTP/1.1
    # Host: hacksaw:7696
    # ....
    
    
    if ( $line =~ /^GET.*HTTP/ ) {
        my ($get, $req, $proto) = split ( /\s+/, $line );
        
        # Gobble up rest of header lines.
        #	for ( ;; ) {
        #	    my $bytes = $session->getline( $line );
        #	    return unless $bytes > 0;
        #	    return if $line eq "\r\n";
        #	}
        if ( $req =~ /\/json_keys/ ) {
            if ( $req =~ /keys=(\S+)/ ) {
                $payload = $self->local_json_keys ( $1, $self->{data} );
            } else {
                $payload = JSON::to_json ( $self->{data} ) . "\n";
            }
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/json/ ) {
            $payload = JSON::to_json ( $self->{data} ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/all/ ) {
            $payload = $self->hash_join ( $self->{data}, " ", "\n" ) . "\n.EOF\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/descriptors/ ) {  ##
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{descriptor}) &&
                    length($self->{PARAMETERS}->{$key}->{descriptor}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{descriptor};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/units/ ) {  ##
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{units}) &&
                    length($self->{PARAMETERS}->{$key}->{units}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{units};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/initial_values/ ) {  ## Initial values.
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{initial_value}) &&
                    length($self->{PARAMETERS}->{$key}->{initial_value}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{initial_value};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/maxs/ ) {  ## Allowed maximum values.
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{max}) &&
                    length($self->{PARAMETERS}->{$key}->{max}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{max};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/mins/ ) {  ## Allowed minimum values.
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{min}) &&
                    length($self->{PARAMETERS}->{$key}->{min}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{min};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/is_actives/ ) {  ## Allowed minimum values.
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{is_active}) &&
                    length($self->{PARAMETERS}->{$key}->{is_active}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{is_active};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/notes/ ) {  ## Notes
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{notes}) &&
                    length($self->{PARAMETERS}->{$key}->{notes}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{notes};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/names/ ) {  ## Notes
            my %hash;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{name}) &&
                    length($self->{PARAMETERS}->{$key}->{name}) > 0  ) {
                        $hash{$key} = $self->{PARAMETERS}->{$key}->{name};
                    }
            }
            $payload = JSON::to_json ( \%hash ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/ports/ ) {  
            $payload = JSON::to_json ( $self->{PORTS} ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/miniserver/ ) {  ##
            $payload = JSON::to_json ( $self->{MINISERVER} ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/parameters/ ) {  ##
            my %big_hash;
            my %descriptors;
            # Adding descriptors for parameters
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{descriptor}) &&
                    length($self->{PARAMETERS}->{$key}->{descriptor}) > 0  ) {
                        $descriptors{$key} = $self->{PARAMETERS}->{$key}->{descriptor};
                    }
            }
            $big_hash{descriptors} = \%descriptors;
            
            # Adding units for parameters
            my %units;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{units}) &&
                    length($self->{PARAMETERS}->{$key}->{units}) > 0  ) {
                        $units{$key} = $self->{PARAMETERS}->{$key}->{units};
                    }
            }
            $big_hash{units} = \%units;
            
            # Adding initial values for parameters
            my %initial_values;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{initial_value}) &&
                    length($self->{PARAMETERS}->{$key}->{initial_value}) > 0  ) {
                        $initial_values{$key} = $self->{PARAMETERS}->{$key}->{initial_value};
                    }
            }
            $big_hash{initial_values} = \%initial_values;
            
            # Adding maximum allowed values for parameters
            my %maxs;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{max}) &&
                    length($self->{PARAMETERS}->{$key}->{max}) > 0  ) {
                        $maxs{$key} = $self->{PARAMETERS}->{$key}->{max};
                    }
            }
            $big_hash{maxs} = \%maxs;
            
            # Adding minimum allowed values for parameters
            my %mins;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{min}) &&
                    length($self->{PARAMETERS}->{$key}->{min}) > 0  ) {
                        $mins{$key} = $self->{PARAMETERS}->{$key}->{min};
                    }
            }
            $big_hash{mins} = \%mins;
            
            # Adding "active" status for parameters
            my %is_actives;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{is_active}) &&
                    length($self->{PARAMETERS}->{$key}->{is_active}) > 0  ) {
                        $is_actives{$key} = $self->{PARAMETERS}->{$key}->{is_active};
                    }
            }
            $big_hash{is_actives} = \%is_actives;
            
            # Adding notes for parameter 
            my %notes;
            foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
                if ( defined($self->{PARAMETERS}->{$key}->{notes}) &&
                    length($self->{PARAMETERS}->{$key}->{notes}) > 0  ) {
                        $notes{$key} = $self->{PARAMETERS}->{$key}->{notes};
                    }
            }
            $big_hash{notes} = \%notes;
            
            $big_hash{ports} = $self->{PORTS};
            
            $big_hash{miniserver} = $self->{MINISERVER};
            
            # Finally, a copy of the current data.
            $big_hash{data} = $self->{data};
            $payload = JSON::to_json( \%big_hash) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/e4x/ ) {
            $payload =  $self->e4x_all ();
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/xml/ ) {
            $payload =  $self->xml_all ();
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/cgi/ ) {
            $payload = $self->hash_join ( $self->{data}, "=", "&" ) . "\n";
            $self->return_http ( $session, $payload );
        } elsif ( $req =~ /\/html/) {
            $payload = $self->get_html ();
            $self->return_http ( $session, $payload );
        } else {	  # / or /index* or /ajax or ...
            $payload = $self->get_ajax ();
            $self->return_http ( $session, $payload );
        }
        
        # Close the connection, this may annoy some
        # browsers that want a persistent connection
        # or cause additional overhead.  If so, we need
        # to skip lines to the blank line and keep the
        # connection open, as in the snippet that follows:
        
        # $session->close ();
        return;
    }
    
    # These are the variables passed to the handle_request subroutine.
    my $arg1 = "";
    my $arg2 = "";
    my $arg3 = "";
    
    print "AOserver Got line: $line\n" if $self->{DEBUG};
    
    # Process the user input line here via handle_request().
    # This allows $arg3 to have spaces, such as for a date/time.
    my @split = ( split /\s+/, $line, 3 );
    if ( defined( $split[0] ) && length( $split[0]) > 0) { 
        $arg1 = $split[0];
    } else {
        $arg1 = "";
    }
    
    if ( defined( $split[1] ) && length( $split[1]) > 0) { 
        $arg2 = $split[1];
    } else {
        $arg2 = "";
    }
    
    if (defined( $split[2] ) && length( $split[2]) > 0) { 
        $arg3 = $split[2];
        chomp($arg3);
        $arg3 = $self->trim( $arg3 );
    } else {
        $arg3 = "";
    }
    
    if ( $self->{DEBUG} ) {
        print "\nargs = " . $arg1 . " " . $arg2 . " " . $arg3 if $self->{DEBUG}; 
    }
    
    my $reply = $self->handle_request( $arg1, $arg2, $arg3 ); 
    
    # Write response back to client, if appropriate.  
    # Non-blocking IO is handled!
    
    $session->write($reply) if length($reply) > 0;
}

sub lookup_data {
    my ( $self, $key ) = @_;
    
    return unless defined($key);
    
    if ( $key =~ /.*\d\d_\d\d$/ ) {
        if ( exists($self->{log}->{$key}) && defined($self->{log}->{$key}) ) {
            return $self->{log}->{$key};
        }
    } else {
        if ( exists($self->{data}->{$key}) && defined($self->{data}->{$key}) ) {
            return $self->{data}->{$key};
        }
    }
    return;
}

# This is common code for the cgi, all, and ... protocols.
sub hash_join {
    my ( $self, $data, $sep1, $sep2 ) = @_;
    my $first = 1;
    my $rv = "";
    
    foreach my $key (sort keys %{$data} ) {
        next unless exists ( $data->{$key} );
        $rv .= $sep2 unless $first;
        undef $first;
        $rv .= $key . $sep1 . $data->{$key};
    }
    return $rv;
}

# as above, but limit to a subset of keys.
sub hash_join_keys {
    my ( $self, $keys, $data, $sep1, $sep2 ) = @_;
    my $first = 1;
    my $rv = "";
    
    foreach my $key ( split( ',' , $keys) ) {
        next unless exists ( $data->{$key} );
        $rv .= $sep2 unless $first;
        undef $first;
        $rv .= $key . $sep1 . $data->{$key};
    }
    return $rv;
}


## Using JSON::to_json() instead to handle all the HTML
## special characters.
##
# sub local_json_all {
#    my ( $self, $data ) = @_;
#    my ( $key, $val, $rv );
#    my $first = 1;
#
#    return "{}\n" unless defined ( $data );
#
#    $rv = "{";
#    foreach $key (sort keys %{$data} ) {
#        $val = $data->{$key};
#	$rv .= "," unless $first;
#	undef $first;
##	$rv .= '"' . $key . '":"' . $val . '"';
#	unless ( $val =~ /^\d*\.?\d*$/ ) {
#	    $val = '"' . $val . '"';
#	}
#	$rv .= '"' . $key . '":' . $val;
#    }
#
#    $rv .= "}\n";
#    return $rv;
# }

sub local_json_keys {
    my ( $self, $keys, $data ) = @_;
    
    return "{}\n" unless defined ( $data );
    return "{}\n" unless defined ( $keys );
    
    my %hash;
    foreach my $key ( split( ',' , $keys) ) {
        next unless exists ( $data->{$key} );
        $hash{$key} = $data->{$key};
    }
    
    return JSON::to_json ( \%hash ) . "\n";
}

sub xml_all {
    my $self = shift @_;
    
    my $now = time();
    my $date = $self->mysql_datetime();
    
    my $reply =  "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n";
    
    $reply .=  "<entries>\n";
    $reply .=  "    <entry timestamp=\"$now\" date=\"$date\"";
    
    foreach my $key (sort keys %{  $self->{'data'} } ) {
        $reply .= " " . $key . "=\"" . $self->{'data'}->{$key} . "\"";
    }
    $reply .=  " />\n";
    $reply .=  "</entries>\n";
    
    return $reply;
}

sub e4x_all {
    my $self = shift @_;
    
    my $now = time();
    my $date = $self->mysql_datetime();
    
    # my $reply =  "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n";
    my $reply =  "";
    $reply .=  "<data>";
    foreach my $key (sort keys %{  $self->{'data'} } ) {
        $reply .= "<" . $key . ">" . $self->{'data'}->{$key} . "<\/" . $key . ">";
    }
    $reply .=  "</data>\n";
    
    return $reply;
}

sub handle_request {
    my ($self, $arg1, $arg2, $arg3) = @_;
    my ($key, $val);
    
    my $reply = "";
    my $first;
    
    return $reply unless length($arg1) > 0;
    
    # Check current data for max, min, and staleness.
    while ( my ($index, $value) = each(%{ $self->{data} }) ) {
        my $max = $self->{PARAMETERS}->{$index}->{max};
        if (defined($max) && $value ne 'NA') {
            # '-999.9' is a "magic" number to not evaluate max values.
            if ( $max  != -999.9 && $value > $max ) {
                $self->{data}->{$index} = "NA";
            }
        }
        my $min = $self->{PARAMETERS}->{$index}->{min};
        if (defined($min)  && $value ne 'NA') {
            # '-999.9' is a "magic" number to not evaluate min values.
            if ( $min  != -999.9 && $value < $min ) {
                $self->{data}->{$index} = "NA";
            }
        }
        my $now = time();
        my $staleness = $self->{MINISERVER}->{staleness};
        my $update = $self->{UPDATE_TIME}; 
        if ( defined($staleness)  && defined($update)  && $value ne 'NA') {
            # Don't overwrite "update", "time", or "timestamp" variables 
            # for stale data so that we know how stale the data is.
            if ( $now - $update  > $staleness && 
                $index !~ /_update$/ &&
                $index !~ /_timestamp$/ && 
                $index !~ /_time$/ ) {
                    $self->{data}->{$index} = "NA";      
                }
        }
    }
    
    if ( $arg1 eq "all" ) {
        
        $reply = $self->hash_join ( $self->{data}, " ", "\n" ) . "\n.EOF\n";
        
    } elsif ($arg1 eq "all_keys") {
        
        $reply = $self->hash_join_keys ( $arg2, $self->{data}, " ", "\n" ) . "\n.EOF\n";
        
    } elsif ($arg1 eq "parameters") {
        
        my %big_hash;
        my %descriptors;
        # Adding descriptors for parameters
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{descriptor}) &&
                length($self->{PARAMETERS}->{$key}->{descriptor}) > 0  ) {
                    $descriptors{$key} = $self->{PARAMETERS}->{$key}->{descriptor};
                }
        }
        $big_hash{descriptors} = \%descriptors;
        
        # Adding units for parameters
        my %units;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{units}) &&
                length($self->{PARAMETERS}->{$key}->{units}) > 0  ) {
                    $units{$key} = $self->{PARAMETERS}->{$key}->{units};
                }
        }
        $big_hash{units} = \%units;
        
        # Adding initial values for parameters
        my %initial_values;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{initial_value}) &&
                length($self->{PARAMETERS}->{$key}->{initial_value}) > 0  ) {
                    $initial_values{$key} = $self->{PARAMETERS}->{$key}->{initial_value};
                }
        }
        $big_hash{initial_values} = \%initial_values;
        
        # Adding maximum allowed values for parameters
        my %maxs;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{max}) &&
                length($self->{PARAMETERS}->{$key}->{max}) > 0  ) {
                    $maxs{$key} = $self->{PARAMETERS}->{$key}->{max};
                }
        }
        $big_hash{maxs} = \%maxs;
        
        # Adding minimum allowed values for parameters
        my %mins;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{min}) &&
                length($self->{PARAMETERS}->{$key}->{min}) > 0  ) {
                    $mins{$key} = $self->{PARAMETERS}->{$key}->{min};
                }
        }
        $big_hash{mins} = \%mins;
        
        # Adding "active" status for parameters
        my %is_actives;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{is_active}) &&
                length($self->{PARAMETERS}->{$key}->{is_active}) > 0  ) {
                    $is_actives{$key} = $self->{PARAMETERS}->{$key}->{is_active};
                }
        }
        $big_hash{is_actives} = \%is_actives;
        
        # Adding notes for parameter 
        my %notes;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{notes}) &&
                length($self->{PARAMETERS}->{$key}->{notes}) > 0  ) {
                    $notes{$key} = $self->{PARAMETERS}->{$key}->{notes};
                }
        }
        $big_hash{notes} = \%notes;
        
        $big_hash{ports} = $self->{PORTS};
        
        $big_hash{miniserver} = $self->{MINISERVER};
        
        # Finally, a copy of the current data.
        $big_hash{data} = $self->{data};
        
        $reply =  JSON::to_json( \%big_hash) . "\n";
        
        # Adding an option to get the descriptors for all variables.
    } elsif ($arg1 eq "descriptors") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{descriptor}) &&
                length($self->{PARAMETERS}->{$key}->{descriptor}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{descriptor};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "units") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{units}) &&
                length($self->{PARAMETERS}->{$key}->{units}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{units};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "initial_values") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{initial_value}) &&
                length($self->{PARAMETERS}->{$key}->{initial_value}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{initial_value};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "maxs") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{max}) &&
                length($self->{PARAMETERS}->{$key}->{max}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{max};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "mins") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{min}) &&
                length($self->{PARAMETERS}->{$key}->{min}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{min};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "is_actives") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{is_active}) &&
                length($self->{PARAMETERS}->{$key}->{is_active}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{is_active};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "notes") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{notes}) &&
                length($self->{PARAMETERS}->{$key}->{notes}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{notes};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "names") {
        my %hash;
        foreach my $key (sort keys %{$self->{PARAMETERS}} ) {
            if ( defined($self->{PARAMETERS}->{$key}->{name}) &&
                length($self->{PARAMETERS}->{$key}->{name}) > 0  ) {
                    $hash{$key} = $self->{PARAMETERS}->{$key}->{name};
                }
        }
        $reply = JSON::to_json ( \%hash ) . "\n";
        
    } elsif ($arg1 eq "ports") {    
        $reply = JSON::to_json ( $self->{PORTS} ) . "\n";
        
    } elsif ($arg1 eq "miniserver") {
        $reply = JSON::to_json ( $self->{MINISERVER} ). "\n";
        
        # return all keys from miniserver in the "CGI" format.
    } elsif ($arg1 eq "cgi") {
        
        $reply = $self->hash_join ( $self->{data}, "=", "&" ) . "\n";
        
        # return some keys from miniserver in the "CGI" format.
    } elsif ($arg1 eq "cgi_keys") {
        
        $reply = $self->hash_join_keys ( $arg2, $self->{data}, "=", "&" ) . "\n";
        
        # like the all command, but JSON format.
    } elsif ($arg1 eq "json") {
        
        $reply = JSON::to_json ( $self->{data} ) . "\n";
        
        # As above, but a select set of keys.
    } elsif ($arg1 eq "json_keys") {
        $reply = $self->local_json_keys ( $arg2, $self->{data} );
        
     } elsif ($arg1 eq "e4x") {
        $reply .=  $self->e4x_all ();
        
    } elsif ($arg1 eq "xml") {
        $reply .=  $self->xml_all ();
        
    } elsif ($arg1 eq "xml_all") {
        $reply .=  $self->xml_all ();
        
    } elsif ($arg1 eq "log") { # GONE
        $reply .= ".EOF\n";
        
    } elsif ($arg1 eq "xml_log") { # GONE
        # Nada - log is gone.
        
    } elsif ($arg1 eq "xml_all_log") { # GONE
        # Nada - log is gone.
        
        # These are used to shutdown a gizmo connection
        # to allow some other thing (like telnet) to
        # access the gizmo.
    } elsif ($arg1 eq "wax") {
        if ( $arg2 eq "off" ) {
            print "Wax off\n";
            $self->{WAXED} = 1;
        } else {
            print "Wax on\n";
            $self->{WAXED} = 0;
        }
        $reply .= "OK\n";
        
    } elsif ($arg1 eq "set") {
        if ( defined ($arg3) && ( length ( $arg3 ) > 0) ) {
            # Writing (or overwriting) a hash entry.
            $self->{'data'}->{$arg2} = $arg3;
        } else {
            # Deleting if $arg3 is empty string..
            delete( $self->{'data'}->{$arg2} ) if defined ( $arg2);
        }
        
    } elsif ($arg1 eq "set_eof") {
        if ( defined ($arg3) && ( length ( $arg3 ) > 0) ) {
            $self->{'data'}->{$arg2} = $arg3;
        } else {
            # Deleting if not $arg3.
            delete ( $self->{'data'}->{$arg2} ) if defined ( $arg2);
        }
        # $reply = $data{$arg2} . "\n";
        $reply .= ".EOF\n";
        
        # tjt  2-12-2008
        # New flavor of get, that always replies.
    } elsif ($arg1 eq "get") {
        $val = $self->lookup_data ( $arg2 );
        if ( $val ) { 
            $reply = $val . "\n";
        } else {
            $reply = "ERR\n" if $self->{always_reply};
        }
        
    } elsif ($arg1 eq "get_eof") {
        $val = $self->lookup_data ( $arg2 );
        $reply = $val . "\n" if $val;
        $reply .= ".EOF\n";
        
    } elsif ($arg1 eq "ajax") {
        $reply = $self->get_ajax ();
        
    } elsif ($arg1 eq "html") {
        $reply = $self->get_html ();
        
    } elsif ($arg1 eq "table") {
        $reply = $self->get_table();
        
    } elsif ($arg1 eq "status") {
        # stuff for debugging status of miniserver.
        $reply .= "Sorry\n";
        $reply .= ".EOF\n";
        
    } else {
        $reply = $self->check_hooks($arg1,$arg2,$arg3);
    }
    
    return $reply;
}

# check for runtime hooks, and call the handlers if set.
sub check_hooks {
    my ( $self, @cmd ) = @_;
    my $hook;
    
    foreach $hook ( sort keys %{$self->{HOOKS}} ) {
        if ( $cmd[0] eq $hook ) {
            return &{$self->{HOOKS}->{$hook}}(@cmd);
        }
    }
    
    return ".ERR\n";
}

# set up a runtime hook for some special command to
# be handled by the server.
sub add_hook {
    my ( $self, $hook_name, $hook_sub ) = @_;
    
    $self->{HOOKS}->{$hook_name} = $hook_sub;
}

sub set_update_interval {
    my ( $self, $interval ) = @_;

    $self->{UPDATE_INTERVAL} = $interval;
    $self->publish ( "interval", $interval );
}

# Specify an update handler and interval.
# Typically the interval is NOT specified, as
# it will be obtained from the miniserver database,
# but if it is specified this will override prior
# settings.

sub add_update_hook {
    my ( $self, $hook_sub, $interval ) = @_;
    
    $self->{update_func} = $hook_sub;
    if ( defined ( $interval ) ) {
    	$self->set_update_interval ( $interval );
    }
}

sub update_rrdtool {
    my ( $self ) = @_;
    
    return unless defined $self->{RRD_KEYS};
    
    my @keys = @{ $self->{RRD_KEYS} };
    my $size = scalar @keys;
    
    if (  $size > 0 ) {
        my @vars = @{ $self->{RRD_VARIABLES} };
        my %status = %{ $self->{'data'} };
        my $str1 = $status{$keys[0]};
        my $str2 = $vars[0];
        for ( my $i = 1;$i < $size; $i++ ) {
            if (defined($status{$keys[$i]})) {
                $str1 .= ":" . $status{$keys[$i]};
            } else {
                $str1 .= ":0";                
            }
            $str2 .= ":" . $vars[$i];
        }
        
        # print "str1 = $str1\n";
        # print "str2 = $str2\n";
        
        #    RRDs::update ("/mmt/thermal/rrd/dusttrak.rrd",
        #	"--template=dusttrak_conc:dusttrak_accum", "N:$conc:$accum");
        RRDs::update ($self->{MINISERVER}->{rrd_file},
            "--template=" . $str2,
            "N:" . $str1);
        my $ERR=RRDs::error;
        print localtime() . ": ERROR while updating dusttrak.rrd: $ERR\n" if $ERR; 
    }
}

# $mini->update_mysql('CARRIER', "carrier_background_log", \@keys );
sub update_mysql {
    my ( $self, $name, $table, $keys ) = @_;
    
    # Dereference data hash.
    my %status = %{ $self->{'data'} };
    # print $keys . "\n";
    # Adding check that $keys is defined.  JDG 07-Jul-2008
    if ( %status && $keys ) {
        my $mysql =  DBI->connect('DBI:mysql:mmtlogs', 'mmtstaff', 'multiple');
        my $query = "INSERT INTO $table ( ";
        my $first = 1;
        
        foreach ( sort @$keys ) {
            if ($first) {
                $query .= $_;
                $first = 0;
            } else {
                $query .= ", " . $_;
            }
        }
        
        $query .= ") VALUES ( ";
        $first = 1;
        
        foreach ( sort @$keys ) {
            if ( defined( $status{$_} ) ) {
                $status{$_} =~ s/\s+$//;
            } else {
                $status{$_} = "";
            }
            if ($first) {
                $query .= $mysql->quote($status{$_});
                $first = 0;
            } else {
                $query .= ", " . $mysql->quote($status{$_}) ;
            }
        }
        
        $query .= ")";
        # print $query . "\n";
        
        $mysql->do( $query );
        $mysql->disconnect();
    }
}

# New function to send email notices (when there are errors).
sub send_email {
    my ($self, $subject, $content, $to, $from) = @_;
    
    unless(open (MAIL, "|/usr/sbin/sendmail -t")) {
        print "sendmailerror.\n";
        # warn "Error starting sendmail: $!";
    }
    else{
        print MAIL "From: $from\n";
        print MAIL "To: $to\n";
        print MAIL "Subject: $subject\n\n";
        print MAIL $content;
        close(MAIL) || warn "Error closing mail: $!";
        # print "Mail sent.\n";
    }
}


# Required!
1;

# THE END
