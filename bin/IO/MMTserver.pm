# MMTserver.pm
#
# Very long ago, this was Lincoln Steins "socket server".
# Then it became the Camelserver in /mmt/dataserver/mini.pm
# Then it became AOserver.pm
# Then (7-16-2009) it became MMTserver.pm
#   7-16-2009 -- AOsession was merged into MMTserver.pm
#   7-16-2009 -- SRV record support begun.
#   7-23-2009 -- internal handling of all housekeeping variables.
#   9-18-2009 -- Added mysql connection timeout.

=pod
The Miniserver Framework
Tom Trebisky  10-13-2009

Here is a long "comment" describing the API provided by this module.

The following lines of code would set up a miniserver:

    $server = MMTserver->new ( $my_ident );

    $server->add_update_hook ( \&update_handler, $update_interval );
    $server->add_gizmo_hook ( \&data_handler );

    $server->server_init ();
    $server->main_loop ();

The argument to the new() method is the miniserver name.
This is something like 'CARRIER' and serves two purposes.

First, it provides the name of the miniserver which is used to perform
a variety of database lookups that allow the miniserver to configure itself.

Second, it is converted to lower case and used to provide variable
name prefixes (in the form "carrier_" in this case).

Note that (sadly) the variable prefix convention is far from standardized
and many miniservers do not follow this convention and do all manner of
things, as will need to be discussed below.  A plethora of methods exist
to make this lack of convention more convenient, such as that is.

The add_update_hook method serves to connect a perl method (that you provide)
to a timer event.  In general, this method will be used to request data
from some device, which is refered to as the "gizmo".

The add_gizmo_hook method serves to connect a perl method (that you provide)
to the "incoming data event".  Data received in response to the request
triggered by the update_hook method is passed as an argument to this routine.
Note that data is passed one line at a time, so the gizmo_hook method will
need to provide logic to gather up multiline responses.

The server_init() and main_loop() methods have been separated, because in
some cases (in particular, debugging) it is desirable to call special methods
after the server is set up, but before it begins processing events.

Note that sometimes it is desirable to run a test version of a server while
the regular server is also running (for example this makes it possible to use
the network "all" command to check the variables from each and verify that
they are the same -- provided the gizmo allows multiple concurrent connections
(which most do not).  To do this, run the test server on an alternate port:

	$server->set_port ( 9999 );
	$server->disable_db_updates ();

Make these calls before calling server_init().  When doing this, it is also
a good idea to disable the database updates, or else there will be duplicate
log entries made in the databases.

-----

When the new() method is called, a database lookup in the miniservers:miniservers
database/table is done.  Really the only thing this record provides that we
don't have already is the name of the "gizmo" the miniserver is supposed to talk
to.  Next two SRV lookups are done.  The first is done using the miniserver
name and provides the host/port this server is to run on/as.  Since we are
already running on some host, we ignore the host part, but the port is used
to determine what TCP port we serve on.  The SRV lookup for the gizmo provides
the host/port of the device we talk to, which is quite important.
If for some reason a SRV record cannot be found, an old bogus record in
the dataports table is located and used.

-----

Once the server is running, its job is to maintain a set of variables.
The gizmo_hook routine parses whatever data arrives from the gizmo and
publishes it as values in the variable set.  The usual way to do this is
to use the publish() method.  If life was perfect, this would be the only
method that would exist.  It prepends the miniserver prefix, so that:

	$server->publish ( "temp", $newtemp );

would set a value for "carrier_temp".  A complementary examine() method
exists to get the value of a variable:

	$curtemp = $server->examine ( "temp" );

A host of other methods handle deviate cases as follows:

	$server->publish_verbatim ( "carrier_temp", $newtemp );
	$curtemp = $server->examine_verbatim ( "carrier_temp" );

This pair of verbatim methods would word identically to the non verbatim
methods above.  They are typically used to bypass the prefix handling
performed by publish() and examine().

Note that examine() will return "NA" if the variable is not defined.
Callers should check against "NA", not check if defined($val).

Also note that there is a method called get(), that does the same
thing as examine, but appends a newline

-----

The following methods allow a variety of hanky-panky to be performed with
ease and grace (speaking tongue in cheek).

	$server->publish_with_alias ( "thermal", "temp", $newtemp );

The above sets a value for both "carrier_temp" and "thermal_temp", which can sometimes
be a useful thing when just one variable needs such dual publishing.

	$server->add_alias ( "thermal" );

The above sets a global alias for all variables.
If all the variables in a miniserver needed to be published with a dual identity,
this could be called just once (sometime before invoking the mainloop() method).

	$server->dup_value ( "thermal_temp", "temp" );

The above method could follow the publish() method, yielding the same effect
as publish_with_alias();


=cut

package MMTserver;

# We had hoped that Doing the following would make plain old "trim" visible
# from this package, but this stirs up other issues which may not be worth
# sorting out just now (even though this would be the "right thing" to do.)
#package IO::MMTserver;
#require Exporter;
#@ISA = qw(Exporter);
#@EXPORT = qw(trim);

use strict;
use warnings;
use DBI;
use RRDs;
use POSIX;
use JSON;

use Time::HiRes qw(gettimeofday);

# for DNS SRV record lookup.
use Net::DNS;

use IO::Socket;
use IO::Handle;
use IO::Select;
use IO::Handle;

use IO::MMTbuffer;

my $verbose = 0;
my $mysql_debug = 0;

my $mysql_timeout = 1;

# This is a class method to switch on debug prior to
# calling MMTserver->new()  This is useful for seeing what
# goes on with the database and SRV record queries and such.
sub verbose {
	my ( $class ) = @_;

	# class will always be MMTserver
	#print "Verbose mode set for class: $class\n";

	$verbose = 1;
}

# another class method so we can turn on mysql debug early.
sub mysql_verbose {
    my ($class) = @_;
    
    $mysql_debug = 1;
}

# does the same as the above, but can be called later once
# we have an instance of the class to turn debug on or even
# off (such as if you only want debug during initialization).
sub mysql_debug {
    my ($self,$arg) = @_;
    
    $mysql_debug = $arg;
}

# We have one MMTserver object per server.
# Each server has multiple "connections", each with an MMTbuffer object.

sub dump_array {
	my $name = shift;
	my $x;

	print "dump of $name\n";

	foreach $x ( @_ ) {
	    print " element: ", $x, "\n";
	}
}

# Code to lookup DNS SRV records.
sub get_srv {
    my ( $ident ) = @_;

    my $res = Net::DNS::Resolver->new;

    $ident =~ s/_/-/g;
    my $key = "_" . lc($ident)  . "._tcp.mmto.arizona.edu";
    #$key="bogus._tcp.mmto.arizona.edu";
    #print "Perform SRV lookup for: $key\n";

    # This returns a Net::DNS::Packet object (or undef)
    my $query = $res->query($key, "SRV");
    return unless $query;

    # answer is a list of Net::DNS::RR objects
    # The RR holds: priority, weight, port ,target
    foreach my $rr ($query->answer) {
	next unless $rr->type eq "SRV";
	return ( $rr->target, $rr->port );
    }
    return;
}

# DEPrecated, use srv records!!
sub get_dataports {
    my ( $ident ) = @_;
    
    # Old version, without mysqld connection timeout.
    # my $dbi =  DBI->connect('DBI:mysql:miniservers', 'mmtstaff', 'multiple');
    
    # New version, with mysqld connection timeout.
    my $database = 'miniservers';
    my $dsn = "DBI:mysql:database=$database;mysql_connection_timeout=$mysql_timeout";
    my $dbi =  DBI->connect($dsn, 'mmtstaff', 'multiple');

    if ($dbi) {
        my $sql = "SELECT * FROM dataports WHERE (name = '"  . $ident  . "')";
        my $statement = $dbi->prepare($sql) or return;
        $statement->execute() or return;

        while ( my $rr = $statement->fetchrow_hashref() )  {      
            my $key = $rr->{'name'};
	    print "Found dataports line for $key\n" if $verbose;

	    return ( $rr->{host}, $rr->{port} ) if ( $key eq $ident );
        }

        $dbi->disconnect();
    }
    return;
}

# Get stuff from parameters table and setup logging.

sub setup_logging {
    my ( $self ) = @_;

    # Get the most recent parameters entries for this miniserver
    # The table may have more than one entry for a parameter (evil!)
    # by specifying order by timestamp, we ensure that the most
    # recent entry will be fetched.

    # Old version, without mysqld connection timeout.
    #my $DBI =  DBI->connect('DBI:mysql:miniservers', 'mmtstaff', 'multiple');
    
    # New version, with mysqld connection timeout.
    my $database = 'miniservers'; 
    my $dsn = "DBI:mysql:database=$database;mysql_connection_timeout=$mysql_timeout";
    my $dbi =  DBI->connect($dsn, 'mmtstaff', 'multiple');
    
    if ($dbi) {

        # dataport is bogus now, since we have switched to SRV records.
        #    $sql = "SELECT * FROM parameters WHERE (dataport = '" . $self->{NAME} . "') ORDER BY `parameters`.`timestamp` ASC";

        my $sql = "SELECT * FROM parameters WHERE (mmtservice = '" . $self->{SERVER} . "') ORDER BY `timestamp` ASC";

        my $statement = $dbi->prepare($sql)
          or die "Couldn't prepare query '$sql': $dbi::errstr\n";

        $statement->execute()
          or die "Couldn't execute query '$sql': $dbi::errstr\n";

        my @sql_keys = ();
        my @sql_hr_keys = ();

        my @rrd_keys = ();
        my @rrd_vars = ();

        my $is_logged;
        # build the key lists for logging.
        while (my $row_ref = $statement->fetchrow_hashref() )  {     
            my $key = $row_ref->{name};
	    $is_logged = 0;

	    # PARAMETERS is a hash of the rows from the parameters database.
            $self->{PARAMETERS}->{$key} = $row_ref;

	    # XXX what is this ????
	    # seems to duplicate all the entries in the data hash
	    # tjt - whatever is using this deserves to break
	    # after I comment this out 7-17-2009
            ### $self->{$key} = $self->{PARAMETERS}->{$key}->{initial_value};
            
            # Publishing initial value for this parameter. JDG 02-Apr-2009
            ##$self->{data}->{$key} = $self->{PARAMETERS}->{$key}->{initial_value};
            $self->publish_verbatim ( $key, $self->{PARAMETERS}->{$key}->{initial_value} );

            # Added to keep track of initial values.
	    # XXX need data objects.
            $self->{'initial_value'}->{$key} = $self->{PARAMETERS}->{$key}->{initial_value};
            
	    # XXX need data objects.
            if ($self->{PARAMETERS}->{$key}->{mysql_logging} eq '1') {
	        print "Initializing parameter $key for regular logging\n" if $mysql_debug;
	        $is_logged = 1;
                push @sql_keys, $key; 
            }
            
	    # XXX need data objects.
            if ($self->{PARAMETERS}->{$key}->{mysql_hi_res_logging} eq '1') {
	        print "Initializing parameter $key for high resolution logging\n" if $mysql_debug;
                push @sql_hr_keys, $key; 
	        $is_logged = 1;
            }

	    unless ( $is_logged ) {
	        print "Parameter $key is NOT logged\n" if $mysql_debug;
	    }
            
	    # XXX need data objects.
            if ($self->{PARAMETERS}->{$key}->{rrd_logging} eq '1') {
	        print "Initializing parameter $key for RRD logging\n" if $mysql_debug;
                push @rrd_keys, $key; 
                #  RRDTool variables typically have shortened names.
                push @rrd_vars, $self->{PARAMETERS}->{$key}->{rrd_variable};
                # print $self->{PARAMETERS}->{$key}->{rrd_variable} . "\n";
            }
        }

        $dbi->disconnect();
        
        if ( $#sql_keys >= 0 ) {
	    my @skeys = sort @sql_keys;
	    $self->{MYSQL_LOGGING} = 1;
	    $self->{MYSQL_KEYS} = \@skeys;
	    $self->{MYSQL_VARS} = \@skeys;
        }

        #dump_array ( "keys1", @skeys );
        #dump_array ( "keys2", @{$self->{MYSQL_KEYS}} );

        # Use a different array so we don't trash the above.
        if ( $#sql_hr_keys >= 0 ) {
	    my @skeys_hr = sort @sql_hr_keys;
	    $self->{MYSQL_HI_RES_LOGGING} = 1;
	    $self->{MYSQL_HI_RES_KEYS} = \@skeys_hr;
	    $self->{MYSQL_HI_RES_VARS} = \@skeys_hr;
        }

        print "Found " . ($#sql_keys+1) . " keys to be logged\n" if $mysql_debug;
        print "Found " . ($#sql_hr_keys+1) . " keys to be hires logged\n" if $mysql_debug;
        print "Found " . ($#rrd_keys+1) . " keys to be RRD logged\n" if $mysql_debug;

        # no need to sort these.
        $self->{RRD_KEYS} = \@rrd_keys;
        $self->{RRD_VARIABLES} = \@rrd_vars;
    }
    return;
}

# This loads up the MINISERVER hash, which later gets copied into $self.
# It is expected to contain:
#    $self->{gizmo} = $self->{MINISERVER}->{gizmo};
#    $self->{MINISERVER}->{name};
#    $self->{MINISERVER}->{host} = $self->{PORTS}->{$name . "_HOST"};
#    $self->{MINISERVER}->{port} = $self->{PORTS}->{$name . "_PORT"};

# XXX was hork_database
sub setup_from_database_internal {
    my ( $self, $server_name, $cold ) = @_;
    my $sql;
    my $statement;
    my $key;

    # Old version, without mysqld connection timeout.
    # my $DBI =  DBI->connect('DBI:mysql:miniservers', 'mmtstaff', 'multiple');
    
    # New version, with mysqld connection timeout.
    my $database = 'miniservers';
    my $dsn = "DBI:mysql:database=$database;mysql_connection_timeout=$mysql_timeout";
    my $dbi =  DBI->connect($dsn, 'mmtstaff', 'multiple');

    if ($dbi) {

        # --------------------------------
        # Get stuff from miniservers table.
        # --------------------------------

        $sql = "SELECT * FROM miniservers WHERE (name = '" . $server_name . "') LIMIT 1";
        $statement = $dbi->prepare($sql)
          or die "Couldn't prepare query '$sql': $dbi::errstr\n";
        $statement->execute()
          or die "Couldn't execute query '$sql': $dbi::errstr\n";

        # only one row possible (see LIMIT 1 above)
        # but still need this silly loop or disconnect() will complain.
        my $row_ref;
        while (my $row = $statement->fetchrow_hashref() )  {      
	    $row_ref = $row;
        	print "MINISERVER row " . $row->{name} . "\n" if $verbose;
        }

        $dbi->disconnect();
        
        unless ( defined ( $row_ref ) ) {
        	print "No entry for $server_name in miniservers table in our database\n";
	    return;
        }

        $self->{MINISERVER} = $row_ref;

        $self->{UPDATE_INTERVAL} = $self->{MINISERVER}->{update_interval};

        # Note: $server_name and $self->{MINISERVER}->{name} are (and MUST be)
        # the same, since that is how we fetched the row from the database.

        print "MINIserver DB name = $self->{MINISERVER}->{name}\n" if $verbose;

    # --------------------------------
    # Get host/port stuff from the DNS SRV records.
    # --------------------------------

        print "My identity is : ", $self->{MINISERVER}->{name}, " in the miniserver table\n" if $verbose;
        print "My gizmo is : ", $self->{MINISERVER}->{gizmo}, " in the miniserver table\n" if $verbose;

        # XXX - allow fallbacks to dataports table if no SRV entry.

        my ($host, $port) = get_srv ( $server_name );
        if ( defined $host and defined $port ) {
	    print "My host is $host (from SRV)\n" if $verbose;
	    print "My port is $port (from SRV)\n" if $verbose;
        } else {
        	print "No SRV entry for $server_name -  using the bogus DATAPORTS table\n";
	    ($host, $port) = get_dataports ( $server_name );
	    print "My host is $host (from DataPorts)\n" if $verbose;
	    print "My port is $port (from DataPorts)\n" if $verbose;
        }

        my ($ghost, $gport) = get_srv ( $self->{MINISERVER}->{gizmo} );
        if ( defined $ghost and defined $gport ) {
	    print "My gizmo host is $ghost (from SRV)\n" if $verbose;
	    print "My gizmo port is $gport (from SRV)\n" if $verbose;
        } else {
        	print "No SRV entry for ", $self->{MINISERVER}->{gizmo}, " - using the wretched DATAPORTS table\n";
	    ($ghost, $gport) = get_dataports ( $self->{MINISERVER}->{gizmo} );
	    print "My gizmo host is $ghost (from DataPorts)\n" if $verbose;
	    print "My gizmo port is $gport (from DataPorts)\n" if $verbose;
        }

        unless ( defined ( $port ) ) {
        	die "I don't know what port to serve on!";
        }

        unless ( defined ( $ghost ) and defined ( $gport ) ) {
        	die "I cannot find my gizmo!";
        }

        $self->{PORT} = $port;

        $self->{gizmo_host} = $ghost;
        $self->{gizmo_port} = $gport;

        # XXX seems like a good heuristic, unless somebody
        # scrapes up a quad port lantronix unit someday
        # or changes to some non standard port.
        if ( $gport == 10001 or $gport == 10002 ) {
        	$self->{IS_LANTRONIX} = 1;
        }

        if ( $self->{SERVER_DEBUG} or $verbose ) {
	    print "my PORT: " . $self->{PORT} . "\n";
        
	    print "Gizmo HOST: " . $self->{gizmo_host} . "\n";
	    print "Gizmo PORT: " . $self->{gizmo_port} . "\n";
	    print "Gizmo is a Lantronix\n" if $self->{IS_LANTRONIX} == 1;
        }

        $self->{MYSQL_TABLE} = $self->{MINISERVER}->{mysql_table};
        $self->{MYSQL_HI_RES_TABLE} = $self->{MINISERVER}->{mysql_hi_res_table};

        $self->{MYSQL_INTERVAL} = $self->{MINISERVER}->{mysql_interval};
        $self->{MYSQL_HI_RES_INTERVAL} = $self->{MINISERVER}->{mysql_hi_res_interval};

        $self->setup_logging ();
        $self->setup_more_stuff ( $cold );

        return 1;
    } else {
        return 0;
    }
}

sub setup_from_database {
    my ( $self, $server_name ) = @_;

    $self->setup_from_database_internal ( $server_name, 0 );
}

sub zap_by_prefix {
    my ($self, $prefix) = @_;

    foreach my $key (sort keys %{  $self->{data} } ) {
    	if ( $key =~ /^$prefix/ ) {
	    $self->{data}->{$key} = "NA";
	}
    }
}

sub new {
    my $class = shift;
    my $name = shift;
    
    my $self = {};
    bless $self, $class;

    # turn on autoflush so log messages don't get buffered.
    select (( select (STDOUT), $|=1)[0]);

    # The new API demands a name argument here!
    die "You gotta give me a miniserver name!" unless defined ( $name );

    print "Mini-server starting for $name\n" if $verbose;
    
    # by convention, $name is upper case.
    $self->{NAME} = $name;
    $self->set_service ( lc($name) );

    # $self->{SERVER} = lc ( $name );
    # $self->{PREFIX} = lc ( $name ) . "_";
    
    # DEBUG is debug of the gizmo client connection.
    # SERVER_DEBUG will include all the connections to us as a server.
    $self->{DEBUG} = 0;
    $self->{SESSION_DEBUG} = 0;
    $self->{SERVER_DEBUG} = 0;
    $self->{special_debug} = 0;

    # Set this to accept special <STX><ETX> wrapped strings for the AO-TSS
    $self->{tss_mode} = 0;
    $self->{neslab_mode} = 0;

    # Set this to allow CR as well as NL to terminate strings.
    $self->{crmode} = 0;

    # from AOsession.
    # sessions is a hash of MMTbuffer objects
    $self->{sessions} = {};

    # from AOsession.
    # these are IO::Select objects, which have methods like
    # add, remove, exists
    # the select call itself is a class method (static method) that
    # takes these as arguments.
    $self->{readers}  = IO::Select->new();
    $self->{writers}  = IO::Select->new();

    # from AOsession.
    # this is the socket the server listens for new connections on.
    $self->{listen_socket} = 0;

    # This gets propogated to the MMTbuffer objects.
    $self->{buffer_size} = 10;

    $self->{'client_session'} = 0;
    $self->{'client_func'} = 0;
    
    # This is the hash of current values stored in each miniserver.
    # Anything placed here goes to the outside world via the all command.
    $self->{data} = {};

    # Added to keep track of initial values.  JDG   June 11, 2009
    $self->{'initial_value'} = {};
    
    $self->{server_count} = 0;
    
    $self->{HOOKS} = {};
    $self->{update_func} = 0;
    $self->{UPDATE_INTERVAL} = 5.0;
    $self->{UPDATE_TIME} = time();
    
    $self->{do_db_updates} = 1;
    $self->{MYSQL_INTERVAL} = 0;
    $self->{MYSQL_LOGGING} = 0;
    $self->{MYSQL_TIME} = time();
    
    $self->{MYSQL_HI_RES_INTERVAL} = 0;
    $self->{MYSQL_HI_RES_LOGGING} = 0;
    $self->{MYSQL_HI_RES_TIME} = time();
    
    $self->{WAXED} = 0;
    
    $self->{always_reply} = 0;
    
    # Details for all of the dataports, including host:port.
    # Obtained from the MySQL miniservers:dataports table.
    ###$self->{PORTS} = {};
    
    # Details for all of the miniservers..
    # Obtained from the MySQL miniservers:miniservers table.
    $self->{MINISERVER} = {};
    
    # Details for all of the parameters for this server.
    # Obtained from the MySQL miniservers:parameters table.
    $self->{PARAMETERS} = {};
    
    # keys for all the parameters, used for MySQL logging.
    $self->{MYSQL_KEYS} = ();
    $self->{MYSQL_HI_RES_KEYS} = ();

    # usually match the keys above, but not always
    $self->{MYSQL_VARS} = ();
    $self->{MYSQL_HI_RES_VARS} = ();
    
    # miniserver parameters keys and RRDTool variable names for RRDTool logging.
    $self->{RRD_KEYS} = ();
    $self->{RRD_VARIABLES}  = ();
    
    # Boolean on whether the gizmo is connected via a Lantronxi unit.
    $self->{IS_LANTRONIX} = 0;

    # --------------------------------------
    # Get miniserver parameters from database.

    $self->setup_from_database_internal ( $self->{NAME}, 1 );

    print "Initialization done for miniserver: " . $self->{NAME} . "\n" if $verbose;
    
    return $self;
    # end of new() function.
}

sub setup_more_stuff {
    my ($self, $cold) = @_;

    if ( $cold != 0 ) {
	$self->{gizmo_sock} = 0;
	$self->{gizmo_session} = 0;
	$self->{gizmo_func} = 0;
	$self->{gizmo_status} = "RUN";
	$self->{gizmo_count} = 0;
    }
    
    # initial values for some stuff we always publish

    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();

    $self->publish ( "restarted", sprintf ( "%4d-%02d-%02d %02d:%02d:%02d", 1900+$year, $mon+1, $mday,  $hour, $min, $sec ) );
    $self->publish ( "interval", $self->{UPDATE_INTERVAL} );
    $self->publish ( "count", $self->{server_count} );

    $self->publish ( "lantronix_status", "INIT" );
    $self->publish ( "unit_status", "INIT" );
    $self->publish ( "server_status", "INIT" );

    $self->publish ( "timestamp", 0 );
    $self->publish ( "update", "INIT" );

    # Setting the interval to update the dataserver, in seconds.
    $self->{dataserver_interval} = 0;

    # Setting initial dataserver update to zero time.
    $self->{dataserver_update} = 0;

    # The timestamp when all data was last pushed, in seconds.s.
    $self->{dataserver_all_pushed_update} = 0;

    $self->{dataserver_host} = 0;
    $self->{dataserver_port} = 0;

    # This is the hash of values that have already been pushed to the dataserver,
    # i.e., it's a cache.
    $self->{dataserver_data} = {};
}

sub dataserver_update {
    my ($self, $rate) = @_;

    $self->{dataserver_interval} = $rate;

    # Running the new dataserver on "dataserver2", tcs-server:8660, for now.
    my ($host, $port) = get_srv ( "dataserver2" );

    $self->{dataserver_host} = $host;
    $self->{dataserver_port} = $port;
}

# This clears out the dataserver_data hash.  It would be called when the dataserver 
# is restarted and miniservers need to set all data, not just new data.
sub nuke_dataserver_data {
  my ($self) = @_;

  $self->{'dataserver_data'} = {};
}


sub set_buffer_size {
  my ($self, $size) = @_;

  $self->{buffer_size} = $size;
}

# used to be AOsession->new
# Create a new Session set.

# from old AOsession
# Object method: sessions()
# Return list of all the sessions currently in the set.
#
#sub sessions {
#    return values %{shift->{sessions}};
#}

# from old AOsession
# tjt
#sub debug {
#  my ($self, $why) = @_;
#
#  print "Debug: $why\n";
#  foreach ( $self->sessions ) {
#  	print " Session: " . $_->name . "\n";
#  }
#}

# from old AOsession
# Object method: add_session()
# Add a handle to our collection.
sub add_session {
  my ($self,$handle,$name) = @_;

  warn "Adding a new session for $handle.\n" if $self->{SESSION_DEBUG};

  my $rv = IO::MMTbuffer->new($self,$handle,$name);
  $self->{sessions}{$handle} = $rv;

  # Pass along buffer size as read limit (if it was ever set).
  if ( $self->{buffer_size} ) {
      $rv->read_limit ( $self->{buffer_size} );
  }

  return $rv;
}

# from old AOsession
# Object method: delete_session()
# Remove a session from the set.
sub delete_session {
  my ($self,$thing) = @_;

  my $handle = $self->to_handle($thing);
  my $sess = $self->to_session($thing);

  warn "Deleting session $sess handle $handle.\n" if $self->{SESSION_DEBUG};

  delete $self->{sessions}{$handle};

  $self->{readers}->remove($handle);
  $self->{writers}->remove($handle);
}

sub client_remove {
    my ($self, $session ) = @_;
    
    $self->delete_session ( $session );
}

# from old AOsession
# Object method: to_handle()
# Return a handle, given either a handle or a IO::MMTbuffer object.

sub to_handle {
  my ($self,$thing) = @_;

  return $thing->handle if $thing->isa('IO::MMTbuffer');
  return $thing if defined (fileno $thing);
  return;  # undefined value
}

# from old AOsession
# Object method: to_session
# Return a IO::MMTbuffer object, given either a handle or the object itself.

# from old AOsession
sub to_session {
  my $self = shift;
  my $thing = shift;

  return $thing if $thing->isa('IO::MMTbuffer');
  return $self->{sessions}{$thing} if defined (fileno $thing);
  return;  # undefined value
}

# from old AOsession
# Object method: activate()
# Called with parameters ($session,'read'|'write' [,$activate])
# If called without the $activate argument, will return true
# if the indicated handle is on the read or write IO::Select set.
# May use either a session object or a handle as first argument.
sub activate {
  my $self = shift;
  my ($thing,$rw,$act) = @_;

  my $handle = $self->to_handle($thing);
  my $select = lc($rw) eq 'read' ? 'readers' : 'writers';
  my $prior = defined $self->{$select}->exists($handle);

  if (defined $act && $act != $prior) {
    $self->{$select}->add($handle)        if $act;
    $self->{$select}->remove($handle) unless $act;

    warn $act ? 'Activating' : 'Inactivating',
           " handle $handle for ",
            $rw eq 'read' ? 'reading':'writing',".\n" if $self->{SESSION_DEBUG};
  }

  return $prior;
}

# from old AOsession
# Object method: wait()
# Wait for I/O.  Handles writes automatically.
# Returns a list of IO::MMTbuffer objects ready for reading.  
# If there is a listen socket, then will automatically do an accept()
# and return a new IO::MMTbuffer object for that.

sub wait {
  my $self = shift;
  my $timeout = shift;
  my @sessions;

#  my ($read,$write) = IO::Select->select($self->{readers},$self->{writers},undef,$timeout)
#    or return;  # timeout

  my @handles = IO::Select->select($self->{readers},$self->{writers},undef,$timeout);

  # This can mean a timeout OR an error.
  # (no way to tell, except that if it is an error, you will probably
  # find yourself in a furious polling loop calling select.
  # You can get an error return from select by closing a handle without
  # removing it from the readers/writers lists.  tjt

  unless ( @handles ) {
#      print "Select: timeout or error: $!\n";
      return;
  }

  my ($read, $write, $errs) = @handles;

  # handle queued writes automatically
  foreach (@$write) {
    my $session = $self->to_session($_);
    warn "Writing pending data (",$session->pending+0," bytes) for $_.\n" if $self->{SESSION_DEBUG};
    my $rc = $session->write;
  }

  # Return list of sessions that are ready for reading.
  # If one of the ready handles is the listen socket,
  # then create a new session.
  # Otherwise, return the ready handles as a list

  foreach (@$read) {
    if ($_ eq $self->{listen_socket}) {

      my $newhandle = $_->accept;

      # tjt adds this for debug.
      ##my $conn_name = $self->peer_name ( $newhandle );
      my $conn_name = "connection: " . $newhandle->peerhost();

      warn "Accepting a new handle $newhandle.\n" if $self->{SESSION_DEBUG};
      if ( $newhandle ) {
	  $self->add_session($newhandle, $conn_name );
      }


# tjt removes this (it will cause blocking), we aren't
# really ready for a read, we just have a new connection at
# this point.  Reading will come later (maybe).
#      push @sessions,$newsess;

    } else {
      push @sessions,$self->to_session($_);
    }
  }

  # XXX - now that we don't add the new connection to the list
  # and rightly so), it is entirely possible that we are
  # now returning with an empty array (and this does in fact
  # happen after each and every connection).

#  my $nio = @sessions;
#  print "Select: returns with $nio handles ready\n";

  return @sessions;
}

# END of stuff from AOsession

sub recover_lantronix {
    my ( $self ) = @_;
    
    # Try to do a remote reboot of the Lantronix.  This can work even if port 10001 is locked up.
    # However, port 9999, which is used here, can also be locked up so the reboot may not work.
    # You would have to manually reset the unit in that case.
    
    my $unit = $self->{gizmo_host};
    $unit =~ s/\..*//;
    print "Doing a lantronix reset on unit: ", $unit, "\n";
    system('/mmt/bin/reset_lantronix', $unit );
}

sub show_gizmo_info {
    my ($self) = @_;

    my $host = $self->{gizmo_host};
    my $port = $self->{gizmo_port};

    print $self->{NAME}, " server running on port ", $self->{PORT}, "\n";
    print "Will communicate with device: $host on port $port\n";
    print "Update interval: ", $self->{UPDATE_INTERVAL}, "\n";
}

# check if we have a healthy socket to the gizmo
sub gizmo_check {
    my ( $self ) = @_;

    return 1 if $self->{gizmo_sock};
    return 0;
}

# This is called immediately before the user update function
# gets called.
# it should find the server status IDLE.
# If it does not, that means the device is not responding.
sub next_update {
    my ( $self ) = @_;

    return if $self->examine ( "server_status" ) eq "IDLE";

    $self->publish ( "unit_status", "DEAD" );
}

# This is usually called internally by good_update when we have
# gotten a full set of data from the gizmo and feel good about
# life.  Some servers (like the up and coming dataserver) do not
# talk to a conventional gizmo, and may need to manipulate this
# directly.
sub server_idle {
    my ( $self ) = @_;

    $self->publish ( "server_status", "IDLE" );

    $self->publish ( "timestamp", time() );
    $self->publish ( "update", $self->mysql_datetime() );
}

# The application calls this whenever a complete update has been
# received and validated from the device.
sub good_update {
    my ( $self ) = @_;

    $self->publish ( "unit_status", "OK" );
    $self->server_idle ();
}

# establish a socket connection to the gizmo
# (this is idempotent, i.e. it does not hurt to 
#  call it when there is already a connection).
sub gizmo_connect {
    my ($self,$type) = @_;
    my $session;
    
    # no change, already connected.
    return 1 if $self->{gizmo_sock};
    
    my $host = $self->{gizmo_host};
    my $port = $self->{gizmo_port};
    
    # Adding a parameter for the type of socket connection:  tcp or udp.
    my $proto = 'tcp';

    if (defined($type) && $type eq 'udp' ) {
        $proto = 'udp';
    }
    
    print "Connecting to: $host on port $port, proto: $proto\n" if $self->{DEBUG};

    my $sock = IO::Socket::INET->new (	PeerAddr => $host,
        PeerPort => $port,
        Proto => $proto,
        Timeout => 1.0 );

    
    unless ( $sock ) {
        if ( $self->{IS_LANTRONIX} ) {
            $self->recover_lantronix ();
            $self->{gizmo_status} ='REBOOT';
            $self->{gizmo_count} = 30 / $self->{UPDATE_INTERVAL};
            $self->publish ( "lantronix_status", "Rebooting" );
	    return;
        }

	$self->publish ( "lantronix_status", "Cannot connect" );
        return;
    }
    
    print "Connection to $host on port $port OK!!\n" if $self->{DEBUG};
    
    die "No gizmo function specified" unless $self->{gizmo_func};
    ##die "No session yet to add gizmo to" unless $self->{'session_set'};
    
    $self->{gizmo_sock} = $sock;
    
    $session = $self->add_session ( $sock, "client (device)" );
    $session->tss_mode() if $self->{tss_mode};
    $session->neslab_mode() if $self->{neslab_mode};
    $session->crmode() if $self->{crmode};
    $session->debug () if $self->{DEBUG};
    $self->{gizmo_session} = $session;
    
    $self->publish ( "lantronix_status", "OK" );
    return 1;
}

sub gizmo_flush {
    my ( $self ) = @_;

    $self->{gizmo_sock}->autoflush();
}

sub gizmo_print {
    my ( $self, $msg ) = @_;
    my $sock = $self->{gizmo_sock};

    print "gizmo_print: $msg" if $self->{DEBUG};
    
    $self->publish ( "server_status", "DATA_WAIT" );
    print $sock $msg;
}

sub add_gizmo_hook {
    my ( $self, $hook_sub ) = @_;
    
    $self->{gizmo_func} = $hook_sub;
}

# This allows us to run experimental miniservers without
# filling the logging database with junk.
sub disable_db_updates {
    my ($self) = @_;

    $self->{do_db_updates} = 0;
}

# Keep this, the tss server (and others) need it.
sub add_client {
    my ($self, $handle, $func) = @_;

    $self->{'client_func'} = $func;

    ##die "client_add: no session yet" unless ( $self->{'session_set'} );

    my $client_session = $self->add_session ( $handle, "client (device)" );
    $client_session->tss_mode () if $self->{tss_mode};
    $client_session->neslab_mode () if $self->{neslab_mode};

    $self->{'client_session'} = $client_session;

    return $client_session;
}

#sub mysql_handler {
#  my ($self) = @_;
#}
#
#sub mysql_hi_res_handler {
#  my ($self) = @_;
#}

# tjt - this was a bad idea, and I am working hard to
# get rid of this.....
sub get_data_ref {
    my ($self) = @_;
    
    # this IS a hash reference.
    return $self->{data};
}

# Some funky miniservers need to offer variables with
# a variety of prefixes, they can call this to set the
# alternate leadin, and then publish will do the work.
sub add_alias {
    my ( $self, $alias ) = @_;

    unless ( defined ( $self->{PREFIX_ALIAS1} ) ) {
    	$self->{PREFIX_ALIAS1} = $alias . "_";
	return;
    }

    unless ( defined ( $self->{PREFIX_ALIAS2} ) ) {
    	$self->{PREFIX_ALIAS2} = $alias . "_";
	return;
    }

    print "Too many aliases, $alias ignored\n";
}

# get the value of a variable using the internal name.
sub examine {
    my ( $self, $var ) = @_;
    my $xname = $self->{PREFIX} . $var;

    if ( defined($self->{data}->{$xname}) ) {
        return $self->{data}->{$xname};
    } else {
        return "NA";
    }
}

# get the value of a variable using the raw name.
# (no prefix added to do lookup)
sub examine_verbatim {
    my ( $self, $var ) = @_;

    if ( defined($self->{data}->{$var}) ) {
        return $self->{data}->{$var};
    } else {
        return "NA";
    }
}

# copy a value xx->dup_val ( dest, source )
# source is looked up using the current prefix.
# This is typically used when some housekeeping
# variable needs to have an alter ego (which is
# an ugly hack in any event).  If all the values
# need to have alternate prefixes, do not use this,
# but instead use add_alias to set a global prefix alias.
sub dup_value {
    my ( $self, $dvar, $svar ) = @_;

    my $xname = $self->{PREFIX} . $svar;

    if ( defined($self->{data}->{$xname}) ) {
        $self->{data}->{$dvar} = $self->{data}->{$xname};
    } else {
        $self->{data}->{$dvar} = "NA";
    }
}

sub validate {
  my ( $self, $keys ) = @_;

  my %data = ();
  
  foreach my $var ( split( ',' , $keys) ) {
 
    # In internal miniserver variable name.
    my $key = $self->{PREFIX} . $var;
    
    # See if the prefix is not needed for the internal variable name.
    if ( $var =~ /^$self->{PREFIX}/) {
      $key = $var;
    }  

    # Each parameter is a hash itself within the data hash.
    $data{$key} = ();
   
    # Set some initial default values.
    $data{$key}{val} = 'NA';
    $data{$key}{descriptor} = 'NA';
    $data{$key}{param} = $key;
    $data{$key}{success} = 'NA';
    $data{$key}{units} = 'NA';
    
    # Get the units for this parameter, if it exists.
    # The unit are stored in the parameters MySQL table.
    if (defined ($self->{PARAMETERS}->{$key}->{units})) {
      $data{$key}{units} = $self->{PARAMETERS}->{$key}->{units};
    }
    
    # The descriptor for this variable from the parameters MySQL table.
    $data{$key}{descriptor} = $self->{PARAMETERS}->{$key}->{descriptor};
    
    # Assume success.  (success==1; failure==0)
    $data{$key}{success} = 1;
    
    # Assume an "ok" status.  Other status states are "off", "error", and "warn".
    $data{$key}{status} = "ok";      
    
    # These are derived keys.
    if ($key eq "123") {
      
      
      # The default case. These are indirection keys, e.g., temptrax5_probe5.
    } else {
      
      # Assign the cached value for this parameter.
      my $value = $self->{data}->{$key};
      
    # Check that the value has been changed from the 
      # initial value, i.e., that it has updated at least once.
      if ( $value eq $self->{PARAMETERS}->{$key}->{initial_value} ||
         $value eq 'NA' ) {
        $data{$key}{descriptor} = "Failed to get a valid value for variable: $var";
        $data{$key}{success} = 0;
        $data{$key}{status} = "error";      
        
      # Perform various tests to see if the data is valid
      } else {
        # Check if the unit is active.
        if ( $self->{PARAMETERS}->{$key}->{is_active} eq '0' ) {
          $data{$key}{descriptor} = "Variable $var is not active";
          $data{$key}{success} = 0;      
          $data{$key}{status} = "off";      
          
          # Check if the unit is stale.
          # Staleness is defined in seconds, typically, 60 seconds or so.
        } elsif (time() - $self->{data}->{$self->{PREFIX} . "timestamp"} > $self->{MINISERVER}->{staleness}) {
          # $data{$key}{descriptor} = "Variable $var is stale, timestamp=" . $self->{data}->{$self->{PREFIX} . "timestamp"} . ", staleness=" . $self->{MINISERVER}->{staleness};
          $data{$key}{descriptor} = "Variable $var is stale";
          $data{$key}{success} = 0;      
          $data{$key}{status} = "off";      
          
          # Check if a maximum allowed value is set in the parameters MySQL table
          # and if the value is above that maximum allowed value.
          # Note:  -999.9 is a magic value to skip this test.
        } elsif ( $self->{PARAMETERS}->{$key}->{max} != -999.9 &&
                  $value > $self->{PARAMETERS}->{$key}->{max} ) {
          $data{$key}{descriptor} = "Variable $var is above allowed maximum" . $self->{PARAMETERS}->{$key}->{max};
          $data{$key}{success} = 0;      
          $data{$key}{status} = "error";      
          
          # Check if a minimum allowed value is set in the parameters MySQL table
          # and if the value is below that minimum allowed value.
          # Note:  -999.9 is a magic value to skip this test.
        } elsif ( $self->{PARAMETERS}->{$key}->{min} != -999.9 &&
                  $value < $self->{PARAMETERS}->{$key}->{min} ) {
          $data{$key}{descriptor} = "Variable $var is below allowed minimum";
          $data{$key}{success} = 0;      
          $data{$key}{status} = "error";       
        }
        
        $data{$key}{val} = $value;
      }
    }
  }
  # Return data hash as a hash reference.
  return \%data ;
}


# get the value of a variable using the internal name.
# return value with newline appended.
sub get {
    my ( $self, $var ) = @_;

    return $self->examine ( $var ) . "\n";
}

sub publish {
    my ($self, $var, $val) = @_;

    return unless defined($var);
    return unless defined($val);
   
    $self->{data}->{$self->{PREFIX} . $var} = $val;

    if ( defined ( $self->{PREFIX_ALIAS1} ) ) {
	$self->{data}->{$self->{PREFIX_ALIAS1} . $var} = $val;
    }

    if ( defined ( $self->{PREFIX_ALIAS2} ) ) {
	$self->{data}->{$self->{PREFIX_ALIAS2} . $var} = $val;
    }
}

# Same as publish(), but does not add the miniserver prefix.
sub publish_verbatim {
  my ($self, $var, $val) = @_;

  $self->{data}->{$var} = $val;
}

# deprecated abbreviation for the above.
#sub pub {
#  my ($self, $var, $val) = @_;
#
#  $self->publish_verbatim ( $var, $val );
#}

# publish as two variables.
# in general, this method should not be used, but
# instead a global alias should be set using add_alias.
# However, this can be useful for servers that have
#a small set of variables with dual names.
sub publish_with_alias {
    my ($self, $alias, $var, $val) = @_;
   
    $self->{data}->{$self->{PREFIX} . $var} = $val;
    $self->{data}->{$alias . '_' . $var} = $val;

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

# switch into the special binary mode the TSS server needs.
sub tss_mode {
    my ($self, $mode) = @_;
    
    $self->{tss_mode} = $mode;
}

# switch into the goofy neslab mode for delimiting data.
sub neslab_mode {
    my ($self, $mode) = @_;
    
    $self->{neslab_mode} = $mode;
}

# Allow CR as well as NL to terminate strings.
# (Also merges CR NL clusters)
sub crmode {
    my ($self) = @_;
    
    $self->{crmode} = 1;
}

# Set the port (override any database info).
sub set_port {
    my ($self, $port) = @_;
    
    $self->{PORT} = $port;
}

# This is to support the HexapodSupport miniserver which switches
# dynamically from f5 to f9 or vice versa.
sub set_service {
    my ($self, $name) = @_;

    $self->{SERVER} = $name;
    $self->{PREFIX} = $name . "_";
}

sub set_name {
    my ($self, $name) = @_;
    
    $self->{NAME} = $name;
    $self->set_service ( lc ( $name ) );
}

sub server_debug {
    my ($self, $debug) = @_;
    
    $self->{SERVER_DEBUG} = $debug;
}

sub debug {
    my ($self, $debug) = @_;
    
    $self->{DEBUG} = $debug;
}

# The purpose of this changes from problem to problem
sub special_debug {
    my ($self, $debug) = @_;
    
    $self->{special_debug} = $debug;
}

# Returns a Perl hash of the host and port values.
#sub get_ports {
#    my $self = shift;
#    
#    return $self->{PORTS};
#}

# Returns a listing of the host and port values, terminated by ".EOF".
# Mainly for testing.
#sub print_ports {
#    my $self = shift;
#    my $str = "";
#    
#    foreach my $port ( sort keys %{ $self->{PORTS} } ) {
#        $str .= $port . " " . $self->{PORTS}->{$port} . "\n";
#    }
#    $str .= ".EOF\n";
#    return $str;
#}

# Returns the host or port for a miniserver.
#sub lookup {
#    my $self = shift;
#    my $key = undef;
#    
#    $key = shift if @_;
#    if ( defined( $key ) && defined ( $self->{PORTS}->{$key} ) ) {
#        return $self->{PORTS}->{$key};
#    } else {
#        return undef;
#    }
#}

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
    my $string = "";
    
    $string = shift if @_;
    if ($string) {
        $string =~ s/^\s+//;
        $string =~ s/\s+$//;
    }
    return $string;
}

# Left trim function to remove leading whitespace
# XXX - never used
sub ltrim {
    my $string = "";
    
    $string = shift if @_;
    $string =~ s/^\s+//;
    return $string;
}

# Right trim function to remove trailing whitespace
# XXX - never used
sub rtrim {
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
#sub data2table {
#    my $self = shift;
#    my $data;
#    
#    $data = shift if @_;
#    
#    $data->{'webserver_time'} = Miniserver->mysql_datetime();
#    $data->{'webserver_timestamp'} = time();
#    
#    my $html = "";
#    $html .= "Content-type: text/html\n";
#    # Required to prevent IE caching.
#    $html .= "Cache-control: no-cache\n";
#    $html .= "Pragma: no-cache\n\n";
#    
#    $html .= "<table border=\"1\" width=\"100%\" style=\"table-layout: fixed\"><col width=\"50%\"><col width=\"50%\">\n";
#    $html .= "<tbody>\n";
#    my $row = 1;
#    
#    foreach my $key ( sort keys %{ $data } ) {
#        my $val = $data->{$key};
#        unless ( defined( $val ) && length( $val ) > 0) {
#            $val = "&nbsp;"
#        }
#        # Alternating the background color for rows in the HTML table.
#        if ($row%2 == 0) {
#            $html .= "<tr align=\"center\"><td>" . $key . "</td><td>" . $val . "</td></tr>\n";
#        } else {
#            $html .= "<tr align=\"center\" bgcolor=\"lightsteelblue\"><td>" . $key . "</td><td>" . $val . "</td></tr>\n";
#        }
#        $row++;
#    }
#    $html .= "</tbody>\n";
#    $html .= "</table>\n";
#    
#    return $html;
#}

# The main socket server subroutine!
# This is a multiplexing server, using Lincoln Stein's
# IO::LineBufferedSet module.
sub server_init {
    my ( $self ) = @_;
    my $port;
    my $handle;
    
    # Adding MySQL hooks.
    my $mysql_interval = $self->{MINISERVER}->{mysql_interval};  # In seconds
    my $mysql_hi_res_interval = $self->{MINISERVER}->{mysql_hi_res_interval};  # In seconds
    
    ## BUG:  5:25PM  13Mar2007 JDG.
    #
    ## Undefined subroutine &main::
    ## called at /mmt/miniserver/IO/MMTserver.pm line 526.
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
    
    # XXX - this used to be where we called AOsession
    # We ALWAYS have a socket to listen on, being a server
    # as we are.  This listening socket handle does NOT
    # go into the sessions hash.
    # It gets added to the readable list,
    #  and is entirely internal to this module.
    # If it becomes readable,
    #  then that means we can accept a connection.

    if ( defined($handle) and $handle->can('accept') ) { 
	$self->{listen_socket} = $handle;
	$self->{readers}->add($handle);
    }

    print "Creating " . $self->{NAME} . " server.\n" if $self->{DEBUG};
}

# This gets called when a timeout has come along.
# The real goal of all this is to call the users update function.
#
# The wax on / wax off business is virtually never used.
# the terminology comes from the movie "karate kid".
# that aside, the idea is that you can make a telnet connection
# to the miniserver and send the wax off command.
# The miniserver then disconnects from the gizmo, which
# allows you to make a telnet connection to the gizmo and
# poke around.  Then when you are done, you give the wax on
# command, and the miniserver picks up where it left off.
#
# In practice, nobody even knows about this capability, so they
# just kill the miniserver, do what they want, then restart it.
#
# The rebooting thing applies to lantronix, which can be remotely
# rebooted, but they take forever and a day to do whatever it is
# they do, we give them 30 seconds before attempting to remake
# a connection.

sub do_update {
    my ( $self ) = @_;
    
    $self->{server_count}++;
    $self->publish ( "count", $self->{server_count} );
    
    # printf ("Update!!\n");
    
    if ( $self->{WAXED} ) {
        $self->{gizmo_status} = "WAXED";
        $self->publish ( "server_status", "WAXED" );
        $self->{gizmo_count} = 0;
        if ( $self->{gizmo_sock} ) {
            #print "OK, waxing ...\n";
            $self->delete_session ( $self->{gizmo_session} );
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
	$self->next_update();
	$self->push_to_dataserver();
        &{$self->{update_func}}();
    }
}

sub main_loop {
    my ( $self ) = @_;
    
    my $timeout = 1.0;
    my $first = 1;
    my $now;
    my $delta;
    
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
        
        my @ready = $self->wait ( $timeout );
        
        #    if ( $! ) {
        #    	warn ( "select error (not timeout): $!\n" );
        #    }
        
        # We don't care what or why, but as long as we are
        # here (for IO, or timeout, or an error), we will do
        # an update if it is time for one.
        
	$now = time ();
	$delta = $now - $self->{UPDATE_TIME};
            
	# time() has whole second resolution, so the >= check
	# is appropriate.
	if ( $first or $delta >= $self->{UPDATE_INTERVAL} ) {
	    $self->{UPDATE_TIME} = $now;
	    $self->do_update ();
	}
        
        undef $first;
        
        my $nr = @ready;
        # Adding number of available handles to object.
        $self->{handles} = $nr;
        print "wait has $nr handles ready\n" if $self->{DEBUG};
        ##    $self->debug ( "readable: $nr" );
        
        # Handle each socket that's ready
        for my $session (@ready) {
            my $buf;
            my $nio;
            my $debug;
            my $rv;
            
            $debug = $self->{SERVER_DEBUG};
            if ( $self->{'gizmo_session'} ) {
                if ( $session eq $self->{'gizmo_session'} ) {
                    $debug = $self->{DEBUG};
                }
            }
            
            # read any available data.
            $session->do_read ();
            
	    # We may get multiple lines for a single read,
	    # added this loop 2-20-2009
	    # fixes the delayed .EOF bug.
	    for ( ;; ) {
		# See if a complete line has accumulated.
		$nio = $session->check_line ( $buf );
            
		if ( defined $nio and defined $buf ) {
		    print "check_line Bingo!! $nio bytes:\n$buf\n" if $debug;
		} else {
		    print "check_line ... not yet\n" if $debug;
		}
            
		last unless ( defined $nio );

		if ( $self->{gizmo_session} && $session eq $self->{gizmo_session} && $nio <= 0 ) {
		    print "gizmo connection gone, closing!\n" if $debug;
		    $session->close ();
		    undef $self->{gizmo_sock};
		    last;
		}
            
		if ( $nio == 0 ) {
		    print "connection or client gone, close\n" if $debug;
		    $session->close ();
		    #undef $buf;
		    last;
		}
            
		if ( $nio < 0 ) {
		    print "IO error: closing\n" if $debug;
		    $session->close ();
		    #undef $buf;
		    last;
		}
            
		print "check_line returns count: $nio\n" if $debug;
            
		# If we are handling client data, and this is some
		# of that data, hand it to the client.
		# NOTE: we really only should have a client session
		# or a gizmo session, and they are flavors of the
		# same thing!

		if ( $self->{'client_session'} && $session eq $self->{'client_session'} ) {
		    &{$self->{'client_func'}}( $buf );
		    print "client data handled by client function\n" if $self->{DEBUG};
		    next;
		}
            
		if ( $self->{gizmo_session} &&  $session eq $self->{gizmo_session} ) {
		    unless ( defined $buf ) {
			# $self->client_close ( $self->{gizmo_session} );
			undef $self->{gizmo_sock};
			print "Lost gizmo connection!\n" if $self->{DEBUG};
			last;
		    }

		    if ( $self->{gizmo_func} ) {
			$rv = &{$self->{gizmo_func}}( $buf );
			if ( $rv && $self->{do_db_updates} ) {
			    $self->update_db();
			}
		    }
		    # keep the check_line loop going!
		    next;
		}
            
		# Any other IO going on must be coming from one of
		# the connections to the network socket.
            
		if ( $buf ) {
		    # remove control-M's that we get in telnet sessions.
		    $buf =~ s/\cM//;
                
		    print "process line: $buf \n" if $debug;
		    $self->parse_request ( $session, $buf );
		    # keep the check_line loop going!
		    next;
		}
            
	    } # end of check_line loop.
        } # end of session loop
        
    } # end of infinite wait loop
    
}	# end for "sub main_loop"

# The following pair of routines allows a specific miniserver to override
# the table and key set originally specified by the parameters table.
# This is to support the HexapodSupport miniserver which switches
# dynamically from f5 to f9 or vice versa.
# Despite the funtion name, we set both table and keys and vars
# keys and vars must be a reference to arrays (already sorted)

sub set_logging_keys {
    my $self = shift;
    $self->{MYSQL_TABLE} = shift;
    $self->{MYSQL_KEYS} = shift;
    $self->{MYSQL_VARS} = shift;
}

sub set_hi_res_logging_keys {
    my $self = shift;
    $self->{MYSQL_HI_RES_TABLE} = shift;
    $self->{MYSQL_HI_RES_KEYS} = shift;
    $self->{MYSQL_HI_RES_VARS} = shift;
}

sub update_db {
    my $self = shift @_;
    my $delta;
    my $now = time ();
    
    #print localtime() . ": update_db called\n";
    
    # Update the RRDTOOL everytime we get new data.
    $self->update_rrdtool();
    
    # print localtime() . ": MYSQL_INTERVAL $self->{MYSQL_INTERVAL}\n"; 
    
    # Add MySQL and HI RES MySQL logging
    if ( $self->{MYSQL_LOGGING} and $self->{MYSQL_INTERVAL} > 0 ) {
    	#print "Vanilla logging to ", $self->{MYSQL_TABLE}, "\n";
        $delta = $now - $self->{MYSQL_TIME};
        # if ( $delta > $self->{MYSQL_INTERVAL} - $self->{UPDATE_INTERVAL} ) {
        if ( $delta >= $self->{MYSQL_INTERVAL} ) {
            # print  localtime() . ": update MySQL\n";
            $self->{MYSQL_TIME} = $now;
            $self->update_mysql ( $self->{MYSQL_TABLE}, $self->{MYSQL_KEYS}, $self->{MYSQL_VARS} ); 
        }
    }
    
    #print "HIRES switch: ", $self->{MYSQL_HI_RES_LOGGING}, "\n";
    #print "HIRES interval: ", $self->{MYSQL_HI_RES_INTERVAL}, "\n";

    if ( $self->{MYSQL_HI_RES_LOGGING} and $self->{MYSQL_HI_RES_INTERVAL} > 0 ) {
    	#print "HIRES logging to ", $self->{MYSQL_HI_RES_TABLE}, "\n";
        $delta = $now - $self->{MYSQL_HI_RES_TIME};    
        # if ( $delta > $self->{MYSQL_HI_RES_INTERVAL} - $self->{UPDATE_INTERVAL} ) {
        if ( $delta >= $self->{MYSQL_HI_RES_INTERVAL} ) {
            $self->{MYSQL_HI_RES_TIME} = $now;
            $self->update_mysql ( $self->{MYSQL_HI_RES_TABLE}, $self->{MYSQL_HI_RES_KEYS}, $self->{MYSQL_HI_RES_VARS} ); 
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
    foreach my $key (sort keys %{ $self->{data} } ) {
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
        $html .= "<td class=\"value\" title=\"" . $title . "\" id=\"" . $key  . "\">" . $self->{data}->{$key} . "</td>";
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
    
    $self->{data}->{'webserver_timestamp'} = time();
    $self->{data}->{'webserver_time'} = $self->mysql_datetime();
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
#        } elsif ( $req =~ /\/ports/ ) {  
#            $payload = JSON::to_json ( $self->{PORTS} ) . "\n";
#            $self->return_http ( $session, $payload );
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
            
#            $big_hash{ports} = $self->{PORTS};
            
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
    
    # These are the variables passed to the request subroutine.
    my $arg1 = "";
    my $arg2 = "";
    my $arg3 = "";
    
    print "MMTserver Got line: $line\n" if $self->{DEBUG};
    
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
        # This doesn't work.  It returns a reference to the class.
        # $arg3 = $self->trim( $arg3 );
        # Doing the trim the "old" way.
        $arg3 =~ s/^\s+//;
        $arg3 =~ s/\s+$//;
    } else {
        $arg3 = "";
    }
    
    if ( $self->{DEBUG} ) {
        print "\nargs = " . $arg1 . " " . $arg2 . " " . $arg3 if $self->{DEBUG}; 
    }
    
    # print "\nargs = " . $arg1 . " " . $arg2 . " " . $arg3;

    my $reply = $self->handle_request( $arg1, $arg2, $arg3 ); 
    
    # Write response back to client, if appropriate.  
    # Non-blocking IO is handled!
    
    $session->write($reply) if length($reply) > 0;
}

sub lookup_data {
    my ( $self, $key ) = @_;
    my $rv;
    
    unless ( defined($key) ) {
	print "lookup_data finds $key undefined\n";
	return;
    }
    
    if ( $key =~ /.*\d\d_\d\d$/ ) {
        if ( exists($self->{log}->{$key}) && defined($self->{log}->{$key}) ) {
            $rv = $self->{log}->{$key};
        }
    } else {
        if ( exists($self->{data}->{$key}) && defined($self->{data}->{$key}) ) {
            $rv = $self->{data}->{$key};
        }
    }
    return $rv;
}

# This is common code for the cgi, all, and ... protocols.
sub hash_join {
    my ( $self, $data, $sep1, $sep2 ) = @_;
    my $first = 1;
    my $rv = "";
    my $val;
    
    foreach my $key (sort keys %{$data} ) {
        next unless exists ( $data->{$key} );
	$val = $data->{$key};
	$val = "NA" unless defined ( $val );
        $rv .= $sep2 unless $first;
        undef $first;
        $rv .= $key . $sep1 . $val;
    }
    return $rv;
}

# as above, but limit to a subset of keys.
sub hash_join_keys {
    my ( $self, $keys, $data, $sep1, $sep2 ) = @_;
    my $first = 1;
    my $rv = "";
    my $val;
    
    foreach my $key ( split( ',' , $keys) ) {
        next unless exists ( $data->{$key} );
	$val = $data->{$key};
	$val = "NA" unless defined ( $val );
        $rv .= $sep2 unless $first;
        undef $first;
        $rv .= $key . $sep1 . $val;
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
    
    foreach my $key (sort keys %{  $self->{data} } ) {
        $reply .= " " . $key . "=\"" . $self->{data}->{$key} . "\"";
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
    foreach my $key (sort keys %{  $self->{data} } ) {
        $reply .= "<" . $key . ">" . $self->{data}->{$key} . "<\/" . $key . ">";
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
    # while ( my ($index, $value) = each(%{ $self->{data} }) ) {
    #    my $max = $self->{PARAMETERS}->{$index}->{max};
    #    if (defined($max) && $value ne 'NA') {
    #        # '-999.9' is a "magic" number to not evaluate max values.
    #        if ( $max  != -999.9 && $value > $max ) {
    #            $self->{data}->{$index} = "NA";
    #        }
    #    }
    #    my $min = $self->{PARAMETERS}->{$index}->{min};
    #    if (defined($min)  && $value ne 'NA') {
    #        # '-999.9' is a "magic" number to not evaluate min values.
    #        if ( $min  != -999.9 && $value < $min ) {
    #            $self->{data}->{$index} = "NA";
    #        }
    #    }
    #    my $now = time();
    #    my $staleness = $self->{MINISERVER}->{staleness};
    #    my $update = $self->{UPDATE_TIME}; 
    #    if ( defined($staleness)  && 
    #        defined($update)  && 
    #        defined($value) && 
    #        ($value ne 'NA')) {
    #        # Don't overwrite "update", "time", or "timestamp" variables 
    #        # for stale data so that we know how stale the data is.
    #        if ( $now - $update  > $staleness && 
    #            $index !~ /_update$/ &&
    #            $index !~ /_timestamp$/ && 
    #            $index !~ /_time$/ ) {
    #                $self->{data}->{$index} = "NA";      
    #            }
    #    } elsif ( defined($index) and ! defined($value) ) {
    #            print "$index has not defined value.\n";
    #    }
    # }
    
    if ( $arg1 eq "all" ) {
        
        $reply = $self->hash_join ( $self->{data}, " ", "\n" ) . "\n.EOF\n";
        
    } elsif ($arg1 eq "all_keys") {
        
        $reply = $self->hash_join_keys ( $arg2, $self->{data}, " ", "\n" ) . "\n.EOF\n";

    } elsif ($arg1 eq "reconnect") {

        nuke_dataserver_data();
        $reply = ".EOF\n";
        
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
        
#        $big_hash{ports} = $self->{PORTS};
        
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
        
#    } elsif ($arg1 eq "ports") {    
#        $reply = JSON::to_json ( $self->{PORTS} ) . "\n";
        
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
      # Testing a new "set json ..." protocol.  JDG 05-Aug-2009
      # Sample command:  'set json {"hi":"ho123","here":"there456"}'
      if ( $arg2 eq "json" )  {
        if ( defined ($arg3) && ( length ( $arg3 ) > 0) ) {
          # Convert JSON string to a hash reference.
          my $hash_ref = JSON::from_json ( $arg3 );
          # from_json() returns undef on a JSON parse error.
          if ($hash_ref) {
            # Dereference new data hash.
            my %new_stuff = %{ $hash_ref  };
            # Merge new data hash with existing data hash.
            foreach my $key (sort keys %new_stuff ) {
              if ( length ( $new_stuff{$key} ) > 0) {
                # Writing (or overwriting) a hash entry.
                $self->{data}->{$key} = $new_stuff{$key};
              } else {
                # Deleting if value is empty string..
                delete( $self->{data}->{$key} ) if defined ( $self->{data}->{$key} );
              }
            }
          }
        }
        # Regular "set" command (anything beside "set json ...").
      } else {
        if ( $self->{special_debug} > 0 ) {
          if ( $arg2 =~ /version/ or $arg2 =~ /motionFlag/ or $arg2 =~ /uttime/ ) {
	    	my $time = gettimeofday;
            print "$time set $arg2 $arg3\n";
          }
        }
        if ( defined ($arg3) && ( length ( $arg3 ) > 0) ) {
          # Writing (or overwriting) a hash entry.
          $self->{data}->{$arg2} = $arg3;
        } else {
          # Deleting if $arg3 is empty string..
          delete( $self->{data}->{$arg2} ) if defined ( $arg2);
        }
      }
    } elsif ($arg1 eq "set_eof") {
        if ( defined ($arg3) && ( length ( $arg3 ) > 0) ) {
            $self->{data}->{$arg2} = $arg3;
        } else {
            # Deleting if not $arg3.
            delete ( $self->{data}->{$arg2} ) if defined ( $arg2);
        }
        # $reply = $data{$arg2} . "\n";
        $reply .= ".EOF\n";
        
        # JDG 8-11-2009.  A validated form of get.  
        # Returns a JSON string of data and metadata.
      } elsif ($arg1 eq "validate") {
        my $hash_ref = $self->validate ( $arg3 );
        # Always return the timestamp.
        $hash_ref->{webserver_timestamp} = time();
        $reply = JSON::to_json($hash_ref);
        
        # tjt  2-12-2008
        # New flavor of get, that always replies.
    } elsif ($arg1 eq "get") {
        $val = $self->lookup_data ( $arg2 );
        if ( defined($val) ) { 
            $reply = $val . "\n";
        } else {
            $reply = "ERR\n" if $self->{always_reply};
        }
        
        # JDG  18-Sep-2009
        # Another flavor of "get" that checks for staleness for the requested variable.
     } elsif ($arg1 eq "get_valid") {
        # First, check for staleness of data within the miniserver.
        if ( defined($self->{data}->{$self->{PREFIX} . "timestamp"})  &&
             defined($self->{MINISERVER}->{staleness})  && 
             ( ( time() - $self->{data}->{$self->{PREFIX} . "timestamp"} ) <= $self->{MINISERVER}->{staleness} ) ) {
             
             # Next, check that we have a value for the requested parameter
             # and that it is within allowed maxs and mins.
             #
             # "-999.9" is a magic value not to do the max or min check.
             $val = $self->lookup_data ( $arg2 );
             if ( defined($val) &&
                  defined( $self->{PARAMETERS}->{$arg2}->{max} ) &&
                  defined ( $self->{PARAMETERS}->{$arg2}->{min} ) ) {
                  if ( ( ( $self->{PARAMETERS}->{$arg2}->{max} == -999.9 ) || 
                         ( $val <= $self->{PARAMETERS}->{$arg2}->{max} ) )  && 
                         ( ( $self->{PARAMETERS}->{$arg2}->{min} == -999.9 ) || 
                         ( $val >= $self->{PARAMETERS}->{$arg2}->{min} ) ) ) {
                     $reply = $val . "\n";
                  } else {
                     $reply = "NA\n";
                  } 
             } else {
                  $reply = "NA\n";
             }
        } else {
             $reply = "NA\n";
        }    
        
    } elsif ($arg1 eq "get_eof") {
        $val = $self->lookup_data ( $arg2 );
        $reply = $val . "\n" if defined($val);
        $reply .= ".EOF\n";
        
    } elsif ($arg1 eq "get_key_value") {
        $val = $self->lookup_data ( $arg2 );
        $reply = "";
        $reply .= $arg2 . " " . $val if defined($val);
        $reply .= "\n";
        
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
        my %status = %{ $self->{data} };
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
        print localtime() . ": ERROR while updating RRD: $ERR\n" if $ERR; 
    }
}

# $mini->update_mysql ( "carrier_background_log", \@keys, \@vars );

sub update_mysql {
    my ( $self, $table, $keys, $vars ) = @_;
    
    return unless $keys;
    return unless $vars;

    #dump_array ( "mysql_keys", @$keys );
    #dump_array ( "mysql_vars", @$vars );

    # Dereference (and copy) the entire data hash.
    my %stuff = %{ $self->{data} };

    return unless %stuff;

    # Old version, without mysqld connection timeout.
    # my $mysql =  DBI->connect('DBI:mysql:mmtlogs', 'mmtstaff', 'multiple');
    
    # New version, with mysqld connection timeout.
    my $database = 'mmtlogs';
    my $dsn = "DBI:mysql:database=$database;mysql_connection_timeout=$mysql_timeout";
    my $mysql =  DBI->connect($dsn, 'mmtstaff', 'multiple');

    if ($mysql) {
        my $query = "INSERT INTO $table ( ";
            
        my $first = 1;
        foreach ( @$keys ) {
    	$query .= ", " unless $first;
    	$first = 0;
    	$query .= $_;
        }
            
        $query .= ") VALUES ( ";
            
        $first = 1;
        foreach ( @$vars ) {
    	if ( defined( $stuff{$_} ) ) {
    	    $stuff{$_} =~ s/\s+$//;
    	} else {
    	    $stuff{$_} = "";
    	}

    	$query .= ", " unless $first;
    	$first = 0;
    	$query .= $mysql->quote($stuff{$_});
        }
            
        $query .= ")";

        # PRINT THE SQL!
        print "SQL: " . $query . "\n" if $mysql_debug;
            
        # returns 1 if OK, undefined on error.
        my $rv = $mysql->do( $query );
        $mysql->disconnect();

        unless ( $rv ) {
    	print "MySQL insert failed for SQL: $query\n";
        }

        # $mysql =  DBI->connect('DBI:mysql:mmtlogs:hoseclamp', 'mmtstaff', 'multiple') or print "Can't connect to hoseclamp";
        # $mysql->do( $query ) or print "Can't execute to hoseclamp";
        # $mysql->disconnect();
        
        # $mysql =  DBI->connect('DBI:mysql:mmtlogs:yggdrasil', 'mmtstaff', 'multiple') ;
        # $mysql->do( $query );
        # $mysql->disconnect();
    }
}

# Sends all of the current data values in the miniserver to the dataserver.
sub push_to_dataserver {
  my ( $self ) = @_;

  my $push_debug = 0;

  return unless $self->{dataserver_interval} > 0;
  
  # Dereference (and copy) the entire data hash.
  my %stuff = %{ $self->{data} };
  return unless %stuff;
  
  my $t = time();
  if ($t - $self->{dataserver_update} >= $self->{dataserver_interval}) {
    
    # Data to push to the dataserver.
    my %new_stuff;
    
    # We want to push all data every 60 seconds.  Among other
    # things, this keeps the dataserver aware of stale data.
    if ($t - $self->{dataserver_all_pushed_update} > $self->{MINISERVER}->{staleness}/2) {
      while (my ($key, $value) = each(%stuff)){
          if ($key !~ /^webserver/) {
              $value = "NA" unless defined ( $value );
              my $ds_name = $self->{PARAMETERS}->{$key}->{ds_name};
              if (defined ($ds_name) ) {
                  # For debugging only.
                  print "PTD1 key, ds_name, val = $key, $ds_name, $value\n" if ($push_debug);
                  $self->{'dataserver_data'}->{$key} = $new_stuff{$ds_name} = $value; 
              }
          }
      }
      $self->{dataserver_all_pushed_update} = $t;
      
    # Only pushing new data.
    } else {
     print "PTD2\n" if ($push_debug);
      while (my ($key, $value) = each(%stuff)){
        if ($key !~ /^webserver/) {
            if (! defined ($self->{'dataserver_data'}->{$key}) || 
                $self->{'dataserver_data'}->{$key} ne $value ) {
                $value = "NA" unless defined ( $value );
                my $ds_name = $self->{PARAMETERS}->{$key}->{ds_name};
                if (defined ($ds_name) ) {
                    # For debugging only.
                    print "PTD2 key, ds_name, val = $key, $ds_name, $value\n" if ($push_debug);
                    $self->{'dataserver_data'}->{$key} = $new_stuff{$ds_name} = $value; 
                }
            }
        }
      }
    }
    
    my $cmd = "";
    
    # Using JSON
    if (1) {
      $cmd .= "set json " . JSON::to_json ( \%new_stuff ) . "\n";      
      
      # Build a multi-line string of 'set' commands.
    } else {
      while (my ($key, $value) = each(%new_stuff)){
        $cmd .= "set $key $value\n";
      }
    }
    
    my $sock = IO::Socket::INET->new (	PeerAddr => $self->{dataserver_host},
                                        PeerPort => $self->{dataserver_port},
                                        Timeout => 1.0 );
    
    # For testing.
    # print "$cmd\n";
    
    if ($sock) {
      $sock->send($cmd);
      $sock->close();
    } else {
      # We couldn't send data to the dataserver.  Nuke the cached copy.
      nuke_dataserver_data();
    }

    $self->{dataserver_update} = $t;
  }
}

# New function to send email notices (when there are errors).
# XXX - php has an "official" mail function -- we could use it.
sub send_email {
    my ($self, $subject, $content, $to, $from) = @_;
    
    unless(open (MAIL, "|/usr/sbin/sendmail -t")) {
        print "sendmailerror.\n";
        # warn "Error starting sendmail: $!";
    } else {
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
