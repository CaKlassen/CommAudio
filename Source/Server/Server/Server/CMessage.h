#ifndef CMESSAGE_H_
#define CMESSAGE_H_

#include <string>
#include <vector>

using std::vector;
using std::string;

/* All possible message types */
enum MessageType { START_CONNECTION };
typedef enum MessageType MessageType;

/* The struct to parse data into and create data from */
struct CMessage
{
	MessageType msgType;
	vector<string> msgData;
};
typedef struct CMessage CMessage;

#endif