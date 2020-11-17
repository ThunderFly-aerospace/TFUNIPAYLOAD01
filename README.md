# TFUNIPAYLOAD
Reference design of PX4 interface for a universal parameter measuring payload.

PX4 is capable to log some data from UART (Telemetry Port) port.  [Pixhawk standard connector pinout](https://github.com/pixhawk/Pixhawk-Standards/blob/master/DS-009%20Pixhawk%20Connector%20Standard.pdf) is following: 


| Pin        | Signal | Voltage levels  |
| ---------- |:------:| ---------------:|
| 1 (red)    | Vcc | +5V |
| 2 (blk)    | TX (OUT)  |   +3.3 V |
| 3 (blk)    | RX (IN)   |   +3.3 V |
| 4 (blk)    | CTS (IN)  |   +3.3 V |
| 5 (blk)    | RTS (OUT) |   +3.3 V |
| 6 (blk)    | GND       |   GND    |


K tomu, aby data byla přijmuta autopilotem musí mít správnou formu. A to musí se po sériovce posílat [MAVLink v2](https://mavlink.io/en/) pakety. Logovány budou pakety [Tunnel (#385)](https://mavlink.io/en/messages/common.html#TUNNEL).

Jde k tomu použít tato knihovna, která je automaticky generována z konfiguračních souborů: [c_library_v2](https://github.com/mavlink/c_library_v2)

Příklad komunikace payloadu založeném na arduinu s autopilotem je zde:
[TFUNIPAYLOAD.ino](/SW/arduino/src/TFUNIPAYLOAD/TFUNIPAYLOAD.ino)

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


## Jak zjistit, jestli autopilot přijímá spravné zprávy?

Existuje několik možností, jak to zjistit. Nejsnazší je pomocí QGC. 

### V QGC

Zpráva je vidět v QGC, pokud zpráva má být broadcastovaná do celé sítě (nebo na adresu QGC). Tj. zpráva musí mďt nastavené cílové sysid a compid 0, 0. 

Následně v časti `mavlink analyzer` je vidět seznam poslaných zpráv. 

### Pomocí konzole
Do konzole autopilota se lze dostat pomocí python [skriptu](). Nebo pomocí [QGC](). 

Pokud používáte python skript, tak stačí spustit tento skript s parametrem sériovky, na které je připojený autopilot/modem. 

V případě QGC je potřeba v QGC otevřít konzoli autopilota. Nahoře kliknout na logo QGC, tam je obrázek logů a následně vybrat konzoli. 


V konzoli lze získat přijatou zprávu pomocí příkazu 

`listener tunnel`

Tento příkaz zobrazí aktuální tunnel zprávu. Pokud chcete, aby se vypisovala nová zpráva sama, stačí přidat parametr `-n 100`, čímž se bude vypisovat 100 zpráv. 



### Z logu

Zpráva je v autopilotu logována. 
