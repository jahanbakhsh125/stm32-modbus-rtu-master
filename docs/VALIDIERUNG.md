# Prüfplan und bekannte Grenzen

## Im Anwendungscode beobachtet, noch nicht korrigiert

- Ein ausbleibendes oder zu kurzes Antworttelegramm kann die DMA-Ablaufkette anhalten; Timeout/Retry fehlen im geprüften Modul.
- Nach CRC-Fehler wird die Zustandsfolge trotzdem fortgesetzt.
- Slave-Adresse, Funktionscode, Bytezahl und Modbus-Exception-Antworten werden nicht vollständig ausgewertet.
- Rückgaben von HAL-UART-Aufrufen und der Callback-Parameter `huart` werden nicht ausreichend berücksichtigt.
- `ADC1->DR` wird für High- und Low-Byte separat gelesen; einen Messwert einmalig zwischenspeichern.
- Zusätzlich liest die Hauptschleife `ADC1->DR`; Einfluss auf Messwertübernahme prüfen.
- ADC-Kommentar, Kanal-/Sampling-Einstellung, ADC-Takt und Kalibrierungsablauf vor Messungen abgleichen.

## Noch auszuführende Tests

| Test | Erwartung nach Fehlerbehandlung | Ergebnis |
|---|---|---|
| Alle vier Funktionen mit dokumentierter Gegenstelle | Werte und GPIO folgen der Registerbelegung | Offen |
| Falsche CRC | Antwort verwerfen und Fehler melden | Offen |
| Antwort fehlt / ist verkürzt | Definierter Timeout, kontrollierter Wiederanlauf | Offen |
| Falscher Slave / Funktionscode / Bytezahl | Antwort verwerfen | Offen |
| Modbus-Exception | Fehler ohne festhängenden DMA-Empfang behandeln | Offen |
| Analogeingang ändern | Konsistente High-/Low-Bytes eines Messwerts | Offen |
| Längerer Betrieb und Busunterbrechung | Verhalten und Wiederanlauf protokollieren | Offen |

Es liegen hier keine Hardware-Testergebnisse vor. Build-Ergebnisse stehen separat in BUILD_STATUS.txt.
