package IO::AOsession;

# This was Lincoln Steins SessionSet.pm and LineBufferedSet.pm

use strict;

use IO::Select;
use IO::Handle;

use IO::AObuffer;

use vars '$DEBUG';
$DEBUG = 0;

# This file provides the "session_set" object, and
# there is one per server.  The usual "session" object
# is actually supported by the AObuffer.pm module.
# There is one AObuffer "session" per connection.
# Confusing? Yes!  Talk to Lincoln Stein.
# This really ought to just be merged into AOserver.pm

# Class method new()
# Create a new Session set.
# If passed a listening socket, use that to
# accept new connections

sub new {
  my $pack = shift;
  my $listen = shift;
  my $self = bless { 
                    sessions     => {},
                    readers      => IO::Select->new(),
                    writers      => IO::Select->new(),
		    tssmode	 => 0,
		    listen_socket => 0
                   },$pack;

  # if initialized with an IO::Handle object (or subclass)
  # then we treat it as a listening socket.
  # NOTE: this does not go into the sessions hash.
  # it gets added to the readable list, and is entirely
  # managed right here in this module.  If it becomes
  # readable, that means we can accept a connection.
  # (this listen socket is a secret taken care of in
  # this module).

  if ( defined($listen) and $listen->can('accept') ) { 
    $self->{listen_socket} = $listen;
    $self->{readers}->add($listen);
  }
  return $self;
}

# Object method: sessions()
# Return list of all the sessions currently in the set.
sub sessions {
    return values %{shift->{sessions}};
}

# tjt
sub debug {
  my ($self, $why) = @_;

  print "Debug: $why\n";
  foreach ( $self->sessions ) {
  	print " Session: " . $_->name . "\n";
  }
}

# Object method: add()
# Add a handle to the session set.
sub add {
  my ($self,$handle,$name) = @_;

  warn "Adding a new session for $handle.\n" if $DEBUG;

  my $buf = $self->{sessions}{$handle} = IO::AObuffer->new($self,$handle,$name);
  $buf->tssmode ( $self->{tssmode} );

  return $buf;
}

# Object method: delete()
# Remove a session from the set.
sub delete {
  my ($self,$thing) = @_;

  my $handle = $self->to_handle($thing);
  my $sess = $self->to_session($thing);

  warn "Deleting session $sess handle $handle.\n" if $DEBUG;

  delete $self->{sessions}{$handle};

  $self->{readers}->remove($handle);
  $self->{writers}->remove($handle);
}

sub tssmode {
  my ( $self, $arg ) = @_;

  ##print "call to AOsession.tssmode $arg\n";
  $self->{tssmode} = $arg;
}

# Object method: to_handle()
# Return a handle, given either a handle or a IO::AObuffer object.

sub to_handle {
  my ($self,$thing) = @_;

  return $thing->handle if $thing->isa('IO::AObuffer');
  return $thing if defined (fileno $thing);
  return;  # undefined value
}

# Object method: to_session
# Return a IO::AObuffer object, given either a handle or the object itself.

sub to_session {
  my $self = shift;
  my $thing = shift;

  return $thing if $thing->isa('IO::AObuffer');
  return $self->{sessions}{$thing} if defined (fileno $thing);
  return;  # undefined value
}

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
            $rw eq 'read' ? 'reading':'writing',".\n" if $DEBUG;
  }

  return $prior;
}

sub peer_name {
    my ($self, $handle) = @_;

    return "connection: " . $handle->peerhost();
}

# Object method: wait()
# Wait for I/O.  Handles writes automatically.
# Returns a list of IO::AObuffer objects ready for reading.  
# If there is a listen socket, then will automatically do an accept()
# and return a new IO::AObuffer object for that.

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
    warn "Writing pending data (",$session->pending+0," bytes) for $_.\n" if $DEBUG;
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
      my $conn_name = "net_" . $self->peer_name ( $newhandle );

      warn "Accepting a new handle $newhandle.\n" if $DEBUG;
      if ( $newhandle ) {
	  $self->add($newhandle, $conn_name ) if $newhandle;
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

1;
