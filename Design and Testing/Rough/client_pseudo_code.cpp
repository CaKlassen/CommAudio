// Initialization

Main Function
{
	Start WinSock
	Create the window
	
	while there are messages left to receive
	{
		handle a message
	}
	
	exit
}


// Idle

Settings Function
{
	Create Settings popup
	Retrieve IP and port from user
	
	return IP and port
}

Help Button Function
{
	Display help information
}

Mic Button Function
{
	set the mode to mic mode
	open a UDP socket for the Receive Microphone Data Function
}

Music Button Function
{
	set the mode to music mode
}

Connect Button Function
{
	call the Connect Function
}

Disconnect Button Function
{
	call the Disconnect Function
	
	if mode is music
	{
		send the server an End Connection message
		close the server control socket
	
		if server mode is unicast
		{
			close the server stream socket
			close the server file saving socket
		}
		else
		{
			unsubscribe from the server's multicast session
			close the server multicast socket
		}
	}
	else
	{
		send the client an End Connection message
		close the client communication socket
		close the client control socket
	}
}


// Connect

Connect Function
{
	Open a TCP socket for the Control Message Handler Function
	Connect to the client/server
	if connection fails
	{
		return false
	}
	
	if the mode is music
	{		
		create a thread on the Play Music Function
	}
	else
	{
		create a thread on the Microphone Function
	}
	
	set connection mode to connected
	return true
}


// Music

Control Message Handler Function
{
	switch on the message type
	{
		case End Connection:
		{
			call the Disconnect Function
		}
		
		case Start Connection:
		{
			set server mode to message data
			
			if the server mode is multicast
			{
				set the application view to multicast mode
			}
			else
			{
				set the application view to unicast mode
			}
		}
		
		case Track List:
		{
			call Set Track List Function with the message data
		}
		
		case Now Playing:
		{
			call the Set Now Playing Function with the message data
		}
		
		case Song Piece:
		{
			add the message data to the save to file buffer
		}
		
		case End Song:
		{
			if the server mode is multicast
			{
				set the song done flag to true
			}
			else
			{
				save the contents of the save to file buffer
			}
		}
	}
}

Play Music Function
{
	while server mode is nothing
	{
		Do nothing
	}

	if server mode is unicast
	{
		return
	}
	else
	{
		create a multicast socket for the Add To Music Buffer Function
		subscribe to the server's multicast session
	
		while connected
		{
			while the music stream buffer has less than 2 messages worth of data
			{
				do nothing
			}
			
			while the song done flag has not been set and we are still connected
			{
				play music from the buffer
			}
			
			set the song done flag to false
		}
	}
}

Add To Music Buffer Function
{
	add the data to the music stream buffer
}

Set Track List Function
{
	clear the existing track list

	loop through each song in the song list
	{
		add it to the track list
	}
}

Set Now Playing Function
{
	set the song name to the song name portion of the message
	set the artist to the artist portion of the message
	set the album to the album portion of the message
	set the length to the length portion of the message
}

Stream Button Function
{
	Pull the selected song from the list
	Send the server a Play Song control message
	
	create a thread on Stream Music Function
}

Stream Music Function
{
	Open a UDP listener on the Add To Music Buffer Function
	
	while connected
	{
		while the music stream buffer has less than 2 messages worth of data
		{
			do nothing
		}
		
		while the song done flag has not been set and we are still connected
		{
			play music from the buffer
		}
		
		set the song done flag to false
	}
}

Mute Button Function
{
	if the volume is 0
	{
		set the volume to max volume
	}
	else
	{
		set the volume to 0
	}
}

Save Button Function
{
	pull the selected song from the list
	send the Save Song message to the server
}


// Microphone

Microphone Function
{
	create a UDP socket to the client
	create a thread on the Listen To Microphone Function
	
	while we are connected
	{
		if the microphone buffer has more than 2 messages of data
		{
			while there is data left in the microphone buffer and we are still connected
			{
				play audio from the microphone buffer
			}
		}
	}
}

Listen To Microphone Function
{
	Read data from the microphone
	Send it to the client
}

Receive Microphone Data Function
{
	add the data to the microphone buffer
}


// Teardown

Disconnect Function
{
	close the connection with the server / client
	set connection mode to disconnected
	set server mode to nothing
	clear the music stream buffer
	clear the save to file buffer
}