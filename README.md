# eagletrt-ubx-parser-master

A C program to parse GGA GLL and RMC messages of ubx files to txt.

## Project purpose

The **telemetry of eagletrt** uses two high-precision **GPS**, one as **base-station** and another in the car as a **rover**. 
The rover GPS data are parsed by the telemetry and saved in a local mongodb. The **base-station** saves instead a raw **ubx log** file.
The purpose of this project is making easy to **parse** the wanted data (latitude, longitude timestamp altitude and speed) and save it in an easy-handable
format such as **json** and **csv**.

## How it was made

The project is a very simple C program and uses no third-party module.

## How to use it

To use this parser:

* Clone this repo
* Install gcc
* Navigate to the root folder of this project
* Move the ubx file which are to be exported in the `/inputs folder`
* Open the `config.json` file, it will contain two properties:
  * `TYPE` is the output format. It can be `CSV` or `JSON`
  * `INPUTS` is an array containing the name (without extension) of the file in the inputs folder that are to be parsed
* Execute `./compile.sh`, the parsed file will be in the `outputs/csv` or `outputs/json` folder

## Notes

From the ubx only the **GGA**, **GLL** and **RMC** messages are considered. 

Only the **latitude** **longitude** **timestamp** **altitude** **speed** and **course** properties are kept.

The coordinates are **converted** in a more suitable format, that can be used directly for instance on Google Earth.

Not all messages contain every parameter. In general, every parameter that is not contained in a message is set to **NaN**.
