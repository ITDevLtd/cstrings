#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <map>
#include <string>
#include <signal.h>

static char buffer[1024];

#define MAP_PAIR(x) { x, #x },
static std::map<uint32_t, std::string> codes = {
	MAP_PAIR(IN_ACCESS)        // File accessed
	MAP_PAIR(IN_ATTRIB)        // Meta data changed
	MAP_PAIR(IN_CLOSE_WRITE)   // File opened for write closed
	MAP_PAIR(IN_CLOSE_NOWRITE) // File/dir not opened for write closed
	MAP_PAIR(IN_CREATE)        // File/dir created
	MAP_PAIR(IN_DELETE)        // File/dir deleted
	MAP_PAIR(IN_DELETE_SELF)   // Watched file/dir itself deleted
	MAP_PAIR(IN_MODIFY)        // File modified
	MAP_PAIR(IN_MOVE_SELF)     // Watched file/dir itself moved
	MAP_PAIR(IN_MOVED_FROM)    // Old dir when file moved
	MAP_PAIR(IN_MOVED_TO)      // New dir when file moved
	MAP_PAIR(IN_OPEN)          // File/dir opened
	MAP_PAIR(IN_IGNORED)       // Watch was removed
	MAP_PAIR(IN_ISDIR)         // Subject of this event is a directory
	MAP_PAIR(IN_Q_OVERFLOW)    // Event queue overflowed (wd is -1)
	MAP_PAIR(IN_UNMOUNT)       // Filesystem containing watched object was unmounted
};

#define IN_ALL (IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE | \
                IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | \
                IN_MOVED_FROM | IN_MOVED_TO | IN_OPEN | IN_IGNORED | IN_ISDIR | \
                IN_Q_OVERFLOW | IN_UNMOUNT)

static volatile bool gblExitRequested = false;

static void handle_signals(int s)
{
   gblExitRequested = true;
}

int main(int argc, char *argv[])
{
	int watchDescriptor;
	ssize_t bytesRead;
	struct inotify_event iNotifyEvent;
	int failure = 0;
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = handle_signals;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	int inotifyFd = inotify_init();
	if (inotifyFd < 0)
	{
		std::cerr << "Failed to create inotify file descriptor" << std::endl;
		return 1;
	}

	watchDescriptor = inotify_add_watch(inotifyFd, argv[1], IN_ALL);
	if (watchDescriptor < 0)
	{
		std::cerr << "Failed to add the directory '" << argv[1] 
		          << "' to the watch list" << std::endl;
		close(inotifyFd);
		return 2;
	}

	while(!gblExitRequested)
	{
		bytesRead = read(inotifyFd, buffer, sizeof(buffer));
		if (bytesRead == -1)
		{
			if (errno == EINTR) continue;
			else                break;
		}
		else if (bytesRead < sizeof(iNotifyEvent))
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

	if (errno != EINTR)
	{
	    if (bytesRead == -1)
	    {
			std::cerr << "Error reading inotify file id" << std::endl;
			std::cerr << "Errno is '" << strerror(errno) << "' (" << errno << ")" << std::endl;
			failure = 3;
	    }
	    else if (bytesRead != sizeof(iNotifyEvent))
	    {
	       std::cerr << "Error reading inotify file id. Unexpected number of bytes" << std::endl;
	       failure = 4;
	    }
	}


    std::cout << "Ending program..." << std::endl;
    inotify_rm_watch(inotifyFd, watchDescriptor);
    close(inotifyFd);

	return failure;
}