# TFUNIPAYLOAD - universal interface for atmospheric sensor payload 

Reference design of PX4 interface for [TF-ATMON](https://www.thunderfly.cz/tf-atmon.html) payload.

![TFUNIPAYLOAD block-schematics](./doc/img/block_schematics.svg)


The sensor is connected to the [TFUNIPAYLOAD](https://github.com/ThunderFly-aerospace/TFUNIPAYLOAD01) board using a serial port. 
ATmega in TFUNIPAYLOAD01 runs the Arduino firmware, which prepares [MAVLink](https://en.wikipedia.org/wiki/MAVLink) messages to be logged and transported to GCS running TF-ATMON. 

## Example of wiring

PX4 is capable to log MAVLink data from UART (Telemetry Port) port. [Pixhawk standard connector pinout](https://github.com/pixhawk/Pixhawk-Standards/blob/master/DS-009%20Pixhawk%20Connector%20Standard.pdf) is as follows:

| Pin        | Signal | Voltage levels  | Read/Write | Write |
| ---------- |:------:| ---------------:|------|------|
| 1 (red)    | Vcc | +5V | Optional | Optional |
| 2 (blk)    | TX (OUT)  |   +3.3 V | PX4 -> Payload | -- |
| 3 (blk)    | RX (IN)   |   +3.3 V | Payload -> PX4 | Payload -> PX4 |
| 4 (blk)    | CTS (IN)  |   +3.3 V | -- | -- |
| 5 (blk)    | RTS (OUT) |   +3.3 V | -- | -- |
| 6 (blk)    | GND       |   GND    | GND | GND |

In order for data to be received by PX4 autopilot, it must have a specific form. Explicitly, it needs to use a serial link with  [MAVLink v2](https://mavlink.io/en/) packets. In that case the [Tunnel (#385)](https://mavlink.io/en/messages/common.html#TUNNEL) packets will be stored in autopilot's log file and forwarded to the GCS. 

The following library [c_library_v2](https://github.com/mavlink/c_library_v2), which is automatically generated from message definion files, could be used. 

## TF-ATMON system sensor devices

| Device identification | Data type | Description |
|----------------|---------|-------|
| [TFPM01](https://github.com/ThunderFly-aerospace/TFPM01) | 1 | Particulate matter sensor |
| [TFHT01](https://github.com/ThunderFly-aerospace/TFHT01) | 2 | Humidity and temperature sensor |
| [THUNDERMILL01](https://github.com/UniversalScientificTechnologies/THUNDERMILL01) | 3 | Electric field sensor |
| AIRDOS03 | 4 | Semiconductor-based ionising radiation spectrometer|

## Firmware examples

There are multiple firmware examples for different use cases. 

#### TFUNIPAYLOAD

[TFUNIPAYLOAD.ino](/SW/arduino/src/TFUNIPAYLOAD/TFUNIPAYLOAD.ino) contains a basic example, which listens to MAVLikn messages from autopilot and sends tunnel packets with random data to the autopilot.

#### TFUNIPAYLOAD_MINIMAL

Since message parsering requires a lot of memory, we have prepared an example, where only [HEARTBEAT](https://mavlink.io/en/messages/common.html#HEARTBEAT) and [TUNNEL](https://mavlink.io/en/messages/common.html#TUNNEL) messages are sent. This example does not require a connected TX (from autopilot) and it is therefore suitable for MCUs with less memory.

Source code: [TFUNIPAYLOAD_MINIMAL.ino](/SW/arduino/src/TFUNIPAYLOAD_MINIMAL/TFUNIPAYLOAD_MINIMAL.ino)

#### Tunnel packet sending function

[Tunnel packet sending function](https://github.com/ThunderFly-aerospace/TFUNIPAYLOAD/blob/79eee22fe32725179d1df2b6ca72e901e2be1834/SW/arduino/src/TFUNIPAYLOAD/TFUNIPAYLOAD.ino#L50)

```  mav.SendTunnelData(data, sizeof(data), 0, 1, 0); ```
This function enables sending tunnel data to autopilot. It takes the following as its arguments:
 * data in uint8_t [127] format
 * data length
 * data type (every type of logged data will have its ID - it serves for easy differentiation of different sensors
 * target sysid
 * target compid

If the data only needs to be logged, the target sysid and compid must match the autopilot’s address, which is usually `sysid: 1, compid: 1`. If the data has to be logged and sent to GCS, broadcast must be set: `sysid: 1, compid: 0` nebo dokonce `sysid: 0, compid: 0`. 

The autopilot only has a limited amount of memory (SDcard) and it is therefore necessary to ensure, on the payload’s side, that it does not fill up during the flight. Alternatively, a maximum bandwidth on MAVLink interface can be set in the autopilot. It is not tested what happens when the level set is exceeded[2021/09]. 

## Autopilot configuration

In the autopilot’s PX4 firmware, it is necessary to correctly set the following parameters (MAV_1_FORWARD and others will be visible only after setting the parameter MAV_1_CONFIG and rebooting the PX4): 

|     Parametr    | Hodnota | Popis |
|-----------------|-----------|------|
| [MAV_1_CONFIG](http://docs.px4.io/main/en/advanced_config/parameter_reference.html#mavlink)    | TELEM 2   | Port, na kterém se budou očekávat MAVLINK pakety. Lze nastavit jakýkoliv, volný, TELEM port |
| [MAV_1_FORWARD](http://docs.px4.io/main/en/advanced_config/parameter_reference.html#mavlink)   | **1**     | Zapnutí forwardování zpráv z tohoto portu |
| [MAV_1_RADIO_CTL](http://docs.px4.io/main/en/advanced_config/parameter_reference.html#mavlink) | **0**     | |
| [MAV_1_RATE](http://docs.px4.io/main/en/advanced_config/parameter_reference.html#mavlink)      | **0 B/s** | |
| [SER_TEL2_BAUD](http://docs.px4.io/main/en/advanced_config/parameter_reference.html#serial)   |     57600 | Je potřeba konfigurovat port, který je nastavený v parametru `MAV_1_CONFIG`. Nastavení baudrate portu. |

Jak nastavit parametry v PX4 je popsáno v [návodu](http://docs.px4.io/master/en/advanced_config/parameters.html#changing-a-parameter).

## How to check, that autopilot correctly receives MAVLink messages?

Existuje několik možností, jak zjistit, že posílání zpráv funguje správně.

### Using the QGC

Zprávu nejsnáze lze zobrazit živě v [QGC](https://github.com/mavlink/qgroundcontrol/releases). Aby tento posutp fungoval, musí být splněny dvě podmínky.

 1. Zpráva musí být broadcastovaná. Tj. zpráva musí mít nastavené cílové sysid a compid 0, 0.
 1. Počítač musí být připojený přes MAVLink instanci, která podporuje přeposílávání zpráv (např. portem TELEM1 - třeba pomocí modemu nebo UART-USB převodníkem), USB v autopilotu přeposílání nepodporuje.

 > Pozor, tento postup nebude fungovat, pokud je autopilot připojen USB kabelem.

Po otevření QGC připojíte autopilota k počítači (modemem nebo externím USB převodníkem). Po spojení autopilota s QGC budou vidět živá data (například náklony autopilota). Následně kliknutím na logo QGC v levém horním rohu se otevře menu, kde vyberete `Analyze tools`. Následně otevřete `MAVLink inspector`. Uvidíte seznam všech správ.

![listener mavlin tunnel](https://user-images.githubusercontent.com/5196729/99434203-cec17d00-290e-11eb-93a7-e089ba893775.png)

Seznam zpráv má při správné funkci obsahovat i zprávy TUNNEL. 

### Using the MAVLink PX4 console

Výhoda připojení přes [PX4 cosoli](https://docs.px4.io/main/en/debug/consoles.html) je v tom, že to není závislá na nastavení broadcastování a zjistíte tím, jestli je zpráva přijatá a rozparserovaná autopilotem. Pokud ji zde uvidíte, tak bude i logována (v případě, že je logování zapnuté). S tímto postupem lze využít USB propojení kabelem do autopilota.

Do konzole autopilota se lze dostat pomocí python [skriptu](https://github.com/ThunderFly-aerospace/PX4Firmware/blob/master/Tools/mavlink_shell.py). Nebo pomocí [QGC](https://github.com/mavlink/qgroundcontrol/releases).

 * Pokud používáte python skript, tak stačí spustit python3 skript s parametrem zvolené sériovky, na které je připojený autopilot/modem.
 * V případě QGC, je potřeba v QGC otevřít konzoli autopilota. Nahoře kliknout na logo QGC, tam je `Analyze tools` a následně vybrat konzoli.

V konzoli lze získat přijatou zprávu pomocí příkazu

`listener mavlink_tunnel`

Tento příkaz zobrazí aktuální tunnel zprávu. Pokud chcete, aby se vypisovala nová zpráva sama, stačí přidat parametr `-n 100`, čímž se bude vypisovat 100 zpráv.

Takto by měl vypadat výstup, pokud komunikace funguje:

![mavlink tunnel uorb message](https://user-images.githubusercontent.com/5196729/99431661-6ae98500-290b-11eb-80a6-a08f8229d600.png)


### From a uLog file

Tunnel zpráva může být v autopilotu logována do běžného [formátu uLog](https://docs.px4.io/main/en/dev_log/ulog_file_format.html), ale protože nástroje jako např. [Flight review](https://review.px4.io/) tato data neukazují, tak je potřeba k  k tomu použít vhodný nástroj. Jedním z nich je [PlotJuggler](https://plotjuggler.io/), ve kterém lze otevřít log a zobrazit tunnel zprávu.

Dalším způsobem, jak otevřít log je připravený [jupiter notebook](https://github.com/ThunderFly-aerospace/TFUNIPAYLOAD/blob/master/SW/LogViewer/ReadTunnelData.ipynb), kde lze otevřít zaznamenaný log a vypsat tunnel zprávy.

## Known limitations

  * Nezahlcení paměti autopilota zprávami je potřeba zajistit na straně payloadu
  * Tímto způsobem je možné připojit nejvýše 2 zařízení + modem, případně 3 zařízení bez modemu, limit vzniká na straně driveru mavlinku, který umí připojit nejvíce 3 instance mavlink zařízení a také díky omezeném počtu UART rozhraní autopilota
  * Na vstupu zpráv z payloadu a vystupu do [sik modemu](https://github.com/ThunderFly-aerospace/TFSIK01) nesmí existovat stejny typ mavlink zprav, ktere se budou lisit pouze cetnosti prenosu. Tj. nelze jednoduse redukovat množství dat, ktera se prenaseji na zem vuci mnozstvi logovanych dat. Pokud je potřeba logovat podrobnější data (s větší četností), tak je nutné logovat jinou MAVLink zprávu. 
