# STM32 Modbus RTU Master

Firmware-Demonstrator fÃ¼r zyklische Modbus-RTU-Kommunikation Ã¼ber RS485. Die Anwendung verbindet UART-DMA, CRC16-PrÃ¼fung und eine timergetriebene Telegrammfolge mit digitalen Ein-/AusgÃ¤ngen und einem ADC-Wert.

**Status:** Dokumentierter Entwicklungsstand. Noch kein nachgewiesener Hardwaretest dieser bereinigten Kopie; bekannte Fehler sind offen. Keine Aussage zur Produktionsreife.

## Technischer Umfang

- Zielcontroller laut Projektdateien: STM32F103C8Tx.
- USART2 mit DMA-Empfang und DMA-Senden; 9600 Baud, gerade ParitÃ¤t, ein Stopbit; HAL-Konfiguration: 9-Bit-WortlÃ¤nge mit ParitÃ¤t.
- GPIO-gesteuerte RS485-Senderichtung fÃ¼r einen im Code als MAX485 bezeichneten Transceiver.
- CRC16 und Funktionscodes `0x0F`, `0x10`, `0x02`, `0x03`.
- TIM4 startet die nÃ¤chste Ãœbertragung; UART-Callbacks setzen die Abfolge fort.

## Ablauf im vorhandenen Code

`Digitale EingÃ¤nge â†’ Write Multiple Coils â†’ ADC-Wert / Write Multiple Registers â†’ Read Discrete Inputs â†’ Read Holding Registers â†’ Wiederholung`

Die Firmware ist auf eine Gegenstelle mit Slave-Adresse `1` und einer festen Registerbelegung zugeschnitten. Sie ist keine allgemeine Modbus-Bibliothek.

## Projektstruktur

| Pfad | Inhalt |
|---|---|
| `firmware/modbus_rtu_7/Core` | Einstieg, Systeminitialisierung und Interrupts |
| `firmware/modbus_rtu_7/modbus/modbus.c` | Zustandsfolge, GPIO und UART-Callbacks |
| `firmware/modbus_rtu_7/modbus/rtu.c` | Telegrammvorlagen und CRC16 |
| `firmware/modbus_rtu_7/Drivers` | Mitgelieferte STM32-HAL-/CMSIS-Dateien |
| `docs/HARDWARE.md` | Aus dem Code abgeleitete AnschlÃ¼sse und Register |
| `docs/VALIDIERUNG.md` | Offene Fehler und nachvollziehbare TestfÃ¤lle |

## Build und Inbetriebnahme

1. STM32CubeIDE mit STM32F1-UnterstÃ¼tzung bereitstellen.
2. `firmware/modbus_rtu_7` als bestehendes Projekt in einen neuen Workspace importieren.
3. Projektname `modbus_rtu_7` beibehalten. Include-Verzeichnisse und Quellordner wurden fÃ¼r Debug und Release in der Projektdatei vereinheitlicht.
4. ZunÃ¤chst `Debug`, anschlieÃŸend `Release` bauen; Ergebnis und Toolchain-Version dokumentieren.
5. Erst nach PrÃ¼fung des tatsÃ¤chlichen Boards, Transceiver-Moduls und der Gegenstelle flashen und die Tests aus `docs/VALIDIERUNG.md` ausfÃ¼hren.

Die `.ioc` bildet zusÃ¤tzliche Registerkonfiguration in `modbus.c` nicht vollstÃ¤ndig ab. Eine automatische Neugenerierung kann Anwendungscode verÃ¤ndern; nicht ungeprÃ¼ft regenerieren.

## Noch zu ergÃ¤nzen

Echtes Anschlussbild, verwendetes Board/Transceiver, Gegenstelle, reproduzierbares Messprotokoll und klar benannte eigene Entwicklungsanteile. Es wurden keine Fotos, Messwerte oder Projektleistungen erfunden.

Siehe auch [Herkunft und Lizenzen](THIRD_PARTY_NOTICES.md).

## Reproduzierbarer Build ohne IDE

Voraussetzungen: Python 3 und Arm-GCC mit Newlib/Nano-Specs. `arm-none-eabi-gcc` muss im PATH liegen; alternativ die Umgebungsvariable `ARM_GCC` auf die Compilerdatei setzen. Keine Bibliotheken werden heruntergeladen.

Im Repository-Verzeichnis:

```text
python tools/build.py --profile Debug
python tools/build.py --profile Release
```

Die Ausgabe liegt unter `build/<Profil>/firmware.elf` und wird durch `.gitignore` ausgeschlossen. Der direkte Build beider Profile wurde mit Arm-GCC 12.3.1 geprüft. Das Skript verwendet dieselben Compiler-/Linkeroptionen; der gesonderte STM32CubeIDE-Import ist noch nicht geprüft. Build-Warnungen stehen in `BUILD_STATUS.txt`.
