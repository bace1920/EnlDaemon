# PrcessDaemon
A Daemon to keep your process alive.

## Usage
Run the program and it will creative a config file at work directory  
Edit the config like this:  

    FlushTime(ms)
    PrcessName
    ProcessStartCommand
    ProcessArgumentsNum
    # some note if you need

For example:  

    10000
    yourProgeamName1.exe
    "path\to\yourProgeamName1.exe" -m -n
    2
    yourProgeamName2.exe
    "path\to\yourProgeamName2.exe" -m
    1

Tips: Does not support inline note.
## License

Open sourced under the [GPLv2](LICENSE).
