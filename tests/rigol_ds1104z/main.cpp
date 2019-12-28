/**
	@file
	@author Andrés MANELLI
	@brief Program entry point
 */

#include "../scopehal/scopehal.h"
#include "../scopehal/Instrument.h"
#include "../scopehal/RigolOscilloscope.h"

using namespace std;

int main(int argc, char* argv[])
{
	Severity console_verbosity = Severity::DEBUG;

	string shost = "";
	string sport = "";
	string sdelay = "";

	//Parse command-line arguments
	for(int i=1; i<argc; i++)
	{
		string s(argv[i]);

		//Let the logger eat its args first
		if(ParseLoggerArguments(i, argc, argv, console_verbosity))
			continue;

		if(s == "--help")
		{
			//not implemented
			return 0;
		}
		else if(s == "--host")
			shost = argv[++i];
		else if(s == "--port")
			sport = argv[++i];
		else if(s == "--delay")
			sdelay = argv[++i];
		else
		{
			fprintf(stderr, "Unrecognized command-line argument \"%s\", use --help\n", s.c_str());
			return 1;
		}
	}

	//Set up logging
	g_log_sinks.emplace(g_log_sinks.begin(), new ColoredSTDLogSink(console_verbosity));

	if (shost == "")
		shost = "210.0.0.120";
	if (sport == "")
		sport = "5555";
	if (sdelay == "")
		sdelay = "250000";

	//Connect to the scopes and sanity check
	RigolOscilloscope scope(new SCPISocketTransport(shost, stoi(sport)));

	if(0 == (scope.GetInstrumentTypes() & Instrument::INST_OSCILLOSCOPE))
	{
		LogError("not an oscilloscope\n");
		return 1;
	}

	//Set up the scope
	int delay = stoi(sdelay);
	usleep(delay);
	scope.EnableChannel(0);
	usleep(delay);
	scope.DisableChannel(1);
	usleep(delay);
	scope.DisableChannel(2);
	usleep(delay);
	scope.DisableChannel(3);
	usleep(delay);

	scope.SetChannelAttenuation(0, 100);
	usleep(delay);
	scope.SetChannelAttenuation(0, 20);
	usleep(delay);
	scope.SetChannelAttenuation(0, 0.05);
	usleep(delay);
	scope.SetChannelAttenuation(0, 1);
	usleep(delay);

	scope.SetChannelBandwidthLimit(0, 0);
	usleep(delay);
	scope.SetChannelBandwidthLimit(0, 30);
	usleep(delay);
	scope.SetChannelBandwidthLimit(0, 20);
	usleep(delay);
	scope.SetChannelBandwidthLimit(0, 0);
	usleep(delay);

	// As attenuation is 1x, range shall be >= 8mv and <= 80V
	scope.SetChannelVoltageRange(0, 85);
	usleep(delay);
	scope.SetChannelVoltageRange(0, 10);
	usleep(delay);
	scope.SetChannelVoltageRange(0, 0.5);
	usleep(delay);
	scope.SetChannelVoltageRange(0, 0.01);
	usleep(delay);
	scope.SetChannelVoltageRange(0, 10);
	usleep(delay);

	//Main loop
	LogNotice("freq_mhz,ref_mv,probe_mv,gain_db\n");
	//for(float mhz = 0.5; mhz <= 10; mhz += 0.01)
	while(true)
	{
		//Configure the function generator and wait a little while (there's some lag on the output)
		usleep(1000 * 50);

		//LogNotice();
		fflush(stdout);
	}

	return 0;
}
