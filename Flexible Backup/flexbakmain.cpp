/* C includes */
#include <stdlib.h> // getenv

/* STL includes */
#include <iostream> // std::cout, std::cin
#include <limits> // std::numeric_limits<std::streamsize>
#include <fstream> // ifstream
#include <sstream> // stringstream

/** Boost includes **/

/* Boost.program_options */
#include <boost/program_options.hpp> // boost::program_options

/* Boost path */

/* Namespace aliases to save typing */
namespace bpo = boost::program_options;
namespace bl = boost::log;
namespace blsrc = boost::log::sources;
namespace blsnk = boost::log::sinks;
namespace blk = boost::log::keywords;

/*
* @name waitForUser
* @desc Prints a message and waits for the user to continue.
* @source http://www.cplusplus.com/forum/articles/7312/
* Modifications are mine.
* @param msg The message to print.
*/
void waitForUser(std::string msg)
{
	std::cout << msg << std::flush;
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}

/**
* \name getHomePath
* \desc Fetches the path to the home directory on both Linux and Windows.
*/
std::string getHomePath()
{
	std::stringstream hpss; // Used to construct the home path

	/*
	* Because we don't know what the OS will be, we need to try several different environment variables.
	*/
	if (getenv("HOME") != NULL) // Linux - HOME env var exists
	{
		hpss << getenv("HOME"); // Use it
	}

	else if (getenv("USERPROFILE") != NULL) // Windows - USERPROFILE exists
	{
		hpss << getenv("USERPROFILE"); // Use it
	}

	else if (getenv("HOMEDRIVE") != NULL && getenv("HOMEPATH") != NULL) // Windows - no USERPROFILE, use HOMEDRIVE and HOMEDIR'
	{
		boost::path
	}

	return hpss.str();
}

int main(int argc, char* argv[])
{
	try
	{
		/** Option handling **/

		/* Option variables */
		std::ifstream configFileStream(""); // Used to read the config file

		/* Set up options */
		bpo::options_description all("All options"); // Object which will hold all options
		all.add_options()
			("help,h", "Produce help message");

		/* Set up option variables */
		bpo::variables_map vm; // Holds option values
		
		store(bpo::parse_config_file(configFileStream, config_file_options), vm);
		notify(vm);

		/* Add options from command line (override config file) */
		store(bpo::command_line_parser(argc, argv).options(all).run(), vm); // Parse all command-line vars
		notify(vm);
	}

	catch (boost::program_options::error& bpoe) // Catch any program-option parsing error
	{
		std::cerr << "Option error: " << bpoe.what() << std::endl;
		return 2;
	}
}