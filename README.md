# PrcessDaemon
A Daemon to restart your process if it dead for any reason.

## Usage
Run the program and it will create a config file at work directory  
Edit the config like this:  

    FlushTime(ms)
    PrcessName
    ProcessStartCommand
    ProcessArgumentsNum
    # some note if you need

For example:  

    10000
    yourProgramName1.exe
    "path\to\yourProgramName1.exe" -m -n
    2
    yourProgramName2.exe
    "path\to\yourProgramName2.exe" -m
    1

Than program will apply your changes automatically.

Tips: Does not support inline note.
## License

Open sourced under the [GPLv2](LICENSE).
