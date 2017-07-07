/* STL includes */
#include <iostream> // std::cout, std::cin
#include <limits> // std::numeric_limits<std::streamsize>

/** Boost includes **/

/* Boost.program_options */
#include <boost/program_options.hpp> // boost::program_options

/* Boost::log */
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

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

int main(int argc, char* argv[])
{
	try
	{
		// Declare the supported options.
		bpo::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("compression", bpo::value<int>(), "set compression level")
			;

		bpo::variables_map vm;
		bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
		bpo::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << "\n";
			return 1;
		}

		if (vm.count("compression")) {
			std::cout << "Compression level was set to "
				<< vm["compression"].as<int>() << ".\n";
		}
		else {
			std::cout << "Compression level was not set.\n";
		}
	}

	catch (boost::program_options::error& bpoe) // Catch any program-option parsing error
	{
		std::cerr << "Option error: " << bpoe.what() << std::endl;
		return 2;
	}
}