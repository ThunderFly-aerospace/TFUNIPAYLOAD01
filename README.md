# TFUNIPAYLOAD
Reference design of PX4 interface for a universal parameter measuring payload.

PX4 is capable to log some data from UART (Telemetry Port) port.  [Pixhawk standard connector pinout](https://github.com/pixhawk/Pixhawk-Standards/blob/master/DS-009%20Pixhawk%20Connector%20Standard.pdf) is following: 


| Pin        | Signal | Voltage levels  | Read/Write | Write |
| ---------- |:------:| ---------------:|------|------|
| 1 (red)    | Vcc | +5V | Optional | Optional |
| 2 (blk)    | TX (OUT)  |   +3.3 V | PX4 -> Payload | -- |
| 3 (blk)    | RX (IN)   |   +3.3 V | Payload -> PX4 | Payload -> PX4 |
| 4 (blk)    | CTS (IN)  |   +3.3 V | -- | -- |
| 5 (blk)    | RTS (OUT) |   +3.3 V | -- | -- |
| 6 (blk)    | GND       |   GND    | GND | GND |


K tomu, aby data byla přijmuta autopilotem musí mít správnou formu. A to musí se po sériovce posílat [MAVLink v2](https://mavlink.io/en/) pakety. Logovány budou pakety [Tunnel (#385)](https://mavlink.io/en/messages/common.html#TUNNEL).

Jde k tomu použít tato knihovna, která je automaticky generována z konfiguračních souborů: [c_library_v2](https://github.com/mavlink/c_library_v2)

## Příklady
Máme připravené dva příklady. 

#### TFUNIPAYLOAD
je příklad, který poslouchá MAVLINK zprávy z autopilota a posílá tunnel zprávy s náhodnými daty do autopilota. Zdrojový kód je [TFUNIPAYLOAD.ino](/SW/arduino/src/TFUNIPAYLOAD/TFUNIPAYLOAD.ino)

#### TFUNIPAYLOAD_MINIMAL
Protože parserování zpráv je náročné na paměť, máme připravený přiklad, který pouze posílá data (HEARTBEAT a TUNNEL zprávy). Tento příklad nevyžaduje připojený TX (z autopilota) vodič. Je vhodný pro MCU s menším množstvím paměti. 

Příklad je [TFUNIPAYLOAD_MINIMAL.ino](/SW/arduino/src/TFUNIPAYLOAD_MINIMAL/TFUNIPAYLOAD_MINIMAL.ino)


#### Funkce
[Funkce na odeslání tunnel paketu](https://github.com/ThunderFly-aerospace/TFUNIPAYLOAD/blob/79eee22fe32725179d1df2b6ca72e901e2be1834/SW/arduino/src/TFUNIPAYLOAD/TFUNIPAYLOAD.ino#L50)

```  mav.SendTunnelData(data, sizeof(data), 0, 1, 0); ```
Tato funkce umožňuje poslat tunnel data do autopilota. Její argumenty jsou:
 * data ve formátu uint8_t [127]
 * délka dat
 * Typ dat (každý payload nebo typ logovaných dat bude mít své ID - na tom se domluvíme spolu - slouží pro snadné odlišení dat
 * Cílové sysid
 * Cílové compid

Pokud budeme chtít data logvat, tak cílové sysid a compid musí být 1, 1 (id autopilota) 

Autopilot má omezené množství paměti. Proto je důležité zajistit na straně payloadu, že nedojde k její zaplňení.


## Nastavení autopilota

V PX4 firmware autopilota je potřeba nastavit správně tyto parametry:
|     Parametr    | Hodnota | Popis |
|-----------------|-----------|------|
| MAV_1_CONFIG    | TELEM 2   | Port, na kterém se budou očekávat MAVLINK pakety. Lze nastavit jakýkoliv, volný, port |
| MAV_1_FORWARD   | **1**     | Zapnutí forwardování zpráv z tohoto portu |
| MAV_1_RADIO_CTL | **0**     | |
| MAV_1_RATE      | **0 B/s** | |
| SER_TEL2_BAUD   |     57600 | Je potřeba konfigurovat port, který je nastavený v parametru `MAV_1_CONFIG`. Nastavuje se zde baudrate. |

Jak nastavit parametry je popsáno v [návodu](http://docs.px4.io/master/en/advanced_config/parameters.html#changing-a-parameter)

## Jak zjistit, jestli autopilot přijímá spravné zprávy?

Existuje několik možností, jak to zjistit.
### V QGC

Zprávu nejsnáze lze zobrazit živě v [QGC](https://github.com/mavlink/qgroundcontrol/releases). Aby tento posutp fungoval, musí být splňeny dvě podmínky. 

 1. Zpráva musí být broadcastovaná. Tj. zpráva musí mít nastavené cílové sysid a compid 0, 0. 
 1. Počítač musí být připojený přes MAVLink instanci, která podporuje přeposílávání zpráv (např. portem TELEM1 - třeba pomocí modemu nebu UART-USB převodníkem) 
 
 > Pozor, tento postup nebude fungovat, pokud je autopilot připojení přes USB. 

Po otevření QGC připojíte autopilota k počítači (modemem/převodníkem). Po spojení autopilota s QGC budou vidět živá data (například náklony autopilota). Následně kliknutím na logo QGC v levém horním rohu se otevře menu, kde vyberete `Analyze tools`. Následně otevřete `MAVLink inspector`. Uvidíte seznam všech správ. 

![obrazek](https://user-images.githubusercontent.com/5196729/99434203-cec17d00-290e-11eb-93a7-e089ba893775.png)


### Pomocí konzole
Výhoda tohoto postupu je, že to není závislé na nastavení broadcastování a zjistíte tím, jestli je zpráva přijatá a rozparserovaná autopilotem. Pokud ji zde uvidíte, tak bude logována (pokud je logování zapnuté). S tímto lze využít USB připojení. 

Do konzole autopilota se lze dostat pomocí python [skriptu](https://github.com/ThunderFly-aerospace/PX4Firmware/blob/master/Tools/mavlink_shell.py). Nebo pomocí [QGC](https://github.com/mavlink/qgroundcontrol/releases). 

 * Pokud používáte python skript, tak stačí spustit tento python3 skript s parametrem sériovky, na které je připojený autopilot/modem. 
 * V případě QGC, je potřeba v QGC otevřít konzoli autopilota. Nahoře kliknout na logo QGC, tam je `Analyze tools` a následně vybrat konzoli. 

V konzoli lze získat přijatou zprávu pomocí příkazu 

`listener tunnel`

Tento příkaz zobrazí aktuální tunnel zprávu. Pokud chcete, aby se vypisovala nová zpráva sama, stačí přidat parametr `-n 100`, čímž se bude vypisovat 100 zpráv. 

Takto by měl vypadat výstup:
![obrazek](https://user-images.githubusercontent.com/5196729/99431661-6ae98500-290b-11eb-80a6-a08f8229d600.png)


### Z logu
Zpráva je v autopilotu logována. Protože nástroje jako [Flight review](https://review.px4.io/) tato data neukazují, je potřeba k tomu použít jiné nástroje. Jedním z nich je [PlotJuggler](https://plotjuggler.io/), ve kterém lze otevřít log a zobrazit tunnel zprávu. 

Dalším způsobem, jak otevřít log je připravený [jupiter notebook](https://github.com/ThunderFly-aerospace/TFUNIPAYLOAD/blob/master/SW/LogViewer/ReadTunnelData.ipynb), kde lze otevřít zaznamenaný log a vypsat tunnel zprávy. 

## Omezení

  * Nezahlcení paměti autopilota zprávami je potřeba zajistit na straně payloadu
  * Tímto způsobem je možné připojit nejvýše 2 zařízení + modem, případně 3 zařízení bez modemu, limit vzniká na straně driveru mavlinku, který umí připojit nejvíce 3 instance mavlink zařízení. 

