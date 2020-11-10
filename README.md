# TFUNIPAYLOAD
Reference design of PX4 interface for a universal parameter measuring payload.

PX4 is capable to log some data from UART port. K tomu, aby data byla přijmuta autopilotem musí mít správnou formu. A to musí se po sériovce posílat [MAVLink v2](https://mavlink.io/en/) pakety. Logovány budou pakety [Tunnel (#385)](https://mavlink.io/en/messages/common.html#TUNNEL).

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
