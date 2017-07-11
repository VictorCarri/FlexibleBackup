/* C includes */
#include <stdlib.h> // _dupenv_, free, getenv

/* STL includes */
#include <iostream> // std::cout, std::cin
#include <limits> // std::numeric_limits<std::streamsize>
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <stdexcept> // runtime_error

/** Boost includes **/
#include <boost/program_options.hpp> // boost::program_options
#include <boost/filesystem.hpp> // boost::filesystem

/* Namespace aliases to save typing */
namespace bpo = boost::program_options;
namespace bf = boost::filesystem;

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
	std::stringstream pathss; // Used to generate the path to the home dir

	/*
	* Need different code for different OSes.
	*/
	#if defined(_WIN32) || defined(_WIN64) // Windows
		char* HOMEDRIVE = nullptr; // Will hold HOMEDRIVE env var
		size_t HOMEDRIVElen; // Length of HOMEDRIVE env var
		char* HOMEPATH = nullptr; // Will hold HOMEPATH env var
		size_t HOMEPATHlen; // Will hold HOMEPATH env var length

		/* Fetch HOMEDRIVE */
		errno_t err = _dupenv_s(&HOMEDRIVE, &HOMEDRIVElen, "HOMEDRIVE"); // Call dupenv to get the value of the environment variable

		if (err) // An error occurred
		{
			std::stringstream ss; // Used to create the error message
			ss << "getHomePath: couldn't duplicate HOMEDRIVE variable: dupenv returned " << err; // Create the error message
			std::runtime_error dupex(ss.str()); // Create an exception with the error message

			/* Cleanup */
			free(HOMEDRIVE);
			free(HOMEPATH);

			throw dupex; // Throw the exception to be handled
		}

		std::clog << "HOMEDRIVE = " << HOMEDRIVE << std::endl;

		/* Fetch HOMEPATH */
		err = _dupenv_s(&HOMEPATH, &HOMEPATHlen, "HOMEPATH"); // Call dupenv to get the value of the environment variable

		if (err) // An error occurred
		{
			std::stringstream ss; // Used to create the error message
			ss << "getHomePath: couldn't duplicate HOMEPATH variable: dupenv returned " << err; // Create the error message
			std::runtime_error dupex(ss.str()); // Create an exception with the error message
			
			/* Cleanup */
			free(HOMEDRIVE);
			free(HOMEPATH);

			throw dupex; // Throw the exception to be handled
		}

		std::cout << "HOMEPATH = " << HOMEPATH << std::endl;

		/* Create the home path */
		pathss << HOMEDRIVE << HOMEPATH; // Concatenate the drive and the 

		/* Cleanup */
		free(HOMEDRIVE);
		free(HOMEPATH);

	#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)) /* UNIX-like system, can just use the HOME var */
		std::cout << "HOME = " << getenv("HOME");
		pathss << getenv("HOME"); // Fetch the HOME path from the env var

	#endif //  __WIN32__

	return pathss.str();
}

int main(int argc, char* argv[])
{
	try
	{
		/** Option handling **/

		/* Option variables */
		std::ifstream configFileStream(""); // Used to read the config file

		/* Set up options and default values */
		bpo::options_description all("All options"); // Object which will hold all options
		all.add_options()
			("help,h", "Produce help message")
			("location,l", bpo::value<std::string>(), "The place where the backup folder (with the backups inside) should be stored. If the folder doesn't exist, it will be created. Relative to the starting directory.");

		/* Set up option variables */
		/*bpo::variables_map vm; // Holds option values
		
		store(bpo::parse_config_file(configFileStream, config_file_options), vm);
		notify(vm);

		/* Add options from command line (override config file) */
		/*store(bpo::command_line_parser(argc, argv).options(all).run(), vm); // Parse all command-line vars
		notify(vm);*/
		std::clog << "Path to home dir: " << getHomePath() << std::endl; // TESTING

		// Wait for user (debugging)
		waitForUser("Press any key to continue...");
	}

	catch (boost::program_options::error& bpoe) // Catch any program-option parsing error
	{
		std::cerr << "Option error: " << bpoe.what() << std::endl;
		return 1;
	}

	catch (std::runtime_error& sre) // Generic error
	{
		std::cerr << "Runtime error: " << sre.what() << std::endl;
		return 2;
	}
}