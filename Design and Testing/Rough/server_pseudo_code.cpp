// Initialization

Main Function
{
	Call the Setup Function
	Call the Load Tracklist Function
	Call the Open Socket Listener Function

	If the mode is multicast
	{
		Call the Multicast Function
	}
	Else
	{
		Call the Unicast Function
	}

	Call the Teardown Function
	If the Teardown Function fails
	{
		Print error message
		exit
	}

	exit
}

Setup Function
{
	Parse the command line arguments
	If the command line arguments are invalid
	{
		Print the usage string
		Return false
	}

	Set the mode to argument 1
	Set the port to argument 2

	Start Winsock

	Return true
}

Load Tracklist Function
{
	If the music folder does not exist
	{
		Print error message
		Return false
	}

	If the number of audio tracks is zero
	{
		Print error message
		Return false
	}

	For each audio track in the music folder
	{
		Add the file name to a music list
	} 

	Return true
}

Open Socket Listener Function
{
	Open a listener socket with the specified port and protocol
	If the socket fails to open
	{
		Print error message
		Return false
	}

	Bind the listener socket to an available port
	If the bind fails
	{
		Print error message
		Return false
	}

	Return true
}

Accept New Client Function
{
	accept the client request
	add the client to a list of connected clients
}


Control Message Handler Function
{
	switch on the message type
	{
		case End Connection:
		{
			Close the connection with the specified client
			remove the specified client from the client list
		}

		case Play Song:
		{
			call the Play Song Function
		}

		case Save Song:
		{
			call the Save Song Function
		}
	}
}


// Multicast

Multicast Function
{
	Open a multicast socket
	Connect the multicast socket to a multicast group
	
	Call the Play Music Function
}

Play Music Function
{
	While the done signal has not been received
	{
		Pick a random song from the tracklist
		Open the song file

		Create a new thread to send clients the current song
		
		While we have not reached the end of the file and
		the done signal has not been received
		{
			Send 512 bytes of the song to the multicast socket
		}
		
		If the done signal has not been received
		{
			Call the Load Tracklist Function
		}
	}
}

Send Current Song Function
{
	for each client in the client list
	{
		send a control message with the current song information to the client
	}
}


// Unicast

Unicast Function
{
	While the done signal has not been received
	{
		Do nothing
	}
}

Play Song Function
{
	open the requested file

	While we have not reached the end of the file and
	the done signal has not been received
	{
		Send 512 bytes of the song to the client via UDP
	}

	Send the End Song message to the client
}

Save Song Function
{
	open the requested file

	While we have not reached the end of the file and
	the done signal has not been received
	{
		Send 512 bytes of the song to the client via control channel
	}

	Send End Song message to the client
}


// Teardown

Teardown Function
{
	for each client in the client list
	{
		send an end connection message to the client
	}

	Close the control channel
	If the control channel fails to close
	{
		Print an error message
		Return false
	}

	Stop Winsock

	Return true
}
