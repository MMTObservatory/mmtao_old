	/*
	  Read the the frame

   	  => Skip the first 4*80 shorts
	*/
	nread = 4*80*sizeof(short) + 7;
	nread -= Socket_Read( Info->socket->sockfd, skipPtr, 4*80*sizeof(short));

	/*
	  => Read the next 72 shorts, convert from network to host format and save into
	     memory.  Do this 72 times.
	     -> Read and throw away 4 edge pixels
	     -> Read 72 pixels, convert ntohl() and put into memory
	     -> Reae and throw away 4 edge pixels
	*/
	nxtShort = (short *)Info->data->dataPtr;
	charPtr = (char *)&tempShort;
	nsaved = 0;
	for ( i=0; i<72; i++) {
	  nread += 4*sizeof(short);
	  nread += Socket_Read( Info->socket->sockfd, skipPtr, 4*sizeof(short));
	  for ( j=0; j<72; j++) {
	    nvalues = Socket_Read( Info->socket->sockfd, charPtr, sizeof(short));
	    nsaved += sizeof(short) - nvalues;
	    nread += sizeof(short) - nvalues;
	    *nxtShort++ = ntohl(tempShort);
	  }
	  nread += 4*sizeof(short);
	  nread -= Socket_Read( Info->socket->sockfd, skipPtr, 4*sizeof(short));
	}

	/*
	  => Skip the first 4*80 shorts
	*/
	nread += 4*80*sizeof(short);
	nread -= Socket_Read( Info->socket->sockfd, skipPtr, 4*80*sizeof(short));

