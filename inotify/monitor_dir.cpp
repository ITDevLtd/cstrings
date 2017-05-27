#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <map>
#include <string>

static char buffer[1024];
#define MAP_PAIR(x) { x, #x },
static std::map<uint32_t, std::string> codes = {
	MAP_PAIR(IN_ACCESS)
	MAP_PAIR(IN_ATTRIB)
	MAP_PAIR(IN_CLOSE_WRITE)
	MAP_PAIR(IN_CLOSE_NOWRITE)
	MAP_PAIR(IN_CREATE)
	MAP_PAIR(IN_DELETE)
	MAP_PAIR(IN_DELETE_SELF)
	MAP_PAIR(IN_MODIFY)
	MAP_PAIR(IN_MOVE_SELF)
	MAP_PAIR(IN_MOVED_FROM)
	MAP_PAIR(IN_MOVED_TO)
	MAP_PAIR(IN_OPEN)
	MAP_PAIR(IN_IGNORED)
	MAP_PAIR(IN_ISDIR)
	MAP_PAIR(IN_Q_OVERFLOW)
	MAP_PAIR(IN_UNMOUNT)
};

int main(int argc, char *argv[])
{
	int success;
	ssize_t bytesRead;
	struct inotify_event iNotifyEvent;

	int inotifyFd = inotify_init();
	if (inotifyFd < 0)
	{
		std::cerr << "Failed to create inotify file descriptor" << std::endl;
		return 1;
	}

	success = inotify_add_watch(inotifyFd, argv[1], IN_CREATE | IN_DELETE | IN_DELETE_SELF);
	if (success < 0)
	{
		std::cerr << "Failed to add the directory '" << argv[1] << "' to the watch list" << std::endl;
		return 1;
	}

	while(true)
	{
		bytesRead = read(inotifyFd, buffer, sizeof(buffer));
		if ( ((bytesRead == -1) && errno != EINTR) || (bytesRead < sizeof(iNotifyEvent)) )
		{
			break;
		}

		memcpy(&iNotifyEvent, buffer, bytesRead);

		std::cout << "Event info:\n";
	    std::cout << "   Watch descriptor.... " << iNotifyEvent.wd << "\n";
	    std::cout << "   Mask................ " << iNotifyEvent.mask << "\n";
	    std::cout << "   Cookie.............. " << iNotifyEvent.cookie << "\n";
	    std::cout << "   Length of name...... " << iNotifyEvent.len << "\n";
	    std::cout << "   Name................ " << iNotifyEvent.name <<"\n";

	    std::cout << "Event mask includes:\n";
	    for (auto it=codes.begin(); it!=codes.end(); ++it)
	    {
	    	if (iNotifyEvent.mask & it->first)
	    	{
	    		std::cout << "   - " << it->second << "\n";
	    	}
	    }

	    std::cout << std::endl;
	}

    if (bytesRead == -1)
    {
		std::cerr << "Error reading inotify file id" << std::endl;
		std::cerr << "Errno is '" << strerror(errno) << "' (" << errno << ")" << std::endl;
    }
    else if (bytesRead != sizeof(iNotifyEvent))
    {
       std::cerr << "Error reading inotify file id. Unexpected number of bytes" << std::endl;
    }

	return 0;
}