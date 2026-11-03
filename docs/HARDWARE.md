# Hardware und Telegramme

Die folgende Zuordnung stammt aus Firmware und `.ioc`; sie ist kein geprüfter Schaltplan.

| Signal | Pin im vorhandenen Projekt |
|---|---|
| USART2 TX / RX | PA2 / PA3 |
| RS485-Richtungssteuerung | PB1, High = Senden, Low = Empfangen im Code |
| CRC-Statusanzeige | PC13 |
| Digitale Statusausgänge | PB5 / PB9 |
| Digitale Eingänge | PB14 / PB15 |
| EXTI-Eingänge | PB12 / PB13 |
| Analogeingang | PA0 im Initialisierungscode; Kommentar und Sampling-Konfiguration überprüfen |

Die konkrete Verbindung zu DE/RE, Pegelverträglichkeit, Versorgung, A/B-Leitungen und Busabschluss hängen vom tatsächlichen Transceiver-Modul ab und sind vor dem Anschluss festzuhalten.

## Feste Telegrammbelegung im Quelltext

| Funktion | Startadresse im Telegramm | Anzahl |
|---|---|---|
| `0x0F` Write Multiple Coils | `0x0501` | 5 Coils |
| `0x10` Write Multiple Registers | `0x1000` | 2 Register |
| `0x02` Read Discrete Inputs | `0x0400` | 8 Eingänge |
| `0x03` Read Holding Registers | `0x0E00` | 1 Register |

Slave-Adresse: `1`. Die Belegung muss zu einer dokumentierten Gegenstelle passen; keine universelle Gerätebelegung. Datenreihenfolge und Registerzuordnung sind im Test zu prüfen.
