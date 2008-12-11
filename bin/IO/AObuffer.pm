package IO::AObuffer;

# was the package IO::LineBufferedSessionData

use strict;

# Notice that perl doesn't set any limits on how big these buffers
# can grow, this is really a sort of "sanity check" used by this
# package.  However, we would like to set it BIG, but have found that
# we need to use small values to get our data?????
# (11/2008 - Duane says this was particularly a problem with the
# Hexapod miniserver with its lengthy "all" command response, but
# is expected to also be the case with the mount and cell).

#use constant BUFSIZE => 5;
use constant BUFSIZE => 1024;

sub new {
  my $package = shift;
  my ($sset,$handle,$name) = @_;

  # tjt - Why in the world do we need non-blocking IO
  # in conjunction with select!  Don't we trust select
  # to tell us when things are readable or not???????
  #
  # (This is what people call belt and suspenders!!)

  # make the handle nonblocking
  #### $handle->blocking(0);

  my $self = bless {
                outbuffer   => '',
                sset        => $sset,
                handle      => $handle,
                write_limit => BUFSIZE,
                name        => $name,
                choker      => undef,
                choked      => undef,
		read_limit  => BUFSIZE,
		inbuffer    => '',
		index       => 0,
		eof         => 0,
		error       => 0,
		tssmode     => 0,
		debug       => 0,
		closed      => 0
               },$package;

  $self->readable(1);

  return $self;
}

sub tssmode {
    my ( $self, $mode ) = @_;

    ##print "aobuffer tssmode called\n";
    $self->{tssmode} = $mode;
}

sub debug {
    $_[0]->{debug} = 1;
}

# Object method: readable($flag)
# Flag the associated IO::SessionSet that we want to do reading on the handle.
sub readable {
  my $self = shift;
  my $is_active = shift;

  return if $self->{writeonly};

  $self->{sset}->activate($self,'read',$is_active);
}

# Object method: writable($flag)
# Flag the associated IO::SessionSet that we want to do writing on the handle.
sub writable {
  my $self = shift;
  my $is_active = shift;

  $self->{sset}->activate($self,'write',$is_active);
}

# Object method: read_limit([$bufsize])
# Get or set the limit on the size of the read buffer.
# Only affects line-oriented reading.
sub read_limit { 
  my $self = shift;

  return defined $_[0] ? $self->{read_limit} = $_[0] 
                       : $self->{read_limit};
}

# Object method: write_limit([$bufsize])
# Get or set the limit on the size of the write buffer.
# Write buffer will grow to this size plus whatever extra you write to it.
sub write_limit { 
  my $self = shift;

  return defined $_[0] ? $self->{write_limit} = $_[0] 
                       : $self->{write_limit};
}

# Object method: pending()
# returns number of bytes pending in the out buffer
sub pending {
    return length shift->{outbuffer}; 
}

# Object method: close()
# Close the session and remove it from the monitored list.
sub close {
    my $self = shift;

    unless ($self->pending) {
	$self->{sset}->delete($self);
	close($self->handle);
    } else {
	$self->readable(0);
	$self->{closing}++;  # delayed close
    }

    # 12-8-2008 - tjt added this to avoid the situation
    # where we were trying to write a delayed response to
    # a session that was closed (the session in question
    # was something like a netcat (but not a netcat) that
    # had send a quick command and not waited for the response.
    $self->{closed} = 1;
}

# just return the whole buffer.
sub read_buffer {
    my $self = shift;

    $_[0] = $self->{inbuffer};
    $self->{inbuffer} = '';
    $self->{index} = 0;
    return length $_[0];
}

# accessor methods
sub error { $_[0]->{error} }
sub eof { $_[0]->{eof} }
sub handle { $_[0]->{handle} }

sub dump_buffer {
	my ( $self, $buf ) = @_;

	$_ = $buf;
	#print length ($_) . "\n";
	while ( /(.)/gs ) {
		if ( $1 =~ /\x0/ ) {
		    print "<NUL>\n";
		    next;
		}
		if ( $1 =~ /\x2/ ) {
		    print "<STX>\n";
		    next;
		}
		if ( $1 =~ /\x3/ ) {
		    print "<ETX>\n";
		    next;
		}
		if ( $1 =~ /\n/ ) {
		    print "<NL>\n";
		    next;
		}
		if ( $1 =~ /\r/ ) {
		    print "<CR>\n";
		    next;
		}
		if ( $1 =~ /\x7f/ ) {
		    print "<DEL>\n";
		    next;
		}
		if ( $1 =~ /[\x00-\x1f]/ ) {
		    print "<lobin>\n";
		    next;
		}
		if ( $1 =~ /[\x80-\xff]/ ) {
		    print "<hibin>\n";
		    next;
		}
		print $1, "\n";
	}
}

# This extracts (from getline below) the business
# of checking the buffer for a complete line.
# new version, for TSS allows ascii or binary.
sub check_line {
    my $self = shift;

    # discard the caller's buffer
    # note that to return values out of this routine
    # to the caller, we MUST acess the $_ list, not a copy
    # of the references
    undef $_[0];
    $_[1] = 0;

    # This is added to support devices that return info without a newline
    # on the end, then close the connection (like the TempTrax).
    if ( $self->{eof} ) {
        $_[0] = $self->{inbuffer};
        $self->{inbuffer} = '';
        return length $_[0];
    }

    return -1 if $self->{error};

    # If inbuffer is gone, then we encountered a read error and returned
    # everything we had on a previous pass.  So return undef.

    print "check_line: " . $self->{inbuffer} . "\n" if $self->{debug};

    #print "check_line: " . length ( $self->{inbuffer} ) . " bytes\n";
    #$self->dump_buffer ( $self->{inbuffer} );
    #print "check_line tssmode is: " . $self->{tssmode} . "\n";

    # Look up position of the line end character in the buffer.
    # the "index" is just an optimization so we don't search
    # the first part of the string over and over.

    if ( $self->{tssmode} ) {
        my $si;
        my $ei;
        my $ni;

        # Look up position of the binary start marker (if any).
        $si = index($self->{inbuffer},"\x02",$self->{index});
	# And position of ascii end marker (if any).
        $ni = index ( $self->{inbuffer}, $/, $self->{index} );
	##print "check_line: (si,ni) = " . $si . " " . $ni . "\n";

	# If we have both an ascii end marker and
	# a binary start marker, we only call this an ascii
	# reply if the newline precedes the binary start.
	# If we only see an ascii end, no problem, and
	# we handle that here too.
	if ( $ni >= 0 ) {
	    if ( $si < 0 || $ni < $si ) {
		# remove the line from the input buffer.
		$_[0] = substr($self->{inbuffer},0,$ni+1);
		substr($self->{inbuffer},0,$ni+1) = '';
		$self->{index} = 0;
		#print "check_line, tss ascii hit: $_[0]\n";
		return length $_[0];
	    }
	}

        # No binary start marker
        return undef if $si < 0;

        # discard stuff prior to the start,
        # but NOT the start marker!!
        if ( $si > 0 ) {
            substr ( $self->{inbuffer},0,$si) = '';
	    $si = 0;
        }

        $self->{index} = 0;

        # Look up position of the end marker.
        $ei = index($self->{inbuffer},"\x03",0);

        # didn't find end marker.
        # Can't adjust index, in this mode, we
        # can never move it past the start marker.
        # (unless we want to keep state here)
        return undef if $ei < 0;

	# extract the "guts" of an STX/ETX wrapped string.
	# note that the STX/ETX are stripped here.
        $_[0] = substr($self->{inbuffer},1,$ei-1);
        substr($self->{inbuffer},0,$ei+1) = '';

	# set the flag indicating binary tss style action
	$_[1] = 1;

	#print "check_line, tss binary hit: $_[0]\n";

        ## XXX - for some reason, if we don't do this, we get
        ## bizarre behavior where we get into a loop where select
        ## says the socket is ready to read, and read says WOULDBLOCK.
        ## why emptying the buffer here (which select and read whould
        ## know nothing about) fixes this is a mystery to me.
        #$self->{inbuffer} = '';

        $self->{index} = 0;
	return length $_[0];

    } else {
        my $i = index ( $self->{inbuffer}, $/, $self->{index} );
        if ( $i >= 0 ) {
            # remove the line from the input buffer.
            $_[0] = substr($self->{inbuffer},0,$i+1);
            substr($self->{inbuffer},0,$i+1) = '';
            $self->{index} = 0;
            return length $_[0];
        }
    }

    # XXX - handle the buffer limit properly.

    # Do not have a complete line yet,
    # just update search pointer and return.
    $self->{index} = length $self->{inbuffer};
    return undef;
}

# This extracts (from getline below) the business
# of checking the buffer for a complete line.
# OLD version had the binary only tssmode algorithm.
###sub check_line_OLD {
###    my $self = shift;
###
###    # empty the caller's buffer
###    undef $_[0];
###
###    # If inbuffer is gone, then we encountered a read error and returned
###    # everything we had on a previous pass.  So return undef.
###
###    print "check_line: " . $self->{inbuffer} . "\n" if $self->{debug};
###
###    # This is added to support devices that return info without a newline
###    # on the end, then close the connection (like the TempTrax).
###    if ( $self->{eof} ) {
###        $_[0] = $self->{inbuffer};
###        $self->{inbuffer} = '';
###        return length $_[0];
###    }
###
###    return -1 if $self->{error};
###
###    # Look up position of the line end character in the buffer.
###    # the "index" is just an optimization so we don't search
###    # the first part of the string over and over.
###
###    if ( $self->{tssmode} ) {
###        my $si;
###        my $ei;
###
###        # Look up position of the start marker.
###        $si = index($self->{inbuffer},"\x02",$self->{index});
###
###        # No start marker, purge buffer
###        if ( $si < 0 ) {
###            $self->{inbuffer} = '';
###            $self->{index} = 0;
###            return;
###        }
###
###        # discard stuff prior to the start,
###        # but NOT the start marker!!
###        if ( $si > 0 ) {
###            substr ( $self->{inbuffer},0,$si) = '';
###        }
###
###        $self->{index} = 0;
###
###        # Look up position of the end marker.
###        $ei = index($self->{inbuffer},"\x03",0);
###
###        # didn't find end marker.
###        # Can't adjust index, in this mode, we
###        # can never move it past the start marker.
###        # (unless we want to keep state here)
###        if ( $ei < 0 ) {
###            return;
###        }
###
###        $_[0] = substr($self->{inbuffer},1,$ei-1);
###        substr($self->{inbuffer},0,$ei+1) = '';
###
###        ## XXX - for some reason, if we don't do this, we get
###        ## bizarre behavior where we get into a loop where select
###        ## says the socket is ready to read, and read says WOULDBLOCK.
###        ## why emptying the buffer here (which select and read whould
###        ## know nothing about) fixes this is a mystery to me.
###        #$self->{inbuffer} = '';
###
###        $self->{index} = 0;
###        return length $_[0];
###
###    } else {
###        my $i = index ( $self->{inbuffer}, $/, $self->{index} );
###        if ( $i >= 0 ) {
###            # remove the line from the input buffer.
###            $_[0] = substr($self->{inbuffer},0,$i+1);
###            substr($self->{inbuffer},0,$i+1) = '';
###            $self->{index} = 0;
###            return length $_[0];
###        }
###    }
###
###    # XXX - handle the buffer limit properly.
###
###    # Do not have a complete line yet,
###    # just update search pointer and return.
###    $self->{index} = length $self->{inbuffer};
###    return;
###}

# This is "the other part" of getline, which gets called
# whenever wait (aka select) tells us we should and can
# do a read.
sub do_read {
    my $self = shift;
    my $rc;

    return unless my $handle = $self->handle;

    $rc = sysread ( $handle, $self->{inbuffer}, BUFSIZE, length $self->{inbuffer} );

    print "do_read: $rc\n" if $self->{debug};

    unless (defined $rc) {  # error
      $self->{error} = $!;
    } elsif ($rc == 0) {    # EOF
      $self->{eof} = 1;
    }
}

# Object method: adjust_state()
# Called periodically from within write() to control the
# status of the handle on the IO::SessionSet's IO::Select sets
sub adjust_state {
  my $self = shift;

  # make writable if there's anything in the out buffer
  $self->writable($self->pending > 0);

  # make readable if there's no write limit, or the amount in the out
  # buffer is less than the write limit.
  $self->choke($self->write_limit <= $self->pending) if $self->write_limit;

  # Try to close down the session if it is flagged
  # as in the closing state.
  $self->close if $self->{closing};
}

# Object method: write($scalar)
# $obj->write([$data]) -- append data to buffer and try to write to handle
# Returns:
#  number of bytes written,
#  On other errors, returns undef.

sub write {
  my $self = shift;
  my $rc;

  # gotta have a handle and buffer set up.
  return unless my $handle = $self->handle;
  return unless defined $self->{outbuffer};
  return if $self->{closed};

  $self->{outbuffer} .= $_[0] if defined $_[0];

  if ($self->pending) { # data in the out buffer to write

    local $SIG{PIPE}='IGNORE';
    $rc = syswrite($handle,$self->{outbuffer});

    # able to write, so truncate out buffer apropriately
    if ($rc) {
      substr($self->{outbuffer},0,$rc) = '';
    } else { # some sort of write error.
      return $self->bail_out($!);
    }
  }
  
  $self->adjust_state;

  # Result code is the number of bytes successfully transmitted
  return $rc;
}

# Object method: bail_out([$errcode]) - used for write errors.
# Called when an error is encountered during writing (such as a PIPE).
# Default behavior is to flush all buffered outgoing data and to close
# the handle.
sub bail_out {
  my $self = shift;
  my $errcode = shift;           # save errorno

  delete $self->{outbuffer};     # drop buffered data
  $self->close;

  $! = $errcode;                 # restore errno
  return;
}

# set a callback to be called when the contents of the write buffer becomes larger
# than the set limit.
sub set_choke {
  my $self = shift;
  return defined $_[0] ? $self->{choker} = $_[0] 
                       : $self->{choker};
}

# choke gets called when the contents of the write buffer are larger
# than the limit.  The default action is to inactivate the session for further
# reading until the situation is cleared.
sub choke {
  my $self = shift;
  my $do_choke = shift;

  # Next two lines added by JDG, 11/7/2004, because of following error:
  # Use of uninitialized value in numeric eq (==) at /mmt/dataserver/IO/SessionData.pm line 147.
  return unless defined($do_choke);
  return unless defined($self->{choked});

  return if $self->{choked} == $do_choke;  # no change in state
  
  if (ref $self->set_choke eq 'CODE') {
    $self->set_choke->($self,$do_choke);
  } else {
    $self->readable(!$do_choke);
  }
  $self->{choked} = $do_choke;
}

1;

## XXX - I sincerely doubt this will work anymore
##    tjt  3-6-2008
##
## This is a special version of getline hacked by tjt  12-12-2007
## It is looking for commands that begin with 0x02 and end with 0x03
## This is used to simulate the AO tss protocol.
#sub aoline {
#    my $self = shift;
#    my $si;
#    my $ei;
#
#    croak "usage: aoline(\$scalar)\n" unless @_ == 1;
#
#    return unless my $handle = $self->handle;
#
#    undef $_[0];  # empty the caller's scalar
#
#    # see if there is room for more stuff in the buffer.
#    # if not, something is wrong, ditch what is there.
#    if ( $self->buffered >= $self->read_limit ) {
#        $self->{inbuffer} = '';
#        $self->{index} = 0;
#    }
#
#    # third argument is offset into buffer.
#    # XXX  --  calls aoread, but just read would probably do ...
#
#    my $rc = $self->SUPER::aoread ( $self->{inbuffer}, BUFSIZE, $self->buffered );
##    print "aoline READ $rc\n";
#
#    unless ( defined $rc ) {  # we got an error
#        return '0E0' if $! == EWOULDBLOCK;  # wouldblock is OK
#        $_[0] = $self->{inbuffer};          # return whatever we have left
#        $self->{error} = $!;                # remember what happened
#        return length $_[0];                # and return the size
#    } 
#
#    # tjt adds this since read sometimes gives us '0E0'
#    # - in fact it ALWAYS does this on the first call
#    #   after a connection is made.
#    return '0E0' if $rc eq '0E0';
#
#    if ( $rc == 0 ) {    # we got EOF
#      $_[0] = $self->{inbuffer};          # return whatever we have left
#      $self->{eof}++;                     # remember what happened
#      return length $_[0];
#    }
#
#    # If inbuffer is gone, then we encountered a read error and returned
#    # everything we had on a previous pass.  So return undef.
#    return 0 if $self->{eof};
#    return   if $self->{error};
#
#}

## Object method: read($scalar,$length [,$offset])
## Just like sysread(), but returns:
##  the number of bytes read on success,
##  0EO ("0 but true") if the read would block,
##  and undef on EOF and other failures.
#
#sub read {
#  my $self = shift;
#  my $off = $_[2] || 0;
#  my $rc;
#
#  return unless my $handle = $self->handle;
#
##  $rc = sysread($handle,$_[0],$_[1],$_[2]||0);
#  $rc = sysread ( $handle, $_[0], $_[1], $off );
#
#  return $rc if defined $rc;
#  return '0E0' if $! == EWOULDBLOCK;
#  return;
#}
#
## forces the offset to zero.
## probably un-necessary.
#
#sub aoread {
#  my $self = shift;
#  my $off = 0;
#
#  return unless my $handle = $self->handle;
#
##  print "SessionData read: $_[0], $_[1], $off\n";
#  my $rc = sysread ( $handle, $_[0], $_[1], $off );
##  print "SessionData read returns: $rc\n";
#
#  return $rc if defined $rc;
#  return '0E0' if $! == EWOULDBLOCK;
#  return;
#}
