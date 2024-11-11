# OPTA-PROG-MODBUS-TCP-IP-NETWORK
This programming aims to establish communication between 4 OPTA's through Modbus TCP/IP

# The project

The project consists of creating a Client and Server server with the Finder OPTA. In this case, this project consists of 4 OPTAs, one of which is the Advanced model and the other three are the Lite model.
The OPTA Advanced acts as a Client, as it requests readings from the OPTA Lites. We then have Modbus TCP/IP communication between the products, with the three OPTA Lites aiming to read 0-10V signals at their inputs and the same signal read at the inputs will be mirrored at the ports of the analog expansion modules.
So if the OPTA is reading 3V at one of the inputs, the ports of the expansion module will also have 3V output.
The OPTA Advanced receives this data from the three OPTA Lites and these readings are made available on the Arduino IoT Cloud.

In total, the following were used:

1x OPTA Advanced
3x OPTA's Lite
5x Analog expansion modules.

Programming with endings 1 and 2 simply reads the OPTA inputs and this signal is mirrored on the outputs of the analog expansion modules corresponding to the programming.

Programming with endings 3 has some additional features, since in this case 1 OPTA Lite and 3 analog expansion modules are used. This programming works as follows:

OPTA Lite - Reads the 0-10V signals on the inputs
Analog expansion 1 - mirrors the value read from the OPTA inputs on the output of this same expansion.

Analog expansion 2 - reads the 0-10V signals on the inputs of this same expansion

Analog expansion 3 - mirrors the value read on the inputs of analog expansion 2 and mirrors this signal read on the outputs of expansion 3.

In this case we are totaling 30 read circuits and also 3 dimmed circuits.

# Does the programming work OFFLINE?

Yes, the product will work if it loses its connection to the Cloud. In this case, communication is independent of OPTA Advanced.
So if OPTA Advanced loses its connection to the Cloud, we will have a product reading and mirroring the signals without any problems.

# Cloud Synchronization

The Modbus server is only enabled when the OPTA Advanced is synchronized with the Cloud, so after synchronization, you will notice that the LAN LEDs are blinking (exchanging data).

If the product loses connection with the Cloud, it will be necessary to restart the system by removing all power and reconnecting it so that the Modbus server is enabled again. This condition can be changed in the programming if necessary.

Remember that it is not necessary to open the IDE Serial Monitor to enable the Modbus server.

# IP Configuration
The IP addresses are divided as follows:

OPTA LITE 1 - Ending IP 106

OPTA LITE 2 - Ending IP 107

OPTA LITE 3 - Ending IP 108

OPTA ADVANCED - Ending IP 109.

# How is a TCP/IP connection made?

All 4 OPTA's communicate via TCP/IP, where each Ethernet cable is connected to a HUB.
